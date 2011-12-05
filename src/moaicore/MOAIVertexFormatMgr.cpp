// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIVertexFormat.h>
#include <moaicore/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIVertexFormatMgr
//================================================================//

//----------------------------------------------------------------//
const MOAIVertexFormat& MOAIVertexFormatMgr::GetPreset ( u32 presetID ) {

	assert ( presetID < TOTAL_PRESETS );
	return this->mFormats [ presetID ];
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::MOAIVertexFormatMgr () {
	
	MOAIVertexFormat* format;
	
	format = &this->mFormats [ XYC ];
		
	format->DeclareAttribute ( XYC_POSITION, GL_FLOAT, 2, MOAIVertexFormat::ARRAY_VERTEX, false );
	format->DeclareAttribute ( XYC_COLOR, GL_UNSIGNED_BYTE, 4, MOAIVertexFormat::ARRAY_COLOR, true );
	
	format = &this->mFormats [ XYUVC ];

	format->DeclareAttribute ( XYUVC_POSITION, GL_FLOAT, 2, MOAIVertexFormat::ARRAY_VERTEX, false );
	format->DeclareAttribute ( XYUVC_TEXCOORD, GL_FLOAT, 2, MOAIVertexFormat::ARRAY_TEX_COORD, false );
	format->DeclareAttribute ( XYUVC_COLOR, GL_UNSIGNED_BYTE, 4, MOAIVertexFormat::ARRAY_COLOR, true );
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::~MOAIVertexFormatMgr () {
}
