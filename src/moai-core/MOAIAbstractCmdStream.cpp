// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAICmdMedium.h>
#include <moai-core/MOAIAbstractCmdStream.h>

//================================================================//
// MOAIAbstractCmdStream
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractCmdStream::MOAIAbstractCmdStream () {
}

//----------------------------------------------------------------//
MOAIAbstractCmdStream::~MOAIAbstractCmdStream () {
}

//----------------------------------------------------------------//
void MOAIAbstractCmdStream::RetainObject ( MOAILuaObject* object ) {
	this->MOAIAbstractCmdStream_RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAIAbstractCmdStream::SubmitCommand ( MOAIAbstractCmdHandler& handler, u32 cmd, const void* param, ZLSize paramSize ) {
	this->MOAIAbstractCmdStream_SubmitCommand ( handler, cmd, param, paramSize );
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractCmdStreamWithMedium::_getInterface ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractCmdStreamWithMedium, "U" )

	return self->AffirmMedium ().PushCmdInterfaceWithHandler ( state, 2 );
}


//================================================================//
// MOAIAbstractCmdStreamWithMedium
//================================================================//

//----------------------------------------------------------------//
MOAICmdMedium& MOAIAbstractCmdStreamWithMedium::AffirmMedium () {

	if ( !this->mMedium ) {
		this->mMedium = new MOAICmdMedium ( *this );
	}
	return *this->mMedium;
}

//----------------------------------------------------------------//
MOAIAbstractCmdStreamWithMedium::MOAIAbstractCmdStreamWithMedium () {

	RTTI_BEGIN ( MOAIAbstractCmdStreamWithMedium )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractCmdStreamWithMedium >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractCmdStreamWithMedium::~MOAIAbstractCmdStreamWithMedium () {
}

//----------------------------------------------------------------//
void MOAIAbstractCmdStreamWithMedium::SetMedium ( MOAICmdMedium* medium ) {
	this->mMedium = medium;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractCmdStreamWithMedium::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractCmdStreamWithMedium::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "_",							_getInterface },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
