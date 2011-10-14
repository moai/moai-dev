//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVideoViewHandler.h"
#import "TJCVideoView.h"
#import "TJCVideoManager.h"
#import "TJCViewCommons.h"



@implementation TJCVideoViewHandler


+ (UIView*)showVideoAdWithFrame:(CGRect)frame offerID:(NSString*)offerID
{
	//NSAssert([TJCVideoManager sharedTJCVideoManager].videoView.isVideoReady, @"Video not ready to be viewed yet");
	
	[[TJCVideoManager sharedTJCVideoManager].videoView.mainView removeFromSuperview];
	[[TJCVideoManager sharedTJCVideoManager].videoView refreshViewWithFrame:frame];
	
	[[TJCVideoManager sharedTJCVideoManager].videoView playVideoWithOfferID:offerID];

	[TJCViewCommons animateTJCView:[TJCVideoManager sharedTJCVideoManager].videoView.mainView
					 withTJCTransition:[[TJCViewCommons sharedObject] getCurrentTransitionEffect] 
								withDelay:[[TJCViewCommons sharedObject] getTransitionDelay]];
	
	return [TJCVideoManager sharedTJCVideoManager].videoView.mainView;
}


+ (UIView*)showVideoAdWithOfferID:(NSString*)offerID
{
	return [self showVideoAdWithFrame:[[UIScreen mainScreen] bounds] offerID:offerID];
}


+ (void)showVideoAdWithViewController:(UIViewController*)vController offerID:(NSString*)offerID
{
	if(!vController || ![vController isKindOfClass:[UIViewController class]])
	{
		NSAssert(NO,@"View Controller provided must not be nil or some other object");
	}
	
	//NSAssert([TJCVideoManager sharedTJCVideoManager].videoView.isVideoReady, @"Video not ready to be viewed yet");
	
	// Remove any offers view that might possibly exist.
	[[TJCVideoManager sharedTJCVideoManager].videoView.mainView removeFromSuperview];
	[[TJCVideoManager sharedTJCVideoManager].videoView refreshViewWithFrame:vController.view.bounds];
	
	[[TJCVideoManager sharedTJCVideoManager].videoView playVideoWithOfferID:offerID];
	
	[vController.view addSubview:[TJCVideoManager sharedTJCVideoManager].videoView.mainView];
	
	[TJCViewCommons animateTJCView:[TJCVideoManager sharedTJCVideoManager].videoView.mainView
					 withTJCTransition:[[TJCViewCommons sharedObject] getCurrentTransitionEffect] 
								withDelay:[[TJCViewCommons sharedObject] getTransitionDelay]];
}


@end


@implementation TapjoyConnect (TJCVideoViewHandler)

+ (void)initVideoAdWithDelegate:(id<TJCVideoAdDelegate>)delegate
{
	[[TJCVideoManager sharedTJCVideoManager] initVideoAdsWithDelegate:delegate];
}


+ (UIView*)showVideoAdWithFrame:(CGRect)frame offerID:(NSString *)offerID
{
	return [[TJCCallsWrapper sharedTJCCallsWrapper] showVideoAdWithFrame:frame offerID:offerID];
}


+ (UIView*)showVideoAdWithOfferID:(NSString*)offerID
{
	return [[TJCCallsWrapper sharedTJCCallsWrapper] showVideoAdWithOfferID:offerID];
}


+ (void)showVideoAdWithViewController:(UIViewController*)vController offerID:(NSString *)offerID
{
	[[TJCCallsWrapper sharedTJCCallsWrapper] showVideoAdWithViewController:vController offerID:offerID];
}


+ (void)setVideoCacheCount:(int)count
{
	[[TJCVideoManager sharedTJCVideoManager] setVideoCacheCount:count];
}

@end



@implementation TJCCallsWrapper (TJCVideoViewHandler)


- (UIView*)showVideoAdWithFrame:(CGRect)frame offerID:(NSString*)offerID
{
	UIView *videoAdView = nil;
	
	[self moveViewToFront];
	
	[self.view setAlpha:1];
	
	videoAdView = [TJCVideoViewHandler showVideoAdWithFrame:frame offerID:offerID];
	
	[self.view addSubview:videoAdView];
	
	return videoAdView;
}


- (UIView*)showVideoAdWithOfferID:(NSString*)offerID
{
	return [self showVideoAdWithFrame:self.view.bounds offerID:offerID];
}


- (void)showVideoAdWithViewController:(UIViewController*)vController offerID:(NSString*)offerID
{
	[TJCVideoViewHandler showVideoAdWithViewController:vController offerID:offerID];
}


@end