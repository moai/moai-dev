// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxComposerVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderMgrVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>

#include <moai-gfx-vk/shaders/MOAIDeck2DShaderVK.frag.spv.h>
#include <moai-gfx-vk/shaders/MOAIDeck2DShaderVK.vert.spv.h>
//#include <moai-gfx-vk/shaders/MOAIDeck2DSnappingShaderVK-fsh.h>
//#include <moai-gfx-vk/shaders/MOAIDeck2DSnappingShaderVK-vsh.h>
//#include <moai-gfx-vk/shaders/MOAIDeck2DTexOnlyShaderVK-fsh.h>
//#include <moai-gfx-vk/shaders/MOAIDeck2DTexOnlyShaderVK-vsh.h>
//#include <moai-gfx-vk/shaders/MOAIFontShaderVK-fsh.h>
//#include <moai-gfx-vk/shaders/MOAIFontShaderVK-vsh.h>
//#include <moai-gfx-vk/shaders/MOAIFontSnappingShaderVK-fsh.h>
//#include <moai-gfx-vk/shaders/MOAIFontSnappingShaderVK-vsh.h>
//#include <moai-gfx-vk/shaders/MOAILineShaderVK-fsh.h>
//#include <moai-gfx-vk/shaders/MOAILineShaderVK-vsh.h>
//#include <moai-gfx-vk/shaders/MOAILineShader3DVK-fsh.h>
//#include <moai-gfx-vk/shaders/MOAILineShader3DVK-vsh.h>
//#include <moai-gfx-vk/shaders/MOAIMeshShaderVK-fsh.h>
//#include <moai-gfx-vk/shaders/MOAIMeshShaderVK-vsh.h>
#include <moai-gfx-vk/shaders/MOAIOneTriShaderVK.frag.spv.h>
#include <moai-gfx-vk/shaders/MOAIOneTriShaderVK.vert.spv.h>

//================================================================//
// lua
//================================================================//

////----------------------------------------------------------------//
///**	@lua	getProgram
//	@text	Return one of the built-in shader programs.
//
//	@in		number shaderID		One of MOAIShaderMgrVK.DECK2D_SHADER, MOAIShaderMgrVK.FONT_SHADER,
//								MOAIShaderMgrVK.LINE_SHADER, MOAIShaderMgrVK.MESH_SHADER
//	@out	nil
//*/
//int MOAIShaderMgrVK::_getProgram ( lua_State* L ) {
//	MOAILuaState state ( L );
//
//	u32 shaderID = state.GetValue < u32 >( 1, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER );
//
//	if ( shaderID < MOAIShaderPresetEnum::TOTAL_SHADERS ) {
//
//		MOAIShaderProgramVK* program = MOAIShaderMgrVK::Get ().GetProgram (( MOAIShaderPresetEnum )shaderID );
//		state.Push ( program );
//		return 1;
//	}
//	return 0;
//}
//
////----------------------------------------------------------------//
//// TODO: doxygen
//int MOAIShaderMgrVK::_getShader ( lua_State* L ) {
//	MOAILuaState state ( L );
//
//	u32 shaderID = state.GetValue < u32 >( 1, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER );
//
//	if ( shaderID < MOAIShaderPresetEnum::TOTAL_SHADERS ) {
//
//		MOAIShaderVK* shader = MOAIShaderMgrVK::Get ().GetShader (( MOAIShaderPresetEnum )shaderID );
//		state.Push ( shader );
//		return 1;
//	}
//	return 0;
//}

//================================================================//
// MOAIShaderMgrVK
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderMgrVK::AffirmAll () {

	if ( this->mOneTexDescriptorSetLayout ) return;

	MOAILogicalDeviceVK& logicalDevice = MOAIGfxMgrVK::Get ().GetLogicalDevice ();

	this->mOneTexDescriptorSetLayout = new MOAIDescriptorSetLayoutVK ();
	this->mOneTexDescriptorSetLayout->Initialize ( logicalDevice, 1 );
	this->mOneTexDescriptorSetLayout->SetBinding ( ZLIndexCast ( 0 ), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT );
	
	this->mOneTexPipelineLayout = new MOAIPipelineLayoutVK ();
	this->mOneTexPipelineLayout->Initialize ( logicalDevice, 1 );
	this->mOneTexPipelineLayout->SetDescriptorSetLayout ( ZLIndexCast ( 0 ), *this->mOneTexDescriptorSetLayout );

	this->mOneTexComposer = new MOAIGfxComposerVK ();
	this->mOneTexComposer->SetPipelineLayout ( *this->mOneTexPipelineLayout );
	this->mOneTexComposer->Reserve ( 1 );
	this->mOneTexComposer->PushTextureCommand ( ZLIndexOp::ZERO, ZLIndexOp::ZERO, ZLIndexOp::ZERO, ZLIndexOp::ZERO );

	for ( u32 i = 0; i < ( u32 )MOAIShaderPresetEnum::TOTAL_SHADERS; ++i ) {
		this->GetShader (( MOAIShaderPresetEnum )i );
	}
}

