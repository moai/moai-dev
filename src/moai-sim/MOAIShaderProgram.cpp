// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/MOAIMaterialMgr.h>
#include <moai-sim/MOAIShaderProgram.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	declareUniform
	@text	Declares a uniform mapping.

	@in		MOAIShaderProgram self
	@in		number idx
	@in		string name
	@opt	number type		One of MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_TYPE_INDEX, MOAIShaderProgram.UNIFORM_TYPE_INT,
							MOAIShaderProgram.UNIFORM_TYPE_MATRIX_3X3, MOAIShaderProgram.UNIFORM_TYPE_MATRIX_4X4
	@opt	number width	Used for vector uniforms. Default value is 1. Should be no greather than 4.
	@opt	number count	Declare an array of uniforms. Default value is 1.
	@out	nil
*/
int MOAIShaderProgram::_declareUniform ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNSN" )

	ZLIndex idx			= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	STLString name		= state.GetValue < cc8* >( 3, "" );
	u32 type			= state.GetValue < u32 >( 4, ZLShaderUniform::UNIFORM_TYPE_FLOAT );
	u32 width			= state.GetValue < u32 >( 5, 1 );
	u32 count			= state.GetValue < u32 >( 6, 1 );

	self->DeclareUniform ( idx, name, type, width, count );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	load
	@text	Load a shader program.

	@in		MOAIShaderProgram self
	@in		string vertexShaderSource
	@in		string fragmentShaderSource
	@out	nil
*/
int MOAIShaderProgram::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "USS" )

	cc8* vtxSource	= state.GetValue < cc8* >( 2, 0 );
	cc8* frgSource	= state.GetValue < cc8* >( 3, 0 );

	self->Load ( vtxSource, frgSource );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgram::_reserveGlobals ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

//	return self->ReserveGlobals ( L, 2 );
	
	u32 nGlobals = state.GetValue < u32 >( 2, 0 );
	self->ReserveGlobals ( nGlobals );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgram::_reserveTextures ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

	self->ReserveTextures ( state.GetValue < u32 >( 2, 0 ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	reserveUniforms
	@text	Reserve shader uniforms.

	@in		MOAIShaderProgram self
	@opt	number nUniforms	Default value is 0.
	@out	nil
*/
int MOAIShaderProgram::_reserveUniforms ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )

	u32 nUniforms = state.GetValue < u32 >( 2, 0 );
	self->ReserveUniforms ( nUniforms );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgram::_setGlobal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNNN" )

	ZLIndex globalIdx	= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	u32 globalID		= state.GetValue < u32 >( 3, ZLGfxStateCache::NULL_GLOBAL );
	ZLIndex uniformID	= state.GetValue < MOAILuaIndex >( 4, ZLIndexOp::ZERO );
	ZLIndex index		= state.GetValue < MOAILuaIndex >( 5, ZLIndexOp::ZERO );
	
	self->SetGlobal ( globalIdx, globalID, uniformID, index );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderProgram::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "U" )
	
	ZLIndex idx		= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	ZLIndex unit	= state.GetValue < MOAILuaIndex >( 3, ZLIndexOp::ZERO );
	
	if ( state.IsType ( 3, LUA_TUSERDATA )) {
	
		self->SetTexture ( idx, state.GetLuaObject < MOAITextureBase >( 3, true ), unit );
	}
	else {
	
		self->SetTexture (
			idx,
			state.GetValue < u32 >( 3, MOAI_UNKNOWN_MATERIAL_GLOBAL + 1 ) - 1,
			unit,
			state.GetLuaObject < MOAITextureBase >( 5, false )
		);
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setVertexAttribute
	@text	Names a shader vertex attribute.

	@in		MOAIShaderProgram self
	@in		number index	Default value is 1.
	@in		string name		Name of attribute.
	@out	nil
*/
int MOAIShaderProgram::_setVertexAttribute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIShaderProgram, "UNS" )

	ZLIndex idx				= state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	STLString attribute		= state.GetValue < cc8* >( 3, "" );

	self->SetVertexAttribute ( idx, attribute );

	return 0;
}

//================================================================//
// MOAIShaderProgram
//================================================================//

