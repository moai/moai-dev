// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCOffersViewHandler.h"
#import "TJCOffersWebView.h"
#import "TJCViewCommons.h"
#import "TJCConstants.h"
#import "TJCUINavigationBarView.h"

@implementation TJCOffersViewHandler


+ (UIView*)showOffersWithFrame:(CGRect)frame
{
	// Remove any offers view that might possibly exist.
	[[TJCOffersWebView sharedTJCOffersWebView] removeFromSuperview];
	 
	[TJCOffersWebView sharedTJCOffersWebView].publisherUserID_ = [TapjoyConnect getUserID];
	// For showOffers methods that do not use a parent view controller, we depend on this to be nil for certain initialization purposes.
	[TJCOffersWebView sharedTJCOffersWebView].parentVController_ = nil;

	[[TJCOffersWebView sharedTJCOffersWebView] refreshWithFrame:frame enableNavBar:YES];
	[[TJCOffersWebView sharedTJCOffersWebView] loadView];
	
	[TJCViewCommons animateTJCView:[TJCOffersWebView sharedTJCOffersWebView] 
					 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons] getCurrentTransitionEffect] 
								withDelay:[[TJCViewCommons sharedTJCViewCommons] getTransitionDelay]];
	
	return [TJCOffersWebView sharedTJCOffersWebView];
}


+ (UIView*)showOffers
{
	return [self showOffersWithFrame:[[UIScreen mainScreen] bounds]];
}


+ (void)showOffersWithViewController:(UIViewController*)vController
{
	[self showOffersWithViewController:vController withInternalNavBar:YES];
}


+ (void)showOffersWithViewController:(UIViewController*)vController withInternalNavBar:(BOOL)visible
{
	if(!vController || ![vController isKindOfClass:[UIViewController class]])
	{
		NSAssert(NO,@"View Controller provided must not be nil or some other object");
	}
	
	// Remove any offers view that might possibly exist.
	[[TJCOffersWebView sharedTJCOffersWebView] removeFromSuperview];
	
	[TJCOffersWebView sharedTJCOffersWebView].publisherUserID_ = [TapjoyConnect getUserID];
	[TJCOffersWebView sharedTJCOffersWebView].parentVController_ = vController;

	[[TJCOffersWebView sharedTJCOffersWebView] refreshWithFrame:vController.view.bounds enableNavBar:visible];
	[[TJCOffersWebView sharedTJCOffersWebView] loadView];
	
	[vController.view addSubview:[TJCOffersWebView sharedTJCOffersWebView]];

	[TJCViewCommons animateTJCView:[TJCOffersWebView sharedTJCOffersWebView]  
					 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons] getCurrentTransitionEffect] 
								withDelay:[[TJCViewCommons sharedTJCViewCommons] getTransitionDelay]];
	
}


+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withFrame:(CGRect)frame withCurrencySelector:(BOOL)isSelectorVisible
{
	// Remove any offers view that might possibly exist.
	[[TJCOffersWebView sharedTJCOffersWebView] removeFromSuperview];

	[TJCOffersWebView sharedTJCOffersWebView].currencyID_ = [NSString stringWithFormat:@"%@=%@", TJC_URL_PARAM_CURRENCY_ID, currencyID];
	[TJCOffersWebView sharedTJCOffersWebView].isSelectorVisible_ = [NSString stringWithFormat:@"%@=%d", TJC_URL_PARAM_SELECTOR_ID, isSelectorVisible];
	[TJCOffersWebView sharedTJCOffersWebView].publisherUserID_ = [TapjoyConnect getUserID];
	// For showOffers methods that do not use a parent view controller, we depend on this to be nil for certain initialization purposes.
	[TJCOffersWebView sharedTJCOffersWebView].parentVController_ = nil;

	[[TJCOffersWebView sharedTJCOffersWebView] refreshWithFrame:frame enableNavBar:YES];
	[[TJCOffersWebView sharedTJCOffersWebView] loadView];
	
	[TJCViewCommons animateTJCView:[TJCOffersWebView sharedTJCOffersWebView] 
					 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons] getCurrentTransitionEffect] 
								withDelay:[[TJCViewCommons sharedTJCViewCommons] getTransitionDelay]];
	
	return [TJCOffersWebView sharedTJCOffersWebView];
}


+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withCurrencySelector:(BOOL)isSelectorVisible
{
	return [self showOffersWithCurrencyID:currencyID
										 withFrame:[[UIScreen mainScreen] bounds]
						  withCurrencySelector:isSelectorVisible];
}


