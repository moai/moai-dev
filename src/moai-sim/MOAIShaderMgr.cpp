// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

#include <moai-sim/shaders/MOAIDeck2DShader-fsh.h>
#include <moai-sim/shaders/MOAIDeck2DShader-vsh.h>
#include <moai-sim/shaders/MOAIDeck2DSnappingShader-fsh.h>
#include <moai-sim/shaders/MOAIDeck2DSnappingShader-vsh.h>
#include <moai-sim/shaders/MOAIDeck2DTexOnlyShader-fsh.h>
#include <moai-sim/shaders/MOAIDeck2DTexOnlyShader-vsh.h>
#include <moai-sim/shaders/MOAIFontShader-fsh.h>
#include <moai-sim/shaders/MOAIFontShader-vsh.h>
#include <moai-sim/shaders/MOAIFontSnappingShader-fsh.h>
#include <moai-sim/shaders/MOAIFontSnappingShader-vsh.h>
#include <moai-sim/shaders/MOAILineShader-fsh.h>
#include <moai-sim/shaders/MOAILineShader-vsh.h>
#include <moai-sim/shaders/MOAILineShader3D-fsh.h>
#include <moai-sim/shaders/MOAILineShader3D-vsh.h>
#include <moai-sim/shaders/MOAIMeshShader-fsh.h>
#include <moai-sim/shaders/MOAIMeshShader-vsh.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getProgram
	@text	Return one of the built-in shader programs.

	@in		number shaderID		One of MOAIShaderMgr.DECK2D_SHADER, MOAIShaderMgr.FONT_SHADER,
								MOAIShaderMgr.LINE_SHADER, MOAIShaderMgr.MESH_SHADER
	@out	nil
*/
int MOAIShaderMgr::_getProgram ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 shaderID = state.GetValue < u32 >( 1, UNKNOWN_SHADER );
	
	if ( shaderID < TOTAL_SHADERS ) {
	
		MOAIShaderProgram* program = MOAIShaderMgr::Get ().GetProgram ( shaderID );
		state.Push ( program );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderMgr::_getShader ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 shaderID = state.GetValue < u32 >( 1, UNKNOWN_SHADER );
	
	if ( shaderID < TOTAL_SHADERS ) {
	
		MOAIShader* shader = MOAIShaderMgr::Get ().GetShader ( shaderID );
		state.Push ( shader );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIShaderMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderMgr::AffirmAll () {

	for ( u32 i = 0; i < TOTAL_SHADERS; ++i ) {
		this->GetShader ( i );
	}
}

//----------------------------------------------------------------//
MOAIShaderProgram* MOAIShaderMgr::GetProgram ( u32 shaderID ) {

	MOAIShaderProgram* program = 0;

	if ( shaderID < TOTAL_SHADERS ) {
	
		program = this->mPrograms [ shaderID ];
		
		if ( !program ) {

			program = new MOAIShaderProgram ();
			this->LuaRetain ( program );
			
			switch ( shaderID ) {
				
				case DECK2D_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
					
					program->Load ( _deck2DShaderVSH, _deck2DShaderFSH );
					break;
				
				case DECK2D_SNAPPING_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
					
					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( 0, "xSnap", MOAIShaderUniform::UNIFORM_TYPE_FLOAT );
					program->DeclareUniform ( 1, "ySnap", MOAIShaderUniform::UNIFORM_TYPE_FLOAT );
					
					program->ReserveGlobals ( 2 );
					program->SetGlobal ( 0, MOAIGfxGlobalsCache::VIEW_HALF_WIDTH, 0, 0 );
					program->SetGlobal ( 1, MOAIGfxGlobalsCache::VIEW_HALF_HEIGHT, 1, 0 );
					
					program->Load ( _deck2DSnappingShaderVSH, _deck2DSnappingShaderFSH );
					break;
				
				case DECK2D_TEX_ONLY_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
					
					program->Load ( _deck2DTexOnlyShaderVSH, _deck2DTexOnlyShaderFSH );
					break;
				
				case FONT_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
					
					program->Load ( _fontShaderVSH, _fontShaderFSH );
					break;
				
				case FONT_SNAPPING_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
					
					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( 0, "xSnap", MOAIShaderUniform::UNIFORM_TYPE_FLOAT );
					program->DeclareUniform ( 1, "ySnap", MOAIShaderUniform::UNIFORM_TYPE_FLOAT );
					
					program->ReserveGlobals ( 2 );
					program->SetGlobal ( 0, MOAIGfxGlobalsCache::VIEW_HALF_WIDTH, 0, 0 );
					program->SetGlobal ( 1, MOAIGfxGlobalsCache::VIEW_HALF_HEIGHT, 1, 0 );
					
					program->Load ( _fontSnappingShaderVSH, _fontSnappingShaderFSH );
					break;
					
				case FONT_EFFECTS_SHADER:
					
					//program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					//program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					//program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
					
					//program->ReserveUniforms ( 2 );
					//program->DeclareUniform ( 0, "xSnap", MOAIShaderUniform::UNIFORM_FLOAT );
					//program->DeclareUniform ( 1, "ySnap", MOAIShaderUniform::UNIFORM_FLOAT );
					
					//program->ReserveGlobals ( 2 );
					//program->SetGlobal ( 0, 0, MOAIShaderProgram::GLOBAL_VIEW_HALF_WIDTH );
					//program->SetGlobal ( 1, 1, MOAIShaderProgram::GLOBAL_VIEW_HALF_HEIGHT );
					
					//program->SetSource ( _fontEffectsShaderVSH, _fontEffectsShaderFSH );
					
					break;
				
				case LINE_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWC_COLOR, "color" );
					program->Load ( _lineShaderVSH, _lineShaderFSH );
					break;
				
				case LINE_SHADER_3D:
					
					program->SetVertexAttribute ( 0, "position" );
					program->SetVertexAttribute ( 1, "color" );
					
					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( 0, "transform", MOAIShaderUniform::UNIFORM_TYPE_FLOAT, MOAIShaderUniform::UNIFORM_WIDTH_MATRIX_4X4 );
					program->DeclareUniform ( 1, "ucolor", MOAIShaderUniform::UNIFORM_TYPE_FLOAT, MOAIShaderUniform::UNIFORM_WIDTH_VEC_4 );
					
					program->ReserveGlobals ( 2 );
					program->SetGlobal ( 0, MOAIGfxGlobalsCache::WORLD_VIEW_PROJ_MTX, 0, 0 );
					program->SetGlobal ( 1, MOAIGfxGlobalsCache::PEN_COLOR, 1, 0 );
					
					program->Load ( _lineShader3DVSH, _lineShader3DFSH );
					
					break;

				case MESH_SHADER:

					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );

					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( 0, "transform", MOAIShaderUniform::UNIFORM_TYPE_FLOAT, MOAIShaderUniform::UNIFORM_WIDTH_MATRIX_4X4 );
					program->DeclareUniform ( 1, "ucolor", MOAIShaderUniform::UNIFORM_TYPE_FLOAT, MOAIShaderUniform::UNIFORM_WIDTH_VEC_4 );
					
					program->ReserveGlobals ( 2 );
					program->SetGlobal ( 0, MOAIGfxGlobalsCache::WORLD_VIEW_PROJ_MTX, 0, 0 );
					program->SetGlobal ( 1, MOAIGfxGlobalsCache::PEN_COLOR, 1, 0 );
					
					program->Load ( _meshShaderVSH, _meshShaderFSH );
					
					break;
			}
			
			this->mPrograms [ shaderID ] = program;
		}
	}
	return program;
}

