//
//  OnlyTabBarViewController.m
//  vungleTabApp
//


#import "OnlyTabBarViewController.h"
#import "vunglepub.h"


@implementation OnlyTabBarViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.title = NSLocalizedString(@"TabBar", @"TabBar");
        self.tabBarItem.image = [UIImage imageNamed:@"first"];
    }
    return self;
}
			
- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
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
    
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    [super viewWillAppear:animated];

}
-(void)viewDidAppear:(BOOL)animated{
    [self.view setFrame:CGRectMake(0, -20, self.view.frame.size.width ,self.view.frame.size.height+20)];

    [super viewDidAppear:animated];
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
