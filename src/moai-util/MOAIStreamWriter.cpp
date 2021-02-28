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
MOAIStreamWriter::MOAIStreamWriter ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIStream ( context ),
	MOAIStreamAdapter ( context ) {
	
	RTTI_BEGIN ( MOAIStreamWriter )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIStreamWriter >)
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
void MOAIStreamWriter::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIStreamWriter::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "openBase64",			MOAIStreamAdapter::_openBase64Writer },
		{ "openDeflate",		MOAIStreamAdapter::_openDeflateWriter },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