//----------------------------------------------------------------//
MOAIShader* MOAIShaderMgr::GetShader ( u32 shaderID ) {

	MOAIShader* shader = 0;

	if ( shaderID < TOTAL_SHADERS ) {
	
		shader = this->mShaders [ shaderID ];
		
		if ( !shader ) {

			MOAIShaderProgram* program = this->GetProgram ( shaderID );
			if ( program ) {

				shader = new MOAIShader ();
				this->LuaRetain ( shader );
				shader->SetProgram ( program );
				this->mShaders [ shaderID ] = shader;
			}
		}
	}
	return shader;
}

//----------------------------------------------------------------//
MOAIShaderMgr::MOAIShaderMgr () {

	RTTI_SINGLE ( MOAILuaObject )

	for ( u32 i = 0; i < TOTAL_SHADERS; ++i ) {
		this->mPrograms [ i ] = 0;
	}
	
	for ( u32 i = 0; i < TOTAL_SHADERS; ++i ) {
		this->mShaders [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIShaderMgr::~MOAIShaderMgr () {

	for ( u32 i = 0; i < TOTAL_SHADERS; ++i ) {
		if ( this->mPrograms [ i ]) {
			this->LuaRelease ( this->mPrograms [ i ]);
		}
	}
	
	for ( u32 i = 0; i < TOTAL_SHADERS; ++i ) {
		if ( this->mShaders [ i ]) {
			this->LuaRelease ( this->mShaders [ i ]);
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderMgr::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "DECK2D_SHADER",			( u32 )DECK2D_SHADER );
	state.SetField ( -1, "DECK2D_SNAPPING_SHADER",	( u32 )DECK2D_SNAPPING_SHADER );
	state.SetField ( -1, "DECK2D_TEX_ONLY_SHADER",	( u32 )DECK2D_TEX_ONLY_SHADER );
	state.SetField ( -1, "FONT_SHADER",				( u32 )FONT_SHADER );
	state.SetField ( -1, "FONT_SNAPPING_SHADER",	( u32 )FONT_SNAPPING_SHADER );
	state.SetField ( -1, "FONT_EFFECTS_SHADER",	    ( u32 )FONT_EFFECTS_SHADER );
	state.SetField ( -1, "LINE_SHADER",				( u32 )LINE_SHADER );
	state.SetField ( -1, "LINE_SHADER_3D",			( u32 )LINE_SHADER_3D );
	state.SetField ( -1, "MESH_SHADER",				( u32 )MESH_SHADER );
	
	luaL_Reg regTable [] = {
		{ "getProgram",				_getProgram },
		{ "getShader",				_getShader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShaderMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}
