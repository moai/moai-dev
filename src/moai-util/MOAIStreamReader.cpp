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
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIStreamReader )
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
void MOAIStreamReader::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIStreamReader::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "openBase64",			MOAIStreamAdapter::_openBase64Reader },
		{ "openDeflate",		MOAIStreamAdapter::_openDeflateReader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

