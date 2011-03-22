#import "AppDelegate.h"

#import <QuartzCore/QuartzCore.h>

#import "BoardView.h"

@interface AppDelegate ()

- (void)resetGame;

@end


@interface ResetGameDelegate : NSObject <UIAlertViewDelegate> {
}
@end

@implementation ResetGameDelegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	[[AppDelegate sharedDelegate] resetGame];
	[self release];
}

@end


@implementation AppDelegate

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
	// Set up the update loop
	link = [[CADisplayLink displayLinkWithTarget:board selector:@selector(update:)] retain];
	link.frameInterval = 2;
	[link addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
	
	flipController.view.frame = window.bounds;
	
	[self resetGame];
	
	[window addSubview:flipController.view];
	[window makeKeyAndVisible];
}

- (void)updateLabels
{
	playerLabel.text = [NSString stringWithFormat:@"Player %d", currentPlayer + 1];
	scoreLabel.text = [NSString stringWithFormat:@"%d points", scores[currentPlayer]];
}

- (int)thisScore{return thisScore;}

- (void)removeSubView:(NSString *)animationID finished:(NSNumber *)finished context:(UIView *)view
{
	[view removeFromSuperview];
}

- (void)addScoreFloaty:(NSString *)text
{
	UILabel *label = [[UILabel alloc] initWithFrame:scoreLabel.frame];
	label.text = text;
	label.backgroundColor = scoreLabel.backgroundColor;
	label.font = scoreLabel.font;
	label.opaque = scoreLabel.opaque;
	label.shadowColor = scoreLabel.shadowColor;
	label.shadowOffset = scoreLabel.shadowOffset;
	label.textAlignment = scoreLabel.textAlignment;
	label.textColor = [UIColor whiteColor];
	label.transform = CGAffineTransformMakeTranslation(0.0, -70.0);
	[window addSubview:label];
	
	[UIView beginAnimations:nil context:nil];
		[UIView setAnimationDuration:2.0];
		[UIView setAnimationCurve:UIViewAnimationCurveEaseOut];
		[UIView setAnimationDelegate:self];
		[UIView setAnimationDidStopSelector:@selector(removeSubView:finished:context:)];
		
		label.transform = CGAffineTransformMakeTranslation(0.0, -100.0);
		label.alpha = 0.0;
	[UIView commitAnimations];
}

- (void)setThisScore:(int)value
{
	[self addScoreFloaty:[NSString stringWithFormat:@"+%d", value]];
	scores[currentPlayer] -= thisScore;
	
	thisScore = value;
	scores[currentPlayer] += thisScore;
	
	[self updateLabels];
}

- (void)switchPlayers
{
	[busy stopAnimating];
	[UIView beginAnimations:nil context:nil];
		meter.alpha = 0.0;
	[UIView commitAnimations];
	
	thisScore = 0;
	currentPlayer = (currentPlayer + 1)%2;
	
	[self updateLabels];
	
	paused = TRUE;
}

- (void)updateScoring
{
	if(currentPlayer != -1){
		if(meter.progress == 0.0){
			int amount = -(thisScore + 5);
			scores[currentPlayer] = MAX(0, scores[currentPlayer] + amount);
			[self addScoreFloaty:[NSString stringWithFormat:@"%d", amount]];
		}
		
		[self updateLabels];
		[self performSelector:@selector(switchPlayers) withObject:nil afterDelay:1.0];
	} else {
		[self switchPlayers];
	}
}

- (BOOL)simulationPaused
{
	return paused;
}

- (void)setSimulationPaused:(BOOL)value
{
	link.paused = value;
	
	if(value){
		[self updateScoring];
	} else {
		paused = FALSE;
		[busy startAnimating];
		meter.alpha = 1.0;
	}
}

+ (AppDelegate *)sharedDelegate;
{
	return (AppDelegate *)[UIApplication sharedApplication].delegate;
}

- (void)dealloc
{
	[window release];
	
	[link invalidate];
	[link release];
		
	[super dealloc];
}

- (IBAction)showInfo:(id)sender;
{
	infoController.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
	[flipController presentModalViewController:infoController animated:YES];
	infoController.view.frame = window.bounds;
}

- (IBAction)backToGame:(id)sender;
{
	[flipController dismissModalViewControllerAnimated:YES];
	flipController.view.frame = window.bounds;
}

- (IBAction)showInfoPage:(id)sender;
{
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://code.google.com/p/chipmunk-physics/"]];
}

- (void)resetGame
{
	meter.alpha = 0.0;
	[busy stopAnimating];
	
	playerLabel.text = @"";
	scoreLabel.text = @"";
	
	currentPlayer = -1;
	thisScore = 0;
	scores[0] = 0;
	scores[1] = 0;
	
	link.paused = TRUE;
	[board startNewGame];
	
	paused = FALSE;
	link.paused = FALSE;
}

- (void)gameOver;
{
	int score1 = scores[0];
	int score2 = scores[1];
	
	NSString *message = nil;
	if(score1 == score2){
		message = [NSString stringWithFormat:@"Tie!\n%d to %d", score1, score2];
	} else {
		message = [NSString stringWithFormat:@"Player %d won!\n%d to %d", (score1 > score2 ? 1 : 2), score1, score2];
	}
	
	UIAlertView *alert = [[UIAlertView alloc]
		initWithTitle:@"Game Over"
		message:message
		delegate:[[ResetGameDelegate alloc] init]
		cancelButtonTitle:@"New Game"
		otherButtonTitles:nil
	];
	
	[alert show];
	[alert release];
}

@end
