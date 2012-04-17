// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>
#import "TJCLog.h"


/*!	\interface TJCCoreFetcher
 *	\brief The Tapjoy Connect Core Fetcher class.
 *
 */
@interface TJCCoreFetcher : NSObject 
{
	
	int requestTag_;						/*!< Used for distinguishing the web request and its response. */
	int retryCount_;						/*!< Keeps track of the retry count for the NSURLConnection requests. */
	NSInteger responseCode_;
	NSTimeInterval	requestTimeout_;	/*!< The time out time in seconds for an NSURLConnection request. */
	NSURLConnection *connection_;		/*!< Used to provide support to perform the loading of a URL request. Delegate methods are defined to handle when a response is receive with associated data. This is used for asynchronous requests only. */
	NSInvocation *invocation_;			/*!< Used to invoke a NSURLConnection request, specifically after a Tap Point is received. This is used for asynchronous requests only. */
	NSString *requestMethod_;			/*!< Used to set NSMutableURLRequest setHTTPMethod. Values can be "GET", "POST", etc... */
	NSDictionary *postParameters_;	/*!< Holds the parameter list when a NSURLConnection request is made. */
	BOOL hasFetched_;						/*!< Indicates whether data has been fetched from calling NSURLConnection. */
	NSString *baseURL_;					/*!< Holds the NSString of the base URL used to do the web fetch. */
	NSString *alternateURL_;			/*!< Holds the NSString of the alternate URL used to do the web fetch. */
	NSDictionary *bindings_;			/*!< Holds the parameter list when a NSURLConnection request is made. */
	NSError *error_;						/*!< Used to store error information if an error occured with the NSURLConnection request. */
	NSMutableData *data_;				/*!< Used to store the data received from the NSURLConnection request. */
	NSData *POSTdata_;					/*!< Used to hold the data to be sent to the server. */
}

@property int requestTag;
@property (nonatomic, readonly) int retryCount;
@property (nonatomic, retain) NSInvocation *invocation;
@property (nonatomic, retain) NSString *requestMethod;
@property (nonatomic, retain) NSDictionary *postParameters;
@property (nonatomic) BOOL hasFetched;
@property (nonatomic, retain) NSString *baseURL;
@property (nonatomic, retain) NSString *alternateURL;
@property (nonatomic, retain) NSDictionary *bindings;
@property (nonatomic, retain) NSError *error;
@property (nonatomic, retain) NSMutableData *data;
@property (nonatomic, retain) NSData *POSTdata;
@property NSInteger responseCode;
@property (nonatomic) NSTimeInterval requestTimeout;
@property (nonatomic, retain) NSURLConnection * connection;


/*!	\fn fetchAsynchronouslyWithCompletionInvocation: (NSInvocation *) myInvocation
 *	\brief Fetches data asynchronously.
 *
 * Delegate methods for #connection are invoked when a response is received.
 *	\param myInvocation The NSInvocation when a response is received.
 *	\return n/a
 */
- (void) fetchAsynchronouslyWithCompletionInvocation: (NSInvocation *) myInvocation;

- (void)initiateConnection;

/*!	\fn retry
 *	\brief Initiates an asynchonous URL request and increments the retry counter.
 *
 *	\param n/a
 *	\return n/a
 */
- (void) retry;

/*!	\fn requestURL
 *	\brief Retrieves the #baseURL if the retry count is zero, or the #alternateURL otherwise.
 *
 *	\param n/a
 *	\return The #baseURL or #alternateURL depending on retry count.
 */
- (NSString *) requestURL;

/*!	\fn urlEncodedBindings
 *	\brief Retrieves the parameter list when a URL request is made.
 *
 *	\param n/a
 *	\return An NSString of all the URL parameters.
 */
- (NSString *) urlEncodedBindings;

/*!	\fn hasError
 *	\brief Indicates whether an error has occured with the URL request.
 *
 *	\param n/a
 *	\return TRUE is an error has occured, FALSE otherwise.
 */
- (BOOL) hasError;

@end
