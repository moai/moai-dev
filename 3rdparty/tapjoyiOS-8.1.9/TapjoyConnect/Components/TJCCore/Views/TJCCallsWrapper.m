// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCCallsWrapper.h"
#import "TJCLog.h"
#import "TJCViewCommons.h"
#import "TJCFeaturedAppView.h"
#import "SynthesizeSingleton.h"


@implementation TJCCallsWrapper

@synthesize currentInterfaceOrientation;


TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCCallsWrapper)


- (id) init
{
	self = [super init];
	if (self)
	{
		[[NSNotificationCenter defaultCenter] addObserver:sharedTJCCallsWrapper
															  selector:@selector(showBoxCloseNotification:) 
																	name:TJC_VIEW_CLOSED_NOTIFICATION
																 object:nil];
		
		UIWindow *window = [[[UIApplication sharedApplication] windows] objectAtIndex:0];
		
		if(!window || ![window isKindOfClass:[UIWindow class]])
		{
			[TJCLog logWithLevel:LOG_FATAL format:@"There must be a UIWindow in the project"];
			NSAssert(NO,@"No UIWindow Found, an application should have atleast one Window object");
		}
		
		[window addSubview:self.view];
		
		[self.view setAlpha:0];
		
		// Let the background show through during the transition.
		[self.view setBackgroundColor:[UIColor clearColor]];
	}
	return self;
}


- (void)viewDidLoad
{
	[super viewDidLoad];
	
	[self.view setBackgroundColor:[UIColor blackColor]];
}


- (void)showBoxCloseNotification:(NSNotification *)notifierObj
{
	// We never want to get rid of the view controller's view if there are still subviews left.
	if ([[self.view subviews] count] <= 0)
	{
		[self.view setAlpha:0];
	}
}


// Override to allow rotation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	UIInterfaceOrientation currentOrientation = [[UIApplication sharedApplication] statusBarOrientation];
	
	return (interfaceOrientation == currentOrientation);
}


- (void)updateViewsWithOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	[[TJCFeaturedAppView sharedTJCFeaturedAppView] updateViewWithOrientation:interfaceOrientation];
	
#if defined (TJC_VIRTUAL_GOODS_SDK)
	[[TJCVGViewHandler sharedTJCVGViewHandler] updateViewWithOrientation:interfaceOrientation];
#endif
	
#if !defined (TJC_CONNECT_SDK)
	// Videos always need to rotate to current 
	[[TJCVideoManager sharedTJCVideoManager] updateViewWithOrientation:interfaceOrientation];
#endif
}


- (void)moveViewToFront
{
	UIWindow *window = [[[UIApplication sharedApplication] windows] objectAtIndex:0];		
	
	if(!window || ![window isKindOfClass:[UIWindow class]])
	{
		[TJCLog logWithLevel:LOG_FATAL format:@"There must be a UIWindow in the project"];
		NSAssert(NO,@"No UIWindow Found, an application should have atleast one Window object");
	}
	
	[window bringSubviewToFront:self.view];
}


- (void) dealloc 
{
	[super dealloc];
}

@end