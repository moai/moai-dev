// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import "TJCVGItemDownloader.h"
#import "TJCVGNotificationsConstants.h"
#import "TJCFileDownloadConnection.h"
#import "TJCVGGenericButtons.h"
#import "TJCSqliteController.h"
#import "TJCVGViewHandler.h"
#import "TJCVGStoreModel.h"
#import "TJCLog.h"

@implementation TJCVGItemDownloader

@synthesize 
localStoreitem,
progressView, 
itemDir, 
itemsArrayKey, 
downloadProgressContentView, 
retryBtn,
//infoIcon,
loadingTitle,
percentTitle,
downloadComplete;


- (id)initWithObject:(TJCVGStoreItem *)obj initSuccessful:(BOOL *)initSuccess
{
	BOOL initError = NO;
	
    if ((self = [super init]))
	{
        // Retain item object and release it in dealloc.
		localStoreitem = [obj retain];
		
		// Retain the itemID, since itemsArrayKey is used in other places and is later released.
		itemsArrayKey = [[localStoreitem itemID] retain];
		
		// Save item data in application download directory.
		NSString *appsFilePath = [TJCVGViewHandler getGlobalDBSavePath];
		self.itemDir = [appsFilePath stringByAppendingFormat:@"/%@", [localStoreitem itemID]];
		
		downloadProgressContentView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 229, 20)];
		[downloadProgressContentView setBackgroundColor:[UIColor clearColor]];
		
		loadingTitle = [[UILabel alloc] initWithFrame:CGRectMake(7,0, 130, 20)];
		[loadingTitle setTextColor:[UIColor darkGrayColor]];
		[loadingTitle setBackgroundColor:[UIColor clearColor]];
		[downloadProgressContentView addSubview:loadingTitle];
		[loadingTitle setTextAlignment:UITextAlignmentLeft];
		loadingTitle.font = [UIFont fontWithName:@"Helvetica" size:12];
		
		percentTitle = [[UILabel alloc] initWithFrame:CGRectMake(107, 0, 42, 20)];
		percentTitle.font = loadingTitle.font; //[UIFont fontWithName:@"Helvetica" size:12];
		[percentTitle setTextColor:[UIColor darkGrayColor]];
		[percentTitle setBackgroundColor:[UIColor clearColor]];
		[percentTitle setText:@"0%"];
		[downloadProgressContentView addSubview:percentTitle];
		
		progressView = [[UIProgressView alloc] initWithProgressViewStyle:UIProgressViewStyleDefault];
		progressView.frame = CGRectMake(141, 6, 70, 9);
		[progressView setProgress:0];
		[progressView setAlpha:0];
		[downloadProgressContentView addSubview:progressView];
		
		//		infoIcon = [[TJCGenericButtons alloc] initWithFrame:CGRectMake(108, 0, 20, 20)];
		//		[infoIcon addTarget:self action:@selector(infoIconAction:) forControlEvents:UIControlEventTouchUpInside];
		//		[infoIcon setBackgroundImage:[TJCVGSettings getInfoIcon] forState:UIControlStateNormal];
		//		[infoIcon setBackgroundImage:[TJCVGSettings getInfoIcon] forState:UIControlStateHighlighted];
		//		[downloadProgressContentView addSubview:infoIcon];
		//		[infoIcon setAlpha:0];
		
		UIImage * customButtonImage = [TJCVGUtil primaryThemeColorGradientButtonImage:20: 60];
		
		retryBtn = [[TJCVGGenericButtons alloc] initWithFrame:CGRectMake(143, 0,60 , 20)];
		retryBtn.customParams = [[localStoreitem itemID] retain];
		[retryBtn setBackgroundImage:customButtonImage forState:UIControlStateNormal];
		[retryBtn setBackgroundImage:customButtonImage forState:UIControlStateHighlighted];
		[retryBtn setBtnTitle:@"Retry"];
		[retryBtn setFont:[UIFont boldSystemFontOfSize:12]];
		[downloadProgressContentView addSubview:retryBtn];
		[retryBtn setAlpha:0];
		
		if (NO == [TJCVGUtil createDirAtPath:self.itemDir]) 
		{
			[TJCLog logWithLevel:LOG_DEBUG format:@"Error: Could not create item directory: %@", self.itemDir];
			itemDir = nil;
			initError = YES;
		}
    }
	
	// Initialization successful
	if (initError == YES || self == nil) 
	{
		*initSuccess = NO;
	} 
	else
	{
		*initSuccess = YES;
	}
    return self;
}


