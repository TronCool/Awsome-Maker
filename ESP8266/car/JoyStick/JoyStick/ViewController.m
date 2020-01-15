//
//  ViewController.m
//  JoyStick
//
//  Created by 草帽~小子 on 2019/5/7.
//  Copyright © 2019 OnePiece. All rights reserved.
//

#import "ViewController.h"
#import "YHHandShankView.h"
#import "CocoaAsyncSocket.h"

NSString* server_ip = @"192.168.4.1";
int       server_port = 80;
CGFloat   max_value;
@interface ViewController ()<YHHandShankViewDelegate,GCDAsyncUdpSocketDelegate>
{
    GCDAsyncUdpSocket *udpSocket;
//    NSData*    sData;
}
@property (nonatomic, strong) UIImageView *manualBackV;
@property (nonatomic, strong) YHHandShankView *customView;
@property (nonatomic)UILabel* label;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self joyStick];
    [self setupSocket];
    // Do any additional setup after loading the view.
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(joyStickCoordinateChangeDataNotification:) name:@"joyStickMoveState" object:nil];
}

#pragma mark 摇杆
- (void)joyStick {
    //摇杆
    CGFloat width = 0;
    if(self.view.frame.size.width < self.view.frame.size.height)
        width = self.view.frame.size.width;
    else
        width = self.view.frame.size.height;
    
    
    
    self.customView = [[YHHandShankView alloc] initWithFrame:CGRectMake(0, 0, width, width)];
    self.label = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, width/2, width/2)];
    self.label.textAlignment = NSTextAlignmentCenter;
    //[self.customView sizeToFit];
//    [self.label sizeToFit];
    self.customView.delegate = self;
    [self.view addSubview:self.customView];
    [self.view addSubview:self.label];
    [self layoutJoyStick];
    
    max_value = (self.customView.stickBack.frame.size.width - self.customView.stick.frame.size.width)/2;
}

- (void)layoutJoyStick
{
    if(self.view.frame.size.width < self.view.frame.size.height){
        self.customView.center = CGPointMake(self.view.frame.size.width / 2, self.view.frame.size.height - self.view.frame.size.width / 2);
        self.label.center = CGPointMake(self.view.frame.size.width / 2,self.view.frame.size.width / 2);
    }
    else{
        self.customView.center = CGPointMake(self.view.frame.size.width - self.view.frame.size.height/ 2, self.view.frame.size.height / 2);
        self.label.center = CGPointMake(self.view.frame.size.height / 2,self.view.frame.size.height / 2);
    }
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
    [self sendData:[self.label.text dataUsingEncoding:NSUTF8StringEncoding]];
    NSLog(@"joyStick x = %f , y = %f",x,y);
}

- (void)joyStickCoordinateChangeDataNotification:(NSNotification *)notice {
    NSLog(@"move end");
    self.label.text = @"stop";
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
        NSLog(@"RECV: %@", msg);
    }
    else
    {
        NSString *host = nil;
        uint16_t port = 0;
        [GCDAsyncUdpSocket getHost:&host port:&port fromAddress:address];
        
        NSLog(@"RECV: Unknown message from: %@:%hu", host, port);
    }
}


@end
