// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>
#import "TJCFetchResponseProtocol.h"
#import "SynthesizeSingleton.h"
#import "TapjoyConnect.h"

typedef enum 
{
	kTJCRequestTagNone = 0,
	kTJCRequestTagAllItems,
	kTJCRequestTagAllItemsCount,
	kTJCRequestTagPurchasedItems,
	kTJCRequestTagPurchasedItemsWithPrompt,
	kTJCRequestTagUpdateItems,
	kTJCRequestTagLocalItems,
	kTJCRequestTagItemPurchased,
	kTJCRequestTagMAX,
} TJCRequestTag;


@class TJCVGStoreModel;
@class TJCVGStoreRequestHandler;
@class TJCVGStoreItem;



@interface TJCVGStoreManager : NSObject <TJCFetchResponseDelegate>
{
	
	TJCVGStoreModel *vgStore_;
	
	TJCVGStoreRequestHandler *vgReqHandlerAllItems_;
	TJCVGStoreRequestHandler *vgReqHandlerAvailableItems_;
	TJCVGStoreRequestHandler *vgReqHandlerPurchasedItems_;
	TJCVGStoreRequestHandler *vgReqHandlerPurchasedItemsWithPrompt_;
	TJCVGStoreRequestHandler *vgReqHandlerBuyItem_;
	TJCVGStoreRequestHandler *vgReqHandlerUpdateItems_;
	
	int startingIndex_;
	int moreDataValue_;
	
	int startingIndexPurchased_;
	int moreDataValuePurchased_;
	
	// Used to see how many items are to be downloaded. Used for game state notification.
	int numItemsToBeDownloaded_;
	
	// The counter for number of vg items downloaded. 
	// This is used to reference the vg item in the array that contains the most up to date items.
	int numItemsUpdatedCount_;
	
	// Holds the most up to date items from the server.
	NSMutableArray *updatedItems_;
}

@property(nonatomic,readonly) TJCVGStoreModel *vgStoreModelObj;

@property (nonatomic) int startingIndex;
@property (nonatomic) int moreDataValue;

@property (nonatomic) int startingIndexPurchased;
@property (nonatomic) int moreDataValuePurchased;

@property (nonatomic) int numItemsToBeDownloaded;

+(TJCVGStoreManager *)sharedTJCVGStoreManager;

// Removes any local virtual good items found in the given array.
- (void)removeLocalItemsFromItemArray:(NSMutableArray*)updatedVGItemsArray;
- (void)itemDownloaded:(id)obj;
- (void)initiateItemUpdate;

- (void) initiateRequestForAllVGStoreItems;
- (void) initiateRequestForAvailableVGStoreItems;
- (void) initiateRequestForPurchasedVGStoreItems;
- (void) buyVGStoreItemWithID:(NSString *)itemID;
- (void) initiateRequestForPurchasedVGStoreItemsWithPrompt;
- (void) initiateRequestForUpdateVGStoreItems;

// Compares the given array of TJCVGStoreItems with the purchased ones saved locally and re-saves if there are differences.
// Returns TRUE if an update occured.
- (BOOL)updateLocalPurchasedVGItemsWithItemArray:(NSMutableArray*)updatedVGItemsArray;

// Compares the give array of TJCVGStoreItems with the purchased ones saved locally and re-saves if there are differences.
// Also saves the item if it doesn't exist at all locally. 
// NOTE: This should only ever be used in conjunction with game state!
- (void)updateAllLocalVGItemsWithItemArray:(NSMutableArray*)updatedVGItemsArray;

-(void) reloadStartForAllVGStoreItems;
-(void) reloadStartForPurchasedVGStoreItems;

-(void) updateStartForAllVGStoreItems;
-(void) updateStartForPurchasedVGStoreItems;

-(int) getMoreDataValueForAllItems;
-(int) getMoreDataValueForPurchasedItems;

-(void)releaseStore;
-(void) releaseRequestHandlers;

-(void) releaseRequestHandlersAllItems;
-(void) releaseRequestHandlersPurchasedItems;
-(void) releaseRequestHandlersBuyItem;

@end


@interface TapjoyConnect (TJCVGStoreManager)

+ (int)getNumVGItemsToBeDownloaded;

@end