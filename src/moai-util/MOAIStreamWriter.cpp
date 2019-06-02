// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIStreamWriter.h>

//================================================================//
// lua
//================================================================//

//================================================================//
// MOAIStreamWriter
//================================================================//

//----------------------------------------------------------------//
MOAIStreamWriter::MOAIStreamWriter () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStreamAdapter )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStreamWriter::~MOAIStreamWriter () {
}

//----------------------------------------------------------------//
void MOAIStreamWriter::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIStreamAdapter, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIStreamWriter::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIStreamAdapter, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "openBase64",			MOAIStreamAdapter::_openBase64Writer },
		{ "openDeflate",		MOAIStreamAdapter::_openDeflateWriter },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

