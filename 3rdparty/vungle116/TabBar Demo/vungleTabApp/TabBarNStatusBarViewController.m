//
//  TabBarNStatusBarViewController.m
//  vungleTabApp
//


#import "TabBarNStatusBarViewController.h"
#import "vunglepub.h"
#import "AppDelegate.h"


@implementation TabBarNStatusBarViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.title = NSLocalizedString(@"Tab+Status", @"Tab+Status");
        self.tabBarItem.image = [UIImage imageNamed:@"second"];
    }
    return self;
}

- (IBAction)playAdButtonTouched:(UIButton*)sender{
    if ([VGVunglePub adIsAvailable]){
        [VGVunglePub playModalAd:self animated:YES];
    }
    else {
        NSLog(@"Ad Not Yet Available");
    }
}
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [[UIApplication sharedApplication] setStatusBarHidden:NO]; 
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return TRUE;
}

@end
