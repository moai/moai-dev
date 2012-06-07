// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCVGStoreManager.h"
#import "TJCVGStoreRequestHandler.h"
#import "TJCVGStoreModel.h"
#import "TBXML.h"
#import "TJCVGNotificationsConstants.h"
#import "TJCVGStoreModel.h"
#import "TJCSqliteController.h"
#import "TJCVGViewHandler.h"
#import "SynthesizeSingleton.h"
#import "TapjoyConnect.h"
#import "TJCFileDownloadHandler.h"



@implementation TJCVGStoreManager


TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCVGStoreManager)


@synthesize vgStoreModelObj = vgStore_ ;
@synthesize startingIndex = startingIndex_, moreDataValue =moreDataValue_;
@synthesize startingIndexPurchased = startingIndexPurchased_, moreDataValuePurchased =moreDataValuePurchased_;
@synthesize numItemsToBeDownloaded = numItemsToBeDownloaded_;

- (id)init
{
	if ((self = [super init]))
	{
		vgStore_ = [[TJCVGStoreModel alloc] init];
		vgReqHandlerAllItems_ = nil;
		vgReqHandlerAvailableItems_ = nil;
		vgReqHandlerPurchasedItems_ = nil;
		vgReqHandlerPurchasedItemsWithPrompt_ = nil;
		vgReqHandlerBuyItem_ = nil;
	}
	return self;
}


- (void)initiateRequestForAllVGStoreItems
{
	if (vgReqHandlerAllItems_)
	{
		[vgReqHandlerAllItems_ release];
		vgReqHandlerAllItems_ = nil;
	}
	
	vgReqHandlerAllItems_ = [[TJCVGStoreRequestHandler alloc] initRequestWithDelegate:self 
																							  andRequestTag:kTJCRequestTagAllItems];
	[vgReqHandlerAllItems_ getAllVGStoreItems];
}


- (void)initiateRequestForAvailableVGStoreItems
{
	if (vgReqHandlerAvailableItems_)
	{
		[vgReqHandlerAvailableItems_ release];
		vgReqHandlerAvailableItems_ = nil;
	}
	
	vgReqHandlerAvailableItems_ = [[TJCVGStoreRequestHandler alloc] initRequestWithDelegate:self 
																									  andRequestTag:kTJCRequestTagAllItemsCount];
	[vgReqHandlerAvailableItems_ getAllVGStoreItems];
}


- (void)initiateRequestForPurchasedVGStoreItems
{
	if (vgReqHandlerPurchasedItems_)
	{
		[vgReqHandlerPurchasedItems_ release];
		vgReqHandlerPurchasedItems_ = nil;
	}
	
	vgReqHandlerPurchasedItems_ = [[TJCVGStoreRequestHandler alloc] initRequestWithDelegate:self 
																									  andRequestTag:kTJCRequestTagPurchasedItems];
	[vgReqHandlerPurchasedItems_ requestPurchasedVGStoreItems];
	
}


- (void)initiateRequestForPurchasedVGStoreItemsWithPrompt
{
	if (vgReqHandlerPurchasedItemsWithPrompt_)
	{
		[vgReqHandlerPurchasedItemsWithPrompt_ release];
		vgReqHandlerPurchasedItemsWithPrompt_ = nil;
	}
	
	vgReqHandlerPurchasedItemsWithPrompt_ = [[TJCVGStoreRequestHandler alloc] initRequestWithDelegate:self 
																													andRequestTag:kTJCRequestTagPurchasedItemsWithPrompt];
	[vgReqHandlerPurchasedItemsWithPrompt_ requestPurchasedVGStoreItems];
}


- (void)buyVGStoreItemWithID:(NSString *)itemID
{
	if (vgReqHandlerBuyItem_)
	{
		[vgReqHandlerBuyItem_ release];
		vgReqHandlerBuyItem_ = nil;
	}
	
	vgReqHandlerBuyItem_ = [[TJCVGStoreRequestHandler alloc] initRequestWithDelegate:self 
																							 andRequestTag:kTJCRequestTagItemPurchased];
	[vgReqHandlerBuyItem_ buyVGStoreItemWithID:itemID];
}


