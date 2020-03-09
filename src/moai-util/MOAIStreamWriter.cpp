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
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIStreamWriter )
		RTTI_EXTEND ( MOAIStreamAdapter )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStreamWriter::~MOAIStreamWriter () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIStreamWriter::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIStreamWriter::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "openBase64",			MOAIStreamAdapter::_openBase64Writer },
		{ "openDeflate",		MOAIStreamAdapter::_openDeflateWriter },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

