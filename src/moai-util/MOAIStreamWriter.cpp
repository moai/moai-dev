// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
void MOAIStreamWriter::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStreamAdapter::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIStreamWriter::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStreamAdapter::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "openBase64",			MOAIStreamAdapter::_openBase64Writer },
		{ "openDeflate",		MOAIStreamAdapter::_openDeflateWriter },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

