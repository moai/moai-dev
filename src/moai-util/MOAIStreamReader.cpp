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
MOAIStreamReader::MOAIStreamReader ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIStream ( context ),
	MOAIStreamAdapter ( context ) {
	
	RTTI_BEGIN ( MOAIStreamReader )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIStreamReader >)
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
void MOAIStreamReader::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIStreamReader::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "openBase64",			MOAIStreamAdapter::_openBase64Reader },
		{ "openDeflate",		MOAIStreamAdapter::_openDeflateReader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