//----------------------------------------------------------------//
MOAIShaderProgramVK* MOAIShaderMgrVK::GetProgram ( MOAIShaderPresetEnum shaderID ) {

	MOAILogicalDeviceVK& logicalDevice = MOAIGfxMgrVK::Get ().GetLogicalDevice ();

	MOAIShaderProgramVK* program = 0;

	if ( shaderID < MOAIShaderPresetEnum::TOTAL_SHADERS ) {

		program = this->mPrograms [ shaderID ];

		if ( !program ) {

			program = new MOAIShaderProgramVK ();
			program->Initialize ( logicalDevice );
			this->LuaRetain ( program );

			switch ( shaderID ) {

				case MOAIShaderPresetEnum::DECK2D_SHADER: {
					
					program->LoadModule ( MOAIShaderProgramVK::VERTEX_MODULE, _deck2DShaderVSH, sizeof ( _deck2DShaderVSH ));
					program->LoadModule ( MOAIShaderProgramVK::FRAGMENT_MODULE, _deck2DShaderFSH, sizeof ( _deck2DShaderFSH ));
					program->SetGfxComposer ( *this->mOneTexComposer );

//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_POSITION, "position" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_TEXCOORD, "uv" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_COLOR, "color" );
//
//					program->Load ( _deck2DShaderVSH, _deck2DShaderFSH );
					break;
				}

//				case MOAIShaderPresetEnum::DECK2D_SNAPPING_SHADER:
//
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_POSITION, "position" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_TEXCOORD, "uv" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_COLOR, "color" );
//
//					program->ReserveUniforms ( 2 );
//					program->DeclareUniform ( ZLIndexCast ( 0 ), "xSnap", MOAIShaderUniformVK::UNIFORM_TYPE_FLOAT );
//					program->DeclareUniform ( ZLIndexCast ( 1 ), "ySnap", MOAIShaderUniformVK::UNIFORM_TYPE_FLOAT );
//
//					program->ReserveGlobals ( 2 );
//					program->SetGlobal ( ZLIndexCast ( 0 ), MOAIGfxMgrVK::VIEW_HALF_WIDTH, ZLIndexCast ( 0 ), ZLIndexCast ( 0 ));
//					program->SetGlobal ( ZLIndexCast ( 1 ), MOAIGfxMgrVK::VIEW_HALF_HEIGHT, ZLIndexCast ( 1 ), ZLIndexCast ( 0 ));
//
//					program->Load ( _deck2DSnappingShaderVSH, _deck2DSnappingShaderFSH );
//					break;
//
//				case MOAIShaderPresetEnum::DECK2D_TEX_ONLY_SHADER:
//
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_POSITION, "position" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_TEXCOORD, "uv" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_COLOR, "color" );
//
//					program->Load ( _deck2DTexOnlyShaderVSH, _deck2DTexOnlyShaderFSH );
//					break;
//
//				case MOAIShaderPresetEnum::FONT_SHADER:
//
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_POSITION, "position" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_TEXCOORD, "uv" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_COLOR, "color" );
//
//					program->Load ( _fontShaderVSH, _fontShaderFSH );
//					break;
//
//				case MOAIShaderPresetEnum::FONT_SNAPPING_SHADER:
//
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_POSITION, "position" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_TEXCOORD, "uv" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_COLOR, "color" );
//
//					program->ReserveUniforms ( 2 );
//					program->DeclareUniform ( ZLIndexCast ( 0 ), "xSnap", MOAIShaderUniformVK::UNIFORM_TYPE_FLOAT );
//					program->DeclareUniform ( ZLIndexCast ( 1 ), "ySnap", MOAIShaderUniformVK::UNIFORM_TYPE_FLOAT );
//
//					program->ReserveGlobals ( 2 );
//					program->SetGlobal ( ZLIndexCast ( 0 ), MOAIGfxMgrVK::VIEW_HALF_WIDTH, ZLIndexCast ( 0 ), ZLIndexCast ( 0 ));
//					program->SetGlobal ( ZLIndexCast ( 1 ), MOAIGfxMgrVK::VIEW_HALF_HEIGHT, ZLIndexCast ( 1 ), ZLIndexCast ( 0 ));
//
//					program->Load ( _fontSnappingShaderVSH, _fontSnappingShaderFSH );
//					break;
//
//				case MOAIShaderPresetEnum::LINE_SHADER:
//
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWC_POSITION, "position" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWC_COLOR, "color" );
//					program->Load ( _lineShaderVSH, _lineShaderFSH );
//					break;
//
//				case MOAIShaderPresetEnum::LINE_SHADER_3D:
//
//					program->SetVertexAttribute ( 0, "position" );
//					program->SetVertexAttribute ( 1, "color" );
//
//					program->ReserveUniforms ( 2 );
//					program->DeclareUniform ( ZLIndexCast ( 0 ), "transform", MOAIShaderUniformVK::UNIFORM_TYPE_FLOAT, MOAIShaderUniformVK::UNIFORM_WIDTH_MATRIX_4X4 );
//					program->DeclareUniform ( ZLIndexCast ( 1 ), "ucolor", MOAIShaderUniformVK::UNIFORM_TYPE_FLOAT, MOAIShaderUniformVK::UNIFORM_WIDTH_VEC_4 );
//
//					program->ReserveGlobals ( 2 );
//					program->SetGlobal ( ZLIndexCast ( 0 ), MOAIGfxMgrVK::MODEL_TO_CLIP_MTX, ZLIndexCast ( 0 ), ZLIndexCast ( 0 ));
//					program->SetGlobal ( ZLIndexCast ( 1 ), MOAIGfxMgrVK::PEN_COLOR, ZLIndexCast ( 1 ), ZLIndexCast ( 0 ));
//
//					program->Load ( _lineShader3DVSH, _lineShader3DFSH );
//
//					break;
//
//				case MOAIShaderPresetEnum::MESH_SHADER:
//
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_POSITION, "position" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_TEXCOORD, "uv" );
//					program->SetVertexAttribute ( MOAIVertexFormatMgrVK::XYZWUVC_COLOR, "color" );
//
//					program->ReserveUniforms ( 2 );
//					program->DeclareUniform ( ZLIndexCast ( 0 ), "transform", MOAIShaderUniformVK::UNIFORM_TYPE_FLOAT, MOAIShaderUniformVK::UNIFORM_WIDTH_MATRIX_4X4 );
//					program->DeclareUniform ( ZLIndexCast ( 1 ), "ucolor", MOAIShaderUniformVK::UNIFORM_TYPE_FLOAT, MOAIShaderUniformVK::UNIFORM_WIDTH_VEC_4 );
//
//					program->ReserveGlobals ( 2 );
//					program->SetGlobal ( ZLIndexCast ( 0 ), MOAIGfxMgrVK::MODEL_TO_CLIP_MTX, ZLIndexCast ( 0 ), ZLIndexCast ( 0 ));
//					program->SetGlobal ( ZLIndexCast ( 1 ), MOAIGfxMgrVK::PEN_COLOR, ZLIndexCast ( 1 ), ZLIndexCast ( 0 ));
//
//					program->Load ( _meshShaderVSH, _meshShaderFSH );
//
//					break;

				case MOAIShaderPresetEnum::ONETRI_SHADER: {
				
					MOAIDescriptorSetLayoutVK* descriptorSetLayout = new MOAIDescriptorSetLayoutVK ();
					descriptorSetLayout->Initialize ( logicalDevice, 2 );
					descriptorSetLayout->SetBinding ( ZLIndexCast ( 0 ), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT );
					descriptorSetLayout->SetBinding ( ZLIndexCast ( 1 ), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT );
					
					MOAIPipelineLayoutVK* pipelineLayout = new MOAIPipelineLayoutVK ();
					pipelineLayout->Initialize ( logicalDevice, 1 );
					pipelineLayout->SetDescriptorSetLayout ( ZLIndexOp::ZERO, *descriptorSetLayout );
					
					MOAIGfxComposerVK* composer = new MOAIGfxComposerVK ();
					composer->SetPipelineLayout ( *this->mOneTexPipelineLayout );
					composer->Reserve ( 1 );
					composer->PushTextureCommand ( ZLIndexCast ( 1 ), ZLIndexOp::ZERO, ZLIndexOp::ZERO, ZLIndexOp::ZERO );
					
					program->LoadModule ( MOAIShaderProgramVK::VERTEX_MODULE, _oneTriShaderVSH, sizeof ( _oneTriShaderVSH ));
					program->LoadModule ( MOAIShaderProgramVK::FRAGMENT_MODULE, _oneTriShaderFSH, sizeof ( _oneTriShaderFSH ));
					program->SetGfxComposer ( *composer );
					
					break;
				}
			}

			this->mPrograms [ shaderID ] = program;
		}
	}
	return program;
}

