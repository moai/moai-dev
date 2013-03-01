// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <aku/AKU-iphone.h>
#import <moaiext-iphone/moaiext-iphone.h>

#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>

#import <OpenUDID/MOAIOpenUDID.h>

//-----------------------------------------------------------------//
void AKUAppDidStartSession ( bool resumed ) {

	MOAIAppIOS::Get ().DidStartSession ( resumed );
}

//-----------------------------------------------------------------//
void AKUAppOpenFromURL ( NSURL* url ) {
	
	MOAIAppIOS::Get ().AppOpenedFromURL ( url );

	#ifndef DISABLE_FACEBOOK
		MOAIFacebookIOS::Get ().HandleOpenURL ( url );
	#endif
}

//-----------------------------------------------------------------//
void AKUAppWillEndSession () {

	MOAIAppIOS::Get ().WillEndSession ();
}

//-----------------------------------------------------------------//
long AKUGetIphoneNetworkReachability ( ) {

	Reachability *reach = [ Reachability reachabilityForInternetConnection ];
	NetworkStatus status = [ reach currentReachabilityStatus ];
	
	if ( status == NotReachable ) {
		
		return ( long )CONNECTION_TYPE_NONE;
		
	} else if ( status == ReachableViaWWAN ) {
		
		MOAIEnvironment& environment = MOAIEnvironment::Get ();
		
		// Update network information
		CTCarrier* carrierInfo = [[[ CTTelephonyNetworkInfo alloc ] init ] subscriberCellularProvider ];
		
		environment.SetValue ( MOAI_ENV_carrierISOCountryCode,		[ carrierInfo.isoCountryCode UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierMobileCountryCode,	[[ carrierInfo mobileCountryCode ] UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierName,				[[ carrierInfo carrierName ] UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierMobileNetworkCode,	[[ carrierInfo mobileNetworkCode ] UTF8String ]);
		
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

	MOAIAppIOS::Affirm ().SetApplication ( application );
			
	// MOAI
	REGISTER_LUA_CLASS ( MOAIAppIOS )
	REGISTER_LUA_CLASS ( MOAIBillingIOS )
	REGISTER_LUA_CLASS ( MOAIDialogIOS )
	REGISTER_LUA_CLASS ( MOAIGameCenterIOS )
	REGISTER_LUA_CLASS ( MOAIKeyboardIOS )
	REGISTER_LUA_CLASS ( MOAIMoviePlayerIOS )
	REGISTER_LUA_CLASS ( MOAISafariIOS )
	REGISTER_LUA_CLASS ( MOAIWebViewIOS )
	REGISTER_LUA_CLASS ( MOAITwitterIOS )
	
	#ifndef DISABLE_TAPJOY
		REGISTER_LUA_CLASS ( MOAITapjoyIOS )
	#endif

	#ifndef DISABLE_NOTIFICATIONS
		REGISTER_LUA_CLASS ( MOAINotificationsIOS )
	#endif

	#ifndef DISABLE_CRITTERCISM
		REGISTER_LUA_CLASS ( MOAICrittercismIOS )
	#endif
		
	#ifndef DISABLE_FACEBOOK
		REGISTER_LUA_CLASS ( MOAIFacebookIOS )
	#endif
	
	// Device properties
	MOAIEnvironment& environment = MOAIEnvironment::Get ();
	
	environment.SetValue ( MOAI_ENV_connectionType, ( int ) AKUGetIphoneNetworkReachability() );
	
	environment.SetValue ( MOAI_ENV_appDisplayName,		[[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleDisplayName" ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_appID,				[[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleIdentifier" ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_appVersion,			[[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleShortVersionString" ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_cacheDirectory,		[[ NSSearchPathForDirectoriesInDomains ( NSCachesDirectory, NSUserDomainMask, YES ) objectAtIndex:0 ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_countryCode,		[[[ NSLocale currentLocale ] objectForKey: NSLocaleCountryCode ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_devModel,			[[ UIDevice currentDevice ].model UTF8String ] );

	//AJV TODO: checking with HBS on intention here
	//environment.SetValue ( MOAI_ENV_devPlatform,		[[ UIDevice currentDevice ].platform UTF8String ]);
	environment.SetValue ( MOAI_ENV_documentDirectory,	[[ NSSearchPathForDirectoriesInDomains ( NSDocumentDirectory, NSUserDomainMask, YES ) objectAtIndex:0 ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_iosRetinaDisplay,	[[ UIScreen mainScreen ] scale ] == 2.0 );
	environment.SetValue ( MOAI_ENV_languageCode,		[[[ NSLocale currentLocale ] objectForKey: NSLocaleLanguageCode ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_osBrand,			"iOS" );
	environment.SetValue ( MOAI_ENV_osVersion,			[[ UIDevice currentDevice ].systemVersion UTF8String ]);
	environment.SetValue ( MOAI_ENV_resourceDirectory,	[[[ NSBundle mainBundle ] resourcePath ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_openUdid,			[[ MOAIOpenUDID value] UTF8String ]);
	environment.SetValue ( MOAI_ENV_horizontalResolution, [[ UIScreen mainScreen ] bounds ].size.width * [[ UIScreen mainScreen ] scale ] );
	environment.SetValue ( MOAI_ENV_verticalResolution, [[ UIScreen mainScreen ] bounds ].size.height * [[ UIScreen mainScreen ] scale ] );
}

//----------------------------------------------------------------//
void AKUNotifyLocalNotificationReceived ( UILocalNotification* notification ) {
	
#ifndef DISABLE_NOTIFICATIONS
	MOAINotificationsIOS::Get ().NotifyLocalNotificationReceived ( notification );
#endif
}

//----------------------------------------------------------------//
void AKUNotifyRemoteNotificationReceived ( NSDictionary* notification ) {

#ifndef DISABLE_NOTIFICATIONS
	MOAINotificationsIOS::Get ().NotifyRemoteNotificationReceived ( notification );
#endif
}

//----------------------------------------------------------------//
void AKUNotifyRemoteNotificationRegistrationComplete ( NSData* deviceToken ) {

#ifndef DISABLE_NOTIFICATIONS
	MOAINotificationsIOS::Get ().NotifyRemoteRegistrationComplete ( deviceToken );
#endif
}

//-----------------------------------------------------------------//
void AKUSetConnectionType ( long type ) {
	
	MOAIEnvironment& environment = MOAIEnvironment::Get ();
	environment.SetValue ( MOAI_ENV_connectionType, ( int )type );
	
	// If we have a cellualr connection, get carrier information
	if ( type == CONNECTION_TYPE_WWAN ) {
	
		CTCarrier* carrierInfo = [[[ CTTelephonyNetworkInfo alloc ] init ] subscriberCellularProvider ];
		
		environment.SetValue ( MOAI_ENV_carrierISOCountryCode,		[ carrierInfo.isoCountryCode UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierMobileCountryCode,	[[carrierInfo mobileCountryCode ] UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierName,				[[carrierInfo carrierName ] UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierMobileNetworkCode,	[[carrierInfo mobileNetworkCode ] UTF8String ]);
	}
}


//-----------------------------------------------------------------//
void AKUSetFrameBuffer ( GLuint frameBuffer ) {
	MOAIGfxDevice::Get ().GetDefaultBuffer ()->SetGLFrameBufferID (frameBuffer);
}