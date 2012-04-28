// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import "TJCSqliteController.h"
#import "TJCLog.h"
#import "TJCUserAccountManager.h"
#import "TJCUserAccountModel.h"
#import "TJCVGViewHandler.h"
#import "SynthesizeSingleton.h"



@implementation TJCSqliteController

TJC_SYNTHESIZE_SINGLETON_FOR_CLASS(TJCSqliteController)


- (id)init
{
	self = [super init];
	
	if (self)
	{
		[self copyDatabaseIfNeeded];
	}
	
	return self;
}


- (void)dealloc
{
	[super dealloc];
}


- (void) updateClientPerVersion // This function will be replaced by the new implementation
{
	// The user defaults contains the client version number for the sqlite database and is used to check whether the update has already occured.
	NSNumber *clientDBVersion = [[NSUserDefaults standardUserDefaults] objectForKey:TJC_SQLITE_DB_VERSION_KEY_NAME];
	
	if(clientDBVersion)
	{
		if([clientDBVersion floatValue] == TJC_SQLITE_DB_VERSION )
		{	
			[TJCLog logWithLevel:LOG_DEBUG format:@"Already Updated"];
			return;
		}	
	}
	
	sqlite3 *database;
	
	[TJCLog logWithLevel:LOG_DEBUG format:@"TEST DB PATH %@",[self getDBPath]];
	// Get total number of records.
	if(sqlite3_open([[self getDBPath] UTF8String], &database) == SQLITE_OK) 
	{
		
		@try 
		{
			// Add Currency Name.
			sqlite3_stmt *tstStmt;
			
			// See if Currency Name Col exists.
			if(sqlite3_prepare_v2(database, "select CurrencyName from tapjoy_VGStoreItems", -1, &tstStmt, NULL) == SQLITE_OK) 
			{
				[TJCLog logWithLevel:LOG_DEBUG format:@"Currency Column Exists"];  
			}
			else
			{
				//[TJCLog logWithLevel:LOG_DEBUG format:@"NO PROBLEM CAME SQLITE NOT OK"];  
				sqlite3_stmt *alterStatement;
				
				// If the column tapjoy_VGStoreItems doesn't exist, create it now.
				if(sqlite3_prepare_v2(database, "alter table tapjoy_VGStoreItems add column CurrencyName TEXT(100)", -1, &alterStatement, NULL) == SQLITE_OK)
				{
					[TJCLog logWithLevel:LOG_DEBUG format:@"Currency Name Col Created"];
				}
				else
				{
					[TJCLog logWithLevel:LOG_DEBUG format:@"Currency Name already Exists"];
				}
				
				// Evaluate the statement.
				sqlite3_step(alterStatement);
				
				// Delete the prepared statement.
				sqlite3_finalize(alterStatement);
			}
			
			sqlite3_stmt *tstStmt2;
			
			// See if the challenges tables exists.
			if(sqlite3_prepare_v2(database, "select 1 from tapjoy_Challenges where 1==2", -1, &tstStmt2, NULL) == SQLITE_OK) 
			{
				[TJCLog logWithLevel:LOG_DEBUG format:@"Challenges table Exists"];  
			}
			else
			{
				//[TJCLog logWithLevel:LOG_DEBUG format:@"NO PROBLEM CAME SQLITE NOT OK"];  
				sqlite3_stmt *createChallengeTabStmt;
				
				// If the challenges table doesn't exist, create it now.
				if(sqlite3_prepare_v2(database, "CREATE TABLE tapjoy_Challenges(id INTEGER PRIMARY KEY, State TEXT,ChallengeID TEXT, Name TEXT, Difficulty TEXT, Description TEXT(1000), PrizeType TEXT, TicketID TEXT, EmailAddress TEXT,Filter1 TEXT, Filter2 TEXT, Filter3 TEXT, Filter4 TEXT, Filter5 TEXT, Filter6 TEXT,CostInPoints INTEGER,MinScore1 INTEGER, MinScore2 INTEGER, MinScore3 INTEGER, MinScore4 INTEGER, MinScore5 INTEGER,MinScore6 INTEGER, MinScore7 INTEGER, MaxScore1 INTEGER, MaxScore2 INTEGER, MaxScore3 INTEGER, MaxScore4 INTEGER, MaxScore5 INTEGER, MaxScore6 INTEGER, MaxScore7 INTEGER, UserScore1 INTEGER, UserScore2 INTEGER, UserScore3 INTEGER, UserScore4 INTEGER, UserScore5 INTEGER, UserScore6 INTEGER, UserScore7 INTEGER,ImageData BLOB)", -1, &createChallengeTabStmt,NULL) == SQLITE_OK)
				{
					[TJCLog logWithLevel:LOG_DEBUG format:@"Challenge Table Created"];
				}
				else
				{
					[TJCLog logWithLevel:LOG_DEBUG format:@"Challenge table already exists"];
				}
				
				// Evaluate the statement.
				sqlite3_step(createChallengeTabStmt);
				
				// Delete the prepared statement.
				sqlite3_finalize(createChallengeTabStmt);
			}
			
			// Set the current verion of the database.
			[[NSUserDefaults standardUserDefaults] setObject:[NSNumber numberWithInt:TJC_SQLITE_DB_VERSION] forKey:TJC_SQLITE_DB_VERSION_KEY_NAME];
			// set client version		
		}
		@catch (NSException * e) 
		{
			[TJCLog logWithLevel:LOG_DEBUG format:@"COLUMN DOESN'T EXIST "];
		}
		@finally 
		{
			sqlite3_close(database);
		} 
	}
}


