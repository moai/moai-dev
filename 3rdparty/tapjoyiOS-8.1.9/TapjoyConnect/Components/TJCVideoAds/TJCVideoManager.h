// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

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
	NSURLConnection *connection_;					/*!< The connection used for initiating and handling the download of video data. */
	NSMutableData *videoData_;						/*!< Used to hold video data as it's downloaded from the server. */
	int videoCacheCount_;							/*!< The number of videos to cache on the device. */
	int downloadIndex_;								/*!< The counter for caching video ads in order. */
	NSArray *unCachedVideoObjects_;				/*!< The video data dictionary in array form for caching purposes. */
	UIInterfaceOrientation currentOrientation_;	/*!< The current orientation of the video view. */
	BOOL shouldShowVideos_;
}

@property (nonatomic, retain) TJCVideoRequestHandler *requestHandler;
@property (nonatomic, retain) TJCVideoView *videoView;
@property (assign) UIInterfaceOrientation currentOrientation;
@property (assign) BOOL shouldShowVideos;


+ (TJCVideoManager*)sharedTJCVideoManager;

- (void)initVideoAdsWithDelegate:(id<TJCVideoAdDelegate>)delegate;

- (void)fetchResponseSuccessWithData:(void*)dataObj withRequestTag:(int)aTag;

- (void)fetchResponseError:(TJCResponseError)errorType errorDescription:(id)errorDescObj requestTag:(int) aTag;

/*!	\fn getCachedVideoIDs
 *	\brief Returns the offer IDs of all cached videos on the device.
 *
 *	\param n/a
 *	\return Array of all cached videos on the device.
 */
- (NSArray*)getCachedVideoIDs;

/*!	\fn getCachedVideoDictonary
 *	\brief Gets the cached dictionary from user defaults.
 *
 *	\param n/a
 *	\return Dictionary of all cached videos saved in User Defaults.
 */
- (NSMutableDictionary*)getCachedVideoDictonary;

/*!	\fn getAllVideosDictionary
 *	\brief Gets the video dictionary from user defaults.
 *
 *	\param n/a
 *	\return Dictionary of all videos saved in User Defaults.
 */
- (NSMutableDictionary*)getAllVideosDictionary;

/*!	\fn setCachedVideoObjectDict(NSDictionary* videoObjectDict, NSString* key)
 *	\brief Sets the given dictionary into user defaults.
 *
 *	\param videoObjectDict The video object to save in the cached videos dictionary.
 *	\param key The dictionary key of the video object being saved.
 *	\return n/a
 */
- (void)setCachedVideoObjectDict:(NSDictionary*)videoObjectDict withKey:(NSString*)key;

/*!	\fn setAllVideosObjectDict(NSDictionary* videoObjectDict, NSString* key)
 *	\brief Sets the given dictionary into user defaults.
 *
 *	\param videoObjectDict The video object to save in the videos dictionary.
 *	\param key The dictionary key of the video object being saved.
 *	\return n/a
 */
- (void)setAllVideosObjectDict:(NSDictionary*)videoObjectDict withKey:(NSString*)key;

- (void)beginVideoCaching;

- (void)updateViewWithOrientation:(UIInterfaceOrientation)interfaceOrientation;

/*!	\fn setVideoCacheCount:(int)count
 *	\brief Sets the cache count for video ads.
 *
 *	\param count The number of videos to cache to device locally.
 *	\return n/a
 */
- (void)setVideoCacheCount:(int)count;

/*!	\fn shouldDisplayLogo:(BOOL)display
 *	\brief Set the visibility of the Tapjoy logo during video playback.
 *
 *	\param display Boolean that indicates whether the Tapjoy logo should be displayed or not during video playback.
 *	\return n/a
 */
- (void)shouldDisplayLogo:(BOOL)display;

@end