// UIViewExtras.m -- extra UIView methods
// by allen brunson  march 2 2009

#import "UIViewExtras.h"

@implementation UIView (UIViewExtras)

-(CGRect)frameUpdate
{
    return self.frame;
}

// layoutSubviews typically gets called way too often. some views react poorly
// to having their frame rect changed, notably UIScrollView. this workaround
// keeps a view's frame rect from getting set every time.

-(void)setFrameUpdate:(CGRect)rect
{
    if (!CGRectEqualToRect(self.frame, rect))
    {
        self.frame = rect;
    }
}

@end