- (void)copyDatabaseIfNeeded
{
	//Using NSFileManager we can perform many file system operations.
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSError *error;
	NSString *dbPath = [self getDBPath];
	BOOL success = [fileManager fileExistsAtPath:dbPath]; 
	
	//TODO: Create db if not exists.
	if(!success)
	{
		//NSString *defaultDBPath = [[NSString stringWithFormat:@"%@/TJC_Images/skins/standard/%@",[[NSBundle mainBundle] bundlePath], TJC_VG_DB_NAME] retain];
		NSString *defaultDBPath = [NSString stringWithFormat:@"%@/%@",[[NSBundle mainBundle] bundlePath], TJC_VG_DB_NAME];
		//NSString *defaultDBPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:TJC_VG_DB_NAME];
		[TJCLog logWithLevel:LOG_DEBUG format:@"Default Path is: %@", defaultDBPath];
		[TJCLog logWithLevel:LOG_DEBUG format:@"Destination Path is: %@", dbPath];
		success = [fileManager copyItemAtPath:defaultDBPath toPath:dbPath error:&error];
		
		if (!success) 
			NSAssert1(0, @"Failed to create writable database file with message '%@'.", [error localizedDescription]);
	}
	else
	{
		// Database already exists, just update the version.
		[self updateClientPerVersion];
	}    
}


- (NSString *)getDBPath
{
	//Search for standard documents using NSSearchPathForDirectoriesInDomains
	//First Param = Searching the documents directory
	//Second Param = Searching the Users directory and not the System
	//Expand any tildes and identify home directories.
	
	//NSString *globalImagePath = [NSString stringWithFormat:@"%@/",[[NSBundle mainBundle] bundlePath]];
	//NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory , NSUserDomainMask, YES);
	NSString *documentsDir = [TJCVGViewHandler getGlobalDBSavePath];
	//[TJCLog logWithLevel:LOG_DEBUG format:[documentsDir stringByAppendingPathComponent:TJC_VG_DB_NAME] ];
	return [documentsDir stringByAppendingPathComponent:TJC_VG_DB_NAME];
}


/*
 * Returns date string, (current date - days back)
 */
- (NSString *)previousDateString:(int)daysBack
{
	// Code to generate date string
	NSDate *now = [NSDate date];
	daysBack *= -1; // Make days negative.
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0
	NSDate *previousDate = [now dateByAddingTimeInterval:60*60*24*daysBack];
#else
	NSDate *previousDate = [now addTimeInterval:60*60*24*daysBack];
#endif
	return [previousDate description];
}


/*
 * Get text value of returned table
 */
- (NSString *)getColTextValue:(sqlite3_stmt *)compiledStatement column:(int)col
{
	NSString *textValue = nil;
	char* column = (char *)sqlite3_column_text(compiledStatement, col);
	
	if( column != NULL ) 
	{
		textValue = [NSString stringWithUTF8String:column];
	}
	else
	{
		textValue = @"";
	}
	
	// Ignore Clang Warning being handled by the caller so not a leak
	return textValue;
}




