// Copyright (C) 2011-2012 by Tapjoy Inc.
//
// This file is part of the Tapjoy SDK.
//
// By using the Tapjoy SDK in your software, you agree to the terms of the Tapjoy SDK License Agreement.
//
// The Tapjoy SDK is bound by the Tapjoy SDK License Agreement and can be found here: https://www.tapjoy.com/sdk/license

#import <Foundation/Foundation.h>
#import "TBXML.h"



/*!	\interface TJCFeaturedAppModel
 *	\brief The Tapjoy Connect Featured App Model class.
 *
 * The #TJCFeaturedAppModel represents a featured app object and is initialized when 
 * the server responds to the URL request for getting the feature app data is completed.
 */
@interface TJCFeaturedAppModel : NSObject 
{
	NSString *cost_;				/*!< The cost of the featured app. */
	NSString *storeID_;			/*!< The store id of the featured app. */
	NSString *name_;				/*!< The name of the featured app. */
	NSString *description_;		/*!< The description for the featured app. */
	int amount_;					/*!< The amount of virtual currency the user gets if they complete the featured app offer. */
	NSString *iconURL_;			/*!< The URL to the location of the icon for the featured app. */
	NSString *largeIconURL_;	/*!< The URL to the location of the large icon for featured app. */
	NSString *redirectURL_;		/*!< The redirect URL for the featured app. */
	int	maxTimesToDisplayThisApp_;	/*!< The maximum number of times this feature app may appear. */
	
	NSString *fullScreenAdURL_;		/*!< The URL to the full screen Ad. This is used when the Tapjoy Full Screen Ad web view is used to display the featured app. */
}

@property (copy) NSString* cost;
@property (copy) NSString* storeID;
@property (copy) NSString* name;
@property (copy) NSString* description;
@property (copy) NSString* iconURL;
@property (copy) NSString* largeIconURL;
@property (copy) NSString* redirectURL;
@property (assign) int amount;
@property (assign) int maxTimesToDisplayThisApp;
@property (copy) NSString* fullScreenAdURL;

/*!	\fn initWithTBXML:(TBXMLElement*) aXMLElement
 *	\brief Initializes the #TJCFeaturedAppModel object with the given xml data.
 *
 *	\param aXMLElement The xml element contains all the relevant #TJCFeaturedAppModel object data.
 *	\return The #TJCFeaturedAppModel object.
 */
- (id) initWithTBXML:(TBXMLElement*) aXMLElement;


@end
