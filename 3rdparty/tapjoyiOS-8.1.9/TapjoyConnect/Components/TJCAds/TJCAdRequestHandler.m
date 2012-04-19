// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCAdRequestHandler.h"
#import "TJCCoreFetcher.h"
#import "TapjoyConnect.h"
#import "TJCLog.h"

// NSNotification name for reporting errors
NSString *kTJCAdErrorNotif = @"TJCAdErrorNotif";

// NSNotification userInfo key for obtaining the error message
NSString *kTJCAdMsgErrorKey = @"TJCAdMsgErrorKey";

NSString *kTJCAdClickURLStr = @"ClickURL";

NSString *kTJCAdImageDataStr = @"Image";


@implementation TJCAdRequestHandler

-(id)initRequestWithDelegate:(id<TJCFetchResponseDelegate>)aDelegate andRequestTag:(int)aTag
{
	if ((self = [super initRequestWithDelegate:aDelegate andRequestTag:aTag]))
	{
		isDataFetchSuccessful_ = NO;
	}
	
	return self;
}


- (void)requestAdWithSize:(NSString*)adSize
{
	[self requestAdWithSize:adSize currencyID:nil];
}


- (void)requestAdWithSize:(NSString*)adSize currencyID:(NSString*)currencyID
{
	// Reset success bool whenever a request is made. It will be set to success when data is retrieved from the server.
	isDataFetchSuccessful_ = NO;
	
	NSString *userID = [TapjoyConnect getUserID];
	
	NSString *requestString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL, TJC_BANNER_URL_NAME];
	
	NSString *alternateString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL_ALTERNATE, TJC_BANNER_URL_NAME];
	
	NSMutableDictionary *paramDict = [[[TapjoyConnect sharedTapjoyConnect] genericParameters] retain];
	
	// Add the publisher user ID to the generic parameters dictionary.
	if (userID)
	{
		[paramDict setObject:userID forKey:TJC_URL_PARAM_USER_ID];
	}
	
	// Add the content size of the ad to the request URL.
	[paramDict setObject:adSize forKey:TJC_URL_PARAM_DISPLAY_AD_SIZE];
	
	// Set the currency ID here, if it exists.
	if (currencyID)
	{
		[paramDict setObject:currencyID forKey:TJC_URL_PARAM_CURRENCY_ID];
	}
	
	[self makeGenericRequestWithURL:requestString alternateURL:alternateString params:paramDict selector:@selector(adDataReceived:)];
	[paramDict release];
}


- (void)adDataReceived:(TJCCoreFetcher*) myFetcher 
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Ad Data Response Returned"];
	
	TBXMLElement *tjcConnectRetObject = [self validateResponseReturnedObject:myFetcher];
	
	// Check for valid return data;
	if(!tjcConnectRetObject) 
	{
		// Initiate fail method call.
		[deleg_ fetchResponseError:kTJCStatusNotOK errorDescription:nil requestTag:myFetcher.requestTag];
		return;
	}
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"Update Account Info Response Returned"];
	
	TBXMLElement *clickURLObj = [TBXML childElementNamed:kTJCAdClickURLStr parentElement:tjcConnectRetObject];
	
	if (!clickURLObj)
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"%s: %d; %s; No data received while attempting to fetch the data from the server", __FILE__, __LINE__, __PRETTY_FUNCTION__];
		
		[deleg_ fetchResponseError:kTJCStatusNotOK errorDescription:nil requestTag:myFetcher.requestTag];
		return;
	}
	
	TBXMLElement *imageDataStrObj = [TBXML childElementNamed:kTJCAdImageDataStr parentElement:tjcConnectRetObject];
	
	if (!imageDataStrObj)
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"%s: %d; %s; No data received while attempting to fetch the data from the server", __FILE__, __LINE__, __PRETTY_FUNCTION__];
		
		[deleg_ fetchResponseError:kTJCStatusNotOK errorDescription:nil requestTag:myFetcher.requestTag];
		return;
	}
	
	if (clickURL_)
	{
		[clickURL_ release];
	}
	clickURL_ = [[NSMutableString stringWithString:[TBXML textForElement:clickURLObj]] retain];
	
	if (imageDataStr_)
	{
		[imageDataStr_ release];
	}
	imageDataStr_ = [[NSMutableString stringWithString:[TBXML textForElement:imageDataStrObj]] retain];
	
	// Create an array to be passed in to the fetch response method.
	NSArray *dataArray = [[NSArray alloc] initWithObjects:clickURL_, imageDataStr_, nil];
	
	// Set success bool only here!
	isDataFetchSuccessful_ = YES;
	
	// We don't use the tag here so just pass in a zero.
	[deleg_ fetchResponseSuccessWithData:dataArray withRequestTag:0];
	
	[dataArray release];
}


- (BOOL)isDataFetchSuccessful
{
	return isDataFetchSuccessful_;
}


- (void)dealloc
{
	[clickURL_ release];
	[imageDataStr_ release];	
	[super dealloc];
}


// an error occurred while parsing the ad data,
// post the error as an NSNotification to our app delegate.
- (void)handleAdError:(NSError*)parseError 
{
	[[NSNotificationCenter defaultCenter] postNotificationName:kTJCAdErrorNotif
																		 object:self
																	  userInfo:[NSDictionary dictionaryWithObject:parseError
																														forKey:kTJCAdMsgErrorKey]];
}

// an error occurred while parsing the ad data,
// pass the error to the main thread for handling.
- (void)parser:(NSXMLParser*)parser parseErrorOccurred:(NSError*)parseError
{
	if ([parseError code] != NSXMLParserDelegateAbortedParseError)
	{
		[self performSelectorOnMainThread:@selector(handleAdError:)
									  withObject:parseError
								  waitUntilDone:NO];
	}
}

@end
