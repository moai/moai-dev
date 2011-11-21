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
	
	format = &this->mFormats [ XYZWC ];
		
	format->DeclareAttribute ( XYZWC_POSITION, GL_FLOAT, 4, GL_VERTEX_ARRAY, false );
	format->DeclareAttribute ( XYZWC_COLOR, GL_UNSIGNED_BYTE, 4, GL_COLOR_ARRAY, true );
	
	format = &this->mFormats [ XYZWUVC ];

	format->DeclareAttribute ( XYZWUVC_POSITION, GL_FLOAT, 4, GL_VERTEX_ARRAY, false );
	format->DeclareAttribute ( XYZWUVC_TEXCOORD, GL_FLOAT, 2, GL_TEXTURE_COORD_ARRAY, false );
	format->DeclareAttribute ( XYZWUVC_COLOR, GL_UNSIGNED_BYTE, 4, GL_COLOR_ARRAY, true );
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::~MOAIVertexFormatMgr () {
}
