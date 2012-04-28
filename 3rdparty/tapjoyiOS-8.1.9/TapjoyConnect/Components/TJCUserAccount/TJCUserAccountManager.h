// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <Foundation/Foundation.h>
#import "TJCFetchResponseProtocol.h"
#import "TapjoyConnect.h"
#import "TBXML.h"


typedef enum
{
	kTJCUserAccountRequestTagGetPoints = 0,
	kTJCUserAccountRequestTagSpendPoints,
	kTJCUserAccountRequestTagAwardPoints,
	kTJCUserAccountRequestTagMAX
} TJCUserAccountRequestTag;

@class TJCUserAccountRequestHandler;
@class TJCUserAccountModel;

/*!	\interface TJCUserAccountManager
 *	\brief The Tapjoy Connect User Account Manager class.
 *
 * The user account manager class handles the account manager model object, which contains user information such as currency name and amount.
 * The user data is used by the virtual goods SDK.
 */
@interface TJCUserAccountManager : NSObject <TJCFetchResponseDelegate>
{
	TJCUserAccountModel *userAccountModelObj_;					/*!< The user account model object contains currency name and amount, among other data. Refer to #TJCUserAccountModel documentation for more info. */
	TJCUserAccountRequestHandler *userAccountGetTapPointsObj_;
	TJCUserAccountRequestHandler *userAccountSpendTapPointsObj_;
	
	BOOL waitingForResponse_;	/*!< Indicates whether the client is waiting for a response from the server. User account methods will not do anything if this is YES. */
}

+ (TJCUserAccountManager*) sharedTJCUserAccountManager;

/*!	\fn getTapPoints
 *	\brief Initiates the URL request to get the currency name and amount for the current user.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)getTapPoints;

/*!	\fn spendTapPoints:(int)points
 *	\brief Initiates the URL request to spend the given amount of virtual currency.
 *
 *	\param points The amount of virtual currency to spend.
 *	\return n/a
 */
- (void)spendTapPoints:(int)points;

/*!	\fn awardTapPoints:(int)points
 *	\brief Initiates the URL request to award the given amount of virtual currency.
 *
 *	\param points The amount of virtual currency to award.
 *	\return n/a
 */
- (void)awardTapPoints:(int)points;

/*!	\fn fetchResponseSuccessWithData:withRequestTag:(void* dataObj, int aTag)
 *	\brief This method is called when the URL request succeeds.
 *
 *	\param dataObj The data object is XML data that contains user info including currency name and amount.
 *	\param aTag The tag id associated with the request.
 *	\return n/a
 */
- (void)fetchResponseSuccessWithData:(void*)dataObj withRequestTag:(int)aTag;

/*!	\fn fetchResponseError:errorDescription:requestTag:(TJCResponseError errorType, id errorDescObj, int aTag)
 *	\brief This method is raised when the URL request fails.
 *
 *	\param errorType Contains error information.
 *	\param errorDescObj Contains error data.
 *	\param aTag ID associated with the request.
 *	\return n/a
 */
- (void)fetchResponseError:(TJCResponseError)errorType errorDescription:(id)errorDescObj requestTag:(int) aTag;

/*!	\fn updateUserAccountObjWithTBXMLElement:(TBXMLElement*) userAccElement
 *	\brief Updates the user account data.
 *
 *	\param userAccElement The XML data that contains user info including currency name and amount.
 *	\return n/a
 */
- (void)updateUserAccountObjWithTBXMLElement:(TBXMLElement*) userAccElement;

/*!	\fn releaseUserAccount
 *	\brief Releases the user account model object and the request handler object.
 *
 *	\param n/a
 *	\return n/a
 */
- (void)releaseUserAccount;

@property(nonatomic,readonly) TJCUserAccountModel *userAccountModelObj;

@end



@interface TapjoyConnect (TJCUserAccountManager)

/*!	\fn getTapPoints
 *	\brief Requests for Tap Points (Virtual Currency) notify via TJC_TAP_POINTS_RESPONSE_NOTIFICATION notification.
 *
 *	\param n/a
 *	\return n/a
 */
+ (void)getTapPoints;

/*!	\fn spendTapPoints:(int)points
 *	\brief Updates the virtual currency for the user with the given spent amount of currency.
 *
 * If the spent amount exceeds the current amount of currency the user has, nothing will happen.
 *	\param points The amount of currency to subtract from the current total amount of currency the user has.
 *	\return n/a
 */
+ (void)spendTapPoints:(int)points;

/*!	\fn awardTapPoints:(int)points
 *	\brief Updates the virtual currency for the user with the given awarded amount of currency.
 *
 *	\param points The amount of currency to add to the current total amount of currency the user has.
 *	\return n/a
 */
+ (void)awardTapPoints:(int)points;

/*!	\fn showDefaultEarnedCurrencyAlert
 *	\brief Shows a UIAlert that tells the user how much currency they just earned.
 *
 *	\param n/a
 *	\return n/a
 */
+ (void)showDefaultEarnedCurrencyAlert;

@end
