#import <UIKit/UIKit.h>

#import "ObjectiveChipmunk.h"

@class CADisplayLink;
@class BoardView;

@interface AppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet UIViewController *flipController;
	IBOutlet UIViewController *infoController;
	IBOutlet BoardView *board;
	
	IBOutlet UIActivityIndicatorView *busy;
	IBOutlet UIProgressView *meter;
	
	IBOutlet UILabel *playerLabel;
	IBOutlet UILabel *scoreLabel;
	
	int currentPlayer;
	int thisScore;
	int scores[2];
	
	BOOL paused;
	CADisplayLink *link;
}

@property BOOL simulationPaused;
@property int thisScore;

+ (AppDelegate *)sharedDelegate;

- (IBAction)showInfo:(id)sender;
- (IBAction)backToGame:(id)sender;
- (IBAction)showInfoPage:(id)sender;

- (void)gameOver;

@end