- (void)initiateRequestForUpdateVGStoreItems
{
	if (vgReqHandlerUpdateItems_)
	{
		[vgReqHandlerUpdateItems_ release];
		vgReqHandlerUpdateItems_ = nil;
	}
	
	vgReqHandlerUpdateItems_ = [[TJCVGStoreRequestHandler alloc] initRequestWithDelegate:self 
																								  andRequestTag:kTJCRequestTagUpdateItems];
	[vgReqHandlerUpdateItems_ getAllVGStoreItems];
}


// called when request succeeeds
- (void)fetchResponseSuccessWithData:(void*)dataObj withRequestTag:(int)aTag
{	
	switch (aTag) 
	{
		case kTJCRequestTagAllItems:
		{
			// The VGStoreItemReturnClass holds one virtual good item data.
			TBXMLElement *vgStoreItemArray = [TBXML childElementNamed:@"VGStoreItemReturnClass" parentElement:dataObj];
			
			// Clear out any previously added objects.
			[[vgStore_ allItems] removeAllObjects];
			
			// Iterate through all the virtual good items that have been returned from the server.
			while (vgStoreItemArray)
			{
				TJCVGStoreItem *storeItemObj = [[TJCVGStoreItem alloc] initWithXMLElement:vgStoreItemArray];
				[vgStore_ addObjectForAllItems:storeItemObj];
				[storeItemObj release];
				vgStoreItemArray = [TBXML nextSiblingNamed:@"VGStoreItemReturnClass" searchFromElement:vgStoreItemArray];
			}
			//For the time being send notifications.
			
			[[NSNotificationCenter defaultCenter] postNotificationName:SUBMIT_ALL_VG_STORE_ITEMS_RECIEVE_DATA object:nil];
			
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_AVAILABLE_ITEMS_RESPONSE_NOTIFICATION object:nil];
		}
			break;
			
		case kTJCRequestTagAllItemsCount:
		{
			//int availableItems = [TBXML numberForElement:[TBXML childElementNamed:@"MoreDataAvailable" parentElement:dataObj]];
			
			[vgReqHandlerAvailableItems_ release];
			vgReqHandlerAvailableItems_ = nil;
			
			//Nothing in model and RequestHandler has done its job
		}
			break;
			
			
		case kTJCRequestTagPurchasedItems:
		{
			TBXMLElement *vgStoreItemArray = [TBXML childElementNamed:@"VGStoreItemReturnClass" parentElement:dataObj];
			
			// Clear out any previously added objects.
			[[vgStore_ purchasedItems] removeAllObjects];
			
			while (vgStoreItemArray)
			{
				TJCVGStoreItem *storeItemObj = [[TJCVGStoreItem alloc] initWithXMLElement:vgStoreItemArray];
				[vgStore_ addObjectForPurchasedItems:storeItemObj];
				[storeItemObj release];
				vgStoreItemArray = [TBXML nextSiblingNamed:@"VGStoreItemReturnClass" searchFromElement:vgStoreItemArray];
			}
			
			[[NSNotificationCenter defaultCenter] postNotificationName:SUBMIT_PURCHASED_VG_STORE_ITEMS_RECIEVE_DATA object:nil];
			
			// Fire notification that purchased items have been retrieved.
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_PURCHASED_ITEMS_RESPONSE_NOTIFICATION object:nil];
		}			
			break;
			
		case kTJCRequestTagPurchasedItemsWithPrompt:
		{
			NSMutableArray *allPurchasedVGStoreArray = [[NSMutableArray alloc] init];
			TBXMLElement *vgStoreItemArray = [TBXML childElementNamed:@"VGStoreItemReturnClass" parentElement:dataObj];
			
			while (vgStoreItemArray)
			{
				// Create the store item object from the xml data and store it in the temporary items array.
				TJCVGStoreItem *storeItemObj = [[TJCVGStoreItem alloc] initWithXMLElement:vgStoreItemArray];
				[allPurchasedVGStoreArray addObject:storeItemObj];
				[storeItemObj release];
				
				// Move to the next xml tag.
				vgStoreItemArray = [TBXML nextSiblingNamed:@"VGStoreItemReturnClass" searchFromElement:vgStoreItemArray];
			}
			
			// Check whether any virtual goods needs to be updated locally.
			BOOL shouldShowVGDownloadAlert = [self updateLocalPurchasedVGItemsWithItemArray:allPurchasedVGStoreArray];
			
			// Display the alert to redownload the virtual goods.
			if (shouldShowVGDownloadAlert)
			{
				UIAlertView *alertview = [[UIAlertView alloc] initWithTitle:@"" 
																					 message:[NSString stringWithFormat: @"Some items have been updated, would you like to view them now?"] 
																					delegate:[TJCVGViewHandler sharedTJCVGViewHandler] 
																		cancelButtonTitle:@"Cancel" 
																		otherButtonTitles:@"Ok",nil];
				
				[alertview show];
				[alertview release];
			}
			
			// Nothing done to Model and Handler has done its Job			
			[vgReqHandlerPurchasedItemsWithPrompt_ release];
			vgReqHandlerPurchasedItemsWithPrompt_ = nil;
			
			[allPurchasedVGStoreArray removeAllObjects];
			[allPurchasedVGStoreArray release];
		}
			break;
			
		case kTJCRequestTagUpdateItems:
		{
			numItemsToBeDownloaded_ = 0;
			numItemsUpdatedCount_ = 0;
			
			// Clear out any previous data and reinit the updated items array. It's grabbed from the xml data below.
			if (updatedItems_)
			{
				[updatedItems_ release];
			}
			updatedItems_ = [[NSMutableArray alloc] init];
			
			// The VGStoreItemReturnClass holds one virtual good item data.
			TBXMLElement *vgStoreItemArray = [TBXML childElementNamed:@"VGStoreItemReturnClass" parentElement:dataObj];
			
			// Iterate through all the virtual good items that have been returned from the server.
			while (vgStoreItemArray)
			{
				// Create the store item object from the xml data and store it in the temporary items array.
				TJCVGStoreItem *storeItemObj = [[TJCVGStoreItem alloc] initWithXMLElement:vgStoreItemArray];
				[updatedItems_ addObject:storeItemObj];
				[storeItemObj release];
				
				// Move to the next xml tag (virtual good item data).
				vgStoreItemArray = [TBXML nextSiblingNamed:@"VGStoreItemReturnClass" searchFromElement:vgStoreItemArray];
			}
			
			// Now remove any items that already exist locally. The remaining items are the items that need to be downloaded from the server.
			[self removeLocalItemsFromItemArray:updatedItems_];
			
			// Get how many items need to be updated.
			numItemsToBeDownloaded_ = [updatedItems_ count];

			// Skip the download logic if there are no item to update.
			if (numItemsToBeDownloaded_ <= 0)
			{
				// This can be safely released since there are no items to update.
				[updatedItems_ release];
				updatedItems_ = nil;
				
				// Now fire the notification that all items have been updated successfully.
				// In this case, there are no items to udpate.
				[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_ALL_ITEMS_SAVED_TO_CLIENT
																					 object:nil];
			}
			else
			{
				// This notification will fire each time an item is downloaded.
				[[NSNotificationCenter defaultCenter] addObserver:self
																	  selector:@selector(itemDownloaded:) 
																			name:ITEM_DOWNLOAD_UPDATED_NOTIFICATION 
																		 object:nil];
				
				// Initiate download for first item
				[self initiateItemUpdate];
				
				// The item update count must be incremented here first, since a download has just been initiated above.
				numItemsUpdatedCount_++;
			}
			
			// Nothing done to Model and Handler has done its Job			
			[vgReqHandlerUpdateItems_ release];
			vgReqHandlerUpdateItems_ = nil;
		}
			break;
			
		case kTJCRequestTagItemPurchased:
		{
			TJCVGItemPurchaseResponse *vgPurchaseReturn = [[TJCVGItemPurchaseResponse alloc] initWithXMLElement:(TBXMLElement *)dataObj];
			[vgStore_ setBuyItemResponse:vgPurchaseReturn]; //release previous one and retains new object
			//No need for this object now
			[vgPurchaseReturn release];
			
			// Fire notification after a purchase has been completed.
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_ITEM_PURCHASE_COMPLETE object:nil];
			
			// Internal notification for when an item has been successfully purchased.
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_ITEM_PURCHASE_RETURN object:nil];
		}	
			break;
			
		default:
			
			break;
	}
}


