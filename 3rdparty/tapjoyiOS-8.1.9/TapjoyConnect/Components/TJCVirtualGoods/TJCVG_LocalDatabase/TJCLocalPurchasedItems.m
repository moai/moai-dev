// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license



#import "TJCLocalPurchasedItems.h"
#import "TJCVGNotificationsConstants.h"
#import "TJCSqliteController.h"
#import "TapjoyConnect.h"
#import "TJCLog.h"
#import "TJCVGViewHandler.h"
#import "TJCVGStoreManager.h"
#import "TJCVGStoreModel.h"
#import "SynthesizeSingleton.h"



@implementation TJCLocalPurchasedItems

TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCLocalPurchasedItems)


/**
 * Add local data update observers
 */
- (void)addLocalDataUpdateObservers
{
	// Add observers
	[[NSNotificationCenter defaultCenter] addObserver:self 
											 selector:@selector(localDataUpdateErrorSelector:) 
												 name:SUBMIT_PURCHASED_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA 
											   object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self 
											 selector:@selector(localDataUpdateErrorSelector:) 
												 name:SUBMIT_PURCHASED_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA 
											   object:nil];
	[[NSNotificationCenter defaultCenter] addObserver:self 
											 selector:@selector(localDataUpdateReceievedSelector:) 
												 name:SUBMIT_PURCHASED_VG_STORE_ITEMS_RECIEVE_DATA 
											   object:nil];
}


/**
 * Remove local data update observers
 */
- (void)removeLocalDataUpdateObservers
{
	// Remove observers
	[[NSNotificationCenter defaultCenter] removeObserver:self name:SUBMIT_PURCHASED_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA object:nil];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:SUBMIT_PURCHASED_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA object:nil];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:SUBMIT_PURCHASED_VG_STORE_ITEMS_RECIEVE_DATA object:nil];
}


/**
 * Local data update error
 */
- (void)localDataUpdateErrorSelector:(id)sender
{
	[self removeLocalDataUpdateObservers];
}


/**
 * Local data updated. Compare with local data
 */
-(void)localDataUpdateReceievedSelector:(id)sender
{
	[self removeLocalDataUpdateObservers];
	
	int newCount = [[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] purchasedItems] count];
	int oldCount = [[TJCSqliteController sharedTJCSqliteController] purchasedItemsCount];
	if (newCount > oldCount) 
	{
		// Data update required
		UIAlertView *av = [[UIAlertView alloc] initWithTitle:nil
													 message:@"New purchased item update available\nDo you want to update ?" 
													delegate:self 
										   cancelButtonTitle:@"NO" 
										   otherButtonTitles:@"YES", nil];
		[av show];
		[av release];
	} 
	else if (newCount < oldCount) 
	{
		
		[TJCLog logWithLevel:LOG_DEBUG format:@"Insane scenario. Local purchased item exceded remote server."];
	}
}


/**
 * Handle user request for update
 */
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	if (buttonIndex == 0) 
	{ // Do not update local records
		[TJCLog logWithLevel:LOG_DEBUG format:@"User refused to get purchased item updates."];
		// Release memory
		[[[TJCVGStoreManager sharedTJCVGStoreManager] vgStoreModelObj] releasePurchasedItems];
    } 
	else if (buttonIndex == 1) 
	{ // YES, (I want to update virtual good data)
		UIView *callingApplicationView = (UIWindow*)[[[UIApplication sharedApplication] windows] objectAtIndex:0];
		
		// Open view with purchased item tab selected
		[callingApplicationView addSubview:[[TJCVGViewHandler sharedTJCVGViewHandler] getVirtualGoodsWithTab:2]];
    }	
}


/**
 * Returns StoreItem object list.
 * Releasing objects is responsibility of user
 */
- (NSMutableArray*)getPurchasedVGStoreItemsArray
{
	NSMutableArray *localItemObjArray = [[TJCSqliteController sharedTJCSqliteController] getAllPurchasedItems];
	NSMutableArray *storeItemArray = [[NSMutableArray alloc] init];
	
	for (int i = 0; i < [localItemObjArray count]; i++) 
	{
		StoreItem *obj = [[StoreItem alloc] initWithObject:[localItemObjArray objectAtIndex:i]];
		[storeItemArray addObject:obj];
		[obj release];
	}
	 
	return storeItemArray;
}


- (NSMutableArray*)getVGStoreItemsArray
{
	return [self getPurchasedVGStoreItemsArray];
}



/**
 * Release memory
 */
-(void)dealloc
{
	[super dealloc];
}

@end
