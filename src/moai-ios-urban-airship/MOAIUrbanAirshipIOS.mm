// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <UIKit/UIKit.h>

#import <moai-ios-urban-airship/MOAIUrbanAirshipIOS.h>

#import "UAirship.h"
#import "UAAnalytics.h"
#import "UAPush.h"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: 3rdparty doxygen
int MOAIUrbanAirshipIOS::_resetBadge ( lua_State* L ) {
	UNUSED ( L );
	[[ UAPush shared ] resetBadge ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: 3rdparty doxygen
int MOAIUrbanAirshipIOS::_setAppKeyAndSecret ( lua_State* L ) {
	MOAILuaState state ( L );
	UAConfig* config				= MOAIUrbanAirshipIOS::Get ().mConfig;
	config.developmentAppKey		= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )];
	config.developmentAppSecret		= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 2, "" )];
	config.productionAppKey			= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 3, "" )];
	config.productionAppSecret		= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 4, "" )];
	return 0;
}

//----------------------------------------------------------------//
// TODO: 3rdparty doxygen
int MOAIUrbanAirshipIOS::_setClearKeychain ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIUrbanAirshipIOS::Get ().mConfig.clearKeychain = state.GetValue < bool >( 1, false );
	return 0;
}

//----------------------------------------------------------------//
// TODO: 3rdparty doxygen
int MOAIUrbanAirshipIOS::_setProductionFlag ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIUrbanAirshipIOS::Get ().mConfig.inProduction = state.GetValue < bool >( 1, true );
	return 0;
}

//----------------------------------------------------------------//
// TODO: 3rdparty doxygen
int MOAIUrbanAirshipIOS::_setTags ( lua_State* L ) {
	MOAILuaState state ( L );
	
	
	int top = state.GetTop ();
	NSMutableArray* tags = [ NSMutableArray arrayWithCapacity:top ];
	
	for ( int i = 0; i < top; ++i ) {
		NSString* tag = [ NSString stringWithUTF8String:state.GetValue < cc8* >( i + 1, "" )];
		[ tags insertObject:tag atIndex:i ];
	}

	[[ UAPush shared ] setTags:tags ];
	[[ UAPush shared ] updateRegistration ];
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: 3rdparty doxygen
int MOAIUrbanAirshipIOS::_takeOff ( lua_State* L ) {
	UNUSED ( L );
	[ UAirship takeOff:MOAIUrbanAirshipIOS::Get ().mConfig ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: 3rdparty doxygen
int MOAIUrbanAirshipIOS::_validate ( lua_State* L ) {
	UNUSED ( L );
	[ MOAIUrbanAirshipIOS::Get ().mConfig validate ];
	return 0;
}

//================================================================//
// MOAIUrbanAirshipIOS
//================================================================//

//----------------------------------------------------------------//
MOAIUrbanAirshipIOS::MOAIUrbanAirshipIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->mConfig = [ UAConfig config ];
}

//----------------------------------------------------------------//
MOAIUrbanAirshipIOS::~MOAIUrbanAirshipIOS () {

	[ this->mConfig release ];
}

//----------------------------------------------------------------//
void MOAIUrbanAirshipIOS::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "resetBadge",				_resetBadge },
		{ "setAppKeyAndSecret",		_setAppKeyAndSecret },
		{ "setClearKeychain",		_setClearKeychain },
		{ "setProductionFlag",		_setProductionFlag },
		{ "setTags",				_setTags },
		{ "takeOff",				_takeOff },
		{ "validate",				_validate },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

