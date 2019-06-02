// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIStreamReader.h>

//================================================================//
// lua
//================================================================//

//================================================================//
// MOAIStreamReader
//================================================================//

//----------------------------------------------------------------//
MOAIStreamReader::MOAIStreamReader () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStreamAdapter )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStreamReader::~MOAIStreamReader () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIStreamReader::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIStreamAdapter, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIStreamReader::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIStreamAdapter, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "openBase64",			MOAIStreamAdapter::_openBase64Reader },
		{ "openDeflate",		MOAIStreamAdapter::_openDeflateReader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

