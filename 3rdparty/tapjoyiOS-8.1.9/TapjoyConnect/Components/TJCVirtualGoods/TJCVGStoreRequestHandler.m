// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCVGStoreRequestHandler.h"
#import "TapjoyConnect.h"
#import "TJCCoreFetcher.h"
#import "TJCUserAccountManager.h"
#import "TJCVGNotificationsConstants.h"
#import "TJCVGStoreManager.h"
#import "TJCConstants.h"

@implementation TJCVGStoreRequestHandler



- (id)initRequestWithDelegate:(id<TJCFetchResponseDelegate>)aDelegate andRequestTag:(int)aTag
{
	if((self = [super initRequestWithDelegate:aDelegate andRequestTag:aTag]))
	{
		
	}
	return self;
}


- (void)getAllVGStoreItems
{
	//Just making sure only the right object calls this method
	if(!(requestTag_ == kTJCRequestTagAllItems || requestTag_ == kTJCRequestTagAllItemsCount || requestTag_ == kTJCRequestTagUpdateItems))
	{
		return;
	}
	
	NSMutableDictionary *paramDict = [[TapjoyConnect sharedTapjoyConnect] genericParameters];
	
	NSString *requestString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL, TJC_URL_PARAM_VG_ITEMS_ALL];
	NSString *alternateString = [NSString stringWithFormat:@"%@%@", TJC_SERVICE_URL_ALTERNATE, TJC_URL_PARAM_VG_ITEMS_ALL];
	
	int start = [[TJCVGStoreManager sharedTJCVGStoreManager] startingIndex];
	int max = TJC_URL_PARAM_VG_PAGE_INDEX;
	
	if (requestTag_ == kTJCRequestTagAllItemsCount)
	{
		//means we just need to get the count of available items
		start = 0;
		max = 0;
	}
	else if (requestTag_ == kTJCRequestTagUpdateItems)
	{
		start = 0;
		max = TJC_URL_PARAM_VG_MAX_INDEX;
	}
	
	NSString *startStr = [NSString stringWithFormat:@"%d", start];
	NSString *maxStr =  [NSString stringWithFormat:@"%d", max];
	
	[paramDict setObject:startStr forKey:TJC_URL_PARAM_VG_ITEMS_START];
	[paramDict setObject:maxStr forKey:TJC_URL_PARAM_VG_ITEMS_MAX];
	
	// Set user ID.
	if ([TapjoyConnect getUserID])
	{
		[paramDict setObject:[TapjoyConnect getUserID] forKey:TJC_URL_PARAM_USER_ID];
	}
	
	[self makeGenericRequestWithURL:requestString 
							 alternateURL:alternateString 
									 params:paramDict 
								  selector:@selector(getAllVGStoreItemsResponse:)];
}


- (void)getAllVGStoreItemsResponse:(TJCCoreFetcher*)myFetcher
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"AllVGStoreItems Response Returned"];
	
	TBXMLElement *tjcConnectRetObject = [self validateResponseReturnedObject:myFetcher];
	
	if(!tjcConnectRetObject) 
		return;
	
	[[TJCUserAccountManager sharedTJCUserAccountManager] updateUserAccountObjWithTBXMLElement:
	 [TBXML childElementNamed:@"UserAccountObject" parentElement:tjcConnectRetObject]];
	
	if(myFetcher.requestTag == kTJCRequestTagAllItemsCount)
	{
		[deleg_ fetchResponseSuccessWithData:tjcConnectRetObject withRequestTag:myFetcher.requestTag];
		return;
	}
	
	int moreValue = [TBXML numberForElement:[TBXML childElementNamed:@"MoreDataAvailable" parentElement:tjcConnectRetObject]];
	
	[[TJCVGStoreManager sharedTJCVGStoreManager] setMoreDataValue: moreValue] ;
	
	int startIndex = [[TJCVGStoreManager sharedTJCVGStoreManager] startingIndex];
	
	startIndex = startIndex + TJC_URL_PARAM_VG_PAGE_INDEX;
	
	[[TJCVGStoreManager sharedTJCVGStoreManager] setStartingIndex:startIndex];
	
	TBXMLElement *vgStore = [TBXML childElementNamed:@"AllVGStoreItemsArray" parentElement:tjcConnectRetObject];
	
	if(!vgStore)
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"%s: %d; %s; No data received while attempting to fetch the data from the server", __FILE__, __LINE__, __PRETTY_FUNCTION__];
		
		[deleg_ fetchResponseError:kTJCStatusNotOK errorDescription:nil requestTag:myFetcher.requestTag];
		return;
	}
	
	[deleg_ fetchResponseSuccessWithData:vgStore withRequestTag:myFetcher.requestTag];
}


