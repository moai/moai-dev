// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>
#import "TapjoyConnect.h"
#import "TJCCallsWrapper.h"
#import "TJCVideoAdProtocol.h"


/*!	\interface TJCVideoViewHandler
 *	\brief The Tapjoy Connect Video Ads View Handler class.
 *
 */
@interface TJCVideoViewHandler : NSObject 
{
    
}

+ (UIView*)showVideoAdWithFrame:(CGRect)frame offerID:(NSString*)offerID;
+ (UIView*)showVideoAdWithOfferID:(NSString*)offerID;
+ (void)showVideoAdWithViewController:(UIViewController*)vController offerID:(NSString*)offerID;

@end



@interface TapjoyConnect (TJCVideoViewHandler)

/*!	\fn initVideoAdWithDelegate:(id<TJCVideoAdDelegate>)delegate
 *	\brief Initializes video ads.
 *
 *	\param delegate The class that implements the TJCVideoAdDelegate protocol.
 *	\return n/a
 */
+ (void)initVideoAdWithDelegate:(id<TJCVideoAdDelegate>)delegate;

/*!	\fn showVideoAdWithFrame(CGRect frame, NSString *offerID)
 *	\brief Shows a video ad with the view constructed using the given frame.
 *
 *	\param frame The frame with which to construct the video ad view.
 *	\param offerID The ID of the video to play. Doubles as the filename.
 *	\return The UIView of the video ad.
 */
+ (UIView*)showVideoAdWithFrame:(CGRect)frame offerID:(NSString *)offerID;

/*!	\fn showVideoAdWithOfferID:(NSString*)offerID
 *	\brief Shows a video ad.
 *
 *	\param offerID The ID of the video to play. Doubles as the filename.
 *	\return The UIView of the video ad.
 */
+ (void)showVideoAdWithOfferID:(NSString*)offerID;

/*!	\fn showVideoAdWithViewController(UIViewController *vController, NSString *offerID)
 *	\brief Shows a video ad within the given view controller.
 *
 *	\param vController The view controller to show the video ad within.
 *	\param offerID The ID of the video to play. Doubles as the filename.
 *	\return The UIView of the video ad.
 */
+ (void)showVideoAdWithViewController:(UIViewController*)vController offerID:(NSString *)offerID;

/*!	\fn setVideoCacheCount:(int)count
 *	\brief Sets the cache count for video ads.
 *
 *	\param count The number of videos to cache to device locally.
 *	\return n/a
 */
+ (void)setVideoCacheCount:(int)count;

/*!	\fn shouldDisplayVideoLogo:(BOOL)display
 *	\brief Set the visibility of the Tapjoy logo during video playback.
 *
 *	\param display Boolean that indicates whether the Tapjoy logo should be displayed or not during video playback.
 *	\return n/a
 */
+ (void)shouldDisplayVideoLogo:(BOOL)display;

@end


@interface TJCCallsWrapper (TJCVideoViewHandler)

- (UIView*)showVideoAdWithFrame:(CGRect)frame offerID:(NSString*)offerID;
- (void)showVideoAdWithOfferID:(NSString*)offerID;
- (void)showVideoAdWithViewController:(UIViewController*)vController offerID:(NSString*)offerID;

@end
