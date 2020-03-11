// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAICanary.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setMessage
	@text	Sets the message to print on destruction.
	
	@in		MOAICanary self
	@out	nil
*/
int MOAICanary::_setMessage ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICanary, "U" ) // this macro initializes the 'self' variable and type checks arguments
	
	self->mMessage = state.GetValue < cc8* >( 2, "" );
	
	return 0;
}

//================================================================//
// MOAICanary
//================================================================//

//----------------------------------------------------------------//
MOAICanary::MOAICanary () {
	
	RTTI_BEGIN ( MOAICanary )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAICanary >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAICanary::~MOAICanary () {

	if ( this->mMessage.size ()) {
		printf ( "DELETED CANARY: %s\n", this->mMessage.c_str ());
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAICanary::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAICanary::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setMessage",		_setMessage },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