//----------------------------------------------------------------//
MOAIShaderProgram::MOAIShaderProgram () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAbstractGfxResource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIShaderProgram::~MOAIShaderProgram () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIShaderProgram::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIGfxResource < ZLShaderProgram >::RegisterLuaClass ( state );

	state.SetField ( -1, "UNIFORM_TYPE_FLOAT",						( u32 )ZLShaderUniform::UNIFORM_TYPE_FLOAT );
	state.SetField ( -1, "UNIFORM_TYPE_INT",						( u32 )ZLShaderUniform::UNIFORM_TYPE_INT );
	
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_2",						( u32 )ZLShaderUniform::UNIFORM_WIDTH_VEC_2 );
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_3",						( u32 )ZLShaderUniform::UNIFORM_WIDTH_VEC_3 );
	state.SetField ( -1, "UNIFORM_WIDTH_VEC_4",						( u32 )ZLShaderUniform::UNIFORM_WIDTH_VEC_4 );
	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_3X3",				( u32 )ZLShaderUniform::UNIFORM_WIDTH_MATRIX_3X3 );
	state.SetField ( -1, "UNIFORM_WIDTH_MATRIX_4X4",				( u32 )ZLShaderUniform::UNIFORM_WIDTH_MATRIX_4X4 );
	
	state.SetField ( -1, "GLOBAL_CLIP_TO_DISPLAY_MTX",				( u32 )ZLGfxStateCache::CLIP_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_MODEL_MTX",				( u32 )ZLGfxStateCache::CLIP_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_VIEW_MTX",					( u32 )ZLGfxStateCache::CLIP_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_WINDOW_MTX",				( u32 )ZLGfxStateCache::CLIP_TO_WINDOW_MTX );
	state.SetField ( -1, "GLOBAL_CLIP_TO_WORLD_MTX",				( u32 )ZLGfxStateCache::CLIP_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_DISPLAY_TO_CLIP_MTX",				( u32 )ZLGfxStateCache::DISPLAY_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_MODEL_MTX",				( u32 )ZLGfxStateCache::DISPLAY_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_VIEW_MTX",				( u32 )ZLGfxStateCache::DISPLAY_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_DISPLAY_TO_WORLD_MTX",				( u32 )ZLGfxStateCache::DISPLAY_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_MODEL_TO_CLIP_MTX",				( u32 )ZLGfxStateCache::MODEL_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_DISPLAY_MTX",				( u32 )ZLGfxStateCache::MODEL_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_UV_MTX",					( u32 )ZLGfxStateCache::MODEL_TO_UV_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_VIEW_MTX",				( u32 )ZLGfxStateCache::MODEL_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_MODEL_TO_WORLD_MTX",				( u32 )ZLGfxStateCache::MODEL_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_DISPLAY_MTX",		( u32 )ZLGfxStateCache::NORMAL_CLIP_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_MODEL_MTX",			( u32 )ZLGfxStateCache::NORMAL_CLIP_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_VIEW_MTX",			( u32 )ZLGfxStateCache::NORMAL_CLIP_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_WINDOW_MTX",		( u32 )ZLGfxStateCache::NORMAL_CLIP_TO_WINDOW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_CLIP_TO_WORLD_MTX",			( u32 )ZLGfxStateCache::NORMAL_CLIP_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_CLIP_MTX",		( u32 )ZLGfxStateCache::NORMAL_DISPLAY_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_MODEL_MTX",		( u32 )ZLGfxStateCache::NORMAL_DISPLAY_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_VIEW_MTX",		( u32 )ZLGfxStateCache::NORMAL_DISPLAY_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_DISPLAY_TO_WORLD_MTX",		( u32 )ZLGfxStateCache::NORMAL_DISPLAY_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_CLIP_MTX",			( u32 )ZLGfxStateCache::NORMAL_MODEL_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_DISPLAY_MTX",		( u32 )ZLGfxStateCache::NORMAL_MODEL_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_UV_MTX",			( u32 )ZLGfxStateCache::NORMAL_MODEL_TO_UV_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_VIEW_MTX",			( u32 )ZLGfxStateCache::NORMAL_MODEL_TO_VIEW_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_MODEL_TO_WORLD_MTX",		( u32 )ZLGfxStateCache::NORMAL_MODEL_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_DISPLAY_MTX",		( u32 )ZLGfxStateCache::NORMAL_WORLD_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_VIEW_MTX",			( u32 )ZLGfxStateCache::NORMAL_WORLD_TO_VIEW_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_UV_TO_MODEL_MTX",			( u32 )ZLGfxStateCache::NORMAL_UV_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_CLIP_MTX",			( u32 )ZLGfxStateCache::NORMAL_VIEW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_DISPLAY_MTX",		( u32 )ZLGfxStateCache::NORMAL_VIEW_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_MODEL_MTX",			( u32 )ZLGfxStateCache::NORMAL_VIEW_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_VIEW_TO_WORLD_MTX",			( u32 )ZLGfxStateCache::NORMAL_VIEW_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_NORMAL_WINDOW_TO_CLIP_MTX",		( u32 )ZLGfxStateCache::NORMAL_WINDOW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_CLIP_MTX",			( u32 )ZLGfxStateCache::NORMAL_WORLD_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_NORMAL_WORLD_TO_MODEL_MTX",		( u32 )ZLGfxStateCache::NORMAL_WORLD_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_PEN_COLOR",						( u32 )ZLGfxStateCache::PEN_COLOR );

	state.SetField ( -1, "GLOBAL_UV_TO_MODEL_MTX",					( u32 )ZLGfxStateCache::UV_TO_MODEL_MTX );

	state.SetField ( -1, "GLOBAL_VIEW_TO_CLIP_MTX",					( u32 )ZLGfxStateCache::VIEW_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_DISPLAY_MTX",				( u32 )ZLGfxStateCache::VIEW_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_MODEL_MTX",				( u32 )ZLGfxStateCache::VIEW_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_VIEW_TO_WORLD_MTX",				( u32 )ZLGfxStateCache::VIEW_TO_WORLD_MTX );

	state.SetField ( -1, "GLOBAL_WINDOW_TO_CLIP_MTX",				( u32 )ZLGfxStateCache::WINDOW_TO_CLIP_MTX );

	state.SetField ( -1, "GLOBAL_WORLD_TO_CLIP_MTX",				( u32 )ZLGfxStateCache::WORLD_TO_CLIP_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_DISPLAY_MTX",				( u32 )ZLGfxStateCache::WORLD_TO_DISPLAY_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_MODEL_MTX",				( u32 )ZLGfxStateCache::WORLD_TO_MODEL_MTX );
	state.SetField ( -1, "GLOBAL_WORLD_TO_VIEW_MTX",				( u32 )ZLGfxStateCache::WORLD_TO_VIEW_MTX );

	state.SetField ( -1, "GLOBAL_VIEW_HEIGHT",						( u32 )ZLGfxStateCache::VIEW_HEIGHT );
	state.SetField ( -1, "GLOBAL_VIEW_WIDTH",						( u32 )ZLGfxStateCache::VIEW_WIDTH );
	
	state.SetField ( -1, "GLOBAL_VIEW_HALF_HEIGHT",					( u32 )ZLGfxStateCache::VIEW_HALF_HEIGHT );
	state.SetField ( -1, "GLOBAL_VIEW_HALF_WIDTH",					( u32 )ZLGfxStateCache::VIEW_HALF_WIDTH );
}

//----------------------------------------------------------------//
void MOAIShaderProgram::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIGfxResource < ZLShaderProgram >::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "declareUniform",				_declareUniform },
		{ "load",						_load },
		{ "reserveGlobals",				_reserveGlobals },
		{ "reserveTextures",			_reserveTextures },
		{ "reserveUniforms",			_reserveUniforms },
		{ "setGlobal",					_setGlobal },
		{ "setTexture",					_setTexture },
		{ "setVertexAttribute",			_setVertexAttribute },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

////----------------------------------------------------------------//
//int ZLShaderProgram::ReserveGlobals ( lua_State* L, int idx ) {
//
//	MOAILuaState state ( L );
//
//	u32 nGlobals = state.GetValue < u32 >( idx, 0 );
//	this->ReserveGlobals ( nGlobals );
//
//	return 0;
//}
