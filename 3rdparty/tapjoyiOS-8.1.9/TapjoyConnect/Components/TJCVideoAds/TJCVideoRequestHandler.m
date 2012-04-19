// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCVideoRequestHandler.h"
#import "TapjoyConnect.h"
#import "TJCConstants.h"
#import "TJCCoreFetcher.h"
#import "TJCVideoManager.h"
#import "TJCLog.h"

@implementation TJCVideoRequestHandler

@synthesize clickSuccessful = clickSuccessful_;

- (id)initRequestWithDelegate:(id<TJCFetchResponseDelegate>) aDelegate andRequestTag:(int)aTag
{
	self = [super initRequestWithDelegate:aDelegate andRequestTag:aTag];
	
	// Reset click flag.
	clickSuccessful_ = NO;
	
	return self;
}


#pragma mark -
#pragma mark Video Request Methods

- (void)requestVideoData
{
	NSString *userID = [TapjoyConnect getUserID];
	
	NSString *requestString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL, TJC_VIDEO_AD_GET_API];
	
	NSString *alternateString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL_ALTERNATE, TJC_VIDEO_AD_GET_API];
	
	NSMutableDictionary *paramDict = [[[TapjoyConnect sharedTapjoyConnect] genericParameters] retain];
	
	// Add the publisher user ID to the generic parameters dictionary.
	if (userID)
	{
		[paramDict setObject:userID forKey:TJC_URL_PARAM_USER_ID];
	}
	
	[self makeGenericRequestWithURL:requestString alternateURL:alternateString params:paramDict selector:@selector(videoDataReceived:)];
	[paramDict release];
}


- (void)recordVideoClickWithURL:(NSString*)URLString
{
	clickSuccessful_ = NO;
	[self makeGenericRequestWithURL:URLString alternateURL:URLString params:nil selector:@selector(videoClickReceived:)];
}


- (void)requestVideoCompleteWithOfferID:(NSString*)offerID
{
	// Only record video complete if click was successful.
	if (!clickSuccessful_)
	{
		return;
	}
	
	NSString *userID = [TapjoyConnect getUserID];
	
	NSString *requestString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL, TJC_CONNECT_API];
	
	NSString *alternateString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL_ALTERNATE, TJC_CONNECT_API];
	
	NSMutableDictionary *paramDict = [[[TapjoyConnect sharedTapjoyConnect] genericParameters] retain];
	
	// Add the publisher user ID to the generic parameters dictionary.
	if (userID)
	{
		[paramDict setObject:userID forKey:TJC_URL_PARAM_USER_ID];
	}
	
	// Set app id to the offer ID cause that's how it works.
	[paramDict setObject:offerID forKey:TJC_APP_ID_NAME];

	[self makeGenericRequestWithURL:requestString alternateURL:alternateString params:paramDict selector:@selector(videoCompleteReceived:)];
	[paramDict release];
}





#pragma mark -
#pragma mark Video Callback Methods

- (void)videoDataReceived:(TJCCoreFetcher*)myFetcher 
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Video Ad Data Response Returned"];
	
	TBXMLElement *tjcConnectRetObject = [self validateResponseReturnedObject:myFetcher];
	
	if (!tjcConnectRetObject)
		return;
	
	[deleg_ fetchResponseSuccessWithData:tjcConnectRetObject withRequestTag:myFetcher.requestTag];
}


- (void)videoClickReceived:(TJCCoreFetcher*)myFetcher
{
	NSData* myData = [myFetcher data];
	NSString *dataString = [[NSString alloc] initWithData:myData encoding:NSUTF8StringEncoding];
	
	if ([dataString isEqualToString:@"OK"])
	{
		clickSuccessful_ = YES;
	}
	
	[dataString release];
}


- (void)videoCompleteReceived:(TJCCoreFetcher*)myFetcher
{
	TBXMLElement *tjcConnectRetObject = [self validateResponseReturnedObject:myFetcher];
	
	if (!tjcConnectRetObject)
		return;
}


@end
