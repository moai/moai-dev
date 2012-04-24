// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>

#define TJC_VG_DUMMY_DATA_LENGTH -1
#define TJC_VG_FILE_DOWNLOAD_TIMEOUT 20.0
#define TJC_VG_ITEM_DATA_FILE_NAME @"dataFile.zip"
#define TJC_VG_DATA_FILE_DIRECTORY @"DataFileDirectory"
#define TJC_VG_ITEM_FULL_IMAGE_FILE_NAME @"fullImageFile"
#define TJC_VG_ITEM_THUMB_IMAGE_FILE_NAME @"thumbImageFile"

#define TJC_VG_DATA_FILE_ID			1
#define TJC_VG_THUMB_FILE_ID		2
#define TJC_VG_FULL_IMAGE_FILE_ID	3

// first 3% (thumb)  next 6% (fullimage)  rest %age (datafile)
#define TJC_VG_THUMB_FILE_PERCENT		(3.0f)
#define TJC_VG_FULL_IMAGE_FILE_PERCENT	(6.0f)
#define TJC_VG_DATA_FILE_PERCENT		(100.0f - TJC_VG_FULL_IMAGE_FILE_PERCENT - TJC_VG_THUMB_FILE_PERCENT)

@class TJCFileDownloadConnection;
@class TJCVGGenericButtons;
@class TJCVGStoreItem;

@interface TJCVGItemDownloader : NSObject 
{
	TJCVGStoreItem *localStoreitem;
	UIView *downloadProgressContentView;
	UILabel *loadingTitle;
	UILabel *percentTitle;
	UIProgressView *progressView;
	TJCVGGenericButtons *retryBtn;
	//UIButton *infoIcon;
	BOOL downloadComplete;
	NSString *mimeType;
	
	// Holds item data and image files.
	NSString *itemDir;
	// Store object's key of TJCFileDownloadConnection itemsArray
	NSString *itemsArrayKey;
}


@property (nonatomic, retain) TJCVGStoreItem *localStoreitem;
@property (nonatomic, retain) UIView *downloadProgressContentView;
@property (nonatomic, retain) UILabel *loadingTitle;
@property (nonatomic, retain) UILabel *percentTitle;
@property (nonatomic, retain) UIProgressView *progressView;
@property (nonatomic, retain) NSString *itemDir;
@property (nonatomic, retain) NSString *itemsArrayKey;
@property (nonatomic, retain) TJCVGGenericButtons *retryBtn;
//@property (nonatomic, retain) UIButton *infoIcon;
@property BOOL downloadComplete;


// Start downloading VG Item data.
- (id)initWithObject:(TJCVGStoreItem *)obj initSuccessful:(BOOL *)initSuccess;

- (void)startItemDataDownload;

- (void)startDataFileDownload;

- (void)dataFileDownloaded;

- (void)startFullImageFileDownload;

- (void)startThumbImageFileDownload;

- (void)saveItemToLocalDB;

- (void)connection:(TJCFileDownloadConnection *)connection didReceiveResponse:(NSURLResponse *)response;

- (void)connection:(TJCFileDownloadConnection *)connection didReceiveData:(NSData *)data;

- (void)connection:(TJCFileDownloadConnection *)connection didFailWithError:(NSError *)error;

- (void)connectionDidFinishLoading:(TJCFileDownloadConnection *)connection;

- (void)dealloc;

@end