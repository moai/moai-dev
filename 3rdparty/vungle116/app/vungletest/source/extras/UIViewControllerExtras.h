// UIViewControllerExtras.h -- extra methods for UIViewController
// by allen brunson  august 19 2009


/******************************************************************************/
/*                                                                            */
/***  UIViewController category                                             ***/
/*                                                                            */
/******************************************************************************/

@interface UIViewController (UIViewControllerExtras)

// properties

@property(nonatomic, retain) UIImage* tabBarImage;

// orientation methods

-(BOOL)isLandscapeMode;
-(BOOL)isPortraitMode;
-(UIInterfaceOrientation)orientation;

// tab bar image

-(void)setTabBarImage:(UIImage*)image;
-(UIImage*)tabBarImage;

@end


/******************************************************************************/
/*                                                                            */
/***  UIViewController category                                             ***/
/*                                                                            */
/******************************************************************************

overview
--------

couple extra methods for UIViewController

*/
