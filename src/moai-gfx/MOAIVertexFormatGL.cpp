// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgrGL.h>
#include <moai-gfx/MOAIVertexFormatGL.h>
#include <moai-gfx/MOAIVertexFormatMgrGL.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormatGL::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormatGL, "U" )
	
	self->Clear ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareAttribute
	@text	Declare a custom attribute (for use with programmable pipeline).
	
	@in		MOAIVertexFormatGL self
	@in		number index		Default value is 1.
	@in		number type			Data type of component elements. See OpenGL ES documentation.
	@in		number size			Number of elements. See OpenGL ES documentation.
	@opt	boolean normalized	See OpenGL ES documentation.
	@opt	number use			One of MOAIVertexFormatGL.ATTRIBUTE_COLOR, MOAIVertexFormatGL.ATTRIBUTE_NORMAL, MOAIVertexFormatGL.ATTRIBUTE_TEX_COORD, MOAIVertexFormatGL.ATTRIBUTE_COORD, MOAIVertexFormatGL.VERTEX_USER,
	@out	nil
*/
int	MOAIVertexFormatGL::_declareAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormatGL, "UNNN" )

	ZLIndex index		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	u32 type			= state.GetValue < u32 >( 3, 0 );
	u32 size			= state.GetValue < u32 >( 4, 0 );
	bool normalized		= state.GetValue < bool >( 5, false );
	
	u32 use				= state.GetValue < u32 >( 6, ATTRIBUTE_USER );

	self->DeclareAttribute ( index, type, size, use, normalized );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormatGL::_declareBoneCount ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormatGL, "U" )
	
	ZLIndex index		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	u32 type			= state.GetValue < u32 >( 3, ZGL_TYPE_UNSIGNED_BYTE );
	
	self->DeclareAttribute ( index, type, 1, ATTRIBUTE_BONE_COUNT, false );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormatGL::_declareBoneIndices ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormatGL, "U" )
	
	ZLIndex index		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	u32 type			= state.GetValue < u32 >( 3, ZGL_TYPE_UNSIGNED_BYTE );
	u32 size			= state.GetValue < u32 >( 4, 4 );
	
	self->DeclareAttribute ( index, type, size, ATTRIBUTE_BONE_INDICES, false );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormatGL::_declareBoneWeights ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormatGL, "U" )
	
	ZLIndex index		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	u32 type			= state.GetValue < u32 >( 3, ZGL_TYPE_FLOAT );
	u32 size			= state.GetValue < u32 >( 4, 4 );
	
	self->DeclareAttribute ( index, type, size, ATTRIBUTE_BONE_WEIGHTS, false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareColor
	@text	Declare a vertex color.
	
	@in		MOAIVertexFormatGL self
	@in		number index
	@in		number type			Data type of component elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormatGL::_declareColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormatGL, "U" )

	ZLIndex index		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	u32 type			= state.GetValue < u32 >( 3, ZGL_TYPE_UNSIGNED_BYTE );
	
	self->DeclareAttribute ( index, type, COLOR_SIZE, ATTRIBUTE_COLOR, true );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareCoord
	@text	Declare a vertex coordinate.
	
	@in		MOAIVertexFormatGL self
	@in		number index
	@in		number type			Data type of coordinate elements. See OpenGL ES documentation.
	@in		number size			Number of coordinate elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormatGL::_declareCoord ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormatGL, "U" )

	ZLIndex index		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	u32 type			= state.GetValue < u32 >( 3, ZGL_TYPE_FLOAT );
	u32 size			= state.GetValue < u32 >( 4, 3 );
	
	self->DeclareAttribute ( index, type, size, ATTRIBUTE_COORD, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareNormal
	@text	Declare a vertex normal.
	
	@in		MOAIVertexFormatGL self
	@in		number index
	@in		number type			Data type of normal elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormatGL::_declareNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormatGL, "U" )
	
	ZLIndex index		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	u32 type			= state.GetValue < u32 >( 3, ZGL_TYPE_FLOAT );
	
	self->DeclareAttribute ( index, type, NORMAL_SIZE, ATTRIBUTE_NORMAL, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	declareUV
	@text	Declare a vertex texture coordinate.
	
	@in		MOAIVertexFormatGL self
	@in		number index
	@in		number type			Data type of texture coordinate elements. See OpenGL ES documentation.
	@in		number size			Number of texture coordinate elements. See OpenGL ES documentation.
	@out	nil
*/
int MOAIVertexFormatGL::_declareUV ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormatGL, "U" )

	ZLIndex index		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	u32 type			= state.GetValue < u32 >( 3, ZGL_TYPE_FLOAT );
	u32 size			= state.GetValue < u32 >( 4, 2 );
	
	self->DeclareAttribute ( index, type, size, ATTRIBUTE_TEX_COORD, false );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getVertexSize
	@text	Return the size (in bytes) of the vertex described by the vertex format.
	
	@in		MOAIVertexFormatGL self
	@out	number vertexSizeInBytes
