#import <UIKit/UIKit.h>

#import "ObjectiveChipmunk.h"

@interface BoardView : UIView {
	IBOutlet UIView *barView;
	IBOutlet UIProgressView *meter;
	
	ChipmunkBody *barBody;
	cpFloat lastBarHeight;
	
	ChipmunkSpace *space;
	ChipmunkBody *staticBody;
	NSMutableSet *pieces;
	
	int idleTicks;
}

- (void)startNewGame;

@end
