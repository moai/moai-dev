// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>

#include "tjczip.h"
#include "tjcunzip.h"

@protocol TJCZipArchiveDelegate <NSObject>

@optional
-(void) ErrorMessage:(NSString*) msg;
-(BOOL) OverWriteOperation:(NSString*) file;

@end


@interface TJCZipArchive : NSObject 
{
@private
	zipFile _zipFile;
	unzFile _unzFile;
	id _delegate;
}

@property (nonatomic, retain) id delegate;

-(BOOL) CreateZipFile2:(NSString*) zipFile;
-(BOOL) addFileToZip:(NSString*) file newname:(NSString*) newname;
-(BOOL) CloseZipFile2;

-(BOOL) UnzipOpenFile:(NSString*) zipFile;
-(BOOL) UnzipFileTo:(NSString*) path overWrite:(BOOL) overwrite;
-(BOOL) UnzipCloseFile;
@end