//----------------------------------------------------------------//
MOAIShaderVK* MOAIShaderMgrVK::GetShader ( MOAIShaderPresetEnum shaderID ) {

	MOAIShaderVK* shader = 0;

	if ( shaderID < MOAIShaderPresetEnum::TOTAL_SHADERS ) {

		shader = this->mShaders [ shaderID ];

		if ( !shader ) {

			MOAIShaderProgramVK* program = this->GetProgram ( shaderID );
			if ( program ) {

				shader = new MOAIShaderVK ();
				this->LuaRetain ( shader );
				shader->SetProgram ( program );
				this->mShaders [ shaderID ] = shader;
			}
		}
	}
	return shader;
}

////----------------------------------------------------------------//
//MOAIShaderVK* MOAIShaderMgrVK::GetShader ( lua_State* L, int idx ) {
//
//	MOAILuaState state ( L );
//	MOAIShaderVK* shader = NULL;
//
//	if ( state.IsType ( idx, LUA_TNUMBER )) {
//		shader = this->GetShader (( MOAIShaderPresetEnum )state.GetValue < u32 >( idx, ( u32 )MOAIShaderPresetEnum::UNKNOWN_SHADER ));
//	}
//	else {
//		shader = state.GetLuaObject < MOAIShaderVK >( idx, true );
//	}
//	return shader;
//}

