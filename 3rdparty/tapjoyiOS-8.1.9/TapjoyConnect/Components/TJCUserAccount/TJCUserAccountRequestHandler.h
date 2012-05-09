// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>
#import "TJCCoreFetcherHandler.h"


@class TJCUserAccountModel;

/*!	\interface TJCUserAccountRequestHandler
 *	\brief The Tapjoy Connect User Request Handler class.
 *
 * This class handles the actual URL request to grab the user account data from the server.
 */
@interface TJCUserAccountRequestHandler : TJCCoreFetcherHandler<TJCWebFetcherDelegate>
{
	
}

/*!	\fn initRequestWithDelegate:andRequestTag:(id<TJCFetchResponseDelegate> aDelegate, int aTag)
 *	\brief Initializes the #TJCUserAccountRequestHandler object with the given delegate and tag id.
 *
 *	\param aDelegate The #TJCFetchResponseDelegate delegate object to initialize with.
 *	\return The #TJCUserAccountRequestHandler object.
 */
- (id)initRequestWithDelegate:(id<TJCFetchResponseDelegate>)aDelegate andRequestTag:(int)aTag;

/*!	\fn requestTapPoints
 *	\brief Initiates the URL request to get the user account data from the server. This includes currency name, id, and amount.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)requestTapPoints;

/*!	\fn subtractTapPoints:(int)points
 *	\brief Initiates the URL request to subtract the given amount of virtual currency from the user's total.
 *
 *	\param points The amount of virtual currency to subtract from the user's current total.
 *	\return n/a
 */
- (void)subtractTapPoints:(int)points;

/*!	\fn addTapPoints:(int)points
 *	\brief Initiates the URL request to add the given amount of virtual currency to the user's total.
 *
 *	\param points The amount of virtual currency to add to the user's current total.
 *	\return n/a
 */
- (void)addTapPoints:(int)points;

@end
