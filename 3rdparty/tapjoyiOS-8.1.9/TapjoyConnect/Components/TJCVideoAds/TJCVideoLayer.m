// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCVideoLayer.h"
#import "TJCVideoView.h"
#import "TJCUtil.h"
#import "TJCVideoObject.h"
#import "TJCVideoManager.h"
#import "TJCVideoRequestHandler.h"


@implementation TJCVideoLayer

@synthesize closeButton = closeButton_,
videoFeed = videoFeed_,
isVideoPlaying = isVideoPlaying_,
fileName = fileName_,
linkURLString = linkURLString_,
offerID = offerID_,
isFinishedWatching = isFinishedWatching_,
shouldDisplayLogo = shouldDisplayLogo_;


- (void)setVideoView:(UIView*)view
{
	[self setFrame:view.bounds];
	[videoFeed_.view setFrame:view.bounds];
	[webView_ setFrame:CGRectMake(0, navBar_.frame.size.height, view.bounds.size.width, view.bounds.size.height - navBar_.frame.size.height)];
	[webView_ setBackgroundColor:[UIColor scrollViewTexturedBackgroundColor]];
	[videoCompleteView_ setFrame:view.bounds];
	
	[self addSubview:videoFeed_.view];
	[self addSubview:webView_];
	[self addSubview:videoCompleteView_];
	
	[self addSubview:statusLabel_];
	[self bringSubviewToFront:statusLabel_];
	[self addSubview:tapjoyLogo_];
	[self bringSubviewToFront:tapjoyLogo_];
	[self addSubview:closeButton_];
	[self bringSubviewToFront:closeButton_];
	
	// Apply some UI customization.
	float cornerRadiusSize = TJC_VIDEO_CORNER_RADIUS;
	float borderWidth = TJC_VIDEO_BORDER_WIDTH;
	if ([TJCUtil isPad])
	{
		cornerRadiusSize = TJC_VIDEO_CORNER_RADIUS_IPAD;
		borderWidth = TJC_VIDEO_BORDER_WIDTH_IPAD;
	}
	
	[messageView_.layer setCornerRadius:cornerRadiusSize];
	[messageView_.layer setMasksToBounds:YES];
	[messageView_.layer setBorderWidth:borderWidth];
	[messageView_.layer setBorderColor:[TJC_HEXCOLOR(TJC_VIDEO_BUTTON_STROKE_COLOR) CGColor]];
	
	[replayButton_.layer setCornerRadius:cornerRadiusSize];
	[replayButton_.layer setMasksToBounds:YES];
	[replayButton_.layer setBorderWidth:borderWidth];
	[replayButton_.layer setBorderColor:[TJC_HEXCOLOR(TJC_VIDEO_BUTTON_STROKE_COLOR) CGColor]];
	
	[backButton_.layer setCornerRadius:cornerRadiusSize];
	[backButton_.layer setMasksToBounds:YES];
	[backButton_.layer setBorderWidth:borderWidth];
	[backButton_.layer setBorderColor:[TJC_HEXCOLOR(TJC_VIDEO_BUTTON_STROKE_COLOR) CGColor]];
	
	// Buttons are initially hidden.
	[customButton1_.layer setCornerRadius:cornerRadiusSize];
	[customButton1_.layer setMasksToBounds:YES];
	[customButton1_.layer setBorderWidth:borderWidth];
	[customButton1_.layer setBorderColor:[TJC_HEXCOLOR(TJC_VIDEO_BUTTON_STROKE_COLOR) CGColor]];
	[customButton1_ setAlpha:0];
	
	[customButton2_.layer setCornerRadius:cornerRadiusSize];
	[customButton2_.layer setMasksToBounds:YES];
	[customButton2_.layer setBorderWidth:borderWidth];
	[customButton2_.layer setBorderColor:[TJC_HEXCOLOR(TJC_VIDEO_BUTTON_STROKE_COLOR) CGColor]];
	[customButton2_ setAlpha:0];
	
	if (buttons_)
	{
		[buttons_ release];
	}
	buttons_ = [[NSArray alloc] initWithObjects:customButton1_, customButton2_, nil];
	
	
	if (buttonURLs_)
	{
		[buttonURLs_ release];
	}
	buttonURLs_ = [[NSMutableArray alloc] init];
	
	// Initially, the web view will always be not visible.
	[webView_ setAlpha:0];
	// The video complete view will also not be visible.
	[videoCompleteView_ setAlpha:0];
}


