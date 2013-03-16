// UIScrollViewExtras.m -- extra methods for UIScrollView
// by allen brunson  september 1 2010

#import "UIScrollViewExtras.h"

@implementation UIScrollView (UIScrollViewExtras)

-(void)scrollToBottom
{
    const CGRect        bnds = self.bounds;
    const UIEdgeInsets  edge = self.contentInset;
    CGPoint             offs = CGPointMake(-edge.left, -edge.top);
    const CGSize        size = self.contentSize;
    
    if (size.height > bnds.size.height)
    {
        offs.y += (size.height - bnds.size.height);
        offs.y += (edge.bottom + edge.top);
    }
    
    [self setContentOffset:offs animated:TRUE];
}

@end