-(void) infoIconAction:(id) sender
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"Info Icon Pressed"];
	UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:nil 
														message:@"An error occured while downloading the contents of acquired item" 
													   delegate:self 
											  cancelButtonTitle:@"OK" 
											  otherButtonTitles:nil];
	[alertView show];
	[alertView release];
}

/**
 * Start downloading data for item
 */
- (void)startItemDataDownload
{	
	// Start item data 
	// 1) download thumb image file
	// 2) download full image file
	// 3) download data file
	
	
	[self startThumbImageFileDownload];
}


/**
 * Downloads data file
 */
- (void)startDataFileDownload
{
	if ([localStoreitem itemDatafileURL] == @"") 
	{
		localStoreitem.itemDataFileDirPath = @"";
		[progressView setProgress:1];
		[percentTitle setText:@"100%"];
		[self saveItemToLocalDB];
		return;
	}
	
	NSString *filePath = [NSString stringWithFormat:@"%@/%@", self.itemDir, TJC_VG_ITEM_DATA_FILE_NAME];
	[TJCLog logWithLevel:LOG_DEBUG format:@"Data file URL: %@", localStoreitem.itemDatafileURL];
	
	// Create file download request
	NSMutableURLRequest *theRequest=[NSMutableURLRequest requestWithURL:[NSURL URLWithString:localStoreitem.itemDatafileURL]
															cachePolicy:NSURLRequestUseProtocolCachePolicy
														timeoutInterval:TJC_VG_FILE_DOWNLOAD_TIMEOUT];
	
	NSString *version = [[UIDevice currentDevice] systemVersion];
	NSString *userAgent = [NSString stringWithFormat:@"Mozilla/5.0 (iPhone; U; CPU like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/%@ Mobile/1A543a Safari/419.3",version];
	[theRequest setValue:userAgent forHTTPHeaderField:@"User-Agent"];
	
	NSFileHandle *fh = [TJCVGUtil createWriteableFileAtPath:filePath];
	if (fh == nil) 
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"Error: Could not create handle for file: %@", filePath];
		// Post item download error notfication
		[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_ERROR_NOTIFICATION object:self];
		return;
	}
	
	TJCFileDownloadConnection *theConnection = [[TJCFileDownloadConnection alloc] initWithRequest:theRequest delegate:self downloadFileHandle:fh];
	theConnection.fileNumber = 1;
    if (theConnection)
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"Connection established"];
		
    } 
	else 
	{
        // inform the user that the download could not be made
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Could not establish conneciton"];
		// Post item download error notfication
		[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_ERROR_NOTIFICATION object:self];
    }
}


/**
 * Data file successfully downloaded. Download image file
 */