//----------------------------------------------------------------//
MOAIShaderMgrVK::MOAIShaderMgrVK () {

	RTTI_SINGLE ( MOAILuaObject )

	for ( u32 i = 0; i < MOAIShaderPresetEnum::TOTAL_SHADERS; ++i ) {
		this->mPrograms [ i ] = 0;
	}

	for ( u32 i = 0; i < MOAIShaderPresetEnum::TOTAL_SHADERS; ++i ) {
		this->mShaders [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIShaderMgrVK::~MOAIShaderMgrVK () {

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
void MOAIShaderMgrVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

//	state.SetField ( -1, "DECK2D_SHADER",			( u32 )MOAIShaderPresetEnum::DECK2D_SHADER );
//	state.SetField ( -1, "DECK2D_SNAPPING_SHADER",	( u32 )MOAIShaderPresetEnum::DECK2D_SNAPPING_SHADER );
//	state.SetField ( -1, "FONT_SHADER",				( u32 )MOAIShaderPresetEnum::FONT_SHADER );
//	state.SetField ( -1, "FONT_SNAPPING_SHADER",	( u32 )MOAIShaderPresetEnum::FONT_SNAPPING_SHADER );
//	state.SetField ( -1, "FONT_EFFECTS_SHADER",	    ( u32 )MOAIShaderPresetEnum::FONT_EFFECTS_SHADER );
//	state.SetField ( -1, "LINE_SHADER",				( u32 )MOAIShaderPresetEnum::LINE_SHADER );
//	state.SetField ( -1, "LINE_SHADER_3D",			( u32 )MOAIShaderPresetEnum::LINE_SHADER_3D );
//	state.SetField ( -1, "MESH_SHADER",				( u32 )MOAIShaderPresetEnum::MESH_SHADER );
//	
//	luaL_Reg regTable [] = {
//		{ "getProgram",				_getProgram },
//		{ "getShader",				_getShader },
//		{ NULL, NULL }
//	};
//
//	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShaderMgrVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );
}
