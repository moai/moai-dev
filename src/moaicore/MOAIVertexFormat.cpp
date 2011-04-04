// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIVertexFormat.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clear
	@text	Removes all format information and returns the format to
			an empty state.
	
	@in		MOAIVertexFormat self
	@out	nil
*/
int	MOAIVertexFormat::_clear ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "U" )

	self->Clear ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	declareColor
	@text	Declare a vertex color.
	
	@in		MOAIVertexFormat self
	@in		number type			Data type of component elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareColor ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "UN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	self->DeclareColorArray ( type );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	declareCoord
	@text	Declare a vertex coord.
	
	@in		MOAIVertexFormat self
	@in		number type			Data type of coord elements. See OpenGL ES documentation.
	@in		number size			Number of coord elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareCoord ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "UNN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	u32 size = state.GetValue < u32 >( 3, 0 );
	self->DeclareVertexArray ( type, size );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	declareNormal
	@text	Declare a vertex normal.
	
	@in		MOAIVertexFormat self
	@in		number type			Data type of normal elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormat::_declareNormal ( lua_State* L ) {
	LUA_SETUP ( MOAIVertexFormat, "UN" )

	u32 type = state.GetValue < u32 >( 2, 0 );
	self->DeclareNormalArray ( type );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	declareUV
	@text	Declare a vertex texture coord.
	
	@in		MOAIVertexFormat self
	@in		number type			Data type of texture coord elements. See OpenGL ES documentation.
	@in		number size			Number of texture coord elements. See OpenGL ES documentation.
	@out	nil
*/
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

	luaL_Reg regTable [] = {
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
