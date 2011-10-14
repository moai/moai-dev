//Copyright (C) 2011 by Tapjoy Inc.
//
//This file is part of the Tapjoy SDK.
//
//By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
//The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>
#import "TJCFetchResponseProtocol.h"
#import "TJCVideoAdProtocol.h"
#import "TJCVideoObject.h"


@class TJCVideoRequestHandler;
@class TJCVideoView;


/*!	\interface TJCVideoManager
 *	\brief The Tapjoy Connect Video Manager Class.
 *
 */
@interface TJCVideoManager : NSObject <TJCFetchResponseDelegate>
{
@private
	TJCVideoRequestHandler *requestHandler_;	/*!< The video request handler. */
	TJCVideoView *videoView_;						/*!< The video view. */
	NSString *currencyID_;							/*!< The name of the currency to load in the offer wall. */
	NSURLConnection *connection_;					/*!< The connection used for initiating and handling the download of video data. */
	NSMutableData *videoData_;						/*!< Used to hold video data as it's downloaded from the server. */
	int videoCacheCount_;							/*!< The number of videos to cache on the device. */
	int downloadIndex_;								/*!< The counter for caching video ads in order. */
	NSArray *unCachedVideoObjects_;				/*!< The video data dictionary in array form for caching purposes. */
}

@property (nonatomic, retain) TJCVideoRequestHandler *requestHandler;
@property (nonatomic, retain) TJCVideoView *videoView;
@property (copy) NSString *currencyID;


+ (TJCVideoManager*)sharedTJCVideoManager;

- (void)initVideoAdsWithDelegate:(id<TJCVideoAdDelegate>)delegate;

- (void)fetchResponseSuccessWithData:(void*)dataObj withRequestTag:(int)aTag;

- (void)fetchResponseError:(TJCResponseError)errorType errorDescription:(id)errorDescObj requestTag:(int) aTag;

// Gets the cached dictionary from user defaults.
- (NSMutableDictionary*)getCachedVideoDictonary;

// Sets the given dictionary into user defaults.
- (void)setCachedVideoObjectDict:(NSDictionary*)videoObjectDict withKey:(NSString*)key;

- (void)beginVideoCaching;

- (void)setVideoCacheCount:(int)count;

@end