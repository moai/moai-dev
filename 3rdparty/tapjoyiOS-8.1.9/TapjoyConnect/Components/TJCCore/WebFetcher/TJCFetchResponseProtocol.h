// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

/*!	\enum TJCResponseError
 *	\brief Tapjoy Connect response error codes.
 */
typedef enum TJCResponseError
{
	kTJCInternetFailure = 0,
	kTJCStatusNotOK = 1,
	kTJCRequestTimeOut = 2
} TJCResponseError;

/*!	\protocol TJCFetchResponseDelegate
 *	\brief The Tapjoy fetch response protocol. 
 */
@protocol TJCFetchResponseDelegate <NSObject>
@required

/*!	\fn fetchResponseSuccessWithData:withRequestTag:(void* dict, int aTag)
 *	\brief Called when a request succeeds.
 *
 *	\param dataObj
 *	\param aTag
 *	\return n/a
 */
-(void)fetchResponseSuccessWithData:(void*)dataObj withRequestTag:(int)aTag;

/*!	\fn fetchResponseError:errorDescription:requestTag:(TJCResponseError errorType, id errorDescObj, int aTag)
 *	\brief Called when an error occurs.
 *
 *	\param errorType
 *	\param errorDescObj
 *	\param aTag
 *	\return n/a
 */
-(void)fetchResponseError:(TJCResponseError)errorType errorDescription:(id)errorDescObj requestTag:(int) aTag;
@end

