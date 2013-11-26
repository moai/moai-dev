// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moai-ios/headers.h>
#import <moai-ios-flurry/MOAIFlurryIOS.h>
#import <Flurry.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_beginEvent ( lua_State* L ) {
	MOAIFlurryIOS::LogEvent ( L, 1, true );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_endEvent ( lua_State* L ) {
	MOAILuaState state ( L );

	NSDictionary* parameters	= nil;
	NSString* event				= [ NSString stringWithUTF8String:state.GetValue < cc8* >( 1, "" )];
	
	if ( state.IsType ( 2, LUA_TTABLE )) {
		parameters = [[[ NSMutableDictionary alloc ] initWithLua:state stackIndex:2 ] autorelease ];
	}
	
	[ Flurry endTimedEvent:event withParameters:parameters ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_getAgentVersion ( lua_State* L ) {
	MOAILuaState state ( L );
	NSString* agentVersion = [ Flurry getFlurryAgentVersion ];
	[ agentVersion toLua:state ];
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_logEvent ( lua_State* L ) {
	MOAIFlurryIOS::LogEvent ( L, 1, false );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setAge ( lua_State* L ) {
	MOAILuaState state ( L );
	int age = state.GetValue < int >( 1, 0 );
	[ Flurry setAge:age ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setAppVersion ( lua_State* L ) {
	MOAILuaState state ( L );
	cc8* version = state.GetValue < cc8* >( 1, "" );
	[ Flurry setAppVersion :[ NSString stringWithUTF8String:version ]];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setDebugLogEnabled ( lua_State* L ) {
	MOAILuaState state ( L );
	bool enabled = state.GetValue < bool >( 1, true );
	[ Flurry setDebugLogEnabled:enabled ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setEventLoggingEnabled ( lua_State* L ) {
	MOAILuaState state ( L );
	bool enabled = state.GetValue < bool >( 1, true );
	[ Flurry setEventLoggingEnabled:enabled ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setGender ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 gender = state.GetValue < u32 >( 1, GENDER_UNKNOWN );
	
	if (( gender == GENDER_MALE ) || ( gender == GENDER_FEMALE )) {
	
		NSString* genderStr = gender == GENDER_MALE ? @"M" : @"F";
		[ Flurry setGender:genderStr ];
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setLocation ( lua_State* L ) {
	MOAILuaState state ( L );
	
	double latitude		= state.GetValue < double >( 1, 0.0 );
	double longitude	= state.GetValue < double >( 2, 0.0 );
	float hAccuracy		= state.GetValue < float >( 3, 0.0f );
	float vAccuracy		= state.GetValue < float >( 4, 0.0f );
	
	[ Flurry
		setLatitude:latitude
		longitude:longitude
		horizontalAccuracy:hAccuracy
		verticalAccuracy:vAccuracy
	];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setSecureTransportEnabled ( lua_State* L ) {
	MOAILuaState state ( L );
	bool enabled = state.GetValue < bool >( 1, true );
	[ Flurry setSecureTransportEnabled :enabled ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setSessionContinueSeconds ( lua_State* L ) {
	MOAILuaState state ( L );
	int seconds = state.GetValue < int >( 1, 0 );
	[ Flurry setSessionContinueSeconds:seconds ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setSessionReportsOnCloseEnabled ( lua_State* L ) {
	MOAILuaState state ( L );
	bool enabled = state.GetValue < bool >( 1, true );
	[ Flurry setSessionReportsOnCloseEnabled:enabled ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setSessionReportsOnPauseEnabled ( lua_State* L ) {
	MOAILuaState state ( L );
	bool enabled = state.GetValue < bool >( 1, true );
	[ Flurry setSessionReportsOnPauseEnabled:enabled ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setShowErrorInLogEnabled ( lua_State* L ) {
	MOAILuaState state ( L );
	bool enabled = state.GetValue < bool >( 1, true );
	[ Flurry setShowErrorInLogEnabled:enabled ];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_setUserID ( lua_State* L ) {
	MOAILuaState state ( L );
	cc8* userID	= state.GetValue < cc8* >( 1, "" );
	[ Flurry setUserID :[ NSString stringWithUTF8String:userID ]];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIFlurryIOS::_startSession ( lua_State* L ) {
	MOAILuaState state ( L );
	cc8* apiKey	= state.GetValue < cc8* >( 1, "" );
	[ Flurry startSession:[ NSString stringWithUTF8String:apiKey ]];
	return 0;
}

//================================================================//
// MOAIFlurryIOS
//================================================================//

//----------------------------------------------------------------//
void MOAIFlurryIOS::LogEvent ( lua_State* L, int idx, bool timed ) {
	MOAILuaState state ( L );

	NSDictionary* parameters	= nil;
	NSString* event				= [ NSString stringWithUTF8String:state.GetValue < cc8* >( idx, "" )];
	
	if ( state.IsType ( idx + 1, LUA_TTABLE )) {
		NSMutableDictionary* mut = [[[ NSMutableDictionary alloc ] init ] autorelease ];
		[ mut initWithLua:state stackIndex:( idx + 1 )];
		parameters = mut;
	}
	
	if ( parameters ) {
		[ Flurry logEvent:event withParameters:parameters timed:timed ];
	}
	else {
		[ Flurry logEvent:event timed:timed ];
	}
}

//----------------------------------------------------------------//
MOAIFlurryIOS::MOAIFlurryIOS () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIFlurryIOS::~MOAIFlurryIOS () {
}

//----------------------------------------------------------------//
void MOAIFlurryIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "GENDER_MALE",			( u32 )GENDER_MALE );
	state.SetField ( -1, "GENDER_FEMALE",		( u32 )GENDER_FEMALE );
	
	luaL_Reg regTable [] = {
		{ "beginEvent",							_beginEvent },
		{ "endEvent",							_endEvent },
		{ "getAgentVersion",					_getAgentVersion },
		{ "logEvent",							_logEvent },
		{ "setAge",								_setAge },
		{ "setAppVersion",						_setAppVersion },
		{ "setDebugLogEnabled",					_setDebugLogEnabled },
		{ "setEventLoggingEnabled",				_setEventLoggingEnabled },
		{ "setGender",							_setGender },
		{ "setLocation",						_setLocation },
		{ "setSecureTransportEnabled",			_setSecureTransportEnabled },
		{ "setSessionContinueSeconds",			_setSessionContinueSeconds },
		{ "setSessionReportsOnCloseEnabled",	_setSessionReportsOnCloseEnabled },
		{ "setSessionReportsOnPauseEnabled",	_setSessionReportsOnPauseEnabled },
		{ "setShowErrorInLogEnabled",			_setShowErrorInLogEnabled },
		{ "setUserID",							_setUserID },
		{ "startSession",						_startSession },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
