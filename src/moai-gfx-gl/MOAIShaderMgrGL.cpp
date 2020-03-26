// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIShaderGL.h>
#include <moai-gfx-gl/MOAIShaderMgrGL.h>
#include <moai-gfx-gl/MOAIShaderProgramGL.h>

#include <moai-gfx-gl/shaders/MOAIDeck2DShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DShaderGL-vsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DSnappingShaderGL-fsh.h>
#include <moai-gfx-gl/shaders/MOAIDeck2DSnappingShaderGL-vsh.h>
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
// lua
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

	u32 shaderID = state.GetValue < u32 >( 1, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER );
	
	if ( shaderID < MOAIShaderPresetEnum::TOTAL_SHADERS ) {
	
		MOAIShaderProgramGL* program = MOAIShaderMgrGL::Get ().GetProgram (( MOAIShaderPresetEnum )shaderID );
		state.Push ( program );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIShaderMgrGL::_getShader ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 shaderID = state.GetValue < u32 >( 1, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER );
	
	if ( shaderID < MOAIShaderPresetEnum::TOTAL_SHADERS ) {
	
		MOAIShaderGL* shader = MOAIShaderMgrGL::Get ().GetShader (( MOAIShaderPresetEnum )shaderID );
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

	for ( u32 i = 0; i < ( u32 )MOAIShaderPresetEnum::TOTAL_SHADERS; ++i ) {
		this->GetShader (( MOAIShaderPresetEnum )i );
	}
}

//----------------------------------------------------------------//
MOAIShaderProgramGL* MOAIShaderMgrGL::GetProgram ( MOAIShaderPresetEnum shaderID ) {

	MOAIShaderProgramGL* program = 0;

	if ( shaderID < MOAIShaderPresetEnum::TOTAL_SHADERS ) {
	
		program = this->mPrograms [ shaderID ];
		
		if ( !program ) {

			program = new MOAIShaderProgramGL ();
			this->LuaRetain ( program );
			
			switch ( shaderID ) {
				
				case MOAIShaderPresetEnum::DECK2D_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
					
					program->Load ( _deck2DShaderVSH, _deck2DShaderFSH );
					break;
				
				case MOAIShaderPresetEnum::DECK2D_SNAPPING_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
					
					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( 0, "xSnap", MOAIUniformDescriptor::UNIFORM_TYPE_FLOAT );
					program->DeclareUniform ( 1, "ySnap", MOAIUniformDescriptor::UNIFORM_TYPE_FLOAT );
					
					program->SetUniform ( MOAIGfxMgrGL::VIEW_HALF_WIDTH, 0, 0 );
					program->SetUniform ( MOAIGfxMgrGL::VIEW_HALF_HEIGHT, 1, 0 );
					program->AffirmGfxScript ().Optimize ();
					
					program->Load ( _deck2DSnappingShaderVSH, _deck2DSnappingShaderFSH );
					break;
				
				case MOAIShaderPresetEnum::FONT_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
					
					program->Load ( _fontShaderVSH, _fontShaderFSH );
					break;
				
				case MOAIShaderPresetEnum::FONT_SNAPPING_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
					
					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( 0, "xSnap", MOAIUniformDescriptor::UNIFORM_TYPE_FLOAT );
					program->DeclareUniform ( 1, "ySnap", MOAIUniformDescriptor::UNIFORM_TYPE_FLOAT );
					
					program->SetUniform ( MOAIGfxMgrGL::VIEW_HALF_WIDTH, 0, 0 );
					program->SetUniform ( MOAIGfxMgrGL::VIEW_HALF_HEIGHT, 1, 0 );
					program->AffirmGfxScript ().Optimize ();
					
					program->Load ( _fontSnappingShaderVSH, _fontSnappingShaderFSH );
					break;
				
				case MOAIShaderPresetEnum::LINE_SHADER:
					
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWC_COLOR, "color" );
					program->Load ( _lineShaderVSH, _lineShaderFSH );
					break;
				
				case MOAIShaderPresetEnum::LINE_SHADER_3D:
					
					program->SetVertexAttribute ( 0, "position" );
					program->SetVertexAttribute ( 1, "color" );
					
					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( 0, "transform", MOAIUniformDescriptor::UNIFORM_TYPE_FLOAT, MOAIUniformDescriptor::UNIFORM_WIDTH_MATRIX_4X4 );
					program->DeclareUniform ( 1, "ucolor", MOAIUniformDescriptor::UNIFORM_TYPE_FLOAT, MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_4 );
					
					program->SetUniform ( MOAIGfxMgrGL::MODEL_TO_CLIP_MTX, 0, 0 );
					program->SetUniform ( MOAIGfxMgrGL::PEN_COLOR, 1, 0 );
					program->AffirmGfxScript ().Optimize ();
					
					program->Load ( _lineShader3DVSH, _lineShader3DFSH );
					
					break;

				case MOAIShaderPresetEnum::MESH_SHADER:

					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
					program->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );

					program->ReserveUniforms ( 2 );
					program->DeclareUniform ( 0, "transform", MOAIUniformDescriptor::UNIFORM_TYPE_FLOAT, MOAIUniformDescriptor::UNIFORM_WIDTH_MATRIX_4X4 );
					program->DeclareUniform ( 1, "ucolor", MOAIUniformDescriptor::UNIFORM_TYPE_FLOAT, MOAIUniformDescriptor::UNIFORM_WIDTH_VEC_4 );
					
					program->SetUniform ( MOAIGfxMgrGL::MODEL_TO_CLIP_MTX, 0, 0 );
					program->SetUniform ( MOAIGfxMgrGL::PEN_COLOR, 1, 0 );
					
					program->Load ( _meshShaderVSH, _meshShaderFSH );
					
					break;
			}
			
			this->mPrograms [ shaderID ] = program;
		}
	}
	return program;
}