- (void)removeLocalItemsFromItemArray:(NSMutableArray*)updatedVGItemsArray
{
	// Retrieve the locally saved virtual good items. These are TJCVGStoreItem objects.
	NSMutableArray *localVGItems = [[TJCSqliteController sharedTJCSqliteController] getAllPurchasedItems];
	
	// Check whether each item is up to date to the latest.
	for (int i = 0; i < [updatedVGItemsArray count]; i++)
	{	
		NSString *updatedVGStoreItemID = ((TJCVGStoreItem*)[updatedVGItemsArray objectAtIndex:i]).itemID;
		NSString *updatedVGStoreItemDataHash = ((TJCVGStoreItem*)[updatedVGItemsArray objectAtIndex:i]).itemDataHash;
		NSString *localVGStoreItemID;
		NSString *localVGStoreDataHash;
		
		// Loop through all the ids and compare them to the newly downloaded items.
		for (int j = 0; j < [localVGItems count]; j++) 
		{
			localVGStoreItemID = ((TJCVGStoreItem*)[localVGItems objectAtIndex:j]).itemID;
			localVGStoreDataHash = ((TJCVGStoreItem*)[localVGItems objectAtIndex:j]).itemDataHash;
			
			// If the vg item IDs match, the current item is up to date.
			// Also check against the data hash value. If any are different, it means the virtual good must be re-downloaded and saved.
			if (([updatedVGStoreItemID isEqualToString:localVGStoreItemID]) &&
				 ([updatedVGStoreItemDataHash isEqualToString:localVGStoreDataHash]))
			{
				// Remove the object since it already exists locally on the device.
				[updatedVGItemsArray removeObjectAtIndex:i];
				// Also move the counter back one to account for the smaller array.
				i--;
				break;
			}
		}
	}
}


