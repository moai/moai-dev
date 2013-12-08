// UIFontExtras.m -- extra UIFont methods
// by allen brunson  april 3 2009

#import "UIFontExtras.h"

@implementation UIFont (UIFontExtras)

-(CGFloat)fontLineHeight
{
    if (VGRespondsTo(self, @selector(lineHeight)))
    {
        return fabs(ceil(self.lineHeight));
    }
    
    if (VGRespondsTo(self, @selector(leading)))
    {
        return fabs(ceil(self.leading));
    }
    
    return 0.0;
}

@end