- (void)refreshViewWithBounds:(CGRect)bounds
{
	[self setFrame:bounds];
	[videoFeed_.view setFrame:bounds];
	[webView_ setFrame:CGRectMake(0, navBar_.frame.size.height, bounds.size.width, bounds.size.height - navBar_.frame.size.height)];
	[webView_ setBackgroundColor:[UIColor scrollViewTexturedBackgroundColor]];
	[videoCompleteView_ setFrame:bounds];
}


- (void)updateViewPropertiesWithOfferID:(NSString*)offerID
{
	NSDictionary* videoObjDict = [[[TJCVideoManager sharedTJCVideoManager] getAllVideosDictionary] objectForKey:offerID];
	
	// Get button data and set button properties.
	NSArray *buttonsData = [videoObjDict objectForKey:TJC_VIDEO_OBJ_BUTTONS];
	
	// Kinda gross handling for custom buttons.
	if ([buttonsData count] > 0)
	{
		//for (NSDictionary *buttonDict in buttons)
		for (int i = 0; i < [buttonsData count]; ++i)
		{
			NSDictionary *buttonDict = [buttonsData objectAtIndex:i];
			NSString *buttonLabel = [buttonDict objectForKey:TJC_VIDEO_BUTTON_LABEL_KEY];
			NSString *buttonURL = [buttonDict objectForKey:TJC_VIDEO_BUTTON_URL_KEY];
			
			UIButton *tmpButton = [buttons_ objectAtIndex:i];
			
			[tmpButton setTitle:buttonLabel forState:UIControlStateNormal];
			[tmpButton setTitle:buttonLabel forState:UIControlStateHighlighted];
			[tmpButton setTitle:buttonLabel forState:UIControlStateSelected];
			[tmpButton setAlpha:1];
			
			[buttonURLs_ addObject:buttonURL];
		}
	}
	
	NSString *currencyAmt = [videoObjDict objectForKey:TJC_VIDEO_OBJ_CURRENCY_AMOUNT];
	NSString *currencyName = [videoObjDict objectForKey:TJC_VIDEO_OBJ_CURRENCY_NAME];
	NSString *adText = [videoObjDict objectForKey:TJC_VIDEO_OBJ_NAME];
	
	NSString *currencyText = [self getDescriptionWithAmountString:currencyAmt currencyName:currencyName adText:adText];
	
	[currencyLabelWebView_ loadHTMLString:currencyText baseURL:nil]; 
}


- (void)buttonAction1
{
	[self loadWebViewWithURL:[buttonURLs_ objectAtIndex:0]];
	// JC: NOTE: Web pages are loaded externally now.
	//	[self transitionToWebView];
}


- (void)buttonAction2
{
	[self loadWebViewWithURL:[buttonURLs_ objectAtIndex:1]];
	// JC: NOTE: Web pages are loaded externally now.
	//	[self transitionToWebView];	
}


- (NSString*)getDescriptionWithAmountString:(NSString*)amountString currencyName:(NSString*)currencyName adText:(NSString*)adText
{
	NSString *currencyText = nil;
	if ([TJCUtil isPad])
	{
		
		currencyText = [NSString stringWithFormat:@"<html><body><font face=%@ color=%@ size=%@><p>You just earned <font color=%@><b>%@</b></font> %@! <br /> <font color=%@ size=%@><b>%@</b></font></p></font></body></html>", TJC_VIDEO_FONT_TYPE, TJC_VIDEO_FONT_COLOR, TJC_VIDEO_FONT_SIZE_IPAD, TJC_VIDEO_CURRENCY_TEXT_COLOR, amountString, currencyName, TJC_VIDEO_AD_TEXT_FONT_COLOR, TJC_VIDEO_AD_TEXT_FONT_SIZE_IPAD, adText];		
	}
	else
	{
		currencyText = [NSString stringWithFormat:@"<html><body><font face=%@ color=%@ size=%@><p>You just earned <font color=%@><b>%@</b></font> %@! <br /> <font color=%@ size=%@><b>%@</b></font></p></font></body></html>", TJC_VIDEO_FONT_TYPE, TJC_VIDEO_FONT_COLOR, TJC_VIDEO_FONT_SIZE, TJC_VIDEO_CURRENCY_TEXT_COLOR, amountString, currencyName, TJC_VIDEO_AD_TEXT_FONT_COLOR, TJC_VIDEO_AD_TEXT_FONT_SIZE, adText];
	}
	
	return currencyText;
}


