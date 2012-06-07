// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#ifndef COMMONS
#define COMMONS

// Sent after an item has been successfully purchased. The item is then downloaded to the device when the selector is fired.
#define TJC_VG_PURCHASE_SUCCESS_NOTIFICATION	@"TJC_VG_PURCHASE_SUCCESS_NOTIFICATION"

// This notification is fired after the store front for the virtual goods is finished downloading. Ceratin views can then be displayed at this point.
#define TJC_VG_DISPLAY_VG_ITEMS	@"TJC_VG_DISPLAY_VG_ITEMS"

// Every time a virtual good has been downloaded to the device, purchased or not, this notification is fired. Used mostly to properly show the loading screen.
#define TJC_VG_ITEM_DOWNLOAD_FINISHED_FOR_LOADING_NOTIFICATION	@"TJC_VG_ITEM_DOWNLOAD_FINISHED_FOR_LOADING_NOTIFICATION"

// After all items have been loaded into the store view, this notification is fired to dismiss the loading view.
#define TJC_VG_DISMISS_LOADING_VIEW	@"TJC_VG_DISMISS_LOADING_VIEW"

#define SUBMIT_ALL_VG_STORE_ITEMS_RECIEVE_DATA					@"SUBMIT_ALL_VG_STORE_ITEMS_RECIEVE_DATA"
#define SUBMIT_ALL_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA	@"SUBMIT_ALL_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA"
#define SUBMIT_ALL_VG_STORE_ITEMS_ERROR_RECIEVE_DATA			@"SUBMIT_ALL_VG_STORE_ITEMS_ERROR_RECIEVE_DATA"

#define SUBMIT_PURCHASED_VG_STORE_ITEMS_RECIEVE_DATA				@"SUBMIT_PURCHASED_VG_STORE_ITEMS_RECIEVE_DATA"
#define SUBMIT_PURCHASED_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA	@"SUBMIT_PURCHASED_VG_STORE_ITEMS_INTERNET_ERROR_RECIEVE_DATA"
#define SUBMIT_PURCHASED_VG_STORE_ITEMS_ERROR_RECIEVE_DATA			@"SUBMIT_PURCHASED_VG_STORE_ITEMS_ERROR_RECIEVE_DATA"

// File download notifications
#define FILE_DOWNLOAD_SUCCESS_NOTIFICATION	@"FILE_DOWNLOAD_SUCCESS_NOTIFICATION"
#define FILE_DOWNLOAD_ERROR_NOTIFICATION	@"FILE_DOWNLOAD_ERROR_NOTIFICATION"

// Item download notifications
#define ITEM_DOWNLOAD_UPDATED_NOTIFICATION	@"ITEM_DOWNLOAD_UPDATED_NOTIFICATION"	// To be used when updating all vg items (game state).
#define ITEM_DOWNLOAD_SUCCESS_NOTIFICATION	@"ITEM_DOWNLOAD_SUCCESS_NOTIFICATION"
#define ITEM_DOWNLOAD_ERROR_NOTIFICATION	@"ITEM_DOWNLOAD_ERROR_NOTIFICATION"

// This is posted after all virtual good items have been downloaded to the client. This is only used for game state.
#define TJC_VG_ALL_ITEMS_SAVED_TO_CLIENT	@"TJC_VG_ALL_ITEMS_SAVED_TO_CLIENT"

// This is posted after virtual good data has finished downloading.
#define ITEM_DOWNLOAD_SUCCESS_NOTIFICATION_FOR_PANEL	@"ITEM_DOWNLOAD_SUCCESS_NOTIFICATION_FOR_PANEL"
#define ITEM_DOWNLOAD_ERROR_NOTIFICATION_FOR_PANEL		@"ITEM_DOWNLOAD_ERROR_NOTIFICATION_FOR_PANEL"

// This is posted after an item has been successfully purchased.
#define TJC_VG_ITEM_PURCHASE_RETURN			@"TJC_VG_ITEM_PURCHASE_RETURN"
#define TJC_VG_ITEM_PURCHASE_NET_ERROR		@"TJC_VG_ITEM_PURCHASE_NET_ERROR"

#endif
