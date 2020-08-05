// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractDSLStream.h>
#include <moai-core/MOAIExampleSharedDSL.h>
#include <moai-core/MOAILogMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIExampleDSLInterface::_dsl_bar ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDSLStream, "U" )
	
	self->GetAPI < MOAIExampleDSLInterface >().Bar ();
	lua_pushvalue ( state, 1 );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIExampleDSLInterface::_dsl_baz ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDSLStream, "U" )
	
	self->GetAPI < MOAIExampleDSLInterface >().Baz ();
	lua_pushvalue ( state, 1 );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIExampleDSLInterface::_dsl_foo ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractDSLStream, "U" )
	
	self->GetAPI < MOAIExampleDSLInterface >().Foo ();
	lua_pushvalue ( state, 1 );
	return 1;
}

//================================================================//
// MOAIExampleDSLInterface
//================================================================//

//----------------------------------------------------------------//
void MOAIExampleDSLInterface::Bar () {

	this->SubmitCommand ( CMD_BAR );
}

//----------------------------------------------------------------//
void MOAIExampleDSLInterface::Baz () {

	this->SubmitCommand ( CMD_BAZ );
}

//----------------------------------------------------------------//
void MOAIExampleDSLInterface::Foo () {

	this->SubmitCommand ( CMD_FOO );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIExampleDSLInterface::MOAIAbstractDSLInterface_InitializeBinding ( MOAIAbstractDSLStream& stream ) const {

	stream.InitializeBinding < MOAIExampleDSLInterface >();
}

//----------------------------------------------------------------//
void MOAIExampleDSLInterface::MOAIAbstractDSLInterface_RegisterLuaAPI ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "bar",						_dsl_bar },
		{ "baz",						_dsl_baz },
		{ "foo",						_dsl_foo },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
