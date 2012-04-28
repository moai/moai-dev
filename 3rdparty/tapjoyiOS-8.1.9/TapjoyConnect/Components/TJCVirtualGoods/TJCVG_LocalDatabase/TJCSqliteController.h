// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license



#import <sqlite3.h>
#import "TJCVGStoreModel.h"


#define TJC_VG_DB_NAME						@"localVGStore.sql"
#define TJC_SQLITE_DB_VERSION_KEY_NAME	@"TJC_SQLITE_DB_VERSION_KEY_NAME"
#define TJC_SQLITE_DB_VERSION				3
#define TJC_VG_DB_VER_FILENAME			@"tjcbdver.txt"


/*!	\interface TJCSqliteController
 *	\brief The Tapjoy Connect SQLite Controller class.
 *
 * This class handles storing and retreiving data from the virtual goods sqlite database. Virtual goods data is returned as an NSMutableArray of TJCLocalVGStoreItem.
 */
@interface TJCSqliteController : NSObject 
{
	
}

+ (TJCSqliteController*) sharedTJCSqliteController;

/*! \fn dealloc
 *	\brief Releases any retained resources.
 *	\param n/a
 *	\return n/a
 */
- (void)dealloc;

/*! \fn copyDatabaseIfNeeded
 *	\brief Executes a check to see whether the database file exists in the predefined database file path, and if not, locates the database file and copies it to the predefined file path.
 *	\param n/a
 *	\return n/a
 */
- (void)copyDatabaseIfNeeded;

/*! \fn getDBPath
 *	\brief Runs a search for the database file in standard documents using NSSearchPathForDirectoriesInDomains.
 *	\param n/a
 *	\return The file path of the database file.
 */
- (NSString*)getDBPath;

/*!	\fn previousDateString:(int)daysBack
 *	\brief Returns date string, calculated by taking the current date minus daysBack
 *	\param daysBack The number of days to back from the current date.
 *	\return The string of the calculated date.
 */
- (NSString*)previousDateString:(int)daysBack;

/*!	\fn getColTextValue:column:(sqlite3_stmt* compiledStatement, int col)
 *	\brief Retrieves the name of a column from the given SQL compiled statement.
 *	\param compiledStatement The SQL compiled statement.
 *	\param col The column number to pull the name from.
 *	\return The string containing the name of column.
 */
- (NSString*)getColTextValue:(sqlite3_stmt*)compiledStatement column:(int)col;

/*!	\fn saveVirtualGoodData:(TJCVGStoreItem*)item
 *	\brief Inserts the virtual goods data from the given virtual good item into the database.
 *	\param item The virtual good item.
 *	\return n/a
 */
- (void)saveVirtualGoodData:(TJCVGStoreItem*)item;

/*!	\fn storeItemIDExists:(NSString*)itemID
 *	\brief Query whether a store item exists or not in the virtual goods database.
 *	\param itemID The item ID.
 *	\return TRUE if the store item exists, FALSE otherwise.
 */
- (BOOL)storeItemIDExists:(NSString*)itemID;

- (int)purchasedItemsCount;

- (NSMutableArray*)getPurchasedItemsIDArray;

//- (NSMutableArray*)getVGPurchasedItems:(int)start max:(int)max moreDataAvailable:(int *)moreDataAvailable;

- (NSMutableArray*)getAllPurchasedItems;

@end


