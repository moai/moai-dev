// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-iphone.h>
#include <moai-iphone/MOAIApp.h>

//----------------------------------------------------------------//
void AKUAppDidFailToRegisterForRemoteNotificationsWithError ( NSError* error ) {

	MOAIApp::Get ().DidFailToRegisterForRemoteNotificationsWithError ( error );
}

//----------------------------------------------------------------//
void AKUAppDidReceiveLocalNotification ( UILocalNotification* notification ) {

	MOAIApp::Get ().DidReceiveLocalNotification ( notification );
}

//----------------------------------------------------------------//
void AKUAppDidReceiveRemoteNotification ( NSDictionary* userInfo ) {

	MOAIApp::Get ().DidReceiveRemoteNotification ( userInfo );
}

//----------------------------------------------------------------//
void AKUAppDidRegisterForRemoteNotificationsWithDeviceToken ( NSData* deviceToken ) {

	MOAIApp::Get ().DidRegisterForRemoteNotificationsWithDeviceToken ( deviceToken );
}

//----------------------------------------------------------------//
void AKUIphoneInit ( UIApplication* application ) {

	MOAIApp::Get ().SetApplication ( application );
	
	// MOAI
	REGISTER_LUA_CLASS ( MOAIApp )
}