- (IBAction)transitionToWebView
{
	[webView_ setAlpha:1];
	[self bringSubviewToFront:webView_];
	
	
	[doneButton_ setAlpha:0];
	[self bringSubviewToFront:backToVideoButton_];
	[backToVideoButton_ setAlpha:1];
	
	// Enable the back to video button.
	[self enableShowBackToVideoButton:YES];
}


- (IBAction)transitionToVideoView
{
	[webView_ setAlpha:0];
	[self sendSubviewToBack:webView_];
	
	[doneButton_ setAlpha:1];
	[self sendSubviewToBack:backToVideoButton_];
	[backToVideoButton_ setAlpha:0];
	
	// Disable the back to video button.
	[self enableShowBackToVideoButton:NO];
}


- (void)prepareVideoWithDelegate:(id<TJCVideoAdDelegate>)delegate videoURL:(NSString*)URLString shouldStream:(BOOL)shouldStream
{
	delegate_ = delegate;
	
	isVideoPlaying_ = YES;
	
	
	NSURL *contentURL = nil;
	if (shouldStream)
	{
		// Load from a link, for streaming.
		contentURL = [NSURL URLWithString:URLString];
	}
	else
	{
		// Load locally from the device.
		contentURL = [NSURL fileURLWithPath:URLString];
	}
	
	// Init the video
	if (videoFeed_)
	{
		[videoFeed_ release];
	}
	videoFeed_ = [[MPMoviePlayerController alloc] initWithContentURL:contentURL];
	[videoFeed_ setControlStyle:MPMovieControlStyleNone];
	[videoFeed_ setShouldAutoplay:NO];
	// Disables any type of unwanted touch events.
	[videoFeed_.view setUserInteractionEnabled:NO];
	
	// Init the web view.
	if (webView_)
	{
		[webView_ release];
	}
	webView_ = [[UIWebView alloc] init];
	
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_3_2
	[[NSNotificationCenter defaultCenter] removeObserver:self name:MPMovieDurationAvailableNotification object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(movieDurationAvailable:)
																name:MPMovieDurationAvailableNotification
															 object:nil];
#endif
	
	[[NSNotificationCenter defaultCenter] removeObserver:self name:MPMoviePlayerPlaybackDidFinishNotification object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self 
														  selector:@selector(movieDidFinishWatching:)
																name:MPMoviePlayerPlaybackDidFinishNotification
															 object:nil];
	
	didIconDownload_ = NO;
	didLogoDownload_ = NO;
}


- (void)enableShowBackToVideoButton:(BOOL)enable
{
	if (enable)
	{
		[backToVideoButton_ setEnabled:YES];
		[backToVideoButton_ setAlpha:0];
		
		[UIView beginAnimations:nil context:nil];
		[UIView setAnimationDuration:TJC_VIDEO_STATUS_TEXT_FADE_DURATION];
		[backToVideoButton_ setAlpha:1];
		[UIView commitAnimations];
	}
	else
	{
		[backToVideoButton_ setEnabled:NO];
		[backToVideoButton_ setAlpha:0];
	}
}


- (void)enableShowVideoCompleteScreen:(BOOL)enable
{
	if (enable)
	{
		// Fade in view only if not visible.
		if (videoCompleteView_.alpha < 1)
		{
			[videoCompleteView_ setAlpha:0];
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:TJC_VIDEO_STATUS_TEXT_FADE_DURATION];
			[videoCompleteView_ setAlpha:1];
		}
	}
	else
	{
		// Fade out view only if visible.
		if (videoCompleteView_.alpha > 0)
		{
			[videoCompleteView_ setAlpha:1];
			[UIView beginAnimations:nil context:nil];
			[UIView setAnimationDuration:TJC_VIDEO_STATUS_TEXT_FADE_DURATION];
			[videoCompleteView_ setAlpha:0];
		}
	}
	
	[UIView commitAnimations];
}


