// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIShader.h>
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

		shader = new MOAIShader ();
		shader->Retain ();
		
		switch ( shaderID ) {
			
			case MOAI_BASIC_NO_TEXTURE:
			
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYC_POSITION, "position" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYC_COLOR, "color" );
				shader->SetSource ( _basicNoTextureVSH, _basicNoTextureFSH );
				break;
				
			case MOAI_BASIC_ONE_TEXTURE:
				
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYUVC_POSITION, "position" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYUVC_COLOR, "color" );
				shader->SetSource ( _basicNoTextureVSH, _basicNoTextureFSH );
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
