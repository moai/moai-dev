// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIVertexFormat.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIVertexFormat::_clear ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "U" )

	self->Clear ();
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormat::_declareColor ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "UN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	self->DeclareColorArray ( type );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormat::_declareCoord ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "UNN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	u32 size = state.GetValue < u32 >( 3, 0 );
	self->DeclareVertexArray ( type, size );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormat::_declareNormal ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "UN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	self->DeclareNormalArray ( type );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormat::_declareUV ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "UNN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	u32 size = state.GetValue < u32 >( 3, 0 );
	self->DeclareTexCoordArray ( type, size );

	return 0;
}

//================================================================//
// MOAIVertexFormat
//================================================================//

//----------------------------------------------------------------//
MOAIVertexFormat::MOAIVertexFormat () {
	
	RTTI_SINGLE ( USLuaObject )
	
	this->Clear ();
}

//----------------------------------------------------------------//
MOAIVertexFormat::~MOAIVertexFormat () {
}

//----------------------------------------------------------------//
void MOAIVertexFormat::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
	
	state.SetField ( -1, "GL_BYTE", ( u32 )GL_BYTE );
	state.SetField ( -1, "GL_FIXED", ( u32 )GL_FIXED );
	state.SetField ( -1, "GL_FLOAT", ( u32 )GL_FLOAT );
	state.SetField ( -1, "GL_SHORT", ( u32 )GL_SHORT );
	state.SetField ( -1, "GL_UNSIGNED_BYTE", ( u32 )GL_UNSIGNED_BYTE );
	state.SetField ( -1, "GL_UNSIGNED_SHORT", ( u32 )GL_UNSIGNED_SHORT );
}

//----------------------------------------------------------------//
void MOAIVertexFormat::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "clear",					_clear },
		{ "declareColor",			_declareColor },
		{ "declareCoord",			_declareCoord },
		{ "declareNormal",			_declareNormal },
		{ "declareUV",				_declareUV },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIVertexFormat::ToString () {

	STLString repr;
	return repr;
}
