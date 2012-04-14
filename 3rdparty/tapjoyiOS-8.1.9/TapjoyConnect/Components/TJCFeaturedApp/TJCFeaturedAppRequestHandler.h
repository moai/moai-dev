// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>
#import "TJCCoreFetcherHandler.h"
#import "TJCFetchResponseProtocol.h"


/*!	\interface TJCFeaturedAppRequestHandler
 *	\brief The Tapjoy Connect Featured App Request Handler class.
 *
 * This class handles the actual URL request to grab the featured app data from the server.
 */
@interface TJCFeaturedAppRequestHandler : TJCCoreFetcherHandler<TJCWebFetcherDelegate>
{

}

/*!	\fn initRequestWithDelegate:andRequestTag:(id<TJCFetchResponseDelegate> aDelegate, int aTag)
 *	\brief Initializes the #TJCFeaturedAppRequestHandler object with the given delegate and tag id.
 *
 *	\param aDelegate The #TJCFetchResponseDelegate delegate object to initialize with.
 *	\return The #TJCFeaturedAppModel object.
 */
- (id)initRequestWithDelegate:(id<TJCFetchResponseDelegate>) aDelegate andRequestTag:(int)aTag;

/*!	\fn requestFeaturedApp
 *	\brief Initiates the URL request to get the featured app data from the server.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)requestFeaturedApp;

/*!	\fn requestFeaturedAppWithCurrencyID:(NSString*)currencyID
 *	\brief Initiates the URL request to get the featured app data from the server.
 *
 *	\param currencyID The GUID for the currency earned when completing this featured ad.
 *	\return n/a
 */
- (void)requestFeaturedAppWithCurrencyID:(NSString*)currencyID;


@end
