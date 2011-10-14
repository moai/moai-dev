//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCCallsWrapper.h"
#import "TJCLog.h"
#import "TJCViewCommons.h"
#import "TJCFeaturedAppView.h"


static TJCCallsWrapper *_sharedTJCCallsWrapperInstance;

@implementation TJCCallsWrapper

+ (TJCCallsWrapper*) sharedTJCCallsWrapper;
{
	if (!_sharedTJCCallsWrapperInstance) 
	{
		_sharedTJCCallsWrapperInstance = [[TJCCallsWrapper alloc] init];
	}
	return _sharedTJCCallsWrapperInstance;
}


- (id) init
{
	if ((self = [super init]))
	{
		@try
		{
			UIWindow *window = [[[UIApplication sharedApplication] windows]objectAtIndex:0];
			
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
		@catch (NSException *exp) 
		{
			[TJCLog logWithLevel:LOG_FATAL format:@"There must be a UIWindow in the project"];
		}
		
		
	}
	return self;
}


- (void) viewDidLoad
{
	[super viewDidLoad];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
														  selector:@selector(showBoxCloseNotification:) 
																name:TJC_VIEW_CLOSED_NOTIFICATION
															 object:nil];
	
	[self.view setBackgroundColor:[UIColor blackColor]];
}


- (void) showBoxCloseNotification:(NSNotification *)notifierObj
{
	// We never want to get rid of the view controller's view if there are still subviews left.
	if ([[self.view subviews] count] <= 0)
	{
		[self.view setAlpha:0];
	}
}


// Override to allow rotation.
- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	UIInterfaceOrientation currentOrientation = [[UIApplication sharedApplication] statusBarOrientation];
	
	return (interfaceOrientation == currentOrientation);
}


- (void) didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	[self updateViewsWithOrientation:fromInterfaceOrientation];
}


- (void) updateViewsWithOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	[[TJCFeaturedAppView sharedTJCFeaturedAppView] didRotateFromInterfaceOrientation:interfaceOrientation];
	
#if defined (TJC_VIRTUAL_GOODS_SDK)
	[[TJCVGViewHandler sharedTJCVGViewHandler] didRotateFromInterfaceOrientation:interfaceOrientation];
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
	[[NSNotificationCenter defaultCenter]removeObserver:TJC_VIEW_CLOSED_NOTIFICATION];
	[super dealloc];
}

@end