// MainViewController.m -- main view controller for test app
// by allen brunson  january 9 2012

#import "AppDelegate.h"
#import "MainView.h"
#import "MainViewController.h"
#import "Prefs.h"

@implementation MainViewController

@synthesize mainView;

-(void)buttonPlayAd:(id)sender
{
    //NSLog(@"Texted? %@ ",[VGVunglePub setCustomCountDownText:@"NANANAAAAAAAAAA"]?@"YES":@"NO");
    if(![Prefs getIncentivized]) {
        [VGVunglePub playModalAd:self animated:TRUE];
    }
    else {
        //[VGVunglePub alertBoxWithTitle:nil Body:nil leftButtonTitle:nil rightButtonTitle:nil];
        [VGVunglePub playIncentivizedAd:self animated:TRUE showClose:TRUE userTag:@"helloworld"];
    }
}

-(void)buttonPrefs:(id)sender
{
    [AppDelegate runPrefs];
    [self.navigationController setNavigationBarHidden:YES];
    [[UIApplication sharedApplication] setStatusBarHidden:NO];
    [self.navigationController setNavigationBarHidden:NO];

}

+(MainViewController*)controller
{
    return [[[self alloc] initWithNibName:nil bundle:nil] autorelease];
}

-(void)dealloc
{
    self.mainView = nil;
    
    [super dealloc];
}

-(id)initWithNibName:(NSString*)nibName bundle:(NSBundle*)nibBundle
{
    self = [super initWithNibName:nibName bundle:nibBundle];
    if (!self) return nil;
    
    self.title = @"Vungle Test";
    return self;
}

-(BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)orient
{
    return TRUE;
}

//-(NSUInteger)supportedInterfaceOrientations
//{
//    return UIInterfaceOrientationMaskAll;
//}

-(void)terminalClear
{
    [self.mainView terminalClear];
}

-(void)viewDidLoad
{
    SEL  prc1 = @selector(buttonPlayAd:);
    SEL  prc2 = @selector(buttonPrefs:);
    
    [super viewDidLoad];
    
    self.mainView = [MainView view];
    
    [self.mainView.playButton addButtonTarget:self action:prc1];
    [self.mainView.prefsButton addButtonTarget:self action:prc2];
     
    self.view = self.mainView;
}
- (void)viewWillAppear:(BOOL)animated
{
//    [self.navigationController setNavigationBarHidden:YES];
    [super viewWillAppear:animated];
    [[UIApplication sharedApplication] setStatusBarHidden:[Prefs hideStatusBarGet]];
    
    [self.navigationController setNavigationBarHidden:YES];
    [self.navigationController setNavigationBarHidden:[Prefs hideNavigationBarGet]];
}
- (void)viewDidAppear:(BOOL)animated
{
    if(![UIDevice currentDevice].generatesDeviceOrientationNotifications) {
        [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    }
    [super viewDidAppear:animated];
    [self.navigationController setNavigationBarHidden:YES];
    [self.navigationController setNavigationBarHidden:[Prefs hideNavigationBarGet]];
}

-(void)viewDidUnload
{
    if([UIDevice currentDevice].generatesDeviceOrientationNotifications) {
        [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
    }
    [super viewDidUnload];
    nlog(@"%@ unloaded", self.className);
    self.mainView = nil;
}

-(void)vungleLogLine:(NSString*)logLine
{
    [self.mainView vungleLogLine:logLine];
}

-(void)vungleMoviePlayed:(VGPlayData*)playData
{
    // movie ad finished, get times from playData
}

-(void)vungleViewDidDisappear:(UIViewController*)viewController
{
    // ad view is gone
}

-(void)vungleViewWillAppear:(UIViewController*)viewController
{
    // ad view will appear
}

-(void)vungleStatusUpdate:(VGStatusData*)statusData
{
    [self.mainView vungleStatusUpdate:statusData];
}

@end
