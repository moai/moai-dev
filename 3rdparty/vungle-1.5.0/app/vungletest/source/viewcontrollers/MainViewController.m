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
    if ([Prefs getIncentivized]) {
        [VGVunglePub playIncentivizedAd:self animated:TRUE showClose:TRUE userTag:@"helloworld"];
    }
    else {
        [VGVunglePub playModalAd:self animated:TRUE];
    }
	[_player stop];
}

-(void)buttonPrefs:(id)sender
{
    [AppDelegate runPrefs];
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

-(void)terminalClear
{
    [self.mainView terminalClear];
}

-(void)viewDidLoad
{
    SEL  prc1 = @selector(buttonPlayAd:);
    SEL  prc2 = @selector(buttonPrefs:);
    
    [super viewDidLoad];
    
    [self setMainView:[MainView view]];
    [self setView:self.mainView];
    
    [self.mainView.playButton addButtonTarget:self action:prc1];
    [self.mainView.prefsButton addButtonTarget:self action:prc2];
	
	// start playing background music
	NSBundle *mainBundle = [NSBundle mainBundle];
	NSString *filePath = [mainBundle pathForResource:@"Music" ofType:@"mp3"];
	NSData *fileData = [NSData dataWithContentsOfFile:filePath];
	NSError *error = nil;
	
	_player = [[AVAudioPlayer alloc] initWithData:fileData error:&error];
	[_player play];
	
	nlog(@"music by http://incompetech.com");
}

-(void)viewDidUnload
{
    [super viewDidUnload];
    [self setMainView:nil];
    
    nlog(@"%@ unloaded", self.className);
}

-(void)vungleLogLine:(NSString*)logLine
{
    [self.mainView vungleLogLine:logLine];
}

-(void)vungleMoviePlayed:(VGPlayData*)playData
{
    // movie ad finished, get times from playData
}

#if 0
// this is the old way
-(void)vungleViewDidDisappear:(UIViewController *)viewController {
	// here you should check in your game whether or not the audio should be resumed, because it might be manually paused/muted
	[_player play];
}
#else
-(void)vungleViewDidDisappear:(UIViewController *)viewController willShowProductView:(BOOL)willShow {
	// here you should check in your game whether or not the audio should be resumed, because it might be manually paused/muted
	// also, if we're about to display a product view (SKStoreProductViewController) we should not turn the audio on yet.
	if (!willShow) {
		[_player play];
	}
}
#endif

-(void)vungleViewWillAppear:(UIViewController*)viewController {
    // ad view will appear
}

- (void)vungleStatusUpdate:(VGStatusData*)statusData {
    [self.mainView vungleStatusUpdate:statusData];
}

- (void)vungleAppStoreWillAppear {
	if ([_player isPlaying]) {
		[_player pause];
	}
}

- (void)vungleAppStoreViewDidDisappear {
	// here you should check in your game whether or not the audio should be resumed, because it might be manually paused/muted
	if (![_player isPlaying]) {
		[_player play];
	}
}

@end
