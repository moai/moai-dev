// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moai-ios/headers.h>
#import <moai-ios-mobile-app-tracker/MOAIMobileAppTrackerIOS.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_setDebugMode ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool debugMode					= state.GetValue < bool >( 1, false );
	bool allowDuplicateRequests		= state.GetValue < bool >( 2, false );
	
	[[ MobileAppTracker sharedManager] setDebugMode:debugMode ];
	[[ MobileAppTracker sharedManager] setAllowDuplicateRequests:allowDuplicateRequests ];
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_setIFA ( lua_State* L ) {
	MOAILuaState state ( L );
	cc8* ifa = state.GetValue < cc8* >( 1, 0 );
	if ( ifa ) {
		NSUUID* uuid = [[[ NSUUID alloc ] initWithUUIDString:[ NSString stringWithUTF8String:ifa ]] autorelease ];
		[[ MobileAppTracker sharedManager] setAppleAdvertisingIdentifier:uuid ];
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_setIFV ( lua_State* L ) {
	MOAILuaState state ( L );
	cc8* ifv = state.GetValue < cc8* >( 1, 0 );
	if ( ifv ) {
		NSUUID* uuid = [[[ NSUUID alloc ] initWithUUIDString:[ NSString stringWithUTF8String:ifv ]] autorelease ];
		[[ MobileAppTracker sharedManager] setAppleVendorIdentifier:uuid ];
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_setLocation ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	double latitude		= state.GetValue < double >( 1, 0.0 );
	double longitude	= state.GetValue < double >( 2, 0.0 );
	
	if ( state.GetTop () > 2 ) {
	
		double altitude = state.GetValue < double >( 3, 0.0 );
		[[ MobileAppTracker sharedManager] setLatitude:latitude longitude:longitude altitude:altitude ];
	}
	else {
		[[ MobileAppTracker sharedManager] setLatitude:latitude longitude:longitude ];
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_setMACAddress ( lua_State* L ) {
	MOAILuaState state ( L );
	cc8* macAddress = state.GetValue < cc8* >( 1, 0 );
	if ( macAddress ) {
		[[ MobileAppTracker sharedManager] setMACAddress:[[ NSString stringWithUTF8String:macAddress ] uppercaseString ]];
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_setOpenUDID ( lua_State* L ) {
	MOAILuaState state ( L );
	cc8* openUDID = state.GetValue < cc8* >( 1, 0 );
	if ( openUDID ) {
		[[ MobileAppTracker sharedManager] setOpenUDID:[ NSString stringWithUTF8String:openUDID ]];
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_setUserId ( lua_State* L ) {
	MOAILuaState state ( L );
	cc8* userId = state.GetValue < cc8* >( 1, 0 );
	if ( userId ) {
		[[ MobileAppTracker sharedManager] setUserId:[ NSString stringWithUTF8String:userId ]];
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	startTracker
	@text	Starts Mobile App Tracker.
 
	@in		string advertiserId
	@in		string conversionKey
	@out	nil
*/
int MOAIMobileAppTrackerIOS::_startTracker ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* advertiserId	= state.GetValue < cc8* >( 1, "" );
	cc8* conversionKey	= state.GetValue < cc8* >( 2, "" );
	cc8* packageName	= state.GetValue < cc8* >( 3, 0 );
	
	[[ MobileAppTracker sharedManager ] setDelegate:[[ MOAIMobileAppTrackerDelegate alloc ] init ]];
	
	if ( packageName ) {
		[[ MobileAppTracker sharedManager ] setPackageName:[ NSString stringWithUTF8String:packageName ]];
	}
	
	[[ MobileAppTracker sharedManager]
		startTrackerWithMATAdvertiserId:[ NSString stringWithUTF8String:advertiserId ]
		MATConversionKey:[ NSString stringWithUTF8String:conversionKey ]
	];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_trackAction ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8*	eventIdOrName	= state.GetValue < cc8* >( 1, "" );
	bool	isId			= state.GetValue < cc8* >( 2, false );
	
	if ( state.GetTop () > 2 ) {
	
		float revenueAmount		= state.GetValue < float >( 3, 0.0f );
		cc8* currencyCode		= state.GetValue < cc8* >( 4, "USD" );
	
		[[ MobileAppTracker sharedManager]
			trackActionForEventIdOrName:[ NSString stringWithUTF8String:eventIdOrName ]
			eventIsId:isId
			revenueAmount:revenueAmount
			currencyCode:[ NSString stringWithUTF8String:currencyCode ]
		];
	}
	else {
	
		[[ MobileAppTracker sharedManager]
			trackActionForEventIdOrName:[ NSString stringWithUTF8String:eventIdOrName ]
			eventIsId:isId
		];
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_trackInstall ( lua_State* L ) {
	UNUSED ( L );
	
	[[ MobileAppTracker sharedManager] trackInstall ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_trackUpdate ( lua_State* L ) {
	UNUSED ( L );
	
	[[ MobileAppTracker sharedManager] trackUpdate ];
	return 0;
}

//================================================================//
// MOAISafariIOS
//================================================================//

//----------------------------------------------------------------//
MOAIMobileAppTrackerIOS::MOAIMobileAppTrackerIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->mDelegate = [[ MOAIMobileAppTrackerDelegate alloc ] init ];
}

//----------------------------------------------------------------//
MOAIMobileAppTrackerIOS::~MOAIMobileAppTrackerIOS () {

	[ this->mDelegate release ];
}

//----------------------------------------------------------------//
void MOAIMobileAppTrackerIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "setDebugMode",			_setDebugMode },
		{ "setIFA",					_setIFA },
		{ "setIFV",					_setIFV },
		{ "setLocation",			_setLocation },
		{ "setMACAddress",			_setMACAddress },
		{ "setOpenUDID",			_setOpenUDID },
		{ "setUserId",				_setUserId },
		{ "startTracker",			_startTracker },
		{ "trackAction",			_trackAction },
		{ "trackInstall",			_trackInstall },
		{ "trackUpdate",			_trackUpdate },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// MOAIMobileAppTrackerDelegate
//================================================================//
@implementation MOAIMobileAppTrackerDelegate

	//----------------------------------------------------------------//
	-( void ) mobileAppTracker:( MobileAppTracker* )tracker didSucceedWithData:( id )data {
		UNUSED ( tracker );
		NSString *response = [[ NSString alloc ] initWithData:data encoding:NSUTF8StringEncoding ];
		NSLog ( @"MAT.success: %@", response );
	}

	//----------------------------------------------------------------//
	// MAT tracking request failure callback
	-( void ) mobileAppTracker:( MobileAppTracker* )tracker didFailWithError:( NSError* )error {
		UNUSED ( tracker );
		NSLog ( @"MAT.failure: %@", error );
	}

@end
