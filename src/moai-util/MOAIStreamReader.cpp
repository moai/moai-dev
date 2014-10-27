// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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

//----------------------------------------------------------------//
void MOAIStreamReader::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStreamAdapter::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIStreamReader::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStreamAdapter::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "openBase64",			MOAIStreamAdapter::_openBase64Reader },
		{ "openDeflate",		MOAIStreamAdapter::_openDeflateReader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

