// UIViewControllerExtras.m -- extra methods for UIViewController
// by allen brunson  august 19 2009

#import "UIViewControllerExtras.h"

@implementation UIViewController (UIViewControllerExtras)

-(BOOL)isLandscapeMode
{
    return ![self isPortraitMode];
}

-(BOOL)isPortraitMode
{
    switch (self.orientation)
    {
        case UIInterfaceOrientationLandscapeLeft:
        case UIInterfaceOrientationLandscapeRight:
        return FALSE;

        case UIInterfaceOrientationPortrait:
        case UIInterfaceOrientationPortraitUpsideDown:
        return TRUE;
    }

    VG_ASSERT(FALSE);
    return TRUE;
}

// this method name is shorter

-(UIInterfaceOrientation)orientation
{
    return self.interfaceOrientation;
}

-(void)setTabBarImage:(UIImage*)image
{
    UITabBarItem*  item = nil;
    NSString*      text = self.title;

    VG_ASSERT(VGStringHasLength(text));
    VG_ASSERT(VGIsKindOf(image, [UIImage class]));

    item = [UITabBarItem alloc];
    item = [[item initWithTitle:text image:image tag:0] autorelease];

    self.tabBarItem = item;
}

-(UIImage*)tabBarImage
{
    return self.tabBarItem.image;
}

@end
