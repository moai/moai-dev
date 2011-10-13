//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license



#import "TJCVideoView.h"
#import "TJCLoadingViewController.h"
#import "TJCUtil.h"
#import "TJCVideoManager.h"


@implementation TJCVideoView

@synthesize isVideoReady = isVideoReady_, mainView = mainView_;


- (void)refreshViewWithFrame:(CGRect)frame
{
	[mainView_ setFrame:frame];
	
	// Rotate the view so it's always landscape.
	mainView_.transform = CGAffineTransformIdentity;
	CGAffineTransform transform = CGAffineTransformMakeRotation((float)(M_PI / 2));
	mainView_.transform = transform;
	
	// Repositions and resizes the view.
	CGRect bounds = [[UIScreen mainScreen] bounds];
	
	if (bounds.size.width > bounds.size.height)
	{
		bounds = CGRectMake(0, 0, bounds.size.width, bounds.size.height);
	}
	else
	{
		bounds = CGRectMake(0, 0, bounds.size.height, bounds.size.width);
	}
	mainView_.bounds = bounds;
	
	[mainVideoLayer_ setVideoView:mainView_];
	
	[loadingViewController_.view setFrame:frame];
}


- (id)initWithDelegate:(id<TJCVideoAdDelegate>)delegate
{
	self = [super init];
	
	if (self)
	{
		adDelegate_ = delegate;
		
		mainView_ = [[[NSBundle mainBundle] loadNibNamed:@"TJCVideoView" owner:self options:nil] objectAtIndex:0];
		
		if ([TJCUtil isPad])
		{
			mainVideoLayer_ = iPadLayer_;
			[iPhoneLayer_ setAlpha:0];
		}
		else
		{
			mainVideoLayer_ = iPhoneLayer_;
			[iPadLayer_ setAlpha:0];
		}
		
		// Init loading view.
		loadingViewController_ = [[TJCLoadingViewController alloc] initWithFrame:CGRectZero];

		// This will set certain flags that indicate that videos are in an init state, which is used to prevent repetitive loading.
		[mainVideoLayer_ cleanupVideo];
		[mainVideoLayer_ setVideoView:mainView_];
		
		isVideoReady_ = NO;
	}
	
	return self;
}



- (void)bufferVideoWithPath:(NSString*)path;
{	
	// Pass along the delegate to each video instance so it can send movie status notifications.
	[mainVideoLayer_ prepareVideoWithDelegate:adDelegate_ videoURL:path];
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(loadStateChanged:)
																name:MPMoviePlayerLoadStateDidChangeNotification
															 object:nil];
}


- (void)loadStateChanged:(NSNotification*)notification
{
	MPMoviePlayerController *player = [notification object];
	//NSLog(@"Movieplayer state: %d", [player loadState]);
	
	if ([player loadState] & MPMovieLoadStatePlaythroughOK)
	{		
		isVideoReady_ = YES;

		[[NSNotificationCenter defaultCenter] removeObserver:self name:MPMoviePlayerLoadStateDidChangeNotification object:nil];
		
		// Remove loading view if it's there.
		[loadingViewController_ disable];
		
		// Play the video.
		[mainVideoLayer_ setVideoView:mainView_];
		[self playActiveVideo];
	}
}


- (void)playVideoWithOfferID:(NSString*)offerID
{	
	NSDictionary *videoObjDict = [[[TJCVideoManager sharedTJCVideoManager] getCachedVideoDictonary] objectForKey:offerID];
	
	// Buffer video ad in the shared video view.
	[self bufferVideoWithPath:[videoObjDict objectForKey:TJC_VIDEO_OBJ_DATA_LOCATION]];
	
	// Save offer ID, to be used to ping offer complete when video is done.
	mainVideoLayer_.offerID = offerID;
	
	mainVideoLayer_.isFinishedWatching = NO;
}


- (IBAction)playActiveVideo
{
	if ([adDelegate_ respondsToSelector:@selector(videoAdBegan)])
	{
		[adDelegate_ videoAdBegan];
	}
	
	[mainVideoLayer_ playVideo];
}


- (void)stopActiveVideo
{
	[mainVideoLayer_ stopVideo];
}


- (BOOL)videoAdCurrentlyPlaying
{
	return [mainVideoLayer_ isVideoPlaying];
}


- (IBAction)customButtonAction1
{
	[mainVideoLayer_ buttonAction1];
}


- (IBAction)customButtonAction2
{
	[mainVideoLayer_ buttonAction2];
}


- (IBAction)backToVideo
{
	[mainVideoLayer_ transitionToVideoView];
}


- (IBAction)closeVideoView
{
	[mainVideoLayer_ stopVideo];
	[[mainVideoLayer_ videoFeed] setShouldAutoplay:NO];

	[TJCViewCommons animateTJCView:mainView_ 
					 withTJCTransition:[[TJCViewCommons sharedObject]getReverseTransitionEffect] 
								withDelay:[[TJCViewCommons sharedObject]getTransitionDelay]];
	[self performSelector:@selector(giveBackNotification) withObject:nil afterDelay:[[TJCViewCommons sharedObject]getTransitionDelay]];
	
	// Refresh offer wall.
	[[TJCOffersWebView sharedTJCOffersWebView] refreshWebView];
}


- (IBAction)cancelVideo:(id)sender
{
	if (mainVideoLayer_.isFinishedWatching)
	{
		[mainVideoLayer_ cancelVideo];
	}
	else
	{
		[mainVideoLayer_ pauseVideo];
		// If this video is being played for the first time, show the cancel alert, that currency will not be awarded.
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Cancel Video?" 
																		message:@"Currency will not be awarded, are you sure you want to cancel the video?"
																	  delegate:self 
														  cancelButtonTitle:@"End" 
														  otherButtonTitles:@"Resume", nil];
		
		// Force orientation to landscape for alert view here.
		[[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
		
		[alert show];
		[alert release];
	}
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (buttonIndex == 0)
	{
		[self closeVideoView];		
	}
	else if (buttonIndex == 1)
	{
		[mainVideoLayer_ resumeVideo];
	}
}


-(void) giveBackNotification
{	
	if ([adDelegate_ respondsToSelector:@selector(videoAdClosed)])
	{
		[adDelegate_ videoAdClosed];
	}
	
	// Undo any transformations.
	mainView_.transform = CGAffineTransformIdentity;
	
	[mainView_ removeFromSuperview];
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VIEW_CLOSED_NOTIFICATION object:nil];
	
	// JC: NOTE: Deprecated since 8.1.0
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_SHOW_BOX_CLOSE_NOTIFICATION object:nil];
}


- (void)dealloc
{
	[loadingViewController_ release];
	[mainVideoLayer_ release];
	[mainView_ release];
	[super dealloc];
}


@end
