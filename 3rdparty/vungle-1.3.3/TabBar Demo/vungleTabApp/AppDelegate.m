//
//  AppDelegate.m
//  vungleTabApp
//
//

#import "AppDelegate.h"

#import "OnlyTabBarViewController.h"
#import "TabBarNStatusBarViewController.h"
#import "TabBarNNavBarViewController.h"
#import "TabBarNStatusBarNNavBarViewController.h"

#import "vunglepub.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize tabBarController = _tabBarController;

- (void)dealloc
{
    [_window release];
    [_tabBarController release];
    [super dealloc];
}

-(void)vungleStart
{
    VGUserData*  data  = [VGUserData defaultUserData];
    NSString*    appID = @"vungleTest";
    
    // set up config data
    data.age             = 23;
    data.gender          = VGGenderMale;
    data.adOrientation   = VGAdOrientationLandscape;
    data.locationEnabled = TRUE;
    [VGVunglePub allowAutoRotate:YES];
    // start vungle publisher library
    [VGVunglePub startWithPubAppID:appID userData:data];
}
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    // Override point for customization after application launch.
   
    UIViewController *viewController1 = [[[OnlyTabBarViewController alloc] initWithNibName:@"OnlyTabBarViewController" bundle:nil] autorelease];
    UIViewController *viewController2 = [[[TabBarNStatusBarViewController alloc] initWithNibName:@"TabBarNStatusBarViewController" bundle:nil] autorelease];
    UIViewController *viewController3 = [[[TabBarNNavBarViewController alloc] initWithNibName:@"TabBarNNavBarViewController" bundle:nil] autorelease];
    UIViewController *viewController4 = [[[TabBarNStatusBarNNavBarViewController alloc] initWithNibName:@"TabBarNStatusBarNNavBarViewController" bundle:nil] autorelease];
    self.tabBarController = [[[UITabBarController alloc] init] autorelease];
    
     UINavigationController *navConroller1 = [[[UINavigationController alloc] initWithRootViewController:viewController3] autorelease];
     UINavigationController *navConroller2 = [[[UINavigationController alloc] initWithRootViewController:viewController4] autorelease];
    self.tabBarController.viewControllers = [NSArray arrayWithObjects:viewController1, viewController2,navConroller1,navConroller2, nil];
    self.window.rootViewController = self.tabBarController;
    [self.window makeKeyAndVisible];
    
    [self vungleStart];
    return YES;
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [VGVunglePub stop];
}

@end
