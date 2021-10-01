#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@protocol MyJoyStickViewDelegate <NSObject>

- (void)joyStickCoordinateChangeDataX:(CGFloat)x WithY:(CGFloat)y;

@end



@interface MyJoyStickView : UIView
{
    NSTimer* timer;
    CGFloat speedX,speedY;
}

@property (nonatomic, strong) UIImageView *stickBack;
@property (nonatomic, strong) UIImageView *stick;
//计算移动的距离相对于摇杆背景滑动的距离
@property (nonatomic, readonly) CGFloat radio;

@property (nonatomic, readonly) double straightSpeed;
@property (nonatomic, readonly) double turnSpeed;
@property (nonatomic, readonly) double M1;
@property (nonatomic, readonly) double M2;


@property (nonatomic, strong) id<MyJoyStickViewDelegate>delegate;

@end
