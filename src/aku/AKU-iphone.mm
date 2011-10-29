// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <aku/AKU-iphone.h>
#import <moaiext-iphone/moaiext-iphone.h>
#import <moaiext-iphone/Reachability.h>
#import <moaicore/MOAIEnvironment.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>

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

//-----------------------------------------------------------------//
void AKUAppDidStartSession () {

	MOAIApp::Get ().DidStartSession ();
}

//-----------------------------------------------------------------//
void AKUAppOpenFromURL ( NSURL* url ) {
	
	MOAIApp::Get ().AppOpenedFromURL ( url );
}

//-----------------------------------------------------------------//
void AKUAppWillEndSession () {

	MOAIApp::Get ().WillEndSession ();
}

//-----------------------------------------------------------------//
const char* AKUGetGUID ( ) {

	CFUUIDRef uuid = CFUUIDCreate( NULL );
	NSString* session_uuid = ( NSString * ) CFUUIDCreateString( NULL, uuid );
	CFRelease( uuid );
	return [ session_uuid UTF8String ];
}

//-----------------------------------------------------------------//
long AKUGetIphoneNetworkReachability ( ) {


	Reachability *reach = [ Reachability reachabilityForInternetConnection ];
	NetworkStatus status = [ reach currentReachabilityStatus ];
		
	if ( status == NotReachable ) {
		return ( long )CONNECTION_TYPE_NONE;
	} else if ( status == ReachableViaWWAN ) {
		// Update network information
		CTCarrier* carrierInfo = [[[ CTTelephonyNetworkInfo alloc ] init ] subscriberCellularProvider ];
		MOAIEnvironment::Get ().SetCarrierISOCountryCode ( [ carrierInfo.isoCountryCode UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierMobileCountryCode ( [[carrierInfo mobileCountryCode ] UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierName ( [[carrierInfo carrierName ] UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierMobileNetworkCode ( [[carrierInfo mobileNetworkCode ] UTF8String ]);
		return ( long )CONNECTION_TYPE_WWAN;
	} else if ( status == ReachableViaWiFi ) {
		return ( long )CONNECTION_TYPE_WIFI;
	}
	
	return ( long )CONNECTION_TYPE_NONE;
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

	MOAIApp::Affirm ().SetApplication ( application );
	MOAIGameCenter::Affirm ();
	
	MOAIEnvironment::Affirm ();
	// Device properties	
	MOAIEnvironment::Get ().SetAppVersion ( [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleVersion" ] UTF8String ] );
	MOAIEnvironment::Get ().SetAppID ( [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleIdentifier" ] UTF8String ] );
	MOAIEnvironment::Get ().SetAppDisplayName ( [[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleDisplayName" ] UTF8String ] );
	MOAIEnvironment::Get ().SetCacheDirectory ( [[ NSSearchPathForDirectoriesInDomains ( NSCachesDirectory, NSUserDomainMask, YES ) objectAtIndex:0 ] UTF8String ]);
	MOAIEnvironment::Get ().SetCountryCode ( [[[ NSLocale currentLocale ] objectForKey: NSLocaleCountryCode ] UTF8String ]);
	MOAIEnvironment::Get ().SetDocumentDirectory ( [[ NSSearchPathForDirectoriesInDomains ( NSDocumentDirectory, NSUserDomainMask, YES ) objectAtIndex:0 ] UTF8String ]);
	MOAIEnvironment::Get ().SetGUIDFunc ( &AKUGetGUID );
	MOAIEnvironment::Get ().SetLanguageCode ( [[[ NSLocale currentLocale ] objectForKey: NSLocaleLanguageCode ] UTF8String ]);
	MOAIEnvironment::Get ().SetOSBrand ( "iOS" );
	MOAIEnvironment::Get ().SetOSVersion ( [[ UIDevice currentDevice ].systemVersion UTF8String ] );
	MOAIEnvironment::Get ().SetResourceDirectory ( [[[ NSBundle mainBundle ] resourcePath ] UTF8String ]);
	MOAIEnvironment::Get ().SetUDID ( [[ UIDevice currentDevice ].uniqueIdentifier UTF8String ] );
	MOAIEnvironment::Get ().SetDevModel ( [[ UIDevice currentDevice ].model UTF8String ] );

	CGRect screenRect = [[UIScreen mainScreen] bounds];
	MOAIEnvironment::Get ().SetScreenSize( screenRect.size.width, screenRect.size.height );

	if ([[ UIScreen mainScreen ] scale ] == 2.0 ) {
		//this is retina
		MOAIEnvironment::Get ().SetIsRetinaDisplay ( true );
	}
	else {
		MOAIEnvironment::Get ().SetIsRetinaDisplay ( false );
	}	
			
	// MOAI
	REGISTER_LUA_CLASS ( MOAIApp )
	REGISTER_LUA_CLASS ( MOAIGameCenter )
	REGISTER_LUA_CLASS ( MOAIWebView )
			
	#ifndef DISABLE_TAPJOY
		REGISTER_LUA_CLASS ( MOAITapjoy )
	#endif
}

//-----------------------------------------------------------------//
void AKUSetConnectionType ( long type ) {

	MOAIEnvironment::Get ().SetConnectionType ( type );
	
	// If we have a cellualr connection, get carrier information
	if ( type == CONNECTION_TYPE_WWAN ) {
	
		CTCarrier* carrierInfo = [[[ CTTelephonyNetworkInfo alloc ] init ] subscriberCellularProvider ];
		MOAIEnvironment::Get ().SetCarrierISOCountryCode ( [ carrierInfo.isoCountryCode UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierMobileCountryCode ( [[ carrierInfo mobileCountryCode ] UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierName ( [[ carrierInfo carrierName ] UTF8String ]);
		MOAIEnvironment::Get ().SetCarrierMobileNetworkCode ( [[ carrierInfo mobileNetworkCode ] UTF8String ]);
	}
}

//-----------------------------------------------------------------//
void AKUSetDefaultFrameBuffer ( GLuint frameBuffer ) {

	MOAIGfxDevice::Get ().SetDefaultFrameBuffer ( frameBuffer );
}

//-----------------------------------------------------------------//
void AKUWasLaunchedWithRemoteNotification ( NSDictionary* remoteNotificationPayload ) {

	if ( remoteNotificationPayload ) {
	
		MOAIApp::Get ().SetRemoteNotificationPayload ( remoteNotificationPayload );
	}
}
