// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAICmdMedium.h>
#include <moai-core/MOAIAbstractCmdStream.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractCmdStream::_getInterface ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractCmdStream, "U" )

	return self->AffirmMedium ().PushWithHandler ( state, 2 );
}


//================================================================//
// MOAIAbstractCmdStream
//================================================================//

//----------------------------------------------------------------//
MOAICmdMedium& MOAIAbstractCmdStream::AffirmMedium () {

	if ( !this->mMedium ) {
		this->mMedium = new MOAICmdMedium ( *this );
	}
	return *this->mMedium;
}

//----------------------------------------------------------------//
MOAIAbstractCmdStream::MOAIAbstractCmdStream () {

	RTTI_BEGIN ( MOAIAbstractCmdStream )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractCmdStream >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractCmdStream::~MOAIAbstractCmdStream () {
}

//----------------------------------------------------------------//
void MOAIAbstractCmdStream::RetainObject ( MOAILuaObject* object ) {
	this->MOAIAbstractCmdStream_RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAIAbstractCmdStream::SetMedium ( MOAICmdMedium* medium ) {
	this->mMedium = medium;
}

//----------------------------------------------------------------//
void MOAIAbstractCmdStream::SubmitCommand ( MOAIAbstractCmdHandler& handler, u32 cmd, const void* param, ZLSize paramSize ) {
	this->MOAIAbstractCmdStream_SubmitCommand ( handler, cmd, param, paramSize );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractCmdStream::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractCmdStream::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "_",							_getInterface },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