- (void)dataFileDownloaded
{
	// Unzip, data file
	NSString *zipFile = [NSString stringWithFormat:@"%@/%@", self.itemDir, TJC_VG_ITEM_DATA_FILE_NAME];
	NSString *zipDir = [NSString stringWithFormat:@"%@/%@", self.itemDir, TJC_VG_DATA_FILE_DIRECTORY];
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"Data File Saved path %@",zipDir];
	
	if([mimeType isEqualToString:@"text/html"])
	{
		NSString *str = [NSString stringWithContentsOfFile:zipFile encoding:NSUTF8StringEncoding error:NULL];
		NSRange rng = [str rangeOfString:@".zip"];
		
		int loc=[str length] - 1;
		
		while ([str characterAtIndex:loc]!='"') 
		{
			--loc;
		}
		
		--loc;
		
		int start=loc;
		
		while ([str characterAtIndex:start]!='"') 
		{
			--start;
		}
		
		rng.location = start + 1;
		rng.length = loc - start;
		NSString * mstr = [[NSString alloc] initWithString:[str substringWithRange:rng]];
		NSString * keyVal = mstr;
		NSString * url = localStoreitem.itemDatafileURL;
		NSString *myRequestString = [NSString stringWithFormat:@"GateWayXWM4X1url=%@&GateWayXWM4X1SaveToDisk=NO&GateWayXWM4X1Orig=%@&GateWayXWM4X1Cache=N",keyVal,url]; 
		NSData *myRequestData = [NSData dataWithBytes:[myRequestString UTF8String] length:[myRequestString length]];
		
		NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:url]];
		[request setHTTPMethod:@"POST"];
		[request setHTTPBody:myRequestData];
		
		NSData *returnRequestData = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
		[returnRequestData writeToFile:zipFile atomically:true];
		[request release];
		[mstr release];
	}
	
	if (NO == [TJCVGUtil unZipFile:zipFile toDir:zipDir]) 
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Unable to unzip file: %@", zipFile];
		
		// delete zip file
		if (NO == [[NSFileManager defaultManager] removeItemAtPath:zipFile error:nil]) 
		{
			[TJCLog logWithLevel:LOG_DEBUG format:@"Unable to delete zip file: %@", zipFile];
		}
		
		//TODO use some different notification since downloaded completed successfully 
		// Post item download error notfication
		[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_ERROR_NOTIFICATION object:self];
		return;
	}
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"Unzipped directory path: %@", zipDir];
	localStoreitem.itemDataFileDirPath = zipDir;
	
	// Delete zip file
	if (NO == [[NSFileManager defaultManager] removeItemAtPath:zipFile error:nil]) 
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"Unable to delete zip file: %@", zipFile];
	}
}


-(void)releaseLater:(id)obj
{
	[obj release];
}


/**
 * Downloads full image file
 */
- (void)startFullImageFileDownload
{
	if (localStoreitem.itemFullImageURL == @"") 
	{
		localStoreitem.itemFullImageFilePath = @"";
		[progressView setProgress: (TJC_VG_FULL_IMAGE_FILE_PERCENT + TJC_VG_THUMB_FILE_PERCENT) / 100];
		[self startDataFileDownload];
		return;
	}
	
	// Start full image download
	NSString *filePath = [NSString stringWithFormat:@"%@/%@", self.itemDir, TJC_VG_ITEM_FULL_IMAGE_FILE_NAME];
	NSMutableURLRequest *theRequest=[NSMutableURLRequest requestWithURL:[NSURL URLWithString:localStoreitem.itemFullImageURL]
															cachePolicy:NSURLRequestUseProtocolCachePolicy
														timeoutInterval:TJC_VG_FILE_DOWNLOAD_TIMEOUT];
	[TJCLog logWithLevel:LOG_DEBUG format:@"Downloading image file from URL: %@", localStoreitem.itemFullImageURL];
	
	// Update user agent
	NSString *version = [[UIDevice currentDevice] systemVersion];
	NSString *userAgent = [NSString stringWithFormat:@"Mozilla/5.0 (iPhone; U; CPU like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/%@ Mobile/1A543a Safari/419.3",version];
	[theRequest setValue:userAgent forHTTPHeaderField:@"User-Agent"];
	
	NSFileHandle *fh = [TJCVGUtil createWriteableFileAtPath:filePath];
	if (fh == nil)
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Could not create file handle for image file"];
		// Post item download error notfication
		[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_ERROR_NOTIFICATION object:self];
		return;
	}
	
	TJCFileDownloadConnection *theConnection = [[TJCFileDownloadConnection alloc] initWithRequest:theRequest delegate:self downloadFileHandle:fh];
	theConnection.fileNumber = TJC_VG_FULL_IMAGE_FILE_ID;
	
    if (theConnection)
	{
        [TJCLog logWithLevel:LOG_DEBUG format:@"Connection established for Full Image"];
    } 
	else
	{
        // inform the user that the download could not be made
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Could not establish conneciton"];
		// Post item download error notfication
		[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_ERROR_NOTIFICATION object:self];
    }
}