*/
int MOAIVertexFormatGL::_getVertexSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVertexFormatGL, "U" )

	state.Push ( MOAILuaSize ( self->mVertexSize ));
	return 1;
}

//================================================================//
// MOAIVertexFormatGL
//================================================================//

//----------------------------------------------------------------//
MOAIVertexFormatGL::MOAIVertexFormatGL () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		RTTI_EXTEND ( ZLVertexFormatGL )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexFormatGL::~MOAIVertexFormatGL () {
}

//----------------------------------------------------------------//
void MOAIVertexFormatGL::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "GL_BYTE",					( u32 )ZGL_TYPE_BYTE );
	state.SetField ( -1, "GL_FLOAT",				( u32 )ZGL_TYPE_FLOAT );
	state.SetField ( -1, "GL_SHORT",				( u32 )ZGL_TYPE_SHORT );
	state.SetField ( -1, "GL_UNSIGNED_BYTE",		( u32 )ZGL_TYPE_UNSIGNED_BYTE );
	state.SetField ( -1, "GL_UNSIGNED_SHORT",		( u32 )ZGL_TYPE_UNSIGNED_SHORT );
	
	state.SetField ( -1, "ATTRIBUTE_BONE_INDICES",	( u32 )ATTRIBUTE_BONE_INDICES );
	state.SetField ( -1, "ATTRIBUTE_BONE_WEIGHTS",	( u32 )ATTRIBUTE_BONE_WEIGHTS );
	state.SetField ( -1, "ATTRIBUTE_COLOR",			( u32 )ATTRIBUTE_COLOR );
	state.SetField ( -1, "ATTRIBUTE_COORD",			( u32 )ATTRIBUTE_COORD );
	state.SetField ( -1, "ATTRIBUTE_NORMAL",		( u32 )ATTRIBUTE_NORMAL );
	state.SetField ( -1, "ATTRIBUTE_TEX_COORD",		( u32 )ATTRIBUTE_TEX_COORD );
	state.SetField ( -1, "ATTRIBUTE_USER",			( u32 )ATTRIBUTE_USER );
}

