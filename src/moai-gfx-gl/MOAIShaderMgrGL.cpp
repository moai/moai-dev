// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>
#include <moai-gfx-gl/MOAIShaderMgrGL.h>
#include <moai-gfx-gl/MOAIShaderProgramGL.h>
#include <moai-gfx-gl/MOAIVertexFormatMgrGL.h>

#include <moai-gfx-gl/shaders/MOAIDeck2DShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DSnappingShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DSnappingShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DTexOnlyShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DTexOnlyShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAIFontShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIFontShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAIFontSnappingShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIFontSnappingShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAILineShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAILineShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAILineShader3DGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAILineShader3DGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAIMeshShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIMeshShaderGL-vsh.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getProgram
	@text	Return one of the built-in shader programs.

	@in		number shaderID		One of MOAIShaderMgrGL.DECK2D_SHADER, MOAIShaderMgrGL.FONT_SHADER,
								MOAIShaderMgrGL.LINE_SHADER, MOAIShaderMgrGL.MESH_SHADER
	@out	nil
*/
int MOAIShaderMgrGL::_getProgram ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 shaderID = state.GetValue < u32 >( 1, UNKNOWN_SHADER );
	
	if ( shaderID < TOTAL_SHADERS ) {
	
		MOAIShaderProgramGL* program = MOAIShaderMgrGL::Get ().GetProgram ( shaderID );
		state.Push ( program );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderMgrGL::_getShader ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 shaderID = state.GetValue < u32 >( 1, UNKNOWN_SHADER );
	
	if ( shaderID < TOTAL_SHADERS ) {
	
		MOAIShaderGL* shader = MOAIShaderMgrGL::Get ().GetShader ( shaderID );
		state.Push ( shader );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIShaderMgrGL
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderMgrGL::AffirmAll () {

	for ( u32 i = 0; i < TOTAL_SHADERS; ++i ) {
		this->GetShader ( i );
	}
}

//----------------------------------------------------------------//
MOAIShaderProgramGL* MOAIShaderMgrGL::GetProgram ( u32 shaderID ) {

	MOAIShaderProgramGL* program = 0;

	if ( shaderID < TOTAL_SHADERS ) {
	
		program = this->mPrograms [ shaderID ];
		
		if ( !program ) {

			program = new MOAIShaderProgramGL ();
			this->LuaRetain ( program );
			
			switch ( shaderID ) {
				
				case DECK2D_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_COLOR, "color" );
					
					program->Load ( _deck2DShaderVSH, _deck2DShaderFSH );
					break;
				
				case DECK2D_SNAPPING_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_COLOR, "color" );
					
					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( ZLIndexCast ( 0 ), "xSnap", MOAIShaderUniformGL::UNIFORM_TYPE_FLOAT );
					program->DeclareUniform ( ZLIndexCast ( 1 ), "ySnap", MOAIShaderUniformGL::UNIFORM_TYPE_FLOAT );
					
					program->ReserveGlobals ( 2 );
					program->SetGlobal ( ZLIndexCast ( 0 ), MOAIGfxMgrGL::VIEW_HALF_WIDTH, ZLIndexCast ( 0 ), ZLIndexCast ( 0 ));
					program->SetGlobal ( ZLIndexCast ( 1 ), MOAIGfxMgrGL::VIEW_HALF_HEIGHT, ZLIndexCast ( 1 ), ZLIndexCast ( 0 ));
					
					program->Load ( _deck2DSnappingShaderVSH, _deck2DSnappingShaderFSH );
					break;
				
				case DECK2D_TEX_ONLY_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_COLOR, "color" );
					
					program->Load ( _deck2DTexOnlyShaderVSH, _deck2DTexOnlyShaderFSH );
					break;
				
				case FONT_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_COLOR, "color" );
					
					program->Load ( _fontShaderVSH, _fontShaderFSH );
					break;
				
				case FONT_SNAPPING_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_COLOR, "color" );
					
					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( ZLIndexCast ( 0 ), "xSnap", MOAIShaderUniformGL::UNIFORM_TYPE_FLOAT );
					program->DeclareUniform ( ZLIndexCast ( 1 ), "ySnap", MOAIShaderUniformGL::UNIFORM_TYPE_FLOAT );
					
					program->ReserveGlobals ( 2 );
					program->SetGlobal ( ZLIndexCast ( 0 ), MOAIGfxMgrGL::VIEW_HALF_WIDTH, ZLIndexCast ( 0 ), ZLIndexCast ( 0 ));
					program->SetGlobal ( ZLIndexCast ( 1 ), MOAIGfxMgrGL::VIEW_HALF_HEIGHT, ZLIndexCast ( 1 ), ZLIndexCast ( 0 ));
					
					program->Load ( _fontSnappingShaderVSH, _fontSnappingShaderFSH );
					break;
				
				case LINE_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWC_COLOR, "color" );
					program->Load ( _lineShaderVSH, _lineShaderFSH );
					break;
				
				case LINE_SHADER_3D:
					
					program->SetVertexAttribute ( 0, "position" );
					program->SetVertexAttribute ( 1, "color" );
					
					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( ZLIndexCast ( 0 ), "transform", MOAIShaderUniformGL::UNIFORM_TYPE_FLOAT, MOAIShaderUniformGL::UNIFORM_WIDTH_MATRIX_4X4 );
					program->DeclareUniform ( ZLIndexCast ( 1 ), "ucolor", MOAIShaderUniformGL::UNIFORM_TYPE_FLOAT, MOAIShaderUniformGL::UNIFORM_WIDTH_VEC_4 );
					
					program->ReserveGlobals ( 2 );
					program->SetGlobal ( ZLIndexCast ( 0 ), MOAIGfxMgrGL::MODEL_TO_CLIP_MTX, ZLIndexCast ( 0 ), ZLIndexCast ( 0 ));
					program->SetGlobal ( ZLIndexCast ( 1 ), MOAIGfxMgrGL::PEN_COLOR, ZLIndexCast ( 1 ), ZLIndexCast ( 0 ));
					
					program->Load ( _lineShader3DVSH, _lineShader3DFSH );
					
					break;

				case MESH_SHADER:

					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgrGL::XYZWUVC_COLOR, "color" );

					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( ZLIndexCast ( 0 ), "transform", MOAIShaderUniformGL::UNIFORM_TYPE_FLOAT, MOAIShaderUniformGL::UNIFORM_WIDTH_MATRIX_4X4 );
					program->DeclareUniform ( ZLIndexCast ( 1 ), "ucolor", MOAIShaderUniformGL::UNIFORM_TYPE_FLOAT, MOAIShaderUniformGL::UNIFORM_WIDTH_VEC_4 );
					
					program->ReserveGlobals ( 2 );
					program->SetGlobal ( ZLIndexCast ( 0 ), MOAIGfxMgrGL::MODEL_TO_CLIP_MTX, ZLIndexCast ( 0 ), ZLIndexCast ( 0 ));
					program->SetGlobal ( ZLIndexCast ( 1 ), MOAIGfxMgrGL::PEN_COLOR, ZLIndexCast ( 1 ), ZLIndexCast ( 0 ));
					
					program->Load ( _meshShaderVSH, _meshShaderFSH );
					
					break;
			}
			
			this->mPrograms [ shaderID ] = program;
		}
	}
	return program;
}

