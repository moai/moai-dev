// AppDelegate.m -- app delegate for vungletest
// by allen brunson  january 2 2012

#import "AppDelegate.h"
#import "ConfigViewController.h"
#import "MainViewController.h"
#import "Prefs.h"
#import "VGDownload.h"

static NSString* bundleStringForKey(NSString* key)
{
    NSBundle*      bndl = [NSBundle mainBundle];
    NSDictionary*  dict = [bndl infoDictionary];
    NSString*      text = [dict objectForKey:key];
    
    VG_ASSERT(VGStringHasLength(key));
    VG_ASSERT(VGStringHasLength(text));
    
    return text;
}

int main(int argc, char** argv)
{
    NSAutoreleasePool*  pool = [[NSAutoreleasePool alloc] init];
    NSString*           name = NSStringFromClass([AppDelegate class]);
    int                 rval = 0;
    
    rval = UIApplicationMain(argc, argv, nil, name);
    
    [pool drain];
    pool = nil;
    
    return rval;
}

@implementation AppDelegate

@synthesize window, navController, mainViewController;

+(void)alertWithMessage:(NSString*)message
{
    [self alertWithTitle:nil message:message];
}

+(void)alertWithTitle:(NSString*)title message:(NSString*)message
{
    UIAlertView*  view = [[[UIAlertView alloc] init] autorelease];
    
    if (VGStringHasLength(title))
    {
        view.title = title;
    }    
    
    if (VGStringHasLength(message))
    {
        view.message = message;
    }    
    
    [view addButtonWithTitle:@"OK"];
    [view show];
}

+(AppDelegate*)appDelegate
{
    UIApplication*  uapp = [UIApplication sharedApplication];
    AppDelegate*    cntl = (AppDelegate*) [uapp delegate];
    
    VG_ASSERT(VGIsKindOf(cntl, [AppDelegate class]));
    
    return cntl;
}

-(BOOL)application:(UIApplication*)application
 didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{
    application.statusBarStyle  = UIStatusBarStyleBlackOpaque;
    application.statusBarHidden = FALSE;
    
    [self startWindow];
    [self vungleVersionTest];
    [VGVunglePub allowAutoRotate:TRUE];
    [self vungleStart];
  
    // this is for testing the VGDownload code
    VGReportDownload([Prefs pubAppIDGet]);
    
    return TRUE;
}

-(void)applicationDidReceiveMemoryWarning:(UIApplication*)application
{
    nprn(@"application memory warning");
}

-(void)applicationDidEnterBackground:(UIApplication *)application
{
    nprn(@"application entered background");
}

-(void)applicationWillTerminate:(UIApplication*)application
{
    [self vungleStop];
}

-(void)dealloc
{
    self.window             = nil;
    self.navController      = nil;
    self.mainViewController = nil;
    
    [super dealloc];
}

-(id)init
{
    self = [super init];
    if (!self) return nil;
    
    return self;
}

-(UINavigationController*)makeNavController
{
    UINavigationController*  cntl = nil;
    
    cntl = [UINavigationController alloc];
    cntl = [cntl initWithRootViewController:self.mainViewController];
    [cntl autorelease];
    
    cntl.navigationBarHidden = FALSE;
    cntl.navigationBar.barStyle = UIBarStyleBlack;
    cntl.navigationBar.translucent = FALSE;
    
    return cntl;
}

-(UIWindow*)makeWindow
{
    const CGRect  rect = [[UIScreen mainScreen] bounds];
    UIWindow*     wind = [[[UIWindow alloc] initWithFrame:rect] autorelease];
    
    wind.backgroundColor = [UIColor grayColor];
    
    return wind;
}

+(void)popToRootViewController
{
    AppDelegate*             cntl = self.appDelegate;
    UINavigationController*  unav = cntl.navController;
    
    VG_ASSERT(VGIsKindOf(unav, [UINavigationController class]));
    
    [unav popToRootViewControllerAnimated:TRUE];
}

+(void)pushViewController:(UIViewController*)controller
{
    AppDelegate*             cntl = self.appDelegate;
    UINavigationController*  unav = cntl.navController;
    
    VG_ASSERT(VGIsKindOf(unav, [UINavigationController class]));
    VG_ASSERT(VGIsKindOf(controller, [UIViewController class]));
    
    [unav pushViewController:controller animated:TRUE];
}

+(void)runPrefs
{
    [self pushViewController:[ConfigViewController controller]];
}

-(void)startWindow
{
    self.window = [self makeWindow];
    self.mainViewController = [MainViewController controller];
    self.navController = [self makeNavController];
    self.window.rootViewController = self.navController;
    
    [self.window addSubview:self.navController.view];
    [self.window makeKeyAndVisible];
}

+(NSString*)versionString
{
    return bundleStringForKey(@"CFBundleVersion");
}

-(void)vungleStart
{
    VGUserData*  data = [VGUserData defaultUserData];
    SEL          proc = @selector(vungleLogLine:);
    
    // config data
    data.age             = 36;
    data.gender          = VGGenderFemale;
    data.adOrientation   = [Prefs adOrientationGet];
    data.locationEnabled = [Prefs locationEnabledGet];
    
    // logging stuff
    [VGVunglePub logToStdout:TRUE];
    [VGVunglePub logTarget:self.mainViewController action:proc];
    
    // debug stuff, for internal use only
    [VGVunglePub alwaysShowClose:[Prefs alwaysShowCloseGet]];
    [VGVunglePub forceOldPlayer:[Prefs forceOldPlayerGet]];
    [VGVunglePub memoryReport:[Prefs memoryReportGet]];
    [VGVunglePub muted:[Prefs mutedGet]];
    [VGVunglePub purgeTest:[Prefs purgeTestGet]];
    [VGVunglePub expiryTest:[Prefs expiryTestGet]];
    [VGVunglePub removeCache:[Prefs removeCacheGet]];
    [VGVunglePub resumeTest:[Prefs resumeTestGet]];
    [VGVunglePub showJSON:[Prefs showJSONGet]];
    [VGVunglePub skipMarkViewed:[Prefs skipMarkViewedGet]];
    
    [VGVunglePub setDelegate:self.mainViewController];
    [VGVunglePub allowAutoRotate:NO];
    [VGVunglePub muteIfMusicPlaying:YES];
    
    // start vungle publisher library
    [VGVunglePub startWithPubAppID:[Prefs pubAppIDGet] userData:data];
}

-(void)vungleStop
{
    [VGVunglePub stop];
}

-(void)vungleVersionTest
{
    NSString*  ver1 = [[self class] versionString];
    NSString*  ver2 = [VGVunglePub versionString];
    
    if ([ver1 isEqualToString:ver2]) return;
    
    [AppDelegate alertWithMessage:@"App version doesn't equal Vungle version."];
}

@end
