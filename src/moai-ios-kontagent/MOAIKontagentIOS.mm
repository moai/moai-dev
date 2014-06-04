// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moai-ios/headers.h>
#import <moai-ios-kontagent/MOAIKontagentIOS.h>
#import <Kontagent/Kontagent.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIKontagentIOS::_startSession ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* apiKey		= state.GetValue < cc8* >( 1, "" );
	bool test		= state.GetValue < bool >( 2, false );
	
	
	[ Kontagent
		startSession:[ NSString stringWithUTF8String:apiKey ]
		mode:( test ? kKontagentSDKMode_TEST : kKontagentSDKMode_PRODUCTION )
	];
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIKontagentIOS::_stopSession ( lua_State* L ) {
	MOAILuaState state ( L );
	[ Kontagent stop ];
	return 0;
}

//================================================================//
// MOAIKontagentIOS
//================================================================//

//----------------------------------------------------------------//
MOAIKontagentIOS::MOAIKontagentIOS () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIKontagentIOS::~MOAIKontagentIOS () {
}

//----------------------------------------------------------------//
void MOAIKontagentIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "startSession",						_startSession },
		{ "stopSession",						_stopSession },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
