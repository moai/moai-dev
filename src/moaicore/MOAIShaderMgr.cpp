// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAIVertexFormatMgr.h>

#include <moaicore/shaders/MOAIDeck2DShader-fsh.h>
#include <moaicore/shaders/MOAIDeck2DShader-vsh.h>
#include <moaicore/shaders/MOAIFontShader-fsh.h>
#include <moaicore/shaders/MOAIFontShader-vsh.h>
#include <moaicore/shaders/MOAILineShader-fsh.h>
#include <moaicore/shaders/MOAILineShader-vsh.h>

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
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYUVC_POSITION, "position" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYUVC_TEXCOORD, "uv" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYUVC_COLOR, "color" );
				break;
				
			case FONT_SHADER:
				
				shader->SetSource ( _fontShaderVSH, _fontShaderFSH );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYUVC_POSITION, "position" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYUVC_TEXCOORD, "uv" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYUVC_COLOR, "color" );
				break;
			
			case LINE_SHADER:
				
				shader->SetSource ( _lineShaderVSH, _lineShaderFSH );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYC_POSITION, "position" );
				shader->SetVertexAttribute ( MOAIVertexFormatMgr::XYC_COLOR, "color" );
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
