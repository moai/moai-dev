// UIColorExtras.m -- extra UIColor methods
// by allen brunson  may 12 2009

#import "UIColorExtras.h"

CG_INLINE CGFloat colorElementToFloat(uint8_t color)
{
    CGFloat elem = color;
    elem = elem / 255.0;
    return elem;
}

@implementation UIColor (UIColorExtras)

+(UIColor*)integerRed:(uint8_t)red green:(uint8_t)green blue:(uint8_t)blue
{
    return [UIColor integerRed:red green:green blue:blue alpha:0xFF];
}

+(UIColor*)integerRed:(uint8_t)red green:(uint8_t)green blue:(uint8_t)blue
 alpha:(uint8_t)alpha
{
    const CGFloat  r = colorElementToFloat(red);
    const CGFloat  g = colorElementToFloat(green);
    const CGFloat  b = colorElementToFloat(blue);
    const CGFloat  a = colorElementToFloat(alpha);

    return [UIColor colorWithRed:r green:g blue:b alpha:a];
}

@end
