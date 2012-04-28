// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import "TJCFetchResponseProtocol.h"
#import "TJCAdDelegateProtocol.h"
#import "TapjoyConnect.h"

#define TJC_AD_BANNERSIZE_320X50		@"320x50"
#define TJC_AD_BANNERSIZE_640X100	@"640x100"
#define TJC_AD_BANNERSIZE_768X90		@"768x90"


#define TJC_AD_REFRESH_DELAY		(15.0)

extern NSString *kTJCAdFailStr;

@class TJCAdRequestHandler;

/*!	\interface TJCAdView
 *	\brief The Tapjoy Connect Ad View class.
 *
 */
@interface TJCAdView : UIView <TJCFetchResponseDelegate>
{
@private
	NSString *clickURL_;					/*!< The redirect url for the ABC Ad. */
	NSString *imageDataStr_;			/*!< The image data in string form. */
	UIImageView *imageView_;			/*!< The Ad image view. */
	UIImageView *previousImageView_;	/*!< The previous Ad image view. */
	NSString *contentSizeStr_;			/*!< The content size string of the Ad. */
	UIView *adViewOverlay_;				/*!< This is used to darken the ad view when the ad is touched. */
	
	TJCAdRequestHandler *adHandlerObj_;	/*!< The Ad request handler handles URL requests and responses. */
	id<TJCAdDelegate> adDelegate_;		/*!< The Ad delegate used to handle delegate protocol methods. */
}


@property (copy) NSString *clickURL;
@property (copy) NSString *imageDataStr;
@property (nonatomic, retain) UIImageView *imageView;
@property (nonatomic, retain) UIImageView *previousImageView;
@property (copy) NSString *contentSizeStr;
@property (nonatomic, retain) UIView *adViewOverlay;

@property (nonatomic, retain) TJCAdRequestHandler *adHandlerObj;
@property (nonatomic, retain) id<TJCAdDelegate> adDelegate;

+ (TJCAdView*) sharedTJCAdView;

/*!	\fn getAdWithDelegate:(id<TJCAdDelegate>)deleg
 *	\brief Initiates a URL request to get the Tapjoy Ad data.
 *
 *	\param deleg The class instance that implements the TJCAdDelegate protocol.
 *	\return n/a
 */
- (id)getAdWithDelegate:(id<TJCAdDelegate>)deleg;

/*!	\fn getDisplayAdWithDelegate:(id<TJCAdDelegate>)deleg currencyID:(NSString*)cu
 *	\brief Initiates a URL request to get the Tapjoy Ad data.
 *
 *	\param deleg The class instance that implements the TJCAdDelegate protocol.
 *	\param currencyID The currency ID specifies which currency to advertise in the banner ad.
 *	\return The TJCAdView singleton object.
 */
- (id) getAdWithDelegate:(id<TJCAdDelegate>)deleg currencyID:(NSString*)currencyID;

/*!	\fn fetchResponseSuccessWithData:withRequestTag:(void* dataObj, int aTag)
 *	\brief Called when the request for the ad data succeeeds.
 *
 *	\param dataObj The ad data returned from the server.
 *	\param aTag ID associated with the request.
 *	\return n/a
 */
- (void)fetchResponseSuccessWithData:(void*)dataObj withRequestTag:(int)aTag;

/*!	\fn fetchResponseError:errorDescription:requestTag:(TJCResponseError errorType, id errorDescObj, int aTag)
 *	\brief Called when and error occurs with the URL request.
 *
 *	\param errorType Contains error information.
 *	\param errorDescObj Contains error data.
 *	\param aTag ID associated with the request.
 *	\return n/a
 */
- (void)fetchResponseError:(TJCResponseError)errorType errorDescription:(id)errorDescObj requestTag:(int)aTag;

/*!	\fn refreshAd:(NSTimer*)timer
 *	\brief Attempts to load a new Ad.
 *
 *	\param n/a
 *	\return n/a
 */
- (void) refreshAd:(NSTimer*)timer;

/*!	\fn refreshAdWithCurrencyID:(NSString*)currencyID
 *	\brief Attempts to load a new Ad.
 *
 *	\param currencyID The currency ID specifies which currency to advertise in the banner ad. 
 *	\return n/a
 */
- (void) refreshAdWithCurrencyID:(NSString*)currencyID;

/*!	\fn isAdLoaded
 *	\brief Indicates whether ad data has been successfully loaded.
 *
 *	\param n/a
 *	\return YES to indicate that the ad data has been successfully loaded, NO otherwise.
 */
- (BOOL) isAdLoaded;

@end




@interface TapjoyConnect (TJCAdView)

/*!	\fn getDisplayAdWithDelegate:(id<TJCAdDelegate>)deleg
 *	\brief Initiates a URL request to get the Tapjoy Ad data.
 *
 *	\param deleg The class instance that implements the TJCAdDelegate protocol.
 *	\return The TJCAdView singleton object.
 */
+ (id) getDisplayAdWithDelegate:(id <TJCAdDelegate>)deleg;

/*!	\fn getDisplayAdWithDelegate:(id<TJCAdDelegate>)deleg currencyID:(NSString*)cu
 *	\brief Initiates a URL request to get the Tapjoy Ad data.
 *
 *	\param deleg The class instance that implements the TJCAdDelegate protocol.
 *	\param currencyID The currency ID specifies which currency to advertise in the banner ad.
 *	\return The TJCAdView singleton object.
 */
+ (id) getDisplayAdWithDelegate:(id<TJCAdDelegate>)deleg currencyID:(NSString*)currencyID;

/*!	\fn refreshDisplayAd
 *	\brief Attempts to load a new Ad.
 *
 *	\param n/a
 *	\return n/a
 */
+ (void) refreshDisplayAd;

/*!	\fn refreshDisplayAdWithCurrencyID:(NSString*)currencyID
 *	\brief Attempts to load a new Ad.
 *
 *	\param currencyID The currency ID specifies which currency to advertise in the banner ad. 
 *	\return n/a
 */
+ (void) refreshDisplayAdWithCurrencyID:(NSString*)currencyID;

/*!	\fn isDisplayAdLoaded
 *	\brief Indicates whether ad data has been successfully loaded.
 *
 *	\param n/a
 *	\return YES to indicate that the ad data has been successfully loaded, NO otherwise.
 */
+ (BOOL) isDisplayAdLoaded;

/*!	\fn getDisplayAdView
 *	\brief The Tapjoy Display Ad UIView returned by this method should be added to the current superview after an Ad has been successfully loaded.
 *
 *	\param n/a
 *	\return The Tapjoy Display Ad UIView.
 */
+ (TJCAdView*) getDisplayAdView;

@end

