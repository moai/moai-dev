// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAIVertexFormatMgr.h>

#include <moaicore/shaders/MOAIDeck2DShader-fsh.h>
#include <moaicore/shaders/MOAIDeck2DShader-vsh.h>
#include <moaicore/shaders/MOAIDeck2DTexOnlyShader-fsh.h>
#include <moaicore/shaders/MOAIDeck2DTexOnlyShader-vsh.h>
#include <moaicore/shaders/MOAIFontShader-fsh.h>
#include <moaicore/shaders/MOAIFontShader-vsh.h>
#include <moaicore/shaders/MOAILineShader-fsh.h>
#include <moaicore/shaders/MOAILineShader-vsh.h>
#include <moaicore/shaders/MOAIMeshShader-fsh.h>
#include <moaicore/shaders/MOAIMeshShader-vsh.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getShader
	@text	Return one of the built-in shaders.

	@in		number shaderID		One of MOAIShaderMgr.DECK2D_SHADER, MOAIShaderMgr.FONT_SHADER,
								MOAIShaderMgr.LINE_SHADER, MOAIShaderMgr.MESH_SHADER
	@out	nil
*/
int MOAIShaderMgr::_getShader ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 shaderID = state.GetValue < u32 >( 1, 0xffffffff );
	
	if ( shaderID < TOTAL_SHADERS ) {
	
		MOAIShader& shader = MOAIShaderMgr::Get ().GetShader ( shaderID );
		shader.PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIShaderMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderMgr::BindShader ( u32 shaderID ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	if ( gfxDevice.IsProgrammable ()) {

		MOAIShader& shader = this->GetShader ( shaderID );
		gfxDevice.SetShader ( &shader );
	}
}

//----------------------------------------------------------------//
MOAIShader& MOAIShaderMgr::GetShader ( u32 shaderID ) {

	assert ( shaderID < TOTAL_SHADERS );
	MOAIShader* shader = this->mShaders [ shaderID ];
	
	if ( !shader ) {

		shader = new MOAIShader ();
		shader->Retain ();
		
		switch ( shaderID ) {
			
			case DECK2D_SHADER:
				
				shader->SetSource ( _deck2DShaderVSH, _deck2DShaderFSH );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
				break;
			
			case DECK2D_TEX_ONLY_SHADER:
				
				shader->SetSource ( _deck2DTexOnlyShaderVSH, _deck2DTexOnlyShaderFSH );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
				break;
			
			case FONT_SHADER:
				
				shader->SetSource ( _fontShaderVSH, _fontShaderFSH );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );
				break;
			
			case LINE_SHADER:
				
				shader->SetSource ( _lineShaderVSH, _lineShaderFSH );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWC_POSITION, "position" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWC_COLOR, "color" );
				break;

			case MESH_SHADER:

				shader->SetSource ( _meshShaderVSH, _meshShaderFSH );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_POSITION, "position" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_TEXCOORD, "uv" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYZWUVC_COLOR, "color" );

				shader->ReserveUniforms ( 2 );
				shader->DeclareUniform ( 0, "transform", MOAIShaderUniform::UNIFORM_WORLD_VIEW_PROJ );
				shader->DeclareUniform ( 1, "ucolor", MOAIShaderUniform::UNIFORM_PEN_COLOR );

				break;
		}
		
		this->mShaders [ shaderID ] = shader;
	}
	
	return *shader;
}

//----------------------------------------------------------------//
MOAIShaderMgr::MOAIShaderMgr () {

	RTTI_SINGLE ( MOAILuaObject )

	for ( u32 i = 0; i < TOTAL_SHADERS; ++i ) {
		this->mShaders [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIShaderMgr::~MOAIShaderMgr () {

	for ( u32 i = 0; i < TOTAL_SHADERS; ++i ) {
		if ( this->mShaders [ i ]) {
			this->mShaders [ i ]->Release ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIShaderMgr::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "DECK2D_SHADER",			( u32 )DECK2D_SHADER );
	state.SetField ( -1, "DECK2D_TEX_ONLY_SHADER",	( u32 )DECK2D_TEX_ONLY_SHADER );
	state.SetField ( -1, "FONT_SHADER",				( u32 )FONT_SHADER );
	state.SetField ( -1, "LINE_SHADER",				( u32 )LINE_SHADER );
	state.SetField ( -1, "MESH_SHADER",				( u32 )MESH_SHADER );
	
	luaL_Reg regTable [] = {
		{ "getShader",				_getShader },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIShaderMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}
