#import "ViewController.h"
#import "MyJoyStickView.h"
#import "CocoaAsyncSocket.h"

NSString* server_ip = @"192.168.4.1";
int       server_port = 80;
CGFloat   max_value;
@interface ViewController ()<MyJoyStickViewDelegate,GCDAsyncUdpSocketDelegate,UITextFieldDelegate>
{
    GCDAsyncUdpSocket *udpSocket;
    NSTimer* beatTimer;
}

@property (nonatomic, strong) MyJoyStickView *joyStickView;
@property (nonatomic)UILabel* label;
@property (nonatomic)UILabel* labelSpeed;
@property (nonatomic) UIView* controlView;
@property (nonatomic)UIButton* btnStop;
@property (nonatomic)UIButton* btnGo;
@property (nonatomic)UIButton* btnOTA;
@property (nonatomic)UITextField* cmdField;
@property (nonatomic)UIButton* btnSendCmd;

@property (nonatomic)BOOL   bConnected;
@property (nonatomic)NSInteger nLostCount;
@property (nonatomic)NSString* strCMD;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self joyStick];
    [self setupSocket];
    self.nLostCount = 0;
    self.bConnected = FALSE;
    self.strCMD = @"";
    beatTimer = [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(HeartBeat) userInfo:nil repeats:YES];
    // Do any additional setup after loading the view.
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(joyStickCoordinateChangeDataNotification:) name:@"joyStickMoveState" object:nil];
}

- (void)viewWillAppear:(BOOL)animated
{
    
}

- (void)HeartBeat
{
    NSString* s = @"heartbeat";
    [self sendData:[s dataUsingEncoding:NSUTF8StringEncoding]];
    self.nLostCount ++;
    if(self.nLostCount >5)
    {
        //显示失联
        self.bConnected = FALSE;
        self.label.backgroundColor = [UIColor yellowColor];
        [self setupSocket];
    }
    NSLog(s);
}

#pragma mark 摇杆
- (void)joyStick {
    CGFloat width = self.view.frame.size.width;
    CGFloat hight = self.view.frame.size.height;

    self.joyStickView = [[MyJoyStickView alloc] initWithFrame:CGRectMake(0, 0, width, width)];
    self.controlView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, width, hight-width)];
    self.label = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, width/2, 50)];
    self.label.textAlignment = NSTextAlignmentCenter;
    self.label.textColor = [UIColor blackColor];
    self.labelSpeed = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, width, 50)];
    self.labelSpeed.textAlignment = NSTextAlignmentCenter;
    self.btnStop = [[UIButton alloc]initWithFrame:CGRectMake(0, 0, width/5, 50)];
    [self.btnStop setTitle:@"停止" forState:UIControlStateNormal];
    self.btnStop.backgroundColor = [UIColor lightGrayColor];
    self.btnStop.layer.cornerRadius = 8.0;
    self.btnGo = [[UIButton alloc]initWithFrame:CGRectMake(0, 0, width/5, 50)];
    [self.btnGo setTitle:@"向前" forState:UIControlStateNormal];
    self.btnGo.backgroundColor = [UIColor lightGrayColor];
    self.btnGo.layer.cornerRadius = 8.0;
    
    self.btnOTA = [[UIButton alloc]initWithFrame:CGRectMake(0, 0, width/5, 50)];
    [self.btnOTA setTitle:@"急停" forState:UIControlStateNormal];
    self.btnOTA.backgroundColor = [UIColor lightGrayColor];
    self.btnOTA.layer.cornerRadius = 8.0;
    
    self.cmdField = [[UITextField alloc]initWithFrame:CGRectMake(0, 0, width/2, 50)];
    self.cmdField.backgroundColor = [UIColor lightGrayColor];
    self.cmdField.layer.cornerRadius = 8.0;
    self.cmdField.delegate = self;
    self.btnSendCmd = [[UIButton alloc]initWithFrame:CGRectMake(0, 0, width/2+20, 50)];
    [self.btnSendCmd setTitle:@"Send CMD" forState:UIControlStateNormal];
    self.btnSendCmd.backgroundColor = [UIColor greenColor];
    [self.btnSendCmd setTitle:@"Sending..." forState:UIControlStateHighlighted];
    [self.btnSendCmd setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    self.btnSendCmd.layer.cornerRadius = 8.0;
    
    //[self.joyStickView sizeToFit];
//    [self.label sizeToFit];
    self.joyStickView.delegate = self;
    [self.view addSubview:self.joyStickView];
    [self.view addSubview:self.controlView];
    [self.controlView addSubview:self.label];
    [self.controlView addSubview:self.labelSpeed];
    [self.controlView addSubview:self.btnGo];
    [self.controlView addSubview:self.btnStop];
    [self.controlView addSubview:self.btnOTA];
    [self.controlView addSubview:self.cmdField];
    [self.controlView addSubview:self.btnSendCmd];
    [self.btnGo addTarget:self action:@selector(sendGo) forControlEvents:UIControlEventTouchUpInside];
    [self.btnStop addTarget:self action:@selector(sendStop) forControlEvents:UIControlEventTouchUpInside];
    [self.btnOTA addTarget:self action:@selector(sendOTA) forControlEvents:UIControlEventTouchUpInside];
    [self.btnSendCmd addTarget:self action:@selector(sendCMD) forControlEvents:UIControlEventTouchUpInside];
    [self layoutJoyStick];
    
    max_value = (self.joyStickView.stickBack.frame.size.width - self.joyStickView.stick.frame.size.width)/2;

}

