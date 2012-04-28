// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>
#import "TJCCoreFetcherHandler.h"

extern NSString *kTJCAdClickURLStr;
extern NSString *kTJCAdImageDataStr;


/*!	\interface TJCAdRequestHandler
 *	\brief The Tapjoy Connect Ad Request Handler class.
 *
 * This class handles the actual URL request to grab the ad data from the server.
 */
@interface TJCAdRequestHandler : TJCCoreFetcherHandler <TJCWebFetcherDelegate>
{
	BOOL accumulatingClickURL;				/*!< Indicates whether the NSXML parser is currently parsing the click URL data. */
	NSMutableString *clickURL_;			/*!< The redirect url for the ABC Ad. */
	BOOL accumulatingImageDataStr;		/*!< Indicates whether the NSXML parser is currently parsinge the image data. */
	NSMutableString *imageDataStr_;		/*!< The image data in string form. */
	BOOL isDataFetchSuccessful_;			/*!< Only set to true if valid data was received from the server. */
}

/*!	\fn initRequestWithDelegate:andRequestTag(id<TJCFetchResponseDelegate> aDelegate, int aTag)
 *	\brief Initializes the #TJCAdRequestHandler object with the given delegate and tag id.
 *
 *	\param aDelegate The #TJCFetchResponseDelegate delegate object to initialize with.
 *	\return The #TJCFeaturedAppModel object.
 */
- (id)initRequestWithDelegate:(id<TJCFetchResponseDelegate>)aDelegate andRequestTag:(int)aTag;

/*!	\fn requestAdWithSize:(NSString*)adSize
 *	\brief Initiates the URL request to get the ad data from the server.
 *
 *  \param adSize Must be one of #TJC_AD_BANNERSIZE_320X50 or #TJC_AD_BANNERSIZE_640X100 or #TJC_AD_BANNERSIZE_768x90.
 *	\return n/a
 */
- (void)requestAdWithSize:(NSString*)adSize;

/*!	\fn requestAdWithSize:(NSString*)adSize
 *	\brief Initiates the URL request to get the ad data from the server.
 *
 *	\param adSize Must be one of #TJC_AD_BANNERSIZE_320X50 or #TJC_AD_BANNERSIZE_640X100 or #TJC_AD_BANNERSIZE_768x90.
 *	\param currencyID The currency ID specifies which currency to advertise in the banner ad.
 *	\return n/a
 */
- (void)requestAdWithSize:(NSString*)adSize currencyID:(NSString*)currencyID;

- (BOOL)isDataFetchSuccessful;

@end
