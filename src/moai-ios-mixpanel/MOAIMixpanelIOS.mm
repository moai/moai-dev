// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moai-ios/headers.h>
#import <moai-ios-mixpanel/MOAIMixpanelIOS.h>
#import <Mixpanel.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIMixpanelIOS::_startSession ( lua_State* L ) {
	MOAILuaState state ( L );
	return 0;
}

//================================================================//
// MOAIMixpanelIOS
//================================================================//

//----------------------------------------------------------------//
MOAIMixpanelIOS::MOAIMixpanelIOS () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIMixpanelIOS::~MOAIMixpanelIOS () {
}

//----------------------------------------------------------------//
void MOAIMixpanelIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "startSession",						_startSession },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