//----------------------------------------------------------------//
void MOAIVertexFormatGL::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "clear",					_clear },
		{ "declareAttribute",		_declareAttribute },
		{ "declareBoneCount",		_declareBoneCount },
		{ "declareBoneIndices",		_declareBoneIndices },
		{ "declareBoneWeights",		_declareBoneWeights },
		{ "declareColor",			_declareColor },
		{ "declareCoord",			_declareCoord },
		{ "declareNormal",			_declareNormal },
		{ "declareUV",				_declareUV },
		{ "getVertexSize",			_getVertexSize },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexFormatGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );

	this->mTotalAttributes		= state.GetFieldValue < cc8*, u32 >( -1, "mTotalAttributes", 0 );
	this->mVertexSize			= state.GetFieldValue < cc8*, u32 >( -1, "mVertexSize", 0 );

	this->mAttributes.Init ( this->mTotalAttributes );

	state.PushField ( -1, "mAttributes" );
	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mTotalAttributes; ++i ) {
		
		ZLVertexAttributeGL& attribute = this->mAttributes [ i ];
		
		state.PushField ( -1, ZLIndexOp::ToInt ( i ) + 1 );
		
		attribute.mIndex			= state.GetFieldValue < cc8*, u32 >( -1, "mIndex", 0 );
		attribute.mSize				= state.GetFieldValue < cc8*, u32 >( -1, "mSize", 0 );
		attribute.mType				= state.GetFieldValue < cc8*, u32 >( -1, "mType", 0 );
		attribute.mNormalized		= state.GetFieldValue < cc8*, bool >( -1, "mNormalized", false );
		attribute.mOffset			= state.GetFieldValue < cc8*, u32 >( -1, "mOffset", 0 );
		
		state.Pop ();
	}
	state.Pop ();
	
	state.PushField ( -1, "mAttributeUseTable" );
	for ( ZLIndex i = ZLIndexOp::ZERO; i < TOTAL_ATTRIBUTE_TYPES; ++i ) {
		
		u32 luaUseIdx = MOAIVertexFormatGL::GetLuaIndexForUseID ( i );
		state.PushField ( -1, luaUseIdx );
		
		u32 nAttributesByUse = ( u32 )state.GetTableSize ( -1 );
		this->mTotalAttributesByUse [ i ] = nAttributesByUse;
		this->mAttributeIDsByUse [ i ].Init ( nAttributesByUse );
		
		for ( ZLIndex j = ZLIndexOp::ZERO; j < nAttributesByUse; ++j ) {
			
			this->mAttributeIDsByUse [ i ][ j ] = state.GetFieldValue < MOAILuaIndex, MOAILuaIndex >( -1, j, ZLIndexOp::ZERO );
		}
		state.Pop ();
	}
	state.Pop ();
}

//----------------------------------------------------------------//
void MOAIVertexFormatGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );

	state.SetField ( -1, "mTotalAttributes", MOAILuaSize ( this->mTotalAttributes ));
	state.SetField ( -1, "mVertexSize", MOAILuaSize ( this->mVertexSize ));

	lua_newtable ( state );
	for ( ZLIndex i = ZLIndexOp::ZERO; i < this->mTotalAttributes; ++i ) {
	
		ZLVertexAttributeGL& attribute = this->mAttributes [ i ];
	
		state.Push ( MOAILuaIndex ( i ));
		lua_newtable ( state );
		
		state.SetField ( -1, "mIndex",			attribute.mIndex );
		state.SetField ( -1, "mSize",			attribute.mSize );
		state.SetField ( -1, "mType",			attribute.mType );
		state.SetField ( -1, "mNormalized",		attribute.mNormalized );
		state.SetField ( -1, "mOffset",			attribute.mOffset );
		
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mAttributes" );

	lua_newtable ( state );
	for ( ZLIndex i = ZLIndexOp::ZERO; i < TOTAL_ATTRIBUTE_TYPES; ++i ) {
		
		state.Push ( MOAIVertexFormatGL::GetLuaIndexForUseID ( i ));
		lua_newtable ( state );
		
		ZLSize nAttributesByUse = this->mTotalAttributesByUse [ i ];
		for ( ZLIndex j = ZLIndexOp::ZERO; j < nAttributesByUse; ++j ) {
			state.SetField ( -1, MOAILuaIndex ( j + i ), MOAILuaIndex ( this->mAttributeIDsByUse [ i ][ j ]));
		}
		lua_settable ( state, -3 );
	}
	lua_setfield ( state, -2, "mAttributeUseTable" );
}
