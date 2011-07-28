// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGlslProgram.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAIVertexFormatMgr.h>

#include <moaicore/shaders/MOAIBasicNoTexture-fsh.h>
#include <moaicore/shaders/MOAIBasicNoTexture-vsh.h>

//================================================================//
// MOAIShaderMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderMgr::BindShader ( u32 shaderID ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	if ( gfxDevice.IsProgrammable ()) {

		MOAIShader& shader = this->GetShader ( shaderID );
		gfxDevice.Flush ();
		shader.Bind ();
	}
}

//----------------------------------------------------------------//
MOAIShader& MOAIShaderMgr::GetShader ( u32 shaderID ) {

	assert ( shaderID < TOTAL_SHADERS );
	MOAIShader* shader = this->mShaders [ shaderID ];
	
	if ( !shader ) {
	
		MOAIGlslProgram* program = new MOAIGlslProgram ();
		program->Retain ();
		shader = program;
		
		switch ( shaderID ) {
			
			case MOAI_BASIC_NO_TEXTURE:
			
				program->SetVertexAttribute ( MOAIVertexFormatMgr::XYC_POSITION, "position" );
				program->SetVertexAttribute ( MOAIVertexFormatMgr::XYC_COLOR, "color" );
				program->SetSource ( _basicNoTextureVSH, _basicNoTextureFSH );
				break;
				
			case MOAI_BASIC_ONE_TEXTURE:
				
				program->SetVertexAttribute ( MOAIVertexFormatMgr::XYUVC_POSITION, "position" );
				program->SetVertexAttribute ( MOAIVertexFormatMgr::XYUVC_COLOR, "color" );
				program->SetSource ( _basicNoTextureVSH, _basicNoTextureFSH );
				break;
		}
		
		this->mShaders [ shaderID ] = shader;
	}
	
	return *shader;
}

//----------------------------------------------------------------//
MOAIShaderMgr::MOAIShaderMgr () {

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
