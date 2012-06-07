// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCAdView.h"
#import "TJCAdRequestHandler.h"
#import "SynthesizeSingleton.h"
#import "TapjoyConnect.h"
#import "TJCUtil.h"

@implementation TJCAdView

TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCAdView)

@synthesize clickURL = clickURL_;
@synthesize imageDataStr = imageDataStr_;
@synthesize imageView = imageView_;
@synthesize previousImageView = previousImageView_;
@synthesize contentSizeStr = contentSizeStr_;
@synthesize adViewOverlay = adViewOverlay_;
@synthesize adHandlerObj = adHandlerObj_;
@synthesize adDelegate = adDelegate_;

- (id)init
{
	if ((self = [super init]))
	{
		if (!adHandlerObj_)
		{
			adHandlerObj_ = [[TJCAdRequestHandler alloc] initRequestWithDelegate:self andRequestTag:0];
		}
		
		if (!adViewOverlay_)
		{
			adViewOverlay_ = [[UIView alloc] init];
			[adViewOverlay_ setAlpha:0.0f];
			[adViewOverlay_ setBackgroundColor:[UIColor blackColor]];
		}
	}
	return self;
}


- (id)getAdWithDelegate:(id<TJCAdDelegate>)deleg
{
	return [self getAdWithDelegate:deleg currencyID:nil];
}


- (id) getAdWithDelegate:(id<TJCAdDelegate>)deleg currencyID:(NSString*)currencyID
{
	// Set the ad delegate. The didReceiveAd method will be called when we get a response from the server.
	adDelegate_ = deleg;
	
	if ([adDelegate_ respondsToSelector:@selector(adContentSize)])
	{
		// Set the view size here, retrieved from the protocol method.
		contentSizeStr_ = [adDelegate_ adContentSize];
	}
	else
	{
		// Default to a certain size.
		contentSizeStr_ = TJC_AD_BANNERSIZE_320X50;
	}
	
	if ([contentSizeStr_ isEqual:TJC_AD_BANNERSIZE_320X50])
	{
		[self setFrame:CGRectMake(0, 0, 320, 50)];
	}
	else if ([contentSizeStr_ isEqual:TJC_AD_BANNERSIZE_640X100])
	{
		[self setFrame:CGRectMake(0, 0, 640, 100)];
	}
	else if ([contentSizeStr_ isEqual:TJC_AD_BANNERSIZE_768X90])
	{
		[self setFrame:CGRectMake(0, 0, 768, 90)];
	}
	else
	{
		// Default to some size, whatever.
		[self setFrame:CGRectMake(0, 0, 320, 50)];
	}
	
	// The ad view overlay should go directly over the ad view.
	[adViewOverlay_ setFrame:sharedTJCAdView.frame];
	
	// Different request handling depending on whether the currency ID is set or not.
	if (currencyID)
	{
		// Save off the currency ID so it can be retrieved for ad refreshes.
		[adHandlerObj_ requestAdWithSize:contentSizeStr_ currencyID:currencyID];
	}
	else
	{
		[adHandlerObj_ requestAdWithSize:contentSizeStr_];
	}
	
	// This is used so that when the image view is made transparent during a touch, it'll look dimmed.
	[self setBackgroundColor:[UIColor blackColor]];
	
	return self;
}


- (void)refreshAd:(NSTimer*)timer
{
	[self refreshAdWithCurrencyID:nil];
}


- (void) refreshAdWithCurrencyID:(NSString*)currencyID
{
	if (!adDelegate_)
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Ad delegate not set, getAdWithDelegate must first be called"];
		return;
	}
	
	if (!adHandlerObj_)
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Ad handler object not set, getAdWithDelegate must first be called"];
		
		adHandlerObj_ = [[TJCAdRequestHandler alloc] initRequestWithDelegate:self andRequestTag:0];
	}
	
	if ([adDelegate_ respondsToSelector:@selector(adContentSize)])
	{
		contentSizeStr_ = [adDelegate_ adContentSize];
	}
	
	if (!contentSizeStr_)
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Content size not set, adContentSize is not properly implemented"];
		return;
	}
	
	if (currencyID)
	{
		[adHandlerObj_ requestAdWithSize:contentSizeStr_ currencyID:currencyID];
	}
	else
	{
		[adHandlerObj_ requestAdWithSize:contentSizeStr_];
	}
}


- (BOOL) isAdLoaded
{
	if (!adHandlerObj_)
	{
		return NO;
	}
	
	return [adHandlerObj_ isDataFetchSuccessful];
}


