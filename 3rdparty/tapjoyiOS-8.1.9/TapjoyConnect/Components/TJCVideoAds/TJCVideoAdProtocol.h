// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <UIKit/UIKit.h>

@class TJCVideoAd;


/*!	\protocol TJCVideoAdDelegate
 *	\brief The Tapjoy Video Ad Delegate Protocol.
 */
@protocol TJCVideoAdDelegate <NSObject>

@optional

/*!	\fn videoAdBegan
 *	\brief Called when the video ad begins playing.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)videoAdBegan;

/*!	\fn videoAdClosed
 *	\brief Called when the video ad is closed.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)videoAdClosed;

@end
