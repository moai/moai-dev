// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

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
	
	format = &this->mFormats [ XYZWC ];
	
	format->DeclareAttribute ( XYZWC_POSITION, ZGL_TYPE_FLOAT, 4, MOAIVertexFormat::ARRAY_VERTEX, false );
	format->DeclareAttribute ( XYZWC_COLOR, ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormat::ARRAY_COLOR, true );
	
	format = &this->mFormats [ XYZWUVC ];

	format->DeclareAttribute ( XYZWUVC_POSITION, ZGL_TYPE_FLOAT, 4, MOAIVertexFormat::ARRAY_VERTEX, false );
	format->DeclareAttribute ( XYZWUVC_TEXCOORD, ZGL_TYPE_FLOAT, 2, MOAIVertexFormat::ARRAY_TEX_COORD, false );
	format->DeclareAttribute ( XYZWUVC_COLOR, ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormat::ARRAY_COLOR, true );
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::~MOAIVertexFormatMgr () {
}
