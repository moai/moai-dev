// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCFeaturedAppRequestHandler.h"
#import "TJCLog.h"
#import "TapjoyConnect.h"
#import "TJCCoreFetcher.h"
#import "TJCOffersWebView.h"
#import "TJCConstants.h"
#import "TJCFeaturedAppView.h"


@implementation TJCFeaturedAppRequestHandler

-(id) initRequestWithDelegate:(id<TJCFetchResponseDelegate>) aDelegate andRequestTag:(int)aTag
{
	self = [super initRequestWithDelegate:aDelegate andRequestTag:aTag];
	
	return self;
}


- (void)requestFeaturedApp
{
	NSString *userID = [TapjoyConnect getUserID];
	
	// Save off the user id since we may need to access it later upon a possible retry.
	[TJCFeaturedAppView sharedTJCFeaturedAppView].publisherUserID_ = userID;
	[TJCFeaturedAppView sharedTJCFeaturedAppView].currencyID_ = nil;
	
	NSString *requestString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL, TJC_FEATURED_URL_NAME];
	
	NSString *alternateString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL_ALTERNATE, TJC_FEATURED_URL_NAME];
	
	NSMutableDictionary *paramDict = [[[TapjoyConnect sharedTapjoyConnect] genericParameters] retain];
	
	// Add the publisher user ID to the generic parameters dictionary.
	if (userID)
	{
		[paramDict setObject:userID forKey:TJC_URL_PARAM_USER_ID];
	}
	
	[self makeGenericRequestWithURL:requestString alternateURL:alternateString params:paramDict selector:@selector(featuredAppDataReceived:)];
	[paramDict release];
}


- (void) requestFeaturedAppWithCurrencyID:(NSString*)currencyID
{
	NSString *userID = [TapjoyConnect getUserID];
	
	// Save off the user id since we may need to access it later upon a possible retry.
	[TJCFeaturedAppView sharedTJCFeaturedAppView].publisherUserID_ = userID;
	[TJCFeaturedAppView sharedTJCFeaturedAppView].currencyID_ = currencyID;
	
	NSString *requestString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL, TJC_FEATURED_URL_NAME];
	
	NSString *alternateString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL_ALTERNATE, TJC_FEATURED_URL_NAME];
	
	NSMutableDictionary *paramDict = [[[TapjoyConnect sharedTapjoyConnect] genericParameters] retain];
	
	// Add the publisher user ID to the generic parameters dictionary.
	if (userID)
	{
		[paramDict setObject:userID forKey:TJC_URL_PARAM_USER_ID];
	}
	
	// Add the currency ID to the generic parameters dictionary.
	[paramDict setObject:currencyID forKey:TJC_URL_PARAM_CURRENCY_ID];
	
	[self makeGenericRequestWithURL:requestString alternateURL:alternateString params:paramDict selector:@selector(featuredAppDataReceived:)];
	[paramDict release];
}


- (void)featuredAppDataReceived:(TJCCoreFetcher*)myFetcher 
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Update Account Info Response Returned"];
	
	TBXMLElement *tjcConnectRetObject = [self validateResponseReturnedObject:myFetcher];
	
	if (!tjcConnectRetObject) 
		return;
	
	TBXMLElement *offerArryObj = [TBXML childElementNamed:@"OfferArray" parentElement:tjcConnectRetObject];
	
	if(!offerArryObj)
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"%s: %d; %s; No data received while attempting to fetch the data from the server", __FILE__, __LINE__, __PRETTY_FUNCTION__];
		
		[deleg_ fetchResponseError:kTJCStatusNotOK errorDescription:nil requestTag:myFetcher.requestTag];
		return;
	}
	
	TBXMLElement *featureApp = [TBXML childElementNamed:@"TapjoyApp" parentElement:offerArryObj];
	
	if(!featureApp)
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"%s: %d; %s; No data received while attempting to fetch the data from the server", __FILE__, __LINE__, __PRETTY_FUNCTION__];
		
		[deleg_ fetchResponseError:kTJCStatusNotOK errorDescription:nil requestTag:myFetcher.requestTag];
		return;
	}
	
	[deleg_ fetchResponseSuccessWithData:featureApp withRequestTag:myFetcher.requestTag];
}


-(void) dealloc
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"TJCFeaturedAppRequestHandler dealloc"];
	[super dealloc];
}

@end