#pragma mark -
#pragma mark Virtual Goods Implementation


- (void)saveVirtualGoodData:(TJCVGStoreItem *)item
{
	//	// Check whether the item already exists. If so, don't continue.
	//	if ([self storeItemIDExists:[item itemID]])
	//	{
	//		return;
	//	}
	
	BOOL flag = TRUE;
	sqlite3 *database = nil;
	
	// Updated for game state. If there is a matching item, just delete the old row and re-download.
	// This will effectively ensure that the db is always up-to-date.
	if ([self storeItemIDExists:[item itemID]])
	{
		if(sqlite3_open([[self getDBPath] UTF8String], &database) == SQLITE_OK)
		{
			@try
			{
				NSMutableString *sqlStatement = [[NSMutableString alloc] initWithString:@""];
				[sqlStatement appendFormat:@"delete from tapjoy_VGStoreItems where VGStoreItemID='%@'", [item itemID]];
				sqlite3_stmt *compiledStatement;
				
				if(sqlite3_prepare_v2(database, [sqlStatement UTF8String], -1, &compiledStatement, NULL) == SQLITE_OK)
				{
					if (SQLITE_DONE != sqlite3_step(compiledStatement))
					{
						[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Error while deleting item row."];
					}
				}
				
				[sqlStatement release];
			}
			@catch (NSException * e)
			{
				// Do nothing
			}
			@finally 
			{
				sqlite3_close(database);
			}
		}
	}
	
	sqlite3_stmt *insertStmt = nil;
	
	// Open the database to prepare to save the virtual good data.
	if (sqlite3_open([[self getDBPath] UTF8String], &database) == SQLITE_OK) 
	{
		@try 
		{
			// Prepare the insert SQL statement.
			const char *sql = "insert into tapjoy_VGStoreItems(VGStoreItemID,AppleProductID,Price,Name,Description,ItemTypeName,ItemsOwned,ThumbImageName,FullImageName,DataFileName,CurrencyName,DataHash) Values(?,?,?,?,?,?,?,?,?,?,?,?)";
			if(sqlite3_prepare_v2(database, sql, -1, &insertStmt, NULL) != SQLITE_OK) 
			{
				[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Couldn't create the statement"];
				return;
				//NSAssert1(0, @"Error while creating add statement. '%s'", sqlite3_errmsg(database));
			}
			
			// Bind the virtual good data to the SQL statement.
			sqlite3_bind_text(insertStmt, 1, [item.itemID UTF8String], -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(insertStmt, 2, [item.itemAppleProductID UTF8String], -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(insertStmt, 3, item.itemPrice);
			sqlite3_bind_text(insertStmt, 4, [item.itemName UTF8String], -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(insertStmt, 5, [item.itemDescription UTF8String], -1, SQLITE_TRANSIENT);
			sqlite3_bind_text(insertStmt, 6, [item.itemTypeName  UTF8String], -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(insertStmt, 7, item.itemNumberOwned);
			
			// Get path to application documents directory
			NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory , NSUserDomainMask, YES);
			NSString *documentsDir = [NSString stringWithFormat:@"%@", [paths objectAtIndex:0]];
			
			// Store thumb image relative path
			NSString *thumbImgRelativePath = [item.itemThumbImageFilePath stringByReplacingOccurrencesOfString:documentsDir withString:@""];
			sqlite3_bind_text(insertStmt, 8, [thumbImgRelativePath UTF8String], -1, SQLITE_TRANSIENT);
			
			// Store full image relative path
			NSString *fullImgRelativePath = [item.itemFullImageFilePath stringByReplacingOccurrencesOfString:documentsDir withString:@""];
			sqlite3_bind_text(insertStmt, 9, [fullImgRelativePath UTF8String], -1, SQLITE_TRANSIENT);
			
			// Store data file directory relative path
			NSString *dataFileDirRelativePath = [item.itemDataFileDirPath stringByReplacingOccurrencesOfString:documentsDir withString:@""];
			sqlite3_bind_text(insertStmt, 10, [dataFileDirRelativePath UTF8String], -1, SQLITE_TRANSIENT);
			
			// Inserted the value came in TJCModelController for last VG Request
			TJCUserAccountModel *tpUser = [[TJCUserAccountManager sharedTJCUserAccountManager] userAccountModelObj];
			
			// Get the currency name of the current user, if it exists.
			NSString *currencyName;
			if(tpUser) 
				currencyName = [tpUser currencyName];
			else
				currencyName = @"";
			
			// Now bind the currency name to the SQL statement.
			sqlite3_bind_text(insertStmt, 11, [currencyName UTF8String], -1, SQLITE_TRANSIENT);
			
			// Bind Data Hash.
			sqlite3_bind_text(insertStmt, 12, [item.itemDataHash UTF8String], -1, SQLITE_TRANSIENT);
			
			// Evaluate the SQL statement.
			if(SQLITE_DONE != sqlite3_step(insertStmt)) 
			{
				//NSAssert1(0, @"Error while inserting data. '%s'", sqlite3_errmsg(database));
				[TJCLog logWithLevel:LOG_NONFATAL_ERROR format:@"Error while inserting data"];
				flag=FALSE;
			}
    		
			// If the insert statement was successfully evaluated, insert the virtual good attributes in separate table.
			if (flag) 
			{
				// Delete the prepared statement.
				sqlite3_finalize(insertStmt);
				
				// Prepare the attributes statement.
				NSString *foreignKey = item.itemID;
				const char *sqlStatement = "insert into tapjoy_VGStoreItemAttribute(VGStoreItemID,AttributeName,AttributeValue) Values(?,?,?)";
				
				// Insert all existing virtual good attributes statements.
				for (int i = 0; i < [item.itemAttributes count]; i++)
				{
					insertStmt=nil;
					
					// Prepare the individual attribute statement.
					if(sqlite3_prepare_v2(database, sqlStatement, -1, &insertStmt, NULL) != SQLITE_OK) 
					{
						NSAssert1(0, @"Error while creating add statement. '%s'", sqlite3_errmsg(database));
					}
					
					// Bind the attribute data to the SQL statement.
					sqlite3_bind_text(insertStmt, 1, [foreignKey UTF8String], -1, SQLITE_TRANSIENT);
					sqlite3_bind_text(insertStmt, 2, [[[item.itemAttributes objectAtIndex:i] attributeType] UTF8String], -1, SQLITE_TRANSIENT);
					sqlite3_bind_text(insertStmt, 3, [[[item.itemAttributes objectAtIndex:i] attributeValue] UTF8String], -1, SQLITE_TRANSIENT);
					
					// Now evaluate the statement.
					if(SQLITE_DONE != sqlite3_step(insertStmt))
					{
						NSAssert1(0, @"Error while inserting data. '%s'", sqlite3_errmsg(database));
					}
					
					// Delete the prepared statement.
					sqlite3_finalize(insertStmt);
				}
				
			}
		}
		@catch (NSException * e) 
		{
			// Do nothing
		}
		@finally 
		{
			sqlite3_close(database);
		}
	}
}


// Returns if item ID exists purchased items.
- (BOOL)storeItemIDExists:(NSString *)itemID
{
	sqlite3 *database;
	int count = 0;
	
	// Get total number of records.
	if(sqlite3_open([[self getDBPath] UTF8String], &database) == SQLITE_OK)
	{
		@try
		{
			NSMutableString *sqlStatement = [[NSMutableString alloc] initWithString:@""];
			[sqlStatement appendFormat:@"select count(*) from tapjoy_VGStoreItems where VGStoreItemID='%@'", itemID];
			sqlite3_stmt *compiledStatement;
			
			if(sqlite3_prepare_v2(database, [sqlStatement UTF8String], -1, &compiledStatement, NULL) == SQLITE_OK)
			{
				while(sqlite3_step(compiledStatement) == SQLITE_ROW) 
				{
					count = sqlite3_column_int(compiledStatement, 0);
				}
				
				sqlite3_finalize(compiledStatement);
			}
			
			[sqlStatement release];
			return ((count > 0) ? YES : NO);
		}
		@catch (NSException * e)
		{
			// Do nothing
		}
		@finally 
		{
			sqlite3_close(database);
		}
	}
	return NO;
}


/**
 * Returns purchased items count
 */
- (int)purchasedItemsCount
{
	sqlite3 *database;
	int count = 0;
	
	// Get total number of records.
	if(sqlite3_open([[self getDBPath] UTF8String], &database) == SQLITE_OK)
	{
		@try
		{
			NSMutableString *sqlStatement = [[NSMutableString alloc] initWithString:@""];
			[sqlStatement appendFormat:@"select count(*) from tapjoy_VGStoreItems"];
			sqlite3_stmt *compiledStatement;
			if(sqlite3_prepare_v2(database, [sqlStatement UTF8String], -1, &compiledStatement, NULL) == SQLITE_OK) 
			{
				while(sqlite3_step(compiledStatement) == SQLITE_ROW) 
				{
					count = sqlite3_column_int(compiledStatement, 0);
				}
				sqlite3_finalize(compiledStatement);
			}
			[sqlStatement release];
			return count;
		}
		@catch (NSException * e)
		{
			// Do nothing
		}
		@finally 
		{
			sqlite3_close(database);
		}
	}
	return count;
}


/**
 * Returns purchased items, Item ID array
 */
- (NSMutableArray *)getPurchasedItemsIDArray
{
	sqlite3 *database;
	NSMutableArray *itemIDArray = [[NSMutableArray alloc] init];
	
	// Get total number of records.
	if(sqlite3_open([[self getDBPath] UTF8String], &database) == SQLITE_OK)
	{
		@try
		{
			NSMutableString *sqlStatement = [[NSMutableString alloc] initWithString:@""];
			[sqlStatement appendFormat:@"select VGStoreItemID from tapjoy_VGStoreItems"];
			sqlite3_stmt *compiledStatement;
			
			// Compile the the SQL query into a byte-code program.
			if(sqlite3_prepare_v2(database, [sqlStatement UTF8String], -1, &compiledStatement, NULL) == SQLITE_OK)
			{
				while(sqlite3_step(compiledStatement) == SQLITE_ROW)
				{
					NSString *itemID = [self getColTextValue:compiledStatement column:0];
					[itemIDArray addObject:itemID];
				}
				sqlite3_finalize(compiledStatement);
			}
			[sqlStatement release];
			return itemIDArray;
		}
		@catch (NSException * e)
		{
			// Do nothing
		}
		@finally 
		{
			sqlite3_close(database);
		}
	}
	return itemIDArray;
}


/**
 * Returns all purchased items list.
 */
- (NSMutableArray *)getAllPurchasedItems
{
	int totalRecords = [self purchasedItemsCount];
	int moreDataAvailable = 0;
	int start = 0;
	int max = totalRecords;
	//return [self getVGPurchasedItems:0 max:totalRecords moreDataAvailable:&moreData];
	
	sqlite3 *database;
	NSMutableArray *data = [[[NSMutableArray alloc] init] autorelease];
	
	// Get total number of records.
	if(sqlite3_open([[self getDBPath] UTF8String], &database) == SQLITE_OK)
	{
		@try
		{
			NSMutableString *sqlStatement = [[NSMutableString alloc] initWithString:@""];
			[sqlStatement appendFormat:@"select count(*) from tapjoy_VGStoreItems"];
			sqlite3_stmt *compiledStatement;
			if(sqlite3_prepare_v2(database, [sqlStatement UTF8String], -1, &compiledStatement, NULL) == SQLITE_OK)
			{
				while(sqlite3_step(compiledStatement) == SQLITE_ROW)
				{
					// Read total data count.
					moreDataAvailable = sqlite3_column_int(compiledStatement, 0);
				}
				sqlite3_finalize(compiledStatement);
			}
			
			[sqlStatement release];
			// Get purchased items and store them in return array.
			sqlStatement = [[NSMutableString alloc] initWithString:@""];
			
			NSString *limitQuery = [NSString stringWithFormat:@"limit %d, %d", start, max];
			[sqlStatement appendFormat:@"select * from tapjoy_VGStoreItems %@", limitQuery];
			if(sqlite3_prepare_v2(database, [sqlStatement UTF8String], -1, &compiledStatement, NULL) == SQLITE_OK)
			{
				// Loop through the results and add them to the feeds array
				while(sqlite3_step(compiledStatement) == SQLITE_ROW) 
				{
					// Read the data from the result row
					TJCVGStoreItem *itemObj = [[TJCVGStoreItem alloc] init];
					itemObj.itemID = [self getColTextValue:compiledStatement column:0];
					itemObj.itemAppleProductID = [self getColTextValue:compiledStatement column:1];
					itemObj.itemPrice = sqlite3_column_int(compiledStatement, 2);
					itemObj.itemName = [self getColTextValue:compiledStatement column:3];
					itemObj.itemDescription = [self getColTextValue:compiledStatement column:4];
					itemObj.itemTypeName = [self getColTextValue:compiledStatement column:5];
					itemObj.itemNumberOwned = sqlite3_column_int(compiledStatement, 6);
					itemObj.itemCurrencyName = [self getColTextValue:compiledStatement column:10];
					// Get path to application documents directory
					NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory , NSUserDomainMask, YES);
					NSString *documentsDir = [[NSString alloc] initWithFormat:@"%@", [paths objectAtIndex:0]];
					
					// Get thumb image file path
					NSString *colValue = [self getColTextValue:compiledStatement column:7];
					if ([colValue isEqualToString:@""]) 
					{
						itemObj.itemThumbImageFilePath = @"";
					}
					else
					{
						//itemObj.thumbImageFilePath = [documentsDir stringByAppendingFormat:@"%@", colValue];
						itemObj.itemThumbImageFilePath = [[NSString alloc] initWithFormat:@"%@%@", documentsDir, colValue];
						[itemObj.itemThumbImageFilePath release];
					}
					
					// Get full image file path
					colValue = [self getColTextValue:compiledStatement column:8];
					if ([colValue isEqualToString:@""])
					{
						itemObj.itemFullImageFilePath = @"";
					}
					else 
					{
						//itemObj.fullImageFilePath = [documentsDir stringByAppendingFormat:@"%@", colValue];
						itemObj.itemFullImageFilePath = [[NSString alloc] initWithFormat:@"%@%@", documentsDir, colValue];
						[itemObj.itemFullImageFilePath release];
					}
					
					// Get data file directory
					colValue = [self getColTextValue:compiledStatement column:9];
					if ([colValue isEqualToString:@""]) 
					{
						itemObj.itemDataFileDirPath = @"";
					}
					else
					{
						//itemObj.dataFileDirPath = [documentsDir stringByAppendingFormat:@"%@", colValue];
						itemObj.itemDataFileDirPath = [[NSString alloc] initWithFormat:@"%@%@", documentsDir, colValue];
						[itemObj.itemDataFileDirPath release];
					}
					
					// Get data hash.
					itemObj.itemDataHash = [self getColTextValue:compiledStatement column:11];
					
					[data addObject:itemObj];
					[itemObj release];
					[documentsDir release];
				}
				sqlite3_finalize(compiledStatement);
			}// End of outer, if (sqlite3_prepare_v2)
			[sqlStatement release];
			
			// Populate attribute values.
			for (int i = 0; i < [data count]; i++)
			{
				TJCVGStoreItem *itemObj = [data objectAtIndex:i];
				sqlStatement = [[NSMutableString alloc] initWithString:@""];
				
				[sqlStatement appendFormat:@"select * from tapjoy_VGStoreItemAttribute where VGStoreItemID ='%@'", itemObj.itemID];
				if(sqlite3_prepare_v2(database, [sqlStatement UTF8String], -1, &compiledStatement, NULL) == SQLITE_OK)
				{
					while(sqlite3_step(compiledStatement) == SQLITE_ROW)
					{
						NSString *aName = [self getColTextValue:compiledStatement column:2]; // Attribute name
						NSString *aValue = [self getColTextValue:compiledStatement column:3]; // Attribute value
						TJCVGStoreItemAttribute *attributeObj = [[TJCVGStoreItemAttribute alloc] init];
						
						attributeObj.attributeType = aName;
						attributeObj.attributeValue = aValue;
						
						[itemObj.itemAttributes addObject:attributeObj];
						[attributeObj release];
					}
					sqlite3_finalize(compiledStatement);
				}
				[sqlStatement release];
			}
			
			sqlStatement = nil;
			sqlite3_close(database);
			
			moreDataAvailable = moreDataAvailable - (start + [data count]);
			
			if (moreDataAvailable < 0)
			{
				moreDataAvailable = 0;
			}
			return data;
		}
		@catch (NSException * e)
		{
			// Do nothing
		}
		@finally 
		{
			sqlite3_close(database);
		}
	}
	return data;
}


//// Return Purchased VG Store Items from local DB
//- (NSMutableArray *)getVGPurchasedItems:(int)start max:(int)max moreDataAvailable:(int *)moreDataAvailable
//{
//
//}


@end