//----------------------------------------------------------------//
MOAIShaderGL* MOAIShaderMgrGL::GetShader ( MOAIShaderPresetEnum shaderID ) {

	MOAIShaderGL* shader = 0;

	if ( shaderID < MOAIShaderPresetEnum::TOTAL_SHADERS ) {
	
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
		shader = this->GetShader (( MOAIShaderPresetEnum )state.GetValue < u32 >( idx, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER ));
	}
	else {
		shader = state.GetLuaObject < MOAIShaderGL >( idx, true );
	}
	return shader;
}

//----------------------------------------------------------------//
MOAIShaderMgrGL::MOAIShaderMgrGL () {
	
	RTTI_BEGIN ( MOAIShaderMgrGL )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIShaderMgrGL >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END

	for ( u32 i = 0; i < MOAIShaderPresetEnum::TOTAL_SHADERS; ++i ) {
		this->mPrograms [ i ] = 0;
	}
	
	for ( u32 i = 0; i < MOAIShaderPresetEnum::TOTAL_SHADERS; ++i ) {
		this->mShaders [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIShaderMgrGL::~MOAIShaderMgrGL () {

	for ( u32 i = 0; i < MOAIShaderPresetEnum::TOTAL_SHADERS; ++i ) {
		if ( this->mPrograms [ i ]) {
			this->LuaRelease ( this->mPrograms [ i ]);
		}
	}
	
	for ( u32 i = 0; i < MOAIShaderPresetEnum::TOTAL_SHADERS; ++i ) {
		if ( this->mShaders [ i ]) {
			this->LuaRelease ( this->mShaders [ i ]);
		}
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderMgrGL::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	state.SetField ( -1, "DECK2D_SHADER",			( u32 )MOAIShaderPresetEnum::DECK2D_SHADER );
	state.SetField ( -1, "DECK2D_SNAPPING_SHADER",	( u32 )MOAIShaderPresetEnum::DECK2D_SNAPPING_SHADER );
	state.SetField ( -1, "FONT_SHADER",				( u32 )MOAIShaderPresetEnum::FONT_SHADER );
	state.SetField ( -1, "FONT_SNAPPING_SHADER",	( u32 )MOAIShaderPresetEnum::FONT_SNAPPING_SHADER );
	state.SetField ( -1, "FONT_EFFECTS_SHADER",	    ( u32 )MOAIShaderPresetEnum::FONT_EFFECTS_SHADER );
	state.SetField ( -1, "LINE_SHADER",				( u32 )MOAIShaderPresetEnum::LINE_SHADER );
	state.SetField ( -1, "LINE_SHADER_3D",			( u32 )MOAIShaderPresetEnum::LINE_SHADER_3D );
	state.SetField ( -1, "MESH_SHADER",				( u32 )MOAIShaderPresetEnum::MESH_SHADER );
	
	luaL_Reg regTable [] = {
		{ "getProgram",				_getProgram },
		{ "getShader",				_getShader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShaderMgrGL::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}
