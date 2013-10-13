// UIControlExtras.m -- extra methods for UIControl
// by allen brunson  february 25 2010

#import "UIControlExtras.h"

@implementation UIControl (UIControlExtras)

-(void)addButtonTarget:(id)target action:(SEL)action
{
    [self addTarget:target action:action
     forControlEvents:UIControlEventTouchUpInside];
}

-(void)removeTarget:(id)target
{
    [self removeTarget:target action:nil
     forControlEvents:UIControlEventAllEvents];
}

@end
