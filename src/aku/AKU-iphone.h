//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_IPHONE_H
#define AKU_IPHONE_H

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import <UIKit/UIKit.h>

void		AKUAppDidFailToRegisterForRemoteNotificationsWithError	( NSError* error );
void		AKUAppDidReceiveLocalNotification						( UILocalNotification* notification );
void		AKUAppDidReceiveRemoteNotification						( NSDictionary* userInfo );
void		AKUAppDidRegisterForRemoteNotificationsWithDeviceToken	( NSData* deviceToken );
void		AKUIphoneInit											( UIApplication* application );

#endif
