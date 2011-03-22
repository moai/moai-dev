#import "iPhoneChipmunk.h"

#import "Accelerometer.h"
#import "ChipmunkDemo.h"

@implementation iPhoneChipmunkDelegate

static NSArray *demos;

+ (void)initialize
{
	static BOOL done = FALSE;
	if(done) return;
	
	demos = [[NSArray alloc] initWithObjects:
		[OneWay class],
		[Tumble class],
		[TheoJansen class],
		[Springies class],
//		[PyramidTopple class],
		[Joints class],
		[Bounce class],
		[PyramidStack class],
		[Plink class],
		[Planet class],
		[Sensors class],
		nil
	];
	
	done = TRUE;
}

- (void)setDemo
{
	[demo removeFromSuperview];
	[demo release];
	
	Class klass = [demos objectAtIndex:demoIndex];
	label.text = [klass description];
	demo = [[klass alloc] initWithFrame:view.bounds];
	[view addSubview:demo];
}

static int
modulo(int n, int m)
{
	int mod = n%m;
	return (mod < 0 ? mod + m : mod);
}

- (IBAction)nextDemo:(id)sender;
{
	if(flip) return;
	demoIndex = modulo(demoIndex + 1, [demos count]);
	[self setDemo];
}

- (IBAction)prevDemo:(id)sender;
{
	if(flip) return;
	demoIndex = modulo(demoIndex - 1, [demos count]);
	[self setDemo];
}

- (IBAction)resetDemo:(id)sender;
{
	[self setDemo];
}

- (IBAction)flip:(id)sender;
{
	if(flip){
		demo.paused = TRUE;
		[flipController dismissModalViewControllerAnimated:YES];
		viewController.view.frame = flipController.view.frame;
	} else {
		flipController.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
		[viewController presentModalViewController:flipController animated:YES];
		flipController.view.frame = view.frame;
		[self setDemo];
	}
	
	flip = !flip;
}

- (IBAction)showInfoPage:(id)sender;
{
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://code.google.com/p/chipmunk-physics/"]];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	[Accelerometer installWithInterval:1.0f/60.0f andAlpha:0.1f];
	
	flipButton.transform = CGAffineTransformMakeRotation(M_PI_2);
	nextButton.transform = CGAffineTransformMake(0.0,  1.0, -1.0, 0.0, 0.0, 0.0);
	prevButton.transform = CGAffineTransformMake(0.0, -1.0, -1.0, 0.0, 0.0, 0.0);
	label.transform = CGAffineTransformMakeRotation(M_PI_2);
	
	[self setDemo];
}

- (void)dealloc {
	[view release];
	
	[super dealloc];
}

@end
