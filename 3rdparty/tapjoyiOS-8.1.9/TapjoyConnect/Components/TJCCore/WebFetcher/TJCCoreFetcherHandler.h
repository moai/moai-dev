// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>
#import "TJCFetchResponseProtocol.h"

extern NSString *kTJCCoreFetcherReturnObjStr;
extern NSString *kTJCCoreFetcherAdStr;

/*!	\protocol TJCWebFetcherDelegate
 *	\brief The Tapjoy Connect Web Fetcher Delegate protocol. 
 */
@protocol TJCWebFetcherDelegate

@required

/*!	
 *	\brief Initializes the TJCFetchRepsonseDelegate with the given delegate and tag.
 *
 * Every web handler must implement this function.
 *	\param aDelegate The delegate to set.
 *	\param aTag The request tag.
 *	\return The initialized #TJCFetchResponseDelegate object.
 */
-(id) initRequestWithDelegate:(id<TJCFetchResponseDelegate>) aDelegate andRequestTag:(int)aTag;

@end




#import "TBXML.h"

@class TJCCoreFetcher;

/*!	\interface TJCCoreFetcherHandler
 *	\brief The Tapjoy Connect Core Fetcher Handler class.
 *
 */
@interface TJCCoreFetcherHandler : NSObject 
{
	id<TJCFetchResponseDelegate> deleg_;	/*!< The delegate for this object. */
	int requestTag_;								/*!< Used for distinguishing the web request and its response. */
	TJCCoreFetcher *myFetcher_;				/*!< The #TJCCoreFetcher is used for handling URL requests. */
}

/*!	
 *	\brief Initializes the TJCFetchRepsonseDelegate with the given delegate and tag.
 *
 * Every web handler must implement this function.
 *	\param aDelegate The delegate to set.
 *	\param aTag The request tag.
 *	\return The initialized #TJCFetchResponseDelegate object.
 */
- (id) initRequestWithDelegate:(id<TJCFetchResponseDelegate>) aDelegate andRequestTag:(int)aTag;

/*!	\fn makeGenericRequestWithURL:alternateURL:params:selector:(NSString* aRequestString, NSString* alterURL, NSMutableDictionary* aParamsList, SEL aSelector)
 *	\brief Sets up and makes a URL request.
 *
 * #myFetcher_ handles the responses from the server.
 *	\param aRequestString The URL request string.
 *	\param alterURL The alternate URL request string if the main one returns an error or times out.
 *	\param aParamsList The parameter list.
 *	\param aSelector The selector that will invoke the URL request.
 *	\return n/a
 */
- (void) makeGenericRequestWithURL:(NSString *)aRequestString 
							 alternateURL:(NSString*)alterURL 
									 params:(NSMutableDictionary *)aParamsList 
								  selector:(SEL)aSelector;

/*!	\fn makeGenericPOSTRequestWithURL:alternateURL:data:params:selector:(NSString* aRequestString, NSString* alterURL, NSData* data, NSMutableDictionary* aParamsList, SEL aSelector)
 *	\brief Sets up and makes a URL request.
 *
 * #myFetcher_ handles the responses from the server.
 *	\param aRequestString The URL request string.
 *	\param alternateURL The alternate URL request string if the main one returns an error or times out.
 *	\param data The data to send to the server as POST data.
 *	\param aParamsList The parameter list.
 *	\param aSelector The selector that will invoke the URL request.
 *	\return n/a
 */
- (void) makeGenericPOSTRequestWithURL:(NSString *)aRequestString 
								  alternateURL:(NSString *)alterURL
											 data:(NSData*)data
										  params:(NSMutableDictionary *)aParamsList 
										selector:(SEL)aSelector;

/*!	\fn makeRequestWithURL:withInvoker:(TJCCoreFetcher* myFetcher, NSInvocation* invoker)
 *	\brief Initiates an synchronous URL request with the given NSInvocation.
 *
 *	\param myFetcher The #TJCCoreFetcher to handle the URL response(s).
 *	\param invoker The NSInvocation that invokes the URL request.
 *	\return n/a
 */
- (void) makeRequestWithURL:(TJCCoreFetcher *)myFetcher withInvoker:(NSInvocation *)invoker;

/*!	\fn parseReturnObjectAsTBXMLElement:(TJCCoreFetcher*) myFetcher
 *	\brief Parses the response data.
 *
 * After a reponse is received, the data must be parsed and stored. This method also checks for any errors that may have occured.
 *	\param myFetcher The #TJCCoreFetcher that contains the response data.
 *	\return The NSDictionary will have all the parsed data contained in it if the connection and response was successful.
 */
- (TBXMLElement*) parseReturnObjectAsTBXMLElement:(TJCCoreFetcher *) myFetcher;

/*!	\fn validateResponseReturnedObject:(TJCCoreFetcher*) myFetcher
 *	\brief Checks the response object for valid data.
 *
 *	\param myFetcher The #TJCCoreFetcher that contains the response data.
 *	\return Nil of the response data is invalid, an XML element otherwise.
 */
- (TBXMLElement*) validateResponseReturnedObject:(TJCCoreFetcher*) myFetcher;


@end
