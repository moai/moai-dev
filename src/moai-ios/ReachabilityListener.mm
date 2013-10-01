// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <ReachabilityListener.h>
#import <Reachability.h>
#import <moai-sim/headers.h>

#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <CoreTelephony/CTCarrier.h>

//================================================================//
// ReachabilityListener
//================================================================//
@implementation ReachabilityListener


//----------------------------------------------------------------//
-(void) reachabilityChanged: ( NSNotification* )note {
	UNUSED ( note );
	[ ReachabilityListener updateMoaiEnvironment ];
}

//----------------------------------------------------------------//
- ( void ) startListener {

	[[ NSNotificationCenter defaultCenter ] addObserver: self selector: @selector ( reachabilityChanged: ) name: kReachabilityChangedNotification object: nil ];
	mReach = [[ Reachability reachabilityForInternetConnection ] retain ];
	[ mReach startNotifier ];
}


//----------------------------------------------------------------//
- ( void ) stopListener {	
	[ mReach release ];
}

//----------------------------------------------------------------//
+( void ) updateMoaiEnvironment {

	NetworkStatus status = [[ Reachability reachabilityForInternetConnection ] currentReachabilityStatus ];
	
	MOAIEnvironment& environment = MOAIEnvironment::Get ();
	environment.SetValue ( MOAI_ENV_connectionType, ( int )status );
	
	// If we have a cellualr connection, get carrier information
	if ( status == ReachableViaWWAN ) {
	
		CTCarrier* carrierInfo = [[[ CTTelephonyNetworkInfo alloc ] init ] subscriberCellularProvider ];
		
		environment.SetValue ( MOAI_ENV_carrierISOCountryCode,		[ carrierInfo.isoCountryCode UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierMobileCountryCode,	[[ carrierInfo mobileCountryCode ] UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierName,				[[ carrierInfo carrierName ] UTF8String ]);
		environment.SetValue ( MOAI_ENV_carrierMobileNetworkCode,	[[ carrierInfo mobileNetworkCode ] UTF8String ]);
	}
}

@end
