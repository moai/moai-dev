// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>

// Class to hold download threads
@class TJCVGItemDownloader;
@class TJCVGStoreItem;

@interface TJCFileDownloadHandler : NSObject 
{
    NSMutableDictionary *itemsArray;
	int itemDownloadCount;
}

@property (nonatomic, retain) NSMutableDictionary *itemsArray;

- (id)init;

- (BOOL)isItemPresentInLocalBD:(TJCVGStoreItem*)item;

// When the all downloads have finished, this method fires off a notification.
- (void)incrementItemDownloadCount:(id)obj;

// Returns the progress Indicator View.
- (void)downloadItem:(TJCVGStoreItem*)obj;

- (UIView*)getVGPurchasedItemDownloadProgressView:(NSString*)vGStoreItemId;

- (void)addItemDownloadObservers:(TJCVGItemDownloader*)dlObj;

- (void)removeItemDownloadObservers:(NSNotification*)dlObj;

- (void)itemDownloaded:(NSNotification*)dlObj;

- (void)itemDownloadError:(NSNotification*)dlObj;

- (void)removeDownloadErrorObjects;

- (void)dealloc;


@end
