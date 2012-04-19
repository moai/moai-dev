// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license


#import <UIKit/UIKit.h>

@class StoreItem;

/*!	\interface TJCLocalPurchasedItems
 *	\brief The Tapjoy Connect Local Purchased Items class.
 *
 */
@interface TJCLocalPurchasedItems : NSObject <UIAlertViewDelegate>
{

}

+ (TJCLocalPurchasedItems *) sharedTJCLocalPurchasedItems;

/*! \fn addLocalDataUpdateObservers
 *	\brief Adds local data update observers.
 * This sets up the callback selector methods that are called when a response from the server for virtual good data is received.
 * Notifications are posted in #fetchResponseSuccessWithData() in the #TJCVGStoreManager class.
 *	\param n/a
 *	\return n/a
 */
- (void) addLocalDataUpdateObservers;

/*! \fn addLocalDataUpdateObservers
 *	\brief Removes local data update observers.
 * By removing the data observers, the selector methods set up in #addLocalDataUpdateObservers() will no longer be called.
 *	\param n/a
 *	\return n/a
 */
- (void) removeLocalDataUpdateObservers;

/*! \fn localDataUpdateErrorSelector:(id)sender
 *	\brief The selector method called when an error occurs with the virtual goods request.
 *	\param sender The #TJCLocalPurchasedItems object.
 *	\return n/a
 */
- (void) localDataUpdateErrorSelector:(id)sender;

/*! \fn localDataUpdateReceievedSelector:(id)sender
 *	\brief The selector method called when the request for virtual goods succeeds.
 *	\param sender The #TJCLocalPurchasedItems object.
 *	\return n/a
 */
- (void) localDataUpdateReceievedSelector:(id)sender;

/*! \fn alertView:clickedButtonAtIndex:(UIAlertView alertView, NSInteger buttonIndex)
 *	\brief Delegate method to handle user input on the purchased items alert view.
 *	\param alertView The UIAlertView.
 *	\param buttonIndex Index of the button on the alert view that was pressed.
 *	\return n/a
 */
- (void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

/*! \fn getPurchasedVGStoreItemsArray
 *	\brief Returns the StoreItem object list for all downloaded purchased items.
 *	\param n/a
 *	\return The StoreItem object list.
 */
- (NSMutableArray*)getPurchasedVGStoreItemsArray;

/*! \fn getVGStoreItemsArray
 *	\brief Returns the StoreItem object list. This is the same as getPurchasedVGStoreItemsArray but is guaranteed to contain all virtual goods if game state is loaded first.
 *	\param n/a
 *	\return The StoreItem object list.
 */
- (NSMutableArray*)getVGStoreItemsArray;

@end
