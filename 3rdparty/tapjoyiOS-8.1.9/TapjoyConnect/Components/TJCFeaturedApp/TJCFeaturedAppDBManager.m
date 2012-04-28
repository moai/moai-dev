// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCFeaturedAppDBManager.h"
#import "sqlite3.h"
#import "TJCLog.h"
#import "SynthesizeSingleton.h"
#import "TJCFeaturedAppModel.h"

@implementation TJCFeaturedAppDBManager


TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCFeaturedAppDBManager)


-(id)init 
{
	
	if((self = [super init]))
	{
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
		NSString *documentsPath = [paths objectAtIndex:0]; 
		currentDBPath_ = [[NSString alloc] initWithString:[documentsPath stringByAppendingPathComponent:TJC_FEATURE_APP_DB_NAME]];
		
		NSString *bundlePath = [[[NSBundle mainBundle] bundlePath] retain];
		
		NSString *defaultDBPath =  [[NSString alloc] initWithString:[NSString stringWithFormat:@"%@/%@", bundlePath,TJC_FEATURE_APP_DB_NAME]] ;
		
		[TJCLog logWithLevel:LOG_DEBUG format:currentDBPath_];
		[TJCLog logWithLevel:LOG_DEBUG format:defaultDBPath];
		
		//int dbVersion = [[NSUserDefaults standardUserDefaults] integerForKey:@"E_DB_V_KEY"];
		
		if( ![[NSFileManager defaultManager] fileExistsAtPath:currentDBPath_] ) 
			//if(dbVersion != DB_VERSION_NUMBER)
		{
			NSError *e ;
			[[NSFileManager defaultManager] removeItemAtPath:currentDBPath_ error:&e]; // delete file if already exists
			[[NSFileManager defaultManager] copyItemAtPath:defaultDBPath toPath:currentDBPath_ error:&e];
			//[[NSUserDefaults standardUserDefaults] setInteger:DB_VERSION_NUMBER forKey:@"E_DB_V_KEY"];
			[e description];
			//NSLog(e);
		}
		[defaultDBPath release];	
		[bundlePath release];
	}
	
	return self;
	
}


-(BOOL) addApp:(TJCFeaturedAppModel*) anAppObj
{
	[anAppObj retain]; //add retain count
	sqlite3 *database = NULL;
	
	@try 
	{
		if ( sqlite3_open([currentDBPath_ UTF8String], &database) == SQLITE_OK )
		{
			sqlite3_stmt *insertStmt = nil;
			const char *sql = "insert into featured_apps(store_id, no_of_times_displayed) Values(?,?)";
			if(sqlite3_prepare_v2(database, sql, -1, &insertStmt, NULL) != SQLITE_OK) 
			{
				[TJCLog logWithLevel:LOG_DEBUG format:@"Error while creating insert statement"];
				return false; 
			}
			
			sqlite3_bind_text(insertStmt, 1, [anAppObj.storeID UTF8String], -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(insertStmt, 2,1); // newly inserted
			
			
			if(SQLITE_DONE != sqlite3_step(insertStmt)) 
			{
				[TJCLog logWithLevel:LOG_DEBUG format:@"Error while inserting record; it may already exist"];
				return false;
			}
			sqlite3_finalize(insertStmt);
			return true;
		}
		else 
		{
			[TJCLog logWithLevel:LOG_DEBUG format:@"Unable to open the database"];
			return false;
		}
	}
	@catch (NSException * e) 
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Some exception in insert"];
		return false;
	}
	@finally 
	{
		if(database)
		{
			[TJCLog logWithLevel:LOG_DEBUG format:@"Closing database"];
			sqlite3_close(database);
			[anAppObj release]; //release a retain count
		}	
	}
	
	return false;
}


-(BOOL) incrementDisplayedCountForStoreID:(NSString*) aStoreID
{
	[aStoreID retain]; //add retain count
	sqlite3 *database = NULL;
	
	@try 
	{
		if ( sqlite3_open([currentDBPath_ UTF8String], &database) == SQLITE_OK )
		{
			sqlite3_stmt *updateStmt = nil;
			
			NSString *sqll = [NSString stringWithFormat:@"UPDATE featured_apps SET no_of_times_displayed = no_of_times_displayed+1 WHERE store_id ='%@'", 
									aStoreID];
			
			const char *sql = [sqll UTF8String];
			
			if(sqlite3_prepare_v2(database, sql, -1, &updateStmt, NULL) != SQLITE_OK) 
			{
				[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Error while updating record"];
				return false; 
			}
			
			if(SQLITE_DONE != sqlite3_step(updateStmt)) 
			{
				[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Error while record update"];
				return false;
			}
			sqlite3_finalize(updateStmt);
			return true;
		}
		else 
		{
			[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"unable to open the database"];
			return false;
		}
	}
	@catch (NSException * e) 
	{
		[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Some exception in update"];
		return false;
	}
	@finally 
	{
		if(database)
		{
			[TJCLog logWithLevel:LOG_DEBUG format:@"Closing database"];
			sqlite3_close(database);
			[aStoreID release]; //release a retain count
		}	
	}
	
	return false;
}


-(int) getDisplayedCountForStoreID:(NSString*) aStoreID
{
	[aStoreID retain];
	
	char **result;
	int r;
	int c;
	char *err;
	sqlite3 *database = NULL;
	int retVal = 0;
	
	@try 
	{
		if ( sqlite3_open([currentDBPath_ UTF8String], &database) == SQLITE_OK )
		{
			NSString *query = [NSString stringWithFormat:@"SELECT no_of_times_displayed FROM featured_apps WHERE store_id = '%@'", aStoreID];
			sqlite3_get_table(database, [query UTF8String], &result, &r, &c, &err);
			
			@try 
			{
				if(r>0 && c>0)
				{	
					retVal = [[NSString stringWithCString:result[c+0] encoding:NSUTF8StringEncoding] intValue];			
				}	
			}
			@catch (NSException * e) 
			{
				retVal = 0;
			}			
			sqlite3_free_table(result);
		}
		
		return retVal;
	}
	@catch (NSException * e) 
	{
		return 0;
	}
	@finally 
	{
		if(database)
		{
			sqlite3_close(database);
		}
	}
	
	return retVal;
}


- (void) dealloc
{
	[currentDBPath_ release];
	[super dealloc];
}

@end
