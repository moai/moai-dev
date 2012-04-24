// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCUserAccountRequestHandler.h"
#import "TJCCoreFetcherHandler.h"
#import "TJCLog.h"
#import "TapjoyConnect.h"
#import "TJCCoreFetcher.h"
#import "TJCUtil.h"
#import "TJCConstants.h"

@implementation TJCUserAccountRequestHandler

-(id) initRequestWithDelegate:(id<TJCFetchResponseDelegate>) aDelegate andRequestTag:(int)aTag
{
	if((self = [super initRequestWithDelegate:aDelegate andRequestTag:aTag]))
	{
	}
	return self;
}


- (void)requestTapPoints
{
	NSString *requestString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL, TJC_URL_PARAM_VG_ITEMS_USER];
	NSString *alternateString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL_ALTERNATE, TJC_URL_PARAM_VG_ITEMS_USER];
	
	NSMutableDictionary *paramDict = [[NSMutableDictionary alloc] initWithDictionary:[[TapjoyConnect sharedTapjoyConnect] genericParameters]];
	
	// Add the publisher user ID to the generic parameters dictionary.
	if ([TapjoyConnect getUserID])
	{
		[paramDict setObject:[TapjoyConnect getUserID] forKey:TJC_URL_PARAM_USER_ID];
	}
	
	[self makeGenericRequestWithURL:requestString 
							 alternateURL:alternateString 
									 params:paramDict
								  selector:@selector(userAccountDataReceived:)];
	
	[paramDict release];
}


- (void)subtractTapPoints:(int)points
{
	NSString *requestString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL, TJC_URL_PARAM_SPEND_TAP_POINTS];
	NSString *alternateString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL_ALTERNATE, TJC_URL_PARAM_SPEND_TAP_POINTS];
	
	NSMutableDictionary *paramDict = [[NSMutableDictionary alloc] initWithDictionary:[[TapjoyConnect sharedTapjoyConnect] genericParameters]];
	
	// Add the publisher user ID to the generic parameters dictionary.
	if ([TapjoyConnect getUserID])
	{
		[paramDict setObject:[TapjoyConnect getUserID] forKey:TJC_URL_PARAM_USER_ID];
	}
	
	// Set points to deduct.
	[paramDict setObject:[NSString stringWithFormat:@"%d", points] forKey:TJC_URL_PARAM_TAP_POINTS];
	
	[self makeGenericRequestWithURL:requestString 
							 alternateURL:alternateString 
									 params:paramDict
								  selector:@selector(userAccountDataReceived:)];
	
	[paramDict release];
}


- (void)addTapPoints:(int)points
{
	NSString *requestString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL, TJC_URL_PARAM_AWARD_TAP_POINTS];
	NSString *alternateString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL_ALTERNATE, TJC_URL_PARAM_AWARD_TAP_POINTS];
	
	NSMutableDictionary *paramDict = [[NSMutableDictionary alloc] initWithDictionary:[[TapjoyConnect sharedTapjoyConnect] genericParameters]];
	
	// Add the publisher user ID to the generic parameters dictionary.
	if ([TapjoyConnect getUserID])
	{
		[paramDict setObject:[TapjoyConnect getUserID] forKey:TJC_URL_PARAM_USER_ID];
	}
	
	// Set points to award.
	[paramDict setObject:[NSString stringWithFormat:@"%d", points] forKey:TJC_URL_PARAM_TAP_POINTS];
	
	NSTimeInterval timeInterval = [[NSDate date] timeIntervalSince1970];
	// Get seconds since Jan 1st, 1970.
	NSString *timeStamp = [NSString stringWithFormat:@"%d", (int)timeInterval];
	
	// Replace existing time stamp with this time stamp, just in case it's changed since genericParameters was generated.
	[paramDict setObject:timeStamp forKey:TJC_TIMESTAMP];
	
	// This guid is guaranteed to be unique (according to apple documentation).
	NSString *guid = [[NSProcessInfo processInfo] globallyUniqueString];
	
	// Replace verifier with a one generated using a guid and currency amount.
	NSString *newVerifier = [TapjoyConnect TJCSHA256CommonParamsWithTimeStamp:timeStamp tapPointsAmount:points guid:guid];
	[paramDict setObject:newVerifier forKey:TJC_VERIFIER];
	
	// Insert guid to the params.
	[paramDict setObject:guid forKey:TJC_GUID];
	
	[self makeGenericRequestWithURL:requestString 
							 alternateURL:alternateString 
									 params:paramDict
								  selector:@selector(userAccountDataReceived:)];
	
	[paramDict release];
}


- (void)userAccountDataReceived:(TJCCoreFetcher*)myFetcher 
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Update Account Info Response Returned"];
	
	TBXMLElement *tjcConnectRetObject = [self validateResponseReturnedObject:myFetcher];
	
	if (!tjcConnectRetObject) 
		return;
	
	TBXMLElement *userAccountObj = [TBXML childElementNamed:@"UserAccountObject" parentElement:tjcConnectRetObject];
	
	if (!userAccountObj)
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"%s: %d; %s; No data received while attempting to fetch the data from the server", 
		 __FILE__, __LINE__, __PRETTY_FUNCTION__];
		
		[deleg_ fetchResponseError:kTJCStatusNotOK errorDescription:nil requestTag:myFetcher.requestTag];
		return;
	}
	
	[deleg_ fetchResponseSuccessWithData:userAccountObj withRequestTag:myFetcher.requestTag];
}


- (void)dealloc
{
	[super dealloc];
}


@end