// called when request succeeeds
- (void) fetchResponseSuccessWithData:(void*)dataObj withRequestTag:(int)aTag
{
	float animDuration = 0.25f;
	
	// We want to fade out the old ad view while simultaneously fade in the new ad view.
	if ([self.subviews count] > 0)
	{
		previousImageView_ = [self.subviews objectAtIndex:0];
		// Previous ad is initially visible.
		[previousImageView_ setAlpha:1.0f];
		
		[UIView setAnimationDelegate:self];
		[UIView setAnimationDidStopSelector:@selector(removeOldAdEnded:finished:context:)];
		
		[UIView beginAnimations:nil context:nil];
		// The fade out animation is a bit slower to make it look better.
		[UIView setAnimationDuration:animDuration + .10];
		
		// Old ad fades out.
		[previousImageView_ setAlpha:0.0f];
		
		[UIView commitAnimations];
	}
	
	NSArray *dataArray = [NSArray arrayWithArray:dataObj];
	
	// Save the click URL.
	[clickURL_ release];
	clickURL_ = [[dataArray objectAtIndex:0] retain];
	
	// Save the image data.
	NSString *imageDataStr = [[dataArray objectAtIndex:1] stringByReplacingOccurrencesOfString:@"\n" withString:@""];
	// JC: TODO: This should be removed when the image data is formatted correctly. For now, strip out the newline chars.
	
	UIImage *image = [[UIImage imageWithData:[TJCUtil dataWithBase64EncodedString:imageDataStr]] retain];
	imageView_ = [[UIImageView alloc] initWithImage: image];
	[image release];
	
	//[imageView_ setFrame:self.frame];
	[imageView_ setBounds:self.bounds];
	[imageView_ setCenter:CGPointMake(imageView_.bounds.size.width * 0.5f, imageView_.bounds.size.height * 0.5f)];
	
	[self addSubview:imageView_];
	[imageView_ addSubview:adViewOverlay_];
	
	// Do a fade-in animation so it looks better.
	// New ad is initially not visible.
	[imageView_ setAlpha:0.0f];
	
	if ([adDelegate_ respondsToSelector:@selector(didReceiveAd:)])
	{
		// Before starting the animation, call the delegate method to notify that an Ad has just been received.
		[adDelegate_ didReceiveAd:self];
	}
	
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:animDuration];
	
	// New ad fades in.
	[imageView_ setAlpha:1.0f];
	[UIView commitAnimations];
	
	if ([adDelegate_ respondsToSelector:@selector(shouldRefreshAd)])
	{
		// Query delegate method to determin whether the ad should be refreshed.
		if ([adDelegate_ shouldRefreshAd])
		{
			// Now start a refresh timer to load the next ad.
			[NSTimer scheduledTimerWithTimeInterval:TJC_AD_REFRESH_DELAY target:self selector:@selector(refreshAd:) userInfo:nil repeats:NO];
		}
	}
}


-(void)removeOldAdEnded:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
	[previousImageView_ removeFromSuperview];
}


- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	// Darken image when it is touched.
	//[[adViewOverlay_ superview] bringSubviewToFront:adViewOverlay_];
	[adViewOverlay_ setAlpha:0.4f];
}


- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches)
	{
		CGPoint location = [touch locationInView:self];
		
		//[[adViewOverlay_ superview] sendSubviewToBack:adViewOverlay_];
		[adViewOverlay_ setAlpha:0.0f];
		
		// Do a touch-rect collision test to make sure that the user has released touch within the ad area.
		if ([self pointInside:location withEvent:event])
		{
			if (clickURL_)
			{
				// Open the URL, which will redirect to the app store.
				NSURL *redirectURL = [NSURL URLWithString:clickURL_];
				[[UIApplication sharedApplication] openURL:redirectURL];
			}
			else
			{
				[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Ad cannot be loaded. Check internet connection."];
			}
			
		}
	}
}

NSString *kTJCAdFailStr = @"Tapjoy Ad request failed to receive any data.";

// raised when error occurs
- (void) fetchResponseError:(TJCResponseError)errorType errorDescription:(id)errorDescObj requestTag:(int)aTag
{
	if ([adDelegate_ respondsToSelector:@selector(didFailWithMessage:)])
	{
		[adDelegate_ didFailWithMessage:kTJCAdFailStr];
	}
}


- (void) dealloc
{
	[adHandlerObj_ release];	
	adHandlerObj_ = nil;
	
	[clickURL_ release];
	[imageDataStr_ release];
	[imageView_ release];
	[previousImageView_ release];
	[adViewOverlay_ release];
	
	[super dealloc];
}

@end



@implementation TapjoyConnect (TJCAdView)

+ (id) getDisplayAdWithDelegate:(id <TJCAdDelegate>)deleg
{
	return [[TJCAdView sharedTJCAdView] getAdWithDelegate:deleg];
}


+ (id) getDisplayAdWithDelegate:(id<TJCAdDelegate>)deleg currencyID:(NSString*)currencyID
{
	return [[TJCAdView sharedTJCAdView] getAdWithDelegate:deleg currencyID:currencyID];
}


+ (void) refreshDisplayAd
{
	[[TJCAdView sharedTJCAdView] refreshAd:nil];
}


+ (void) refreshDisplayAdWithCurrencyID:(NSString*)currencyID
{
	[[TJCAdView sharedTJCAdView] refreshAdWithCurrencyID:currencyID];
}


+ (BOOL) isDisplayAdLoaded
{
	return [[TJCAdView sharedTJCAdView] isAdLoaded];
}


+ (TJCAdView*) getDisplayAdView
{
	return [TJCAdView sharedTJCAdView];
}

@end