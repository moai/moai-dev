// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#import <moai-apple/MOAIReachability.h>
#import <moai-apple/MOAIReachabilityMgr.h>
#import <moai-apple/NSData+MOAILib.h>
#import <moai-apple/NSDate+MOAILib.h>
#import <moai-apple/NSDictionary+MOAILib.h>
#import <moai-apple/NSError+MOAILib.h>
#import <moai-apple/NSString+MOAILib.h>

#import <ifaddrs.h>
#import <arpa/inet.h>

#import <CoreFoundation/CoreFoundation.h>

#ifdef MOAI_OS_IPHONE
	#import<CoreTelephony/CTCallCenter.h>
	#import<CoreTelephony/CTCall.h>   
	#import<CoreTelephony/CTCarrier.h>    
	#import<CoreTelephony/CTTelephonyNetworkInfo.h>
#endif

//================================================================//
// MOAIReachabilityListener
//================================================================//
@interface MOAIReachabilityListener : NSObject {

	MOAIGlobals*			mContext;
	MOAIReachability*		mReach;
}

	//----------------------------------------------------------------//
	-( void )	startListener;
	-( void )	stopListener;
	-( void )	updateMoaiEnvironment;

@end

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
	[ super dealloc ];
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
	
	#ifdef MOAI_OS_IPHONE
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
	#endif
}

@end

//================================================================//
// MOAIReachabilityMgr
//================================================================//

//----------------------------------------------------------------//
MOAIReachabilityMgr::MOAIReachabilityMgr () {
	
	this->mReachabilityListener = [ MOAIReachabilityListener alloc ];
	[ this->mReachabilityListener startListener ];
}

//----------------------------------------------------------------//
MOAIReachabilityMgr::~MOAIReachabilityMgr () {
}

//----------------------------------------------------------------//
void MOAIReachabilityMgr::OnGlobalsFinalize () {

	[ this->mReachabilityListener release ];
	this->mReachabilityListener = nil;
}

//----------------------------------------------------------------//
void MOAIReachabilityMgr::UpdateReachability () {

	[ this->mReachabilityListener updateMoaiEnvironment ];
}
