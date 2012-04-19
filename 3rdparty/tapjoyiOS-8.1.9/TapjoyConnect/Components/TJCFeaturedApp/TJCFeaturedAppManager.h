// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>
#import "TJCFetchResponseProtocol.h"

@class TJCFeaturedAppModel;
@class TJCFeaturedAppRequestHandler;

/*!	\interface TJCFeaturedAppManager
 *	\brief The Tapjoy Connect Featured App Manager class.
 *
 */
@interface TJCFeaturedAppManager : NSObject <TJCFetchResponseDelegate>
{
	TJCFeaturedAppRequestHandler *featuredAppHandlerObj_;
	TJCFeaturedAppModel *featuredAppModelObj_;
	int featuredAppDisplayCount_;
}

@property (nonatomic, retain) TJCFeaturedAppModel *featuredAppModelObj;
@property (nonatomic, assign) int featuredAppDisplayCount;

+ (TJCFeaturedAppManager*) sharedTJCFeaturedAppManager;

/*!	\fn getFeaturedApp
 *	\brief Initiates a URL request to get the featured application data.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)getFeaturedApp;

/*!	\fn getFeaturedAppWithCurrencyID:(NSString*)currencyID
 *	\brief Initiates a URL request to get the featured application data.
 *
 *	\param currencyID The GUID for the currency earned when completing this featured ad.
 *	\return n/a
 */
- (void)getFeaturedAppWithCurrencyID:(NSString*)currencyID;

/*!	\fn fetchResponseSuccessWithData:withRequestTag:(void* dataObj, int aTag)
 *	\brief Called when the request for the featured app data succeeeds.
 *
 *	\param dataObj The featured application data returned from the server.
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

/*!	\fn setFeaturedAppDisplayCount:(int)displayCount
 *	\brief Sets the display count for featured app.
 *
 *	\param displayCount The number of times that the featured app should be displayed.
 *	\return n/a
 */
- (void)setFeaturedAppDisplayCount:(int)displayCount;

/*!	\fn releaseFeaturedAppHandler
 *	\brief Releases the featured application handler object.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)releaseFeaturedAppHandler;

@end
