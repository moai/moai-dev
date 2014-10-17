// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <moai-ios/MOAIReachabilityListener.h>
#import <moai-ios/MOAIReachability.h>
#import <moai-sim/headers.h>

#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>

//================================================================//
// MOAIReachabilityListener ()
//================================================================//
@interface MOAIReachabilityListener ()

	//----------------------------------------------------------------//
	-( void )	reachabilityChanged			:( NSNotification* )note;
@end

//================================================================//
// MOAIReachabilityListener
//================================================================//
@implementation MOAIReachabilityListener

//----------------------------------------------------------------//
-( void ) dealloc {
	
	[ self stopListener ];
}

//----------------------------------------------------------------//
-( void ) reachabilityChanged: ( NSNotification* )note {
	UNUSED ( note );
	[ self updateMoaiEnvironment ];
}

//----------------------------------------------------------------//
- ( void ) startListener {

	[ self stopListener ];

	mContext = MOAIGlobalsMgr::Get ();

	[[ NSNotificationCenter defaultCenter ] addObserver: self selector: @selector ( reachabilityChanged: ) name: kReachabilityChangedNotification object: nil ];
	mReach = [[ MOAIReachability reachabilityForInternetConnection ] retain ];
	[ mReach startNotifier ];
}


//----------------------------------------------------------------//
- ( void ) stopListener {

	if ( mReach ) {
		[ mReach stopNotifier ];
		[ mReach release ];
		[[ NSNotificationCenter defaultCenter ] removeObserver: self name: kReachabilityChangedNotification object: nil ];
	}
	mReach = nil;
}

//----------------------------------------------------------------//
-( void ) updateMoaiEnvironment {

	MOAIScopedContext scopedContext;

	if ( !MOAIGlobalsMgr::Check ( mContext )) return;
	MOAIGlobalsMgr::Set ( mContext );
	
	if ( !MOAIEnvironment::IsValid ()) return;
	MOAIEnvironment& environment = MOAIEnvironment::Get();

	NetworkStatus status = [[ MOAIReachability reachabilityForInternetConnection ] currentReachabilityStatus ];
	environment.SetValue ( MOAI_ENV_connectionType, ( int )status );
	
	// If we have a cellualr connection, get carrier information
	if ( status == ReachableViaWWAN ) {
	
		Class networkInfoClass = NSClassFromString ( @"CTTelephonyNetworkInfo" );
		if ( networkInfoClass ) {
		
			CTCarrier* carrierInfo = [[[ networkInfoClass alloc ] init ] subscriberCellularProvider ];
			
			environment.SetValue ( MOAI_ENV_carrierISOCountryCode,		[ carrierInfo.isoCountryCode UTF8String ]);
			environment.SetValue ( MOAI_ENV_carrierMobileCountryCode,	[[ carrierInfo mobileCountryCode ] UTF8String ]);
			environment.SetValue ( MOAI_ENV_carrierName,				[[ carrierInfo carrierName ] UTF8String ]);
			environment.SetValue ( MOAI_ENV_carrierMobileNetworkCode,	[[ carrierInfo mobileNetworkCode ] UTF8String ]);
		}
	}
}

@end
