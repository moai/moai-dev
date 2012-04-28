// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCZipArchive.h"
#import "zlib.h"
#import "zconf.h"
#import "TJCLog.h"


@interface TJCZipArchive (Private)

-(void) OutputErrorMessage:(NSString*) msg;
-(BOOL) OverWrite:(NSString*) file;
-(NSDate*) Date1980;
@end



@implementation TJCZipArchive
@synthesize delegate = _delegate;

-(id) init
{
	if(( self=[super init] ))
	{
		_zipFile = NULL ;
	}
	return self;
}


-(void) dealloc
{
	[self CloseZipFile2];
	[super dealloc];
}


-(BOOL) CreateZipFile2:(NSString*) zipFile
{
	_zipFile = TJCzipOpen( (const char*)[zipFile UTF8String], 0 );
	if( !_zipFile ) 
		return NO;
	return YES;
}


-(BOOL) addFileToZip:(NSString*) file newname:(NSString*) newname;
{
	if( !_zipFile )
		return NO;
	//	tm_zip filetime;
	time_t current;
	time( &current );
	
	zip_fileinfo zipInfo = {{0}};
	zipInfo.dosDate = (unsigned long) current;
	
	NSDictionary* attr = [[NSFileManager defaultManager] attributesOfItemAtPath:file error:NULL];
	if( attr )
	{
		NSDate* fileDate = (NSDate*)[attr objectForKey:NSFileModificationDate];
		if( fileDate )
		{
			zipInfo.dosDate = (uLong)[fileDate timeIntervalSinceDate:[self Date1980]];
		}
	}
	
	int ret = TJCzipOpenNewFileInZip( _zipFile,
									 (const char*) [newname UTF8String],
									 &zipInfo,
									 NULL,0,
									 NULL,0,
									 NULL,//comment
									 Z_DEFLATED,
									 Z_DEFAULT_COMPRESSION );
	if( ret!=Z_OK )
	{
		return NO;
	}
	NSData* data = [ NSData dataWithContentsOfFile:file];
	unsigned int dataLen = [data length];
	ret = TJCzipWriteInFileInZip( _zipFile, (const void*)[data bytes], dataLen);
	if( ret!=Z_OK )
	{
		return NO;
	}
	ret = TJCzipCloseFileInZip( _zipFile );
	if( ret!=Z_OK )
		return NO;
	return YES;
}


-(BOOL) CloseZipFile2
{
	if( _zipFile==NULL )
		return NO;
	BOOL ret =  TJCzipClose( _zipFile,NULL )==Z_OK?YES:NO;
	_zipFile = NULL;
	return ret;
}


-(BOOL) UnzipOpenFile:(NSString*) zipFile
{
	_unzFile = TJCunzOpen( (const char*)[zipFile UTF8String] );
	if( _unzFile )
	{
		unz_global_info  globalInfo = {0};
		if( TJCunzGetGlobalInfo(_unzFile, &globalInfo )==UNZ_OK )
		{
			[TJCLog logWithLevel:LOG_DEBUG format:[NSString stringWithFormat:@"%d entries in the zip file",globalInfo.number_entry]];
		}
	}
	return _unzFile!=NULL;
}