/**
 * Downloads thumb image file.
 */
- (void)startThumbImageFileDownload
{
	if (localStoreitem.itemThumbImageURL == @"") 
	{
		localStoreitem.itemThumbImageFilePath = @"";
		[progressView setProgress: ( TJC_VG_THUMB_FILE_PERCENT) /100];		
		
		[self startFullImageFileDownload];
		return;
	}
	
	// Download thumb image file
	NSString *filePath = [NSString stringWithFormat:@"%@/%@", self.itemDir, TJC_VG_ITEM_THUMB_IMAGE_FILE_NAME];
	NSMutableURLRequest *theRequest=[NSMutableURLRequest requestWithURL:[NSURL URLWithString:localStoreitem.itemThumbImageURL]
															cachePolicy:NSURLRequestUseProtocolCachePolicy
														timeoutInterval:TJC_VG_FILE_DOWNLOAD_TIMEOUT];
	NSString *version = [[UIDevice currentDevice] systemVersion];
	NSString *userAgent = [NSString stringWithFormat:@"Mozilla/5.0 (iPhone; U; CPU like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/%@ Mobile/1A543a Safari/419.3",version];
	[theRequest setValue:userAgent forHTTPHeaderField:@"User-Agent"];
	
	NSFileHandle *fh = [TJCVGUtil createWriteableFileAtPath:filePath];
	if (fh == nil) 
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Could not create file handle for thumb image file"];
		// Post item download error notfication
		[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_ERROR_NOTIFICATION object:self];
		return;
	}
	
	TJCFileDownloadConnection *theConnection = [[TJCFileDownloadConnection alloc] initWithRequest:theRequest delegate:self downloadFileHandle:fh];
	theConnection.fileNumber = TJC_VG_THUMB_FILE_ID;
	
    if (theConnection) 
	{
        [TJCLog logWithLevel:LOG_DEBUG format:@"Connection established"];
    }
	else 
	{
        // inform the user that the download could not be made
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Could not establish connection"];
		// Post item download error notfication
		[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_ERROR_NOTIFICATION object:self];
    }
}


/**
 * Save item data in local database
 */
- (void)saveItemToLocalDB
{
	// TODO: Post Success/Failure notification, based on value returned from local DB
	[[TJCSqliteController sharedTJCSqliteController] saveVirtualGoodData:localStoreitem];
	
	// Post success notification.
	[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_SUCCESS_NOTIFICATION object:self];
	[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_UPDATED_NOTIFICATION object:self];
}


// NSURLConnect event handlers.
/**
 * Returns response object to server request.
 */
- (void)connection:(TJCFileDownloadConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    // this method is called when the server has determined that it
    // has enough information to create the NSURLResponse
    // it can be called multiple times, for example in the case of a
    // redirect, so each time we reset the data.
	
	mimeType = [response MIMEType];
	long long abc = [response expectedContentLength];
	connection.totalData = (long double) abc;
	
	if (-1 == connection.totalData) 
	{
		[TJCLog logWithLevel:LOG_DEBUG format:@"Unable to determine object data length"];
		connection.totalData = TJC_VG_DUMMY_DATA_LENGTH;
	}
}


/**
 * Receive data in chunks
 */
