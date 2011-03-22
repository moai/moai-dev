#import <UIKit/UIKit.h>

#import <QuartzCore/CADisplayLink.h>

#import "FallingButton.h"


@interface ViewController : UIViewController <UIAccelerometerDelegate> {
	CADisplayLink *displayLink;
	ChipmunkSpace *space;
	FallingButton *fallingButton;
}

@end

