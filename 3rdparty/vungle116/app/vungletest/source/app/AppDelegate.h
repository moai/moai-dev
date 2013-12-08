// AppDelegate.h -- app delegate for vungletest
// by allen brunson  january 2 2012

#import "MainViewController.h"


/******************************************************************************/
/*                                                                            */
/***  AppDelegate class                                                     ***/
/*                                                                            */
/******************************************************************************/

@interface AppDelegate: NSObject <UIApplicationDelegate>
{
    @private
    
    UIWindow*                window;
    UINavigationController*  navController;
    MainViewController*      mainViewController;
}

// properties

@property(nonatomic, retain) UIWindow*                window;
@property(nonatomic, retain) UINavigationController*  navController;
@property(nonatomic, retain) MainViewController*      mainViewController;

// class methods

+(void)alertWithMessage:(NSString*)message;
+(void)alertWithTitle:(NSString*)title message:(NSString*)message;

+(AppDelegate*)appDelegate;

+(void)popToRootViewController;
+(void)pushViewController:(UIViewController*)controller;

+(void)runPrefs;
+(NSString*)versionString;

// construction and destruction

-(void)dealloc;
-(id)init;

// @private

-(UINavigationController*)makeNavController;
-(UIWindow*)makeWindow;

-(void)startWindow;

-(void)vungleStart;
-(void)vungleStop;
-(void)vungleVersionTest;

// UIApplicationDelegate methods

-(BOOL)application:(UIApplication*)application
 didFinishLaunchingWithOptions:(NSDictionary*)launchOptions;
 
-(void)applicationDidReceiveMemoryWarning:(UIApplication*)application;
-(void)applicationWillTerminate:(UIApplication*)application;

@end


/******************************************************************************/
/*                                                                            */
/***  AppDelegate class                                                     ***/
/*                                                                            */
/******************************************************************************

overview
--------

app delegate for vungle test app

*/
