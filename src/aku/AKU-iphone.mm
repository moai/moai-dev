// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <aku/AKU-iphone.h>
#include <moaiext-iphone/moaiext-iphone.h>

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
	USLuaStateHandle state = USLuaRuntime ().Get (). State ();
	MOAISim::Get ().PushLuaClassTable ( state );
	
	state.SetField ( -1, "DEVICE_UDID", [[ UIDevice currentDevice ].uniqueIdentifier UTF8String ]);
	state.SetField ( -1, "DEVICE_OS_BRAND", "iOS" );
	state.SetField ( -1, "DEVICE_OS_VERSION", [[ UIDevice currentDevice ].systemVersion UTF8String ]);
	state.SetField ( -1, "DEVICE_CONNECTION_TYPE", [ AKUGetIphoneNetworkReachability () UTF8String ]);
	state.SetField ( -1, "APPLICATION_VERSION", [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleVersion" ] UTF8String ]);
	state.SetField ( -1, "APPLICATION_ID", [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleIdentifier" ] UTF8String ]);
	state.SetField ( -1, "APPLICATION_DISPLAY_NAME", [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleDisplayName" ] UTF8String ]);
	
	lua_pop ( state, 1 );
	
	// MOAI
	REGISTER_LUA_CLASS ( MOAIApp )
	REGISTER_LUA_CLASS ( MOAIWebView )
}

//-----------------------------------------------------------------//
NSString* AKUGetIphoneNetworkReachability ( ) {

	Reachability *reach = [ Reachability reachabilityForInternetConnection ];
	NetworkStatus status = [ reach currentReachabilityStatus ];
		
	if ( status == NotReachable ) {
		return @"NO_CONNECTION";
	} else if ( status == ReachableViaWWAN ) {
		return @"WWAN_CONNECTION";
	} else if ( status == ReachableViaWiFi ) {
		return @"WIFI_CONNECTION";
	}
	
	return @"NO_CONNECTION";
}

