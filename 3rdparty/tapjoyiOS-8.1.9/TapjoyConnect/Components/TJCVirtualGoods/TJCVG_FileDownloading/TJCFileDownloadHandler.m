// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCFileDownloadHandler.h"
#import "TJCVGNotificationsConstants.h"
#import "TJCSqliteController.h"
#import "TJCFileDownloadHandler.h"
#import "TJCVGGenericButtons.h"
#import "TJCLog.h"
#import "TJCSqliteController.h"
#import "TJCVGItemDownloader.h"
#import "TJCVGStoreModel.h"
#import "TJCVGStoreManager.h"
#import "TapjoyConnect.h"

@implementation TJCFileDownloadHandler

@synthesize itemsArray;

- (id)init
{
	if ((self = [super init]))
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"Downloader Object created"];
		itemsArray = [[NSMutableDictionary alloc] init];
		[[NSNotificationCenter defaultCenter] addObserver:self
															  selector:@selector(incrementItemDownloadCount:) 
																	name:ITEM_DOWNLOAD_UPDATED_NOTIFICATION 
																 object:nil];
		itemDownloadCount = 0;
	}
	return self;
}


- (void)incrementItemDownloadCount:(id)obj
{
	itemDownloadCount++;
	
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_ITEM_DOWNLOADED_INCREMENT
																		 object:[NSNumber numberWithInt:itemDownloadCount]];
	
	if (itemDownloadCount >= [[TJCVGStoreManager sharedTJCVGStoreManager] numItemsToBeDownloaded])
	{
		[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_ALL_ITEMS_SAVED_TO_CLIENT
																			 object:nil];
		
		[[NSNotificationCenter defaultCenter] removeObserver:self 
																		name:ITEM_DOWNLOAD_UPDATED_NOTIFICATION
																	 object:nil];
		itemDownloadCount = 0;
	}
}


- (BOOL)isItemPresentInLocalBD:(TJCVGStoreItem*)item
{
	// Return if item already present in DB
	if ([[TJCSqliteController sharedTJCSqliteController] storeItemIDExists:[item itemID]]) 
	{
		return YES;
	}
	
	return NO;
}


/**
 * Download data for purchased item
 */
-(void) downloadItem:(TJCVGStoreItem *)obj
{
	// Updated for game state. If there is a matching item, just delete the old row and re-download.
	// This will effectively ensure that the db is always up-to-date.
	
	//    // Return if item already present in DB
	//	if ([[TJCSqliteController sharedTJCSqliteController] storeItemIDExists:[obj itemID]]) 
	//	{
	//        [TJCLog logWithLevel:LOG_DEBUG format:@"Item %@ already exists in DB", [obj itemID]];
	//		return;
	//	}
	
	BOOL initSuccessful;
	
	//Check that the store Item is not already present in the Downlading Dictionary
	TJCVGItemDownloader *dictItem = [itemsArray objectForKey:[obj itemID]];
	if(dictItem)
	{	
		[TJCLog logWithLevel:LOG_DEBUG format:@"Item %@ already available in downloading queue", [obj itemID]];
		return ; // Already downloading in queue
	}	
	
	TJCVGItemDownloader *dlObj = [[TJCVGItemDownloader alloc] initWithObject:obj initSuccessful:&initSuccessful];
	
	if (initSuccessful == NO) 
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"Error: Could not initialize item download"];
		[dlObj release];
		return;
	}
	
	// Store download object in ItemsArray with ItemID as key
	if(obj)
		[TJCLog logWithLevel:LOG_DEBUG format:@"Start Downoad Called for Item Id = %@",[obj itemID]];
	else
		[TJCLog logWithLevel:LOG_DEBUG format:@"Passed VG Item Object is nil"];
	NSString *keyStr = [obj itemID];
	
	[itemsArray setObject:dlObj forKey:keyStr]; // add to dictionary
	
	[self addItemDownloadObservers:dlObj];
	[dlObj startItemDataDownload]; // initiate the downloading process
	[dlObj.loadingTitle setText:@"Downloading ..."];
	[dlObj.progressView setAlpha:1];
	[dlObj.percentTitle setAlpha:1];	
	[dlObj.progressView setProgress:0.0f];
	[dlObj.retryBtn setAlpha:0];
	//[dlObj.infoIcon setAlpha:0];
	
	TJCVGGenericButtons *retBtn = dlObj.retryBtn;
	[retBtn addTarget:self action:@selector(retryBtnAction:) forControlEvents:UIControlEventTouchUpInside];
	[dlObj release]; // decrement in retain count as added in dictionary
}