-(BOOL) UnzipFileTo:(NSString*) path overWrite:(BOOL) overwrite
{
	BOOL success = YES;
	int ret = TJCunzGoToFirstFile( _unzFile );
	unsigned char		buffer[4096] = {0};
	NSFileManager* fman = [NSFileManager defaultManager];
	if( ret!=UNZ_OK )
	{
		[self OutputErrorMessage:@"Failed"];
	}
	
	do{
		ret = TJCunzOpenCurrentFile( _unzFile );
		if( ret!=UNZ_OK )
		{
			[self OutputErrorMessage:@"Error occurs"];
			success = NO;
			break;
		}
		// reading data and write to file
		int read ;
		unz_file_info	fileInfo ={0};
		ret = TJCunzGetCurrentFileInfo(_unzFile, &fileInfo, NULL, 0, NULL, 0, NULL, 0);
		if( ret!=UNZ_OK )
		{
			[self OutputErrorMessage:@"Error occurs while getting file info"];
			success = NO;
			TJCunzCloseCurrentFile( _unzFile );
			break;
		}
		char* filename = (char*) malloc( fileInfo.size_filename +1 );
		TJCunzGetCurrentFileInfo(_unzFile, &fileInfo, filename, fileInfo.size_filename + 1, NULL, 0, NULL, 0);
		filename[fileInfo.size_filename] = '\0';
		
		// check if it contains directory
		NSString * strPath = [NSString  stringWithCString:filename encoding:NSUTF8StringEncoding];
		BOOL isDirectory = NO;
		if( filename[fileInfo.size_filename-1]=='/' || filename[fileInfo.size_filename-1]=='\\')
			isDirectory = YES;
		free( filename );
		if( [strPath rangeOfCharacterFromSet:[NSCharacterSet characterSetWithCharactersInString:@"/\\"]].location!=NSNotFound )
		{// contains a path
			strPath = [strPath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
		}
		NSString* fullPath = [path stringByAppendingPathComponent:strPath];
		
		if( isDirectory )
			[fman createDirectoryAtPath:fullPath withIntermediateDirectories:YES attributes:nil error:nil];
		else
			[fman createDirectoryAtPath:[fullPath stringByDeletingLastPathComponent] withIntermediateDirectories:YES attributes:nil error:nil];
		if( [fman fileExistsAtPath:fullPath] && !isDirectory && !overwrite )
		{
			if( ![self OverWrite:fullPath] )
			{
				TJCunzCloseCurrentFile( _unzFile );
				ret = TJCunzGoToNextFile( _unzFile );
				continue;
			}
		}
		FILE* fp = fopen( (const char*)[fullPath UTF8String], "wb");
		while( fp )
		{
			read=TJCunzReadCurrentFile(_unzFile, buffer, 4096);
			if( read > 0 )
			{
				fwrite(buffer, read, 1, fp );
			}
			else if( read<0 )
			{
				[self OutputErrorMessage:@"Failed to reading zip file"];
				break;
			}
			else 
				break;				
		}
		if( fp )
		{
			fclose( fp );
			// set the orignal datetime property
			if( fileInfo.dosDate!=0 )
			{
				NSDate* orgDate = [[NSDate alloc] 
								   initWithTimeInterval:(NSTimeInterval)fileInfo.dosDate 
								   sinceDate:[self Date1980] ];
				
				NSDictionary* attr = [NSDictionary dictionaryWithObject:orgDate forKey:NSFileModificationDate]; //[[NSFileManager defaultManager] fileAttributesAtPath:fullPath traverseLink:YES];
				if( attr )
				{
					//	[attr  setValue:orgDate forKey:NSFileCreationDate];
					if( ![[NSFileManager defaultManager] setAttributes:attr ofItemAtPath:fullPath error:nil] )
					{
						// cann't set attributes 
						[TJCLog logWithLevel:LOG_DEBUG format:@"Failed to set attributes"];
					}
					
				}
				[orgDate release];
				orgDate = nil;
			}
			
		}
		TJCunzCloseCurrentFile( _unzFile );
		ret = TJCunzGoToNextFile( _unzFile );
	}while( ret==UNZ_OK && UNZ_OK!=UNZ_END_OF_LIST_OF_FILE );
	return success;
}


-(BOOL) UnzipCloseFile
{
	if( _unzFile )
		return TJCunzClose( _unzFile )==UNZ_OK;
	return YES;
}



#pragma mark wrapper for delegate

-(void) OutputErrorMessage:(NSString*) msg
{
	if( _delegate && [_delegate respondsToSelector:@selector(ErrorMessage)] )
		[_delegate ErrorMessage:msg];
}


-(BOOL) OverWrite:(NSString*) file
{
	if( _delegate && [_delegate respondsToSelector:@selector(OverWriteOperation)] )
		return [_delegate OverWriteOperation:file];
	return YES;
}



#pragma mark get NSDate object for 1980-01-01

-(NSDate*) Date1980
{
	NSDateComponents *comps = [[NSDateComponents alloc] init];
	[comps setDay:1];
	[comps setMonth:1];
	[comps setYear:1980];
	NSCalendar *gregorian = [[NSCalendar alloc]
							 initWithCalendarIdentifier:NSGregorianCalendar];
	NSDate *date = [gregorian dateFromComponents:comps];
	
	[comps release];
	[gregorian release];
	return date;
}

@end