- (void)itemDownloaded:(id)obj
{
	if (numItemsUpdatedCount_ >= numItemsToBeDownloaded_)
	{
		// This can be safely released since we're done updating items.
		[updatedItems_ release];
		updatedItems_ = nil;
		
		// All items have been updated, remove the item download notification.
		[[NSNotificationCenter defaultCenter] removeObserver:self 
																		name:ITEM_DOWNLOAD_UPDATED_NOTIFICATION
																	 object:nil];
		return;
	}
	
	// Start download for next item.
	[self initiateItemUpdate];
	
	// If this is reached, it means that an item was just successfully downloaded; increment the counter.
	numItemsUpdatedCount_++;
}


- (void)initiateItemUpdate
{
	// Retrieve the locally saved virtual good items. These are TJCVGStoreItem objects.
	NSMutableArray *localVGItems = [[TJCSqliteController sharedTJCSqliteController] getAllPurchasedItems];
	
	// Used as a flag for each item that is checked on whether it is up to date.
	BOOL isItemUpToDate;
	
	// Check whether the item is up to date to the latest.
	isItemUpToDate = NO;
	NSString *updatedVGStoreItemID = ((TJCVGStoreItem*)[updatedItems_ objectAtIndex:numItemsUpdatedCount_]).itemID;
	NSString *updatedVGStoreItemDataHash = ((TJCVGStoreItem*)[updatedItems_ objectAtIndex:numItemsUpdatedCount_]).itemDataHash;
	NSString *localVGStoreItemID;
	NSString *localVGStoreDataHash;
	
	// Loop through all the ids and compare them to the newly downloaded items.
	for (int j = 0; j < [localVGItems count]; j++) 
	{
		localVGStoreItemID = ((TJCVGStoreItem*)[localVGItems objectAtIndex:j]).itemID;
		localVGStoreDataHash = ((TJCVGStoreItem*)[localVGItems objectAtIndex:j]).itemDataHash;
		
		// If the vg item IDs match, the current item is up to date.
		// Also check against the data hash value. If any are different, it means the virtual good must be re-downloaded and saved.
		if (([updatedVGStoreItemID isEqualToString:localVGStoreItemID]) &&
			 ([updatedVGStoreItemDataHash isEqualToString:localVGStoreDataHash]))
		{
			isItemUpToDate = YES;
			break;
		}
	}
	
	// If the item is not up to date, show a prompt asking whether the goods should be re-downloaded.
	if (!isItemUpToDate)
	{
		TJCFileDownloadHandler *fileDownloadHandler = [TJCVGViewHandler sharedTJCVGViewHandler].fileDownloadHandler;
		
		// Initiate item download.
		[fileDownloadHandler downloadItem:[updatedItems_ objectAtIndex:numItemsUpdatedCount_]];
	}
	else
	{
		// The reason we need to increment the update counter here is because we need to make sure itemDownloaded reaches the end case.
		numItemsUpdatedCount_++;
		
		// If the item is up to date, fake an item downloaded notification. 
		// This makes sure that we always eventually reach the TJC_VG_ALL_ITEMS_SAVED_TO_CLIENT notification.
		[self itemDownloaded:nil];
	}
}


