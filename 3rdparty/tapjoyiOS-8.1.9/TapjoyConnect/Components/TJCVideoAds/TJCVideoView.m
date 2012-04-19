// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVideoView.h"
#import "TJCLoadingView.h"
#import "TJCUtil.h"
#import "TJCVideoManager.h"
#import "TJCCallsWrapper.h"


@implementation TJCVideoView

@synthesize isVideoReady = isVideoReady_, mainView = mainView_;


- (void)refreshViewWithViewController:(UIViewController*)vController
{
	// Rotate the view so it's always landscape.
	[vController.view addSubview:mainView_];
	
	[self updateVideoBounds];
}


- (void)refreshViewWithFrame:(CGRect)frame
{
	[mainView_ setFrame:frame];
	
	[self updateVideoBounds];
}


- (void)updateVideoBounds
{
	// Repositions and resizes the view.
	CGRect bounds = [[UIScreen mainScreen] bounds];
	
	// Make the bounding rect to always be landscape.
	if (bounds.size.width > bounds.size.height)
	{
		bounds = CGRectMake(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
	}
	else
	{
		bounds = CGRectMake(bounds.origin.x, bounds.origin.y, bounds.size.height, bounds.size.width);
	}
	mainView_.bounds = bounds;
	
	// Adjust center if the status bar is visible.
	if (![UIApplication sharedApplication].statusBarHidden)
	{
		mainView_.center = CGPointMake(mainView_.center.x, mainView_.center.y - 10);
		shouldShowStatusBar_ = YES;
		[[UIApplication sharedApplication] setStatusBarHidden:YES];
	}
	
	[mainVideoLayer_ setVideoView:mainView_];
	
	[loadingView_.mainView setFrame:bounds];
}


- (void)refreshViewWithInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	// Rotate the view so it's always landscape.
	mainView_.transform = CGAffineTransformIdentity;
	CGAffineTransform transform;
	
	if (interfaceOrientation == UIInterfaceOrientationLandscapeLeft)
	{
		transform = CGAffineTransformMakeRotation((float)(-M_PI / 2));
	}
	else if (interfaceOrientation == UIInterfaceOrientationLandscapeRight)
	{
		transform = CGAffineTransformMakeRotation((float)(M_PI / 2));
	}
	else
	{
		// Default to 90 degree rotation.
		transform = CGAffineTransformMakeRotation((float)(M_PI / 2));
	}

	mainView_.transform = transform;
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
		loadingView_ = [[TJCLoadingView alloc] initWithFrame:mainView_.frame];
		[mainView_ addSubview:loadingView_.mainView];
		[loadingView_ disable];
		
		// This will set certain flags that indicate that videos are in an init state, which is used to prevent repetitive loading.
		[mainVideoLayer_ cleanupVideo];
		[mainVideoLayer_ setVideoView:mainView_];
		
		isVideoReady_ = NO;
		isAlertShowing_ = NO;
		shouldShowStatusBar_ = NO;
	}
	
	return self;
}



- (void)bufferVideoWithPath:(NSString*)path shouldStream:(BOOL)shouldStream
{	
	// Pass along the delegate to each video instance so it can send movie status notifications.
	[mainVideoLayer_ prepareVideoWithDelegate:adDelegate_ videoURL:path shouldStream:shouldStream];
	
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(loadStateChanged:)
																name:MPMoviePlayerLoadStateDidChangeNotification
															 object:nil];
	
	// Set video view here. This is here since iOS5 errors out otherwise.
	[mainVideoLayer_ setVideoView:mainView_];

	[loadingView_ fadeIn];
	
	[mainView_ addSubview:mainVideoLayer_.closeButton];
}


- (void)loadStateChanged:(NSNotification*)notification
{
	MPMoviePlayerController *player = [notification object];
	[TJCLog logWithLevel:LOG_DEBUG format:@"Movieplayer state: %d", [player loadState]];
	
	if ([player loadState] & MPMovieLoadStatePlaythroughOK)
	{
		isVideoReady_ = YES;
		
		[[NSNotificationCenter defaultCenter] removeObserver:self name:MPMoviePlayerLoadStateDidChangeNotification object:nil];
		
		// Remove loading view if it's there.
		[loadingView_ fadeOut];
		
		[self playActiveVideo];
		
		// Special case for if the alert view is somehow showing right as the video is loaded.
		if (isAlertShowing_)
		{
			[mainVideoLayer_ pauseVideo];
		}
	}
}


