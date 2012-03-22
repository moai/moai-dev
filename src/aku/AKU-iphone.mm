// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <aku/AKU-iphone.h>
#import <moaiext-iphone/moaiext-iphone.h>
#import <moaiext-iphone/MOAIFacebook.h>
#import <moaiext-iphone/Reachability.h>
#import <moaicore/MOAIEnvironment.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>

#import <Crittercism.h>
#import <TapjoyConnect.h>

//-----------------------------------------------------------------//
void AKUAppDidStartSession ( bool resumed ) {

	MOAIApp::Get ().DidStartSession ( resumed );
}

//-----------------------------------------------------------------//
void AKUAppOpenFromURL ( NSURL* url ) {
	
	MOAIApp::Get ().AppOpenedFromURL ( url );
	MOAIFacebook::Get ().HandleOpenURL ( url );
}

//-----------------------------------------------------------------//
void AKUAppWillEndSession () {

	MOAIApp::Get ().WillEndSession ();
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
		
		MOAIEnvironment& environment = MOAIEnvironment::Get ();
		
		environment.SetValue ( MOAI_ENV_carrierISOCountryCode,		[ carrierInfo.isoCountryCode UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierMobileCountryCode,	[[carrierInfo mobileCountryCode ] UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierName,				[[carrierInfo carrierName ] UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierMobileNetworkCode,	[[carrierInfo mobileNetworkCode ] UTF8String ]);
		
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
	
	// Device properties
	MOAIEnvironment& environment = MOAIEnvironment::Get ();
	
	environment.SetValue ( MOAI_ENV_appDisplayName,		[[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleDisplayName" ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_appID,				[[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleIdentifier" ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_appVersion,			[[[[ NSBundle mainBundle ] infoDictionary ] objectForKey:@"CFBundleShortVersionString" ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_cacheDirectory,		[[ NSSearchPathForDirectoriesInDomains ( NSCachesDirectory, NSUserDomainMask, YES ) objectAtIndex:0 ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_countryCode,		[[[ NSLocale currentLocale ] objectForKey: NSLocaleCountryCode ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_devModel,			[[ UIDevice currentDevice ].model UTF8String ] );
	environment.SetValue ( MOAI_ENV_documentDirectory,	[[ NSSearchPathForDirectoriesInDomains ( NSDocumentDirectory, NSUserDomainMask, YES ) objectAtIndex:0 ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_iosRetinaDisplay,	[[ UIScreen mainScreen ] scale ] == 2.0 );
	environment.SetValue ( MOAI_ENV_languageCode,		[[[ NSLocale currentLocale ] objectForKey: NSLocaleLanguageCode ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_osBrand,			"iOS" );
	environment.SetValue ( MOAI_ENV_osVersion,			[[ UIDevice currentDevice ].systemVersion UTF8String ]);
	environment.SetValue ( MOAI_ENV_resourceDirectory,	[[[ NSBundle mainBundle ] resourcePath ] UTF8String ]);
	environment.SetValue ( MOAI_ENV_udid,				[[ UIDevice currentDevice ].uniqueIdentifier UTF8String ]);
			
	// MOAI
	REGISTER_LUA_CLASS ( MOAIApp )
	REGISTER_LUA_CLASS ( MOAIGameCenter )
	REGISTER_LUA_CLASS ( MOAIWebView )
			
	#ifndef DISABLE_TAPJOY
		REGISTER_LUA_CLASS ( MOAITapjoy )
	#endif

	#ifndef DISABLE_NOTIFICATIONS
		REGISTER_LUA_CLASS ( MOAINotifications )
	#endif

	#ifndef DISABLE_CRITTERCISM
		REGISTER_LUA_CLASS ( MOAICrittercism )
	#endif
		
	REGISTER_LUA_CLASS ( MOAIFacebook )
}

//----------------------------------------------------------------//
void AKUNotifyRemoteNotificationReceived ( NSDictionary* notification ) {

	MOAINotifications::Get ().NotifyRemoteNotificationReceived ( notification );
}

//----------------------------------------------------------------//
void AKUNotifyRemoteNotificationRegistrationComplete ( NSData* deviceToken ) {

	MOAINotifications::Get ().NotifyRemoteRegistrationComplete ( deviceToken );
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
void AKUSetDefaultFrameBuffer ( GLuint frameBuffer ) {

	MOAIGfxDevice::Get ().SetDefaultFrameBuffer ( frameBuffer );
}