//----------------------------------------------------------------//
MOAIShaderGL* MOAIShaderMgrGL::GetShader ( u32 shaderID ) {

	MOAIShaderGL* shader = 0;

	if ( shaderID < TOTAL_SHADERS ) {
	
		shader = this->mShaders [ shaderID ];
		
		if ( !shader ) {

			MOAIShaderProgramGL* program = this->GetProgram ( shaderID );
			if ( program ) {

				shader = new MOAIShaderGL ();
				this->LuaRetain ( shader );
				shader->SetProgram ( program );
				this->mShaders [ shaderID ] = shader;
			}
		}
	}
	return shader;
}

//----------------------------------------------------------------//
MOAIShaderGL* MOAIShaderMgrGL::GetShader ( lua_State* L, int idx ) {

	MOAILuaState state ( L );
	MOAIShaderGL* shader = NULL;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		shader = this->GetShader ( state.GetValue < u32 >( idx, MOAIShaderMgrGL::UNKNOWN_SHADER ));
	}
	else {
		shader = state.GetLuaObject < MOAIShaderGL >( idx, true );
	}
	return shader;
}

//----------------------------------------------------------------//
MOAIShaderMgrGL::MOAIShaderMgrGL () {

	RTTI_SINGLE ( MOAILuaObject )

	for ( u32 i = 0; i < TOTAL_SHADERS; ++i ) {
		this->mPrograms [ i ] = 0;
	}
	
	for ( u32 i = 0; i < TOTAL_SHADERS; ++i ) {
		this->mShaders [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIShaderMgrGL::~MOAIShaderMgrGL () {

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
void MOAIShaderMgrGL::RegisterLuaClass ( MOAILuaState& state ) {

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
void MOAIShaderMgrGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}