- (BOOL)updateLocalPurchasedVGItemsWithItemArray:(NSMutableArray*)updatedVGItemsArray
{
	// Retrieve the locally saved virtual good items. These are TJCVGStoreItem objects.
	NSMutableArray *localVGItems = [[TJCSqliteController sharedTJCSqliteController] getAllPurchasedItems];
	
	// Used as a flag for each item that is checked on whether it is up to date.
	BOOL isItemUpToDate;
	// This is set to YES if any of the items needs to be redownloaded.
	BOOL shouldShowVGDownloadAlert = NO;
	
	// Check whether each item is up to date to the latest.
	for (int i = 0; i < [updatedVGItemsArray count]; i++)
	{	
		isItemUpToDate = NO;
		NSString *updatedVGStoreItemID = ((TJCVGStoreItem*)[updatedVGItemsArray objectAtIndex:i]).itemID;
		NSString *updatedVGStoreItemDataHash = ((TJCVGStoreItem*)[updatedVGItemsArray objectAtIndex:i]).itemDataHash;
		NSString *localVGStoreItemID;
		NSString *localVGStoreDataHash;
		
		// Loop through all the ids and compare them to the newly downloaded items.
		for (int j = 0; j < [localVGItems count]; j++) 
		{
			localVGStoreItemID = ((TJCVGStoreItem*)[localVGItems objectAtIndex:j]).itemID;
			localVGStoreDataHash = ((TJCVGStoreItem*)[localVGItems objectAtIndex:j]).itemDataHash;
			
			// If the vg item IDs match, the current item is up to date.
			// Also check against the data hash value. If any are different, it means the virtual good must be re-downloaded and saved.
			if (([updatedVGStoreItemID isEqualToString:localVGStoreItemID]) &&
				 ([updatedVGStoreItemDataHash isEqualToString:localVGStoreDataHash]))
			{
				isItemUpToDate = YES;
				break;
			}
		}
		
		// If the item is not up to date, show a prompt asking whether the goods should be re-downloaded.
		if (!isItemUpToDate)
		{
			shouldShowVGDownloadAlert = YES;
			
			TJCFileDownloadHandler *fileDownloadHandler = [TJCVGViewHandler sharedTJCVGViewHandler].fileDownloadHandler;
			
			// Initiate item download.
			[fileDownloadHandler downloadItem:[updatedVGItemsArray objectAtIndex:i]];
		}
	}

	return shouldShowVGDownloadAlert;
}


- (void)updateAllLocalVGItemsWithItemArray:(NSMutableArray*)updatedVGItemsArray
{
	// Retrieve the locally saved virtual good items. These are TJCVGStoreItem objects.
	NSMutableArray *localVGItems = [[TJCSqliteController sharedTJCSqliteController] getAllPurchasedItems];
	
	// Used as a flag for each item that is checked on whether it is up to date.
	BOOL isItemUpToDate;
	
	// Check whether each item is up to date to the latest.
	for (int i = 0; i < [updatedVGItemsArray count]; i++)
	{	
		isItemUpToDate = NO;
		NSString *updatedVGStoreItemID = ((TJCVGStoreItem*)[updatedVGItemsArray objectAtIndex:i]).itemID;
		NSString *updatedVGStoreItemDataHash = ((TJCVGStoreItem*)[updatedVGItemsArray objectAtIndex:i]).itemDataHash;
		NSString *localVGStoreItemID;
		NSString *localVGStoreDataHash;
		
		// Loop through all the ids and compare them to the newly downloaded items.
		for (int j = 0; j < [localVGItems count]; j++) 
		{
			localVGStoreItemID = ((TJCVGStoreItem*)[localVGItems objectAtIndex:j]).itemID;
			localVGStoreDataHash = ((TJCVGStoreItem*)[localVGItems objectAtIndex:j]).itemDataHash;
			
			// If the vg item IDs match, the current item is up to date.
			// Also check against the data hash value. If any are different, it means the virtual good must be re-downloaded and saved.
			if (([updatedVGStoreItemID isEqualToString:localVGStoreItemID]) &&
				 ([updatedVGStoreItemDataHash isEqualToString:localVGStoreDataHash]))
			{
				isItemUpToDate = YES;
				break;
			}
		}
		
		// If the item is not up to date, download it to the client now.
		if ((!isItemUpToDate) || ![[TJCSqliteController sharedTJCSqliteController] storeItemIDExists:updatedVGStoreItemID])
		{
			TJCFileDownloadHandler *fileDownloadHandler = [TJCVGViewHandler sharedTJCVGViewHandler].fileDownloadHandler;
			
			// Initiate item download.
			[fileDownloadHandler downloadItem:[updatedVGItemsArray objectAtIndex:i]];
			
			numItemsToBeDownloaded_++;
		}
	}
}