- (void)layoutJoyStick
{


    self.joyStickView.center = CGPointMake(self.view.frame.size.width / 2, self.view.frame.size.height - self.view.frame.size.width / 2);
    CGFloat width = self.controlView.frame.size.width;
    CGFloat hight = self.controlView.frame.size.height;
    CGPoint p = self.controlView.center;
    self.labelSpeed.center = p;
    p.y -= 50;
    self.label.center = p;
    
    self.btnStop.center = CGPointMake(width*1/4, hight-self.btnGo.frame.size.height/2-30);
    self.btnGo.center = CGPointMake(width*2/4, hight-self.btnGo.frame.size.height/2 - 30);
    self.btnOTA.center = CGPointMake(width*3/4, hight-self.btnGo.frame.size.height/2 - 30);
//    self.btnStop.hidden = YES;
//    self.btnOTA.hidden = YES;
//    self.btnGo.hidden = YES;
    
    p.y += 60;
    self.cmdField.center = p;
    p.y += 60;
    self.btnSendCmd.center = p;
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
    [coordinator animateAlongsideTransition:^(id<UIViewControllerTransitionCoordinatorContext> context) {
        [self layoutJoyStick];
    }
    completion:^(id<UIViewControllerTransitionCoordinatorContext> context){

    }];
}

- (void)joyStickCoordinateChangeDataX:(CGFloat)x WithY:(CGFloat)y {
    //映射成 0-255
    x = roundf(x/max_value*255);
    y = roundf(y/max_value*255);
    NSString* s = [NSString stringWithFormat:@"%d,%d",(int)x,(int)y];
    self.label.text = s;
    if([self.strCMD compare:s] == NSOrderedSame)
        return;
    self.strCMD = s;
    [self sendData:[s dataUsingEncoding:NSUTF8StringEncoding]];
    NSLog(@"joyStick x = %f , y = %f",x,y);
}

- (void)joyStickCoordinateChangeDataNotification:(NSNotification *)notice {
    NSLog(@"move end");
    self.label.text = @"reset";
    self.strCMD = self.label.text;
    [self sendData:[self.label.text dataUsingEncoding:NSUTF8StringEncoding]];
}


- (void)setupSocket
{
    udpSocket = [[GCDAsyncUdpSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
    
    NSError *error = nil;
    
    if (![udpSocket bindToPort:0 error:&error])
    {
        NSLog(@"Error binding: %@", error);
        return;
    }
    if (![udpSocket beginReceiving:&error])
    {
        NSLog(@"Error receiving: %@", error);
        return;
    }
    
    NSLog(@"Ready");
}

- (void)sendData:(NSData*)data
{
    [udpSocket sendData:data toHost:server_ip port:server_port withTimeout:-1 tag:0];
}
- (void)udpSocket:(GCDAsyncUdpSocket *)sock didSendDataWithTag:(long)tag
{
    // You could add checks here
}

- (void)udpSocket:(GCDAsyncUdpSocket *)sock didNotSendDataWithTag:(long)tag dueToError:(NSError *)error
{
    // You could add checks here
}

- (void)udpSocket:(GCDAsyncUdpSocket *)sock didReceiveData:(NSData *)data
                                               fromAddress:(NSData *)address
                                         withFilterContext:(id)filterContext
{
    NSString *msg = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    if (msg)
    {
        if([msg compare:@"heartbeat"] == kCFCompareEqualTo)
        {
            self.bConnected = TRUE;
            self.nLostCount = 0;
            self.label.backgroundColor = [UIColor greenColor];
        }
        else{
            NSLog(@"RECV: %@", msg);
            self.labelSpeed.text = msg;
        }
        
    }
    else
    {
        NSString *host = nil;
        uint16_t port = 0;
        [GCDAsyncUdpSocket getHost:&host port:&port fromAddress:address];
        
        NSLog(@"RECV: Unknown message from: %@:%hu", host, port);
    }
}

-(void) sendGo
{
    NSString* s = @"go";
    self.label.text = s;
    [self sendData:[s dataUsingEncoding:NSUTF8StringEncoding]];
}

-(void) sendStop
{
    NSString* s = @"stop";
    self.label.text = s;
    [self sendData:[s dataUsingEncoding:NSUTF8StringEncoding]];
}

-(void) sendOTA
{
    NSString* s = @"frozen";
    self.label.text = s;
    [self sendData:[s dataUsingEncoding:NSUTF8StringEncoding]];
}

-(void) sendCMD
{
    NSString* s = self.cmdField.text;
    if(s.length == 0)
        return;
    s = [s stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    s = [s lowercaseString];
    self.label.text = s;
    [self sendData:[s dataUsingEncoding:NSUTF8StringEncoding]];
}


- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [self sendCMD];
    [self.cmdField resignFirstResponder];
    return YES;
}
@end