- (void)enableShowStatusLabel:(BOOL)enable
{
	if (enable)
	{
		[statusLabel_ setAlpha:0];
		[tapjoyLogo_ setAlpha:0];
		[closeButton_ setAlpha:0];
		[UIView beginAnimations:nil context:nil];
		[UIView setAnimationDuration:TJC_VIDEO_STATUS_TEXT_FADE_DURATION];
		[statusLabel_ setAlpha:1];
		[closeButton_ setAlpha:1];
		if (shouldDisplayLogo_)
		{
			[tapjoyLogo_ setAlpha:1];
		}
		else
		{
			[tapjoyLogo_ setAlpha:0];
		}
	}
	else
	{
		// Get rid of status text.
		[statusLabel_ setAlpha:1];
		[tapjoyLogo_ setAlpha:1];
		[closeButton_ setAlpha:1];
		[UIView beginAnimations:nil context:nil];
		[UIView setAnimationDuration:TJC_VIDEO_STATUS_TEXT_FADE_DURATION];
		[statusLabel_ setAlpha:0];
		[tapjoyLogo_ setAlpha:0];
		[closeButton_ setAlpha:0];
	}
	
	[UIView commitAnimations];
}


- (void)movieDurationAvailable:(id)sender
{
	duration_ = [videoFeed_ duration];
	
	//NSLog(@"Movie Duration:%f", duration_);
	
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_3_2
	NSString *statusStr = [NSString stringWithFormat:@"%d seconds left", (int)duration_];
#else
	NSString *statusStr = [NSString stringWithFormat:@"Application will resume shortly", (int)duration_];
#endif
	
	[statusLabel_ setText:statusStr];
}


- (void)movieDidFinishWatching:(NSNotification*)notification
{
	//MPMoviePlayerController *playerController = [notification object];
	//NSNumber *finishReason = [[notification userInfo] objectForKey:@"MPMoviePlayerPlaybackDidFinishReasonUserInfoKey"];	
	//NSLog(@"VideoFinishReason: %@", finishReason);
	
	// Show video complete screen.
	[self enableShowVideoCompleteScreen:YES];
	
	// Disable timer status text.
	[self enableShowStatusLabel:NO];
	
	isVideoPlaying_ = NO;
	
	// Ping servers to notify that video has been completed, currency should be awarded.
	if (offerID_)
	{
		[[[TJCVideoManager sharedTJCVideoManager] requestHandler] requestVideoCompleteWithOfferID:offerID_];
		[self updateViewPropertiesWithOfferID:offerID_];
		offerID_ = nil;
	}
	
	// Set video complete flag for special behavior when cancelling a video.
	isFinishedWatching_ = YES;
}


- (void)loadWebViewWithURL:(NSString*)URLString
{
	// Open link in mobile safari.
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:URLString]];
}


- (void)playVideo
{
	[self enableShowVideoCompleteScreen:NO];
	
	[self enableShowBackToVideoButton:NO];
	
	// Stop and play from the beginning.
	//[videoFeed_ stop];
	[videoFeed_ play];
	
	// Fade status label in.
	[self enableShowStatusLabel:YES];
	
	// Start the tick to update the status text of the time remaining on the video.
	[NSTimer scheduledTimerWithTimeInterval:0.25f 
												target:self 
											 selector:@selector(updateStatusText:)
											 userInfo:nil 
											  repeats:YES];
	
	isVideoPlaying_ = YES;
	
	// Begin download of video icon and tapjoy logo.
	if (offerID_)
	{
		if (!didIconDownload_)
		{
			NSOperationQueue *queue = [[[NSOperationQueue alloc] init] autorelease];
			NSInvocationOperation *operation = [[NSInvocationOperation alloc] initWithTarget:self 
																											selector:@selector(loadIconImage)
																											  object:nil];
			[queue addOperation:operation];
			[operation release];
		}
	}
	
	if (!didLogoDownload_)
	{
		NSOperationQueue *queue = [[[NSOperationQueue alloc] init] autorelease];
		NSInvocationOperation *operation = [[NSInvocationOperation alloc] initWithTarget:self 
																										selector:@selector(loadLogoImage)
																										  object:nil];
		[queue addOperation:operation];
		[operation release];
	}
}


