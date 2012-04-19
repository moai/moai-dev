// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>
#import "TJCCoreFetcherHandler.h"


/*!	\interface TJCVideoRequestHandler
 *	\brief The Tapjoy Connect Video Request Handler Class.
 *
 */
@interface TJCVideoRequestHandler : TJCCoreFetcherHandler<TJCWebFetcherDelegate>
{
	BOOL clickSuccessful_;
}

@property (assign) BOOL clickSuccessful;


- (id)initRequestWithDelegate:(id<TJCFetchResponseDelegate>) aDelegate andRequestTag:(int)aTag;



// Video request methods.
// Requests video data.
- (void)requestVideoData;

// Request video click.
- (void)recordVideoClickWithURL:(NSString*)URLString;

// Request video complete.
- (void)requestVideoCompleteWithOfferID:(NSString*)offerID;





// Server callback methods.
// Video data received, which includes video URL, Offer ID, and other data. Caching should begin here.
- (void)videoDataReceived:(TJCCoreFetcher*)myFetcher;

// Video click received, this indicates that the server has received the video has begun playing message.
- (void)videoClickReceived:(TJCCoreFetcher*)myFetcher;

// Video complete callback, this indicates that the server has received the video complete message.
- (void)videoCompleteReceived:(TJCCoreFetcher*)myFetcher;

@end