// raised when error occurs
-(void)fetchResponseError:(TJCResponseError)errorType errorDescription:(id)errorDescObj requestTag:(int) aTag
{
	switch (aTag) 
	{
		case kTJCRequestTagAllItems:
			
			[[NSNotificationCenter defaultCenter] postNotificationName:SUBMIT_ALL_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA object:nil];	
			
			break;
		case kTJCRequestTagAllItemsCount:
			
			[vgReqHandlerAvailableItems_ release];
			vgReqHandlerAvailableItems_ = nil;
			
			//Nothing added to model and RequestHandler has done its job
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_AVAILABLE_ITEMS_RESPONSE_NOTIFICATION_ERROR object:nil];
			
			
			break;
			
		case kTJCRequestTagPurchasedItems:
			
			[[NSNotificationCenter defaultCenter] postNotificationName:SUBMIT_PURCHASED_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA object:nil];
			
			break;
			
		case kTJCRequestTagPurchasedItemsWithPrompt:
			
			//Nothing done to Model and Handler has done its Job			
			[vgReqHandlerPurchasedItemsWithPrompt_ release];
			vgReqHandlerPurchasedItemsWithPrompt_ = nil;
			
			[[NSNotificationCenter defaultCenter] postNotificationName:SUBMIT_PURCHASED_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA object:nil];
			
			break;
			
		case kTJCRequestTagItemPurchased:
			
			[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_ITEM_PURCHASE_NET_ERROR object:nil];
			
			break;
			
		default:
			break;
	}
}


- (void) reloadStartForAllVGStoreItems
{
	startingIndex_ = 0;
}


- (void) reloadStartForPurchasedVGStoreItems
{
	startingIndexPurchased_ = 0;
	
}


-(void) updateStartForAllVGStoreItems
{
	startingIndex_ = startingIndex_ + TJC_URL_PARAM_VG_PAGE_INDEX;
}


-(void) updateStartForPurchasedVGStoreItems
{
	startingIndexPurchased_ = startingIndexPurchased_ + TJC_URL_PARAM_VG_PAGE_INDEX;
}


-(int) getMoreDataValueForAllItems
{
	return  moreDataValue_;
}


-(int) getMoreDataValueForPurchasedItems
{
	return  moreDataValuePurchased_;
}


//This needs to be some way to release the model
-(void) releaseStore
{
	if(vgStore_)
	{
		[vgStore_ release];
		vgStore_ = nil;
	}
	[self releaseRequestHandlers];
}


-(void) releaseRequestHandlers
{
	[self releaseRequestHandlersAllItems];
	[self releaseRequestHandlersPurchasedItems];
	[self releaseRequestHandlersBuyItem];
}


-(void) releaseRequestHandlersAllItems
{
	if(vgReqHandlerAllItems_)
	{
		[vgReqHandlerAllItems_ release];
		vgReqHandlerAllItems_ = nil;
	}
}


-(void) releaseRequestHandlersPurchasedItems
{
	if(vgReqHandlerPurchasedItems_)
	{
		[vgReqHandlerPurchasedItems_ release];
		vgReqHandlerPurchasedItems_ = nil;
	}
}


-(void) releaseRequestHandlersBuyItem
{
	if(vgReqHandlerBuyItem_)
	{
		[vgReqHandlerBuyItem_ release];
		vgReqHandlerBuyItem_ = nil;
	}
}


-(void)dealloc
{
	if(vgStore_)
	{
		[vgStore_ release];
		vgStore_ = nil;
	}
	[super dealloc];
}

@end



@implementation TapjoyConnect (TJCVGStoreManager)

+ (int)getNumVGItemsToBeDownloaded
{
	return [[TJCVGStoreManager sharedTJCVGStoreManager] numItemsToBeDownloaded];
}

@end