- (void)loadIconImage
{
	NSDictionary* videoObjDict = [[[TJCVideoManager sharedTJCVideoManager] getCachedVideoDictonary] objectForKey:offerID_];
	NSString *iconURLString = [videoObjDict objectForKey:TJC_VIDEO_OBJ_ICON_URL];
	
	// Safety check to see if there is any icon file to download.
	if ([iconURLString isEqualToString:TJC_VIDEO_NO_ICON] || [iconURLString isEqualToString:@""])
	{
		[iconImageView_ setImage:[UIImage imageNamed:@"tapjoy_icon.png"]];
	}
	else
	{
		NSURL *iconURL = [NSURL URLWithString:[videoObjDict objectForKey:TJC_VIDEO_OBJ_ICON_URL]];
		NSData *iconData = [[NSData alloc] initWithContentsOfURL:iconURL];
		UIImage *iconImg = [[[UIImage alloc] initWithData:iconData] autorelease];
		[iconData release];
		
		[self performSelectorOnMainThread:@selector(displayIcon:) withObject:iconImg waitUntilDone:NO];
	}
}


- (void)loadLogoImage
{
	// Now download Tapjoy logo, located in the lower right hand corner of the video.
	NSString *cachesDirectory = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) objectAtIndex:0];
	// Set video file path.
	NSString *videoPath = [cachesDirectory stringByAppendingFormat:@"/VideoAds/watermark.png"];
	if (![[NSFileManager defaultManager] fileExistsAtPath:videoPath])
	{
		NSURL *logoURL = [NSURL URLWithString:TJC_VIDEO_LOGO_IMAGE_URL];
		NSData *logoData = [[NSData alloc] initWithContentsOfURL:logoURL];
		UIImage *logoImg = [[[UIImage alloc] initWithData:logoData] autorelease];
		[logoData release];
		
		[self performSelectorOnMainThread:@selector(displayLogo:) withObject:logoImg waitUntilDone:NO];
	}
}


- (void)displayIcon:(UIImage*)iconImage
{
	didIconDownload_ = YES;
	[iconImageView_ setImage:iconImage];
}


- (void)displayLogo:(UIImage*)logoImage
{
	didLogoDownload_ = YES;
	[tapjoyLogo_ setAlpha:0];
	
	[tapjoyLogo_ setImage:logoImage];
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:TJC_VIDEO_STATUS_TEXT_FADE_DURATION];
	if (shouldDisplayLogo_)
	{
		[tapjoyLogo_ setAlpha:1];
	}
	else
	{
		[tapjoyLogo_ setAlpha:0];
	}
	[UIView commitAnimations];
}


- (void)enableLogo:(BOOL)enable
{
	shouldDisplayLogo_ = enable;
	if (enable)
	{
		[tapjoyLogo_ setAlpha:1];
	}
	else
	{
		[tapjoyLogo_ setAlpha:0];
	}
}


- (void)stopVideo
{
	isVideoPlaying_ = NO;
	
	[[NSNotificationCenter defaultCenter] removeObserver:self name:MPMovieDurationAvailableNotification object:nil];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:MPMoviePlayerPlaybackDidFinishNotification object:nil];
	[videoFeed_ stop];
}


- (void)pauseVideo
{
	[videoFeed_ pause];
}


- (void)resumeVideo
{
	[videoFeed_ play];
}


- (void)cancelVideo
{
	[self stopVideo];
	
	// Show video complete screen.
	[self enableShowVideoCompleteScreen:YES];
	
	// Disable timer status text.
	[self enableShowStatusLabel:NO];
	
	isVideoPlaying_ = NO;
}


- (void)cleanupVideo
{	
	isVideoPlaying_ = NO;
	
	if (videoFeed_)
	{
		[videoFeed_ release];
		videoFeed_ = nil;
	}
	if (webView_)
	{
		[webView_ release];
		webView_ = nil;
	}
	
	[[NSNotificationCenter defaultCenter] removeObserver:self name:MPMovieDurationAvailableNotification object:nil];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:MPMoviePlayerPlaybackDidFinishNotification object:nil];
}


- (void)updateStatusText:(NSTimer*)timer
{
	// Stop the tick when we reach the end of the video.
	if ((duration_ - videoFeed_.currentPlaybackTime) < 0)
	{
		[timer invalidate];
	}
	
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_3_2
	NSString *statusStr = [NSString stringWithFormat:@"%d seconds left", (int)(duration_ - videoFeed_.currentPlaybackTime)];
#else
	NSString *statusStr = @"Application will resume shortly";
#endif	

	[statusLabel_ setText:statusStr];
}


- (void)dealloc
{
	[offerID_ release];
	[buttons_ release];
	[buttonURLs_ release];
	[webView_ release];
	[videoFeed_ release];
	[super dealloc];
}

@end