- (void)connection:(TJCFileDownloadConnection *)connection didReceiveData:(NSData *)data
{
	connection.dataReceived += (long double)[data length];
	//	[TJCLog logWithLevel:LOG_DEBUG format:@"Total Data Received %d",connection.totalData];
	//	[TJCLog logWithLevel:LOG_DEBUG format:@"Current Bytes Received %d",connection.dataReceived];
	
	float progress = 0;
	
	float filePercentage = (float)(connection.dataReceived / connection.totalData);
	if (filePercentage < 0)
	{
		filePercentage = 0;
	}
	
	switch (connection.fileNumber)
	{
		case TJC_VG_THUMB_FILE_ID:
			progress = (float)(TJC_VG_THUMB_FILE_PERCENT  * filePercentage);
			break;
			
		case TJC_VG_FULL_IMAGE_FILE_ID:
			progress =  (float)(TJC_VG_THUMB_FILE_PERCENT +  (TJC_VG_FULL_IMAGE_FILE_PERCENT  * filePercentage));
			break;
			
		case TJC_VG_DATA_FILE_ID:
			progress =  (float)(TJC_VG_THUMB_FILE_PERCENT +  TJC_VG_FULL_IMAGE_FILE_PERCENT  + (TJC_VG_DATA_FILE_PERCENT * filePercentage));
			break;
			
		default:
			break;
	}
	
	[connection.fh writeData:data];	//write data to corresponding file
	
	[percentTitle setText:[NSString stringWithFormat:@"%d%@",(int)(progress),@"%"]];
	[progressView setProgress:progress/100];
}


/**
 * Connection terminated
 */
- (void)connection:(TJCFileDownloadConnection *)connection didFailWithError:(NSError *)error
{   
	switch (connection.fileNumber) 
	{
		case TJC_VG_THUMB_FILE_ID:
			[TJCLog logWithLevel:LOG_DEBUG format:@"Download error for thumb image"];
			
			break;
			
		case TJC_VG_FULL_IMAGE_FILE_ID:
			[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"download error for full image "];
			
			break;
			
		case TJC_VG_DATA_FILE_ID:
			[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"download error for data file"];			
			
			NSString *zipFile = [NSString stringWithFormat:@"%@/%@", self.itemDir, TJC_VG_ITEM_DATA_FILE_NAME];
			if (NO == [[NSFileManager defaultManager] removeItemAtPath:zipFile error:nil]) 
				[TJCLog logWithLevel:LOG_DEBUG format:@"Could not delete file: %@", zipFile];			
			break;
			
		default:
			break;
	}
	
	[connection release];
	// Post item download error notfication
	[[NSNotificationCenter defaultCenter] postNotificationName:ITEM_DOWNLOAD_ERROR_NOTIFICATION object:self];
}


/**
 * File download completed.
 */
- (void)connectionDidFinishLoading:(TJCFileDownloadConnection *)connection
{
	[TJCLog logWithLevel:LOG_DEBUG format:@"File download complete"];
	
	switch (connection.fileNumber) 
	{
		case TJC_VG_THUMB_FILE_ID:
			[TJCLog logWithLevel:LOG_DEBUG format:@"Thumb image downloaded successfully"];
			localStoreitem.itemThumbImageFilePath = [NSString stringWithFormat:@"%@/%@", self.itemDir, TJC_VG_ITEM_THUMB_IMAGE_FILE_NAME];
			[TJCLog logWithLevel:LOG_DEBUG format:@"Thumb image file path: %@", localStoreitem.itemThumbImageFilePath];
			
			[self startFullImageFileDownload];
			
			break;
			
		case TJC_VG_FULL_IMAGE_FILE_ID:
			[TJCLog logWithLevel:LOG_DEBUG format:@"Full image downloaded successfully"];
			localStoreitem.itemFullImageFilePath = [NSString stringWithFormat:@"%@/%@", self.itemDir, TJC_VG_ITEM_FULL_IMAGE_FILE_NAME];
			
			[self startDataFileDownload];
			break;
			
		case TJC_VG_DATA_FILE_ID:
			[self dataFileDownloaded];
			
			[progressView setProgress:1];
			[percentTitle setText:@"100%"];
			[self saveItemToLocalDB];
			break;
			
		default:
			break;
	}
	
	[connection release];
}


- (void)dealloc
{
	[itemsArrayKey release];
	[percentTitle release];
	[progressView release];
	[itemDir release];
	[localStoreitem release];
	[downloadProgressContentView release];
	[retryBtn release];
    [loadingTitle release];
	[super dealloc];
}

@end