- (void)requestPurchasedVGStoreItems
{
	//Just making sure only the right object calls this method
	if (!(requestTag_ == kTJCRequestTagPurchasedItems || requestTag_ == kTJCRequestTagPurchasedItemsWithPrompt))
	{
		return;
	}
	
	NSMutableDictionary *paramDict = [[TapjoyConnect sharedTapjoyConnect] genericParameters];
	
	NSString *requestString = [NSString stringWithFormat:@"%@%@",TJC_SERVICE_URL, TJC_URL_PARAM_VG_ITEMS_PURCHASED];
	NSString *alternateString = [NSString stringWithFormat:@"%@%@",TJC_SERVICE_URL_ALTERNATE, TJC_URL_PARAM_VG_ITEMS_PURCHASED];
	
	int startIndex = [[TJCVGStoreManager sharedTJCVGStoreManager] startingIndexPurchased];
	
	NSString *startStr = [NSString stringWithFormat:@"%d", startIndex];
	NSString *maxStr =  [NSString stringWithFormat:@"%d", 100000];
	
	[paramDict setObject:startStr forKey:TJC_URL_PARAM_VG_ITEMS_START];
	[paramDict setObject:maxStr forKey:TJC_URL_PARAM_VG_ITEMS_MAX];
	
	// Set user ID.
	if ([TapjoyConnect getUserID])
	{
		[paramDict setObject:[TapjoyConnect getUserID] forKey:TJC_URL_PARAM_USER_ID];
	}
	
	NSMutableDictionary *params = [NSMutableDictionary dictionaryWithDictionary:paramDict];
	
	[self makeGenericRequestWithURL:requestString 
							 alternateURL:alternateString 
									 params:params
								  selector:@selector(requestPurchasedVGStoreItemsResponse:)];
}


- (void)requestPurchasedVGStoreItemsResponse:(TJCCoreFetcher*)myFetcher
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Purchased VGStoreItems Response Returned"];
	
	TBXMLElement *tjcConnectRetObject = [self validateResponseReturnedObject:myFetcher];
	
	if(!tjcConnectRetObject)
		return;
	
	//TODO:Cache User Account Object
	[[TJCUserAccountManager sharedTJCUserAccountManager] updateUserAccountObjWithTBXMLElement:
	 [TBXML childElementNamed:@"UserAccountObject" parentElement:tjcConnectRetObject]];
	
	int moreValue = [TBXML numberForElement:[TBXML childElementNamed:@"MoreDataAvailable" parentElement:tjcConnectRetObject]];
	
	[[TJCVGStoreManager sharedTJCVGStoreManager] setMoreDataValuePurchased: moreValue] ;
	
	int startIndex = [[TJCVGStoreManager sharedTJCVGStoreManager] startingIndexPurchased];
	
	startIndex += TJC_URL_PARAM_VG_PAGE_INDEX;
	
	[[TJCVGStoreManager sharedTJCVGStoreManager] setStartingIndexPurchased:startIndex];
	
	TBXMLElement *vgStore = [TBXML childElementNamed:@"PurchasedVGStoreItemsArray" parentElement:tjcConnectRetObject];
	
	if(!vgStore)
	{
		[TJCLog logWithLevel: LOG_NONFATAL_ERROR
						  format: @"%s: %d; %s; No data received while attempting to fetch the data from the server", __FILE__, __LINE__, __PRETTY_FUNCTION__];
		
		[deleg_ fetchResponseError:kTJCStatusNotOK errorDescription:nil requestTag:myFetcher.requestTag];
		return;
	}
	
	[deleg_ fetchResponseSuccessWithData:vgStore withRequestTag:myFetcher.requestTag];
}


- (void)buyVGStoreItemWithID:(NSString*)itemID
{
	if(requestTag_ != kTJCRequestTagItemPurchased)//Just making sure only the right object calls this method
	{
		return;
	}
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"Virtual Good Request Starting with Item Id %@ ",itemID];
	NSString *requestString = [NSString stringWithFormat:@"%@%@",TJC_SERVICE_URL,TJC_URL_PARAM_PURCHASE_VG];
	NSString *alternateString = [NSString stringWithFormat:@"%@%@",TJC_SERVICE_URL_ALTERNATE,TJC_URL_PARAM_PURCHASE_VG];
	
	NSMutableDictionary *genericParams = [[TapjoyConnect sharedTapjoyConnect] genericParameters];
	
	[genericParams setObject:itemID forKey:TJC_URL_PARAM_VG_ID];
	
	[self makeGenericRequestWithURL:requestString 
							 alternateURL:alternateString 
									 params:genericParams 
								  selector:@selector(buyVGStoreItemWithIDResponse:)];
}


- (void)buyVGStoreItemWithIDResponse:(TJCCoreFetcher*)myFetcher
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Virtual Good Purchase Response Returned"];
	
	TBXMLElement *tjcConnectRetObject = [self validateResponseReturnedObject:myFetcher];
	
	if(!tjcConnectRetObject) return;
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"Virtual Good Purchase Response Data Success, now caching data"];
	
	[[TJCUserAccountManager sharedTJCUserAccountManager] updateUserAccountObjWithTBXMLElement:
	 [TBXML childElementNamed:@"UserAccountObject" parentElement:tjcConnectRetObject]];
	
	[deleg_ fetchResponseSuccessWithData:tjcConnectRetObject withRequestTag:myFetcher.requestTag];
}


- (void)dealloc
{
	//[TJCLog logWithLevel:LOG_DEBUG format:@"TJCVGStoreRequestHandler dealloc"];
	[super dealloc];
}

@end
