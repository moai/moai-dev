#import <UIKit/UIKit.h>

@class ChipmunkDemo;

@interface iPhoneChipmunkDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIView *view;
	IBOutlet UIViewController *viewController;
	IBOutlet UIViewController *flipController;
	
	IBOutlet UIButton *flipButton;
	IBOutlet UIButton *nextButton;
	IBOutlet UIButton *prevButton;
	IBOutlet UILabel *label;
	
	ChipmunkDemo *demo;
	
	int demoIndex;
	
	BOOL flip;
}

- (IBAction)nextDemo:(id)sender;
- (IBAction)prevDemo:(id)sender;
- (IBAction)resetDemo:(id)sender;

- (IBAction)flip:(id)sender;
- (IBAction)showInfoPage:(id)sender;

@end