- (void)playVideoWithOfferID:(NSString*)offerID
{	
	NSDictionary *cachedVideoDict = [[TJCVideoManager sharedTJCVideoManager] getCachedVideoDictonary];
	NSDictionary *videoObjDict = [[[TJCVideoManager sharedTJCVideoManager] getCachedVideoDictonary] objectForKey:offerID];
	
	BOOL shouldStream = YES;
	NSString *videoURL = nil;
	
	// Check whether the video has finished caching. If so, play it. If not, stream it.
	if ([cachedVideoDict objectForKey:offerID])
	{
		shouldStream = NO;
		videoURL = [videoObjDict objectForKey:TJC_VIDEO_OBJ_DATA_LOCATION];
	}
	else
	{
		NSDictionary *fullVideoDict = [[TJCVideoManager sharedTJCVideoManager] getAllVideosDictionary];
		NSDictionary *tmpVideoObjDict = [fullVideoDict objectForKey:offerID];
		videoURL = [tmpVideoObjDict objectForKey:TJC_VIDEO_OBJ_VIDEO_URL];
	}
	
	// Buffer video ad in the shared video view.
	[self bufferVideoWithPath:videoURL shouldStream:shouldStream];	

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
					 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons]getReverseTransitionEffect] 
								withDelay:[[TJCViewCommons sharedTJCViewCommons]getTransitionDelay]];
	[self performSelector:@selector(giveBackNotification) withObject:nil afterDelay:[[TJCViewCommons sharedTJCViewCommons]getTransitionDelay]];
	
	[mainVideoLayer_ cleanupVideo];
	
	// Refresh offer wall.
	[[TJCOffersWebView sharedTJCOffersWebView] refreshWebView];
	
	if (shouldShowStatusBar_)
	{
		[[UIApplication sharedApplication] setStatusBarHidden:NO];
		shouldShowStatusBar_ = NO;
	}
}


- (void)videoActionFromAppResume
{
	if ((!isAlertShowing_) && (mainVideoLayer_.isVideoPlaying) && (!mainVideoLayer_.isFinishedWatching))
	{
		[mainVideoLayer_ pauseVideo];
		// If this video is being played for the first time, show the cancel alert, that currency will not be awarded.
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Cancel Video?" 
																		message:@"Currency will not be awarded, are you sure you want to cancel the video?"
																	  delegate:self 
														  cancelButtonTitle:@"End" 
														  otherButtonTitles:@"Resume", nil];
		
		// Force orientation to landscape for alert view here.
		if (([UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationPortrait) ||
			 ([UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationPortraitUpsideDown))
		{
			if ([TJCVideoManager sharedTJCVideoManager].currentOrientation == UIInterfaceOrientationLandscapeLeft)
			{
				[[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft animated:NO];
			}
			else if ([TJCVideoManager sharedTJCVideoManager].currentOrientation == UIInterfaceOrientationLandscapeRight)
			{
				[[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
			}
		}
		
		[alert show];
		[alert release];
		
		isAlertShowing_ = YES;
	}
	else if ((!isAlertShowing_) && (mainVideoLayer_.isVideoPlaying))
	{
		[mainVideoLayer_ resumeVideo];
	}
}


- (IBAction)cancelVideo:(id)sender
{
	if (mainVideoLayer_.isFinishedWatching)
	{
		[mainVideoLayer_ cancelVideo];
	}
	else if ((!isAlertShowing_) && (mainVideoLayer_.isVideoPlaying))
	{
		[mainVideoLayer_ pauseVideo];
		// If this video is being played for the first time, show the cancel alert, that currency will not be awarded.
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Cancel Video?" 
																		message:@"Currency will not be awarded, are you sure you want to cancel the video?"
																	  delegate:self 
														  cancelButtonTitle:@"End" 
														  otherButtonTitles:@"Resume", nil];
		
		// Force orientation to landscape for alert view here.
		if (([UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationPortrait) ||
			 ([UIApplication sharedApplication].statusBarOrientation == UIInterfaceOrientationPortraitUpsideDown))
		{
			if ([TJCVideoManager sharedTJCVideoManager].currentOrientation == UIInterfaceOrientationLandscapeLeft)
			{
				[[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft animated:NO];
			}
			else if ([TJCVideoManager sharedTJCVideoManager].currentOrientation == UIInterfaceOrientationLandscapeRight)
			{
				[[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
			}
		}
		
		[alert show];
		[alert release];
		
		isAlertShowing_ = YES;
	}
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	isAlertShowing_ = NO;
	
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
	
	// Manually remove TJCCallsWrapper uiviewcontroller view because we don't want to send a close box notification here.
	[[TJCCallsWrapper sharedTJCCallsWrapper] showBoxCloseNotification:nil];
}


- (void)shouldDisplayLogo:(BOOL)display
{
	[mainVideoLayer_ enableLogo:display];
}


- (void)dealloc
{
	[loadingView_ release];
	[mainVideoLayer_ release];
	[mainView_ release];
	[super dealloc];
}


@end
