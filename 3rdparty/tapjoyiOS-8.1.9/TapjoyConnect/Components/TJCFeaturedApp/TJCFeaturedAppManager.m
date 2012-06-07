// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCFeaturedAppManager.h"
#import "SynthesizeSingleton.h"
#import "TJCFeaturedAppRequestHandler.h"
#import "TJCFeaturedAppDBManager.h"
#import "TJCFeaturedAppModel.h"
#import "TapjoyConnect.h"


@implementation TJCFeaturedAppManager

TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCFeaturedAppManager)

@synthesize featuredAppModelObj = featuredAppModelObj_;
@synthesize featuredAppDisplayCount = featuredAppDisplayCount_;


- (id) init
{
	if ((self = [super init]))
	{
		featuredAppHandlerObj_ = [[TJCFeaturedAppRequestHandler alloc] initRequestWithDelegate:self andRequestTag:0];
		featuredAppDisplayCount_ = TJC_FEATURED_APP_DEFAULT_MAX_DISPLAY_COUNT;
	}
	return self;
}


- (void)getFeaturedApp
{
	[featuredAppHandlerObj_ requestFeaturedApp];
}


- (void) getFeaturedAppWithCurrencyID:(NSString*)currencyID
{
	[featuredAppHandlerObj_ requestFeaturedAppWithCurrencyID:currencyID];
}


- (void) setFeaturedAppDisplayCount:(int) displayCount
{
	if (displayCount == TJC_FEATURED_COUNT_INF)
	{
		featuredAppDisplayCount_ = TJC_FEATURED_COUNT_INF;
	}
	else
	{
		// Display count is subtracted here by one to account for counting from 0.
		featuredAppDisplayCount_ = displayCount - 1;
	}
}


// called when request succeeeds
- (void) fetchResponseSuccessWithData:(void*)dataObj withRequestTag:(int)aTag
{
	// Store all the featured app data inside the featured app model.
	[featuredAppModelObj_ release];
	featuredAppModelObj_ = [[TJCFeaturedAppModel alloc] initWithTBXML:dataObj];
	
	int dispCount = [[TJCFeaturedAppDBManager sharedTJCFeaturedAppDBManager] getDisplayedCountForStoreID:featuredAppModelObj_.storeID];
	
	// Check whether we have reached our display count so that featured app will not keep showing up.
	// Also, if the store id happens to match the app id, then we will show the featured app indefinitely.
	if ((featuredAppDisplayCount_ == TJC_FEATURED_COUNT_INF) || 
		 [[[TapjoyConnect sharedTapjoyConnect] appID] isEqualToString:featuredAppModelObj_.storeID])
	{
		// The count is set to infinity, always return the object. 
		[[TJCFeaturedAppDBManager sharedTJCFeaturedAppDBManager] addApp:featuredAppModelObj_];
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_FEATURED_APP_RESPONSE_NOTIFICATION 
																			 object:featuredAppModelObj_];
	}
	else if (dispCount == 0 && dispCount <= featuredAppDisplayCount_)
	{
		// On the initial request, store the featured app data.
		[[TJCFeaturedAppDBManager sharedTJCFeaturedAppDBManager] addApp:featuredAppModelObj_];
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_FEATURED_APP_RESPONSE_NOTIFICATION 
																			 object:featuredAppModelObj_];
	}
	else if (dispCount <= featuredAppDisplayCount_)
	{
		// Udpate the display count each time featured app data is requested and returned.
		[[TJCFeaturedAppDBManager sharedTJCFeaturedAppDBManager] incrementDisplayedCountForStoreID:featuredAppModelObj_.storeID];
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_FEATURED_APP_RESPONSE_NOTIFICATION 
																			 object:featuredAppModelObj_];
	}
	else
	{
		// After the max featured app display count has been reached, just return nil.
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_FEATURED_APP_RESPONSE_NOTIFICATION object:nil];
		[featuredAppModelObj_ release];
		featuredAppModelObj_ = nil;
	}
}


// raised when error occurs
- (void) fetchResponseError:(TJCResponseError)errorType errorDescription:(id)errorDescObj requestTag:(int) aTag
{
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_FEATURED_APP_RESPONSE_NOTIFICATION object:nil];
	//No Object is sent No Model created so just release the Handler
	[self releaseFeaturedAppHandler];
}


- (void) releaseFeaturedAppHandler
{
	if(featuredAppHandlerObj_)
	{
		[featuredAppHandlerObj_ release];
		featuredAppHandlerObj_ = nil;
	}
}


- (void) dealloc
{
	if(featuredAppHandlerObj_)
	{
		[featuredAppHandlerObj_ release];
		featuredAppHandlerObj_ = nil;
	}
	
	[featuredAppModelObj_ release];
	[super dealloc];
}

@end
