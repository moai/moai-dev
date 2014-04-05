// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_MOBILEAPPTRACKER

#include "pch.h"
#import <moai-iphone/MOAIMobileAppTrackerIOS.h>
#import <moai-iphone/NSString+MOAILib.h>

#import <MobileAppTracker/MobileAppTracker.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	trackInstall
	@text	Tracks an install through the WebAppTracking service
 
	@in		string advertiserId
	@in		string appId
	@in		string userID
	@in		string deviceID
	@out	nil
*/
int MOAIMobileAppTrackerIOS::_trackInstall ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* advertiserId = state.GetValue < cc8* >( 1, "" );
	cc8* appId = state.GetValue < cc8* >( 2, "" );
	cc8* userId = state.GetValue < cc8* >( 3, "" );
	cc8* deviceId = state.GetValue < cc8* >( 4, "" );
	
	printf ( "tracking: %s %s %s %s\n", advertiserId, appId, userId, deviceId );
	
	// MobileAppTracking
	NSError *error = [[ NSError alloc ] autorelease ];
	NSString *idNum = [[[ NSString alloc ] initWithUTF8String:advertiserId ] autorelease ];
	NSString *adKey = [[[ NSString alloc ] initWithUTF8String:appId ] autorelease ];
	
	[[ MobileAppTracker sharedManager] startTrackerWithAdvertiserId:idNum advertiserKey:adKey withError:&error ];
	
	NSString *user = [[[ NSString alloc ] initWithUTF8String:userId ] autorelease ];
	NSString *device = [[[ NSString alloc ] initWithUTF8String:deviceId ] autorelease ];
	
	[[ MobileAppTracker sharedManager ] setUserId:user ];
	[[ MobileAppTracker sharedManager ] setDeviceId:device ];
	
	[[ MobileAppTracker sharedManager ] trackInstallWithUpdateOnly:NO ];
	
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
		{ "trackInstall",			_trackInstall },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

#endif // DISABLE_MOBILEAPPTRACKER
