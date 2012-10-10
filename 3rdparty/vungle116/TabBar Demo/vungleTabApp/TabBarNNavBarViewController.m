//
//  TabBarNNavBarViewController.m
//  vungleTabApp
//


#import "TabBarNNavBarViewController.h"
#import "vunglepub.h"


@implementation TabBarNNavBarViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{

    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        
        self.title = NSLocalizedString(@"Tab+Nav", @"Tab+Nav");
        self.tabBarItem.image = [UIImage imageNamed:@"first"];
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
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated{
    [self.navigationController.navigationBar setHidden:NO];
    
    [self.navigationController.navigationBar setFrame:CGRectMake(0, 0, self.view.frame.size.width,45)];
  
    [super viewDidAppear:animated];
}
- (void)viewDidLoad
{
    [super viewDidLoad];

    // [self.view setFrame:CGRectMake(0, 0, self.view.frame.size.width ,self.view.frame.size.height+20)];// Do any additional setup after loading the view, typically from a nib.
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