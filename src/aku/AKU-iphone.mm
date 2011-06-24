// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-iphone.h>
#include <moaiext-iphone/moaiext-iphone.h>
#include <moaicore/MOAIEnvironment.h>

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

	loadMoaiLib_NSArray ();
	loadMoaiLib_NSData ();
	loadMoaiLib_NSDate ();
	loadMoaiLib_NSDictionary ();
	loadMoaiLib_NSError ();
	loadMoaiLib_NSNumber ();
	loadMoaiLib_NSObject ();
	loadMoaiLib_NSString ();
	
	MOAIApp::Get ().SetApplication ( application );
	
	// Device properties	
	MOAIEnvironment::Get ().SetAppVersion ( [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleVersion" ] UTF8String ] );
	MOAIEnvironment::Get ().SetAppID ( [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleIdentifier" ] UTF8String ] );
	MOAIEnvironment::Get ().SetAppDisplayName ( [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleDisplayName" ] UTF8String ] );
	MOAIEnvironment::Get ().SetConnectivityFunc ( &AKUGetIphoneNetworkReachability );
	MOAIEnvironment::Get ().SetGUIDFunc ( &AKUGetGUID );
	MOAIEnvironment::Get ().SetOSBrand ( "iOS" );
	MOAIEnvironment::Get ().SetOSVersion ( [[ UIDevice currentDevice ].systemVersion UTF8String ] );
	MOAIEnvironment::Get ().SetUDID ( [[ UIDevice currentDevice ].uniqueIdentifier UTF8String ] );
	
	// MOAI
	REGISTER_LUA_CLASS ( MOAIApp )
	REGISTER_LUA_CLASS ( MOAIWebView )
}

//-----------------------------------------------------------------//
long AKUGetIphoneNetworkReachability ( ) {


	Reachability *reach = [ Reachability reachabilityForInternetConnection ];
	NetworkStatus status = [ reach currentReachabilityStatus ];
		
	if ( status == NotReachable ) {
		return ( long )CONNECTION_TYPE_NONE;
	} else if ( status == ReachableViaWWAN ) {
		return ( long )CONNECTION_TYPE_WWAN;
	} else if ( status == ReachableViaWiFi ) {
		return ( long )CONNECTION_TYPE_WIFI;
	}
	
	return ( long )CONNECTION_TYPE_NONE;
}

//-----------------------------------------------------------------//
const char* AKUGetGUID ( ) {

	CFUUIDRef uuid = CFUUIDCreate( NULL );
	NSString* session_uuid = ( NSString * ) CFUUIDCreateString( NULL, uuid );
	CFRelease( uuid );
	return [ session_uuid UTF8String ];
}