+ (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withCurrencySelector:(BOOL)isSelectorVisible
{
	[self showOffersWithCurrencyID:currencyID
					withViewController:vController 
					withInternalNavBar:YES
				 withCurrencySelector:isSelectorVisible];
}


+ (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withInternalNavBar:(BOOL)visible withCurrencySelector:(BOOL)isSelectorVisible
{
	if(!vController || ![vController isKindOfClass:[UIViewController class]])
	{
		NSAssert(NO,@"View Controller provided must not be nil or some other object");
	}
	
	// Remove any offers view that might possibly exist.
	[[TJCOffersWebView sharedTJCOffersWebView] removeFromSuperview];

	[TJCOffersWebView sharedTJCOffersWebView].publisherUserID_ = [TapjoyConnect getUserID];
	[TJCOffersWebView sharedTJCOffersWebView].parentVController_ = vController;
	[TJCOffersWebView sharedTJCOffersWebView].currencyID_ = [NSString stringWithFormat:@"%@=%@", TJC_URL_PARAM_CURRENCY_ID, currencyID];
	[TJCOffersWebView sharedTJCOffersWebView].isSelectorVisible_ = [NSString stringWithFormat:@"%@=%d", TJC_URL_PARAM_SELECTOR_ID, isSelectorVisible];
	//[[TJCOffersWebView sharedTJCOffersWebView] initWithFrame:vController.view.bounds enableNavBar:visible];
	[[TJCOffersWebView sharedTJCOffersWebView] refreshWithFrame:vController.view.bounds enableNavBar:visible];
	[[TJCOffersWebView sharedTJCOffersWebView] loadView];
	
	[vController.view addSubview:[TJCOffersWebView sharedTJCOffersWebView]];
	
	[TJCViewCommons animateTJCView:[TJCOffersWebView sharedTJCOffersWebView]  
					 withTJCTransition:[[TJCViewCommons sharedTJCViewCommons] getCurrentTransitionEffect] 
								withDelay:[[TJCViewCommons sharedTJCViewCommons] getTransitionDelay]];
}

@end



@implementation TapjoyConnect (TJCOffersViewHandler)

#pragma mark OFFERS METHODS 

+ (UIView*)showOffersWithFrame:(CGRect)frame
{
	return [[TJCCallsWrapper sharedTJCCallsWrapper] showOffersWithFrame:frame];
}


+ (UIView*)showOffers
{
	return [[TJCCallsWrapper sharedTJCCallsWrapper] showOffers];
}


+ (void)showOffersWithViewController:(UIViewController *)vController
{
	[[TJCCallsWrapper sharedTJCCallsWrapper] showOffersWithViewController:vController];
}


+ (void)showOffersWithViewController:(UIViewController *)vController withInternalNavBar:(BOOL)visible
{
	[[TJCCallsWrapper sharedTJCCallsWrapper] showOffersWithViewController:vController
																		withInternalNavBar:visible];
}


+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withFrame:(CGRect)frame withCurrencySelector:(BOOL)isSelectorVisible
{
	return [[TJCCallsWrapper sharedTJCCallsWrapper] showOffersWithCurrencyID:currencyID
																						withFrame:frame
																		 withCurrencySelector:isSelectorVisible];
}


+ (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withCurrencySelector:(BOOL)isSelectorVisible
{
	return [[TJCCallsWrapper sharedTJCCallsWrapper] showOffersWithCurrencyID:currencyID
																		 withCurrencySelector:isSelectorVisible];
}


+ (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withCurrencySelector:(BOOL)isSelectorVisible
{
	[[TJCCallsWrapper sharedTJCCallsWrapper] showOffersWithCurrencyID:currencyID
																  withViewController:vController 
																withCurrencySelector:isSelectorVisible];
}


+ (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withInternalNavBar:(BOOL)visible withCurrencySelector:(BOOL)isSelectorVisible
{
	[[TJCCallsWrapper sharedTJCCallsWrapper] showOffersWithCurrencyID:currencyID 
																  withViewController:vController
																  withInternalNavBar:visible
																withCurrencySelector:isSelectorVisible];	
}


+ (void)setOffersNavBarImage:(UIImage*)image
{
	[[TJCOffersWebView sharedTJCOffersWebView] setCustomNavBarImage:image];
}


@end



@implementation TJCCallsWrapper (TJCOffersViewHandler)

- (UIView*)showOffersWithFrame:(CGRect)frame
{
	UIView *offersView = nil;
	
	[self moveViewToFront];
	
	[self.view setAlpha:1];
	
	offersView = [TJCOffersViewHandler showOffersWithFrame:frame];
	[self.view addSubview:offersView];
	
	return offersView;
}


- (UIView*)showOffers
{
	return [self showOffersWithFrame:self.view.bounds];
}


- (void)showOffersWithViewController:(UIViewController *)vController;
{
	[self.view setAlpha: 0];
	
	[TJCOffersViewHandler showOffersWithViewController:vController];
}


- (void)showOffersWithViewController:(UIViewController *)vController withInternalNavBar:(BOOL)visible;
{
	[self.view setAlpha: 0];
	
	[TJCOffersViewHandler showOffersWithViewController:vController withInternalNavBar:visible];
}


- (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withFrame:(CGRect)frame withCurrencySelector:(BOOL)isSelectorVisible
{
	UIView *offersView = nil;
	
	[self moveViewToFront];
	
	[self.view setAlpha:1];
	
	offersView = [TJCOffersViewHandler showOffersWithCurrencyID:currencyID
																	  withFrame:frame
														withCurrencySelector:isSelectorVisible];
	[self.view addSubview:offersView];
	
	return offersView;
}


- (UIView*)showOffersWithCurrencyID:(NSString*)currencyID withCurrencySelector:(BOOL)isSelectorVisible
{
	return [self showOffersWithCurrencyID:currencyID
										 withFrame:self.view.bounds
						  withCurrencySelector:isSelectorVisible];
}


- (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withCurrencySelector:(BOOL)isSelectorVisible
{
	[self.view setAlpha: 0];
	
	[TJCOffersViewHandler showOffersWithCurrencyID:currencyID
										 withViewController:vController
									  withCurrencySelector:isSelectorVisible];
}


- (void)showOffersWithCurrencyID:(NSString*)currencyID withViewController:(UIViewController*)vController withInternalNavBar:(BOOL)visible withCurrencySelector:(BOOL)isSelectorVisible
{
	[self.view setAlpha: 0];
	
	[TJCOffersViewHandler showOffersWithCurrencyID:currencyID
										 withViewController:vController
										 withInternalNavBar:visible
									  withCurrencySelector:isSelectorVisible];	
}

@end