-(UIView*) getVGPurchasedItemDownloadProgressView:(NSString*) vGStoreItemId
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Downloaded view returned for %@",vGStoreItemId];
	//Get object from dictionary
	TJCVGItemDownloader *tmpObj = [itemsArray objectForKey:vGStoreItemId];
	
	if(tmpObj)
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"Object found for item %@",vGStoreItemId];
		
		
		return tmpObj.downloadProgressContentView;
	}	
	
	else
	{	
		if ([[TJCSqliteController sharedTJCSqliteController] storeItemIDExists:vGStoreItemId]) 
		{
			[TJCLog logWithLevel:LOG_DEBUG format:@"Item already downloaded %@", vGStoreItemId];
			
			
			UIView *tempDownloadProgressContentView = [[UIView alloc] initWithFrame:CGRectMake(55, 66, 229, 20)];
			[tempDownloadProgressContentView setBackgroundColor:[UIColor clearColor]];
			
			UILabel *tempLoadingTitle = [[UILabel alloc] initWithFrame:CGRectMake(7,0, 130, 20)];
			[tempLoadingTitle setTextColor:[UIColor darkGrayColor]];
			[tempLoadingTitle setBackgroundColor:[UIColor clearColor]];
			[tempLoadingTitle setText:@"Download Complete"];
			tempLoadingTitle.font = [UIFont fontWithName:@"Helvetica" size:12];
			[tempDownloadProgressContentView addSubview:tempLoadingTitle];
			[tempLoadingTitle release];
			return [tempDownloadProgressContentView autorelease];
		}
	}
	[TJCLog logWithLevel:LOG_DEBUG format:@"returning nil indictor for %@",vGStoreItemId];
	return nil;
}


/**
 * Add observers for item download notification
 */
- (void)addItemDownloadObservers:(TJCVGItemDownloader *)dlObj
{
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(itemDownloaded:) name:ITEM_DOWNLOAD_SUCCESS_NOTIFICATION object:dlObj];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(itemDownloadError:) name:ITEM_DOWNLOAD_ERROR_NOTIFICATION object:dlObj];
}


/**
 * Remove observers for item download notification
 */
- (void)removeItemDownloadObservers:(NSNotification *)notificationObj
{
	[[NSNotificationCenter defaultCenter] removeObserver:self name:ITEM_DOWNLOAD_SUCCESS_NOTIFICATION object:notificationObj];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:ITEM_DOWNLOAD_ERROR_NOTIFICATION object:notificationObj];
}


/**
 * Item downloaded successfully
 */
- (void)itemDownloaded:(NSNotification *)notificationObj
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"DOWNLOAD COMPLETED"];
	[self removeItemDownloadObservers:notificationObj];
	NSString *key = ((TJCVGItemDownloader *)[notificationObj object]).itemsArrayKey;
	
	//[((TJCVGItemDownloder*)[notificationObj object]).loadingTitle setAlpha:0];//downloading stopped
	[((TJCVGItemDownloader*)[notificationObj object]).retryBtn setAlpha:0];
	[((TJCVGItemDownloader*)[notificationObj object]).loadingTitle setText:@"Download Complete"];//downloading stopped	
	[((TJCVGItemDownloader*)[notificationObj object]).loadingTitle setAlpha:0];//downloading stopped	
	((TJCVGItemDownloader*)[notificationObj object]).downloadComplete = true; 
	[UIView beginAnimations:nil context:nil];
	[UIView setAnimationDuration:0.5];
	
	[((TJCVGItemDownloader*)[notificationObj object]).loadingTitle setAlpha:1];//downloading stopped		
	[((TJCVGItemDownloader*)[notificationObj object]).progressView setAlpha:0];//downloading stopped
	[((TJCVGItemDownloader*)[notificationObj object]).percentTitle setAlpha:0];//downloading stopped
	
	[UIView commitAnimations];
	
	[itemsArray removeObjectForKey:key];
	
	[[NSNotificationCenter defaultCenter] postNotificationName:TJC_VG_ITEM_DOWNLOAD_COMPLETE object:nil];
	
	// Deprecated notification.
	[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_SUCCESS_NOTIFICATION_FOR_PANEL object:nil];
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"Complete END"];
}


