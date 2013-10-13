// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moai-ios/headers.h>
#import <moai-ios-mobile-app-tracker/MOAIMobileAppTrackerIOS.h>
#import <MobileAppTracker/MobileAppTracker.h>

//================================================================//
// lua
//================================================================//

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
int MOAIMobileAppTrackerIOS::_setOpenUDID ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* openUDID = state.GetValue < cc8* >( 1, "" );
	
	[[ MobileAppTracker sharedManager] setUserId:[ NSString stringWithUTF8String:openUDID ]];
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMobileAppTrackerIOS::_setUserId ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* userId = state.GetValue < cc8* >( 1, "" );
	
	[[ MobileAppTracker sharedManager] setUserId:[ NSString stringWithUTF8String:userId ]];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	startTracker
	@text	Starts Mobile App Tracker.
 
	@in		string advertiserId
	@in		string conversionKey
	@out	nil
*/
int MOAIMobileAppTrackerIOS::_startTracker ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* advertiserId	= state.GetValue < cc8* >( 1, "" );
	cc8* conversionKey	= state.GetValue < cc8* >( 2, "" );
	
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
}

//----------------------------------------------------------------//
MOAIMobileAppTrackerIOS::~MOAIMobileAppTrackerIOS () {
}

//----------------------------------------------------------------//
void MOAIMobileAppTrackerIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "setLocation",			_setLocation },
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