-(void)showRetryWithDelay:(NSNotification*)notificationObj
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"download Error"];
	NSString *key = ((TJCVGItemDownloader *)[notificationObj object]).itemsArrayKey;
	
	if(!key)
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"DOWNLOAD ALREADY COMPLETED"];
		return;
	}
	[TJCLog logWithLevel:LOG_DEBUG format:@"download Error for Key %@",key];
	
	TJCVGItemDownloader *downloadObj = [itemsArray objectForKey:key];
	
	if(!downloadObj || downloadObj.downloadComplete)
	{	
		[TJCLog logWithLevel:LOG_DEBUG format:@"DOWNLOAD ALREADY COMPLETED"];
		return; // Item is already downloaded
	}
	
	[((TJCVGItemDownloader*)[notificationObj object]).retryBtn setAlpha:1];//
	//[((TJCVGItemDownloder*)[notificationObj object]).infoIcon setAlpha:1];//downloading stopped
	
	[((TJCVGItemDownloader*)[notificationObj object]).loadingTitle setText:@"Download Error!"];//downloading stopped
	[((TJCVGItemDownloader*)[notificationObj object]).progressView setAlpha:0];//downloading stopped
	[((TJCVGItemDownloader*)[notificationObj object]).percentTitle setAlpha:0];//downloading stopped
}


/**
 * Item download error
 */
- (void)itemDownloadError:(NSNotification *)notificationObj
{
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"download Error"];
	NSString *key = ((TJCVGItemDownloader *)[notificationObj object]).itemsArrayKey;
	
	if(!key)
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"DOWNLOAD ALREADY COMPLETED"];
		return;
	}
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"download Error for Key %@",key];
	
	TJCVGItemDownloader *downloadObj = [itemsArray objectForKey:key];
	
	if(!downloadObj || downloadObj.downloadComplete)
	{	
		[TJCLog logWithLevel:LOG_DEBUG format:@"DOWNLOAD ALREADY COMPLETED"];
		return; // Item is already downloaded
	}
	
	[self removeItemDownloadObservers:notificationObj];
	[self performSelector:@selector(showRetryWithDelay:) withObject:notificationObj afterDelay:0.5];
	[TJCLog logWithLevel:LOG_DEBUG format:@"FAILED END"];
}


/**
 * Item download retry
 */
- (void) retryBtnAction:(id)sender
{
	TJCVGGenericButtons *btn = sender;
	[TJCLog logWithLevel:LOG_DEBUG format:@"Item downlaod retry handler"];
	NSString *key = btn.customParams;
	TJCVGItemDownloader * dlObj = [itemsArray objectForKey:key];
	
	[dlObj.loadingTitle setText:@"Downloading ..."];//downloading stopped
	[dlObj.progressView setAlpha:1];
	[dlObj.progressView setProgress:0.0f];
	[dlObj.retryBtn setAlpha:0];
	[dlObj.percentTitle setAlpha:1];
	//[dlObj.infoIcon setAlpha:0];
	[dlObj.percentTitle setText:@"0%"];
	[dlObj startItemDataDownload];
}


/**
 * Release memory for objects user does not want to download
 */
- (void)removeDownloadErrorObjects
{
	if ([itemsArray count] == 0) 
	{
		return;
	}
	//[TJCLog logWithLevel:LOG_DEBUG format:@"Total Objects in array: %d", [itemsArray count]];
	
	// Find keys to be removed
	NSMutableArray *removeKeyArray = [[NSMutableArray alloc] init];
	for (id key in itemsArray) 
	{
		TJCVGItemDownloader *obj = [itemsArray objectForKey:key];
		if ([obj.retryBtn alpha] == 1) 
		{
			// Mark this object to be removed
			[removeKeyArray addObject:key];
			//[TJCLog logWithLevel:LOG_DEBUG format:@"Marked object to be removed from items array"];
		}
	}
	
	// Remove associated objects
	NSString *keyString = nil;
	for(int i = 0; i < [removeKeyArray count]; i++)
	{
		//[TJCLog logWithLevel:LOG_DEBUG format:@"Removed marked item"];
		keyString = [removeKeyArray objectAtIndex:i];
		[itemsArray removeObjectForKey:keyString];
	}
	[removeKeyArray removeAllObjects];
	[removeKeyArray release];
}


/**
 * Release memory
 */
- (void)dealloc
{
	[itemsArray removeAllObjects];
	[itemsArray release];
	[super dealloc];
}

@end
