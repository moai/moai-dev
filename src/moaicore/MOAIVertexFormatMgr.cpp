// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIVertexFormatMgr.h>

//================================================================//
// MOAIVertexFormatMgr
//================================================================//

//----------------------------------------------------------------//
const MOAIVertexFormat& MOAIVertexFormatMgr::GetPreset ( u32 presetID ) {

	assert ( presetID < TOTAL_PRESETS );
	return MOAIGfxDevice::Get ().IsProgrammable () ? this->mFormatsPP [ presetID ] : this->mFormatsFF [ presetID ];
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::MOAIVertexFormatMgr () {

	MOAIVertexFormat* format;
	
	// fixed function
	
	format = &this->mFormatsFF [ XYC ];
		
		format->DeclareVertexArray ( GL_FLOAT, 2 );
		format->DeclareColorArray ( GL_UNSIGNED_BYTE );
	
	format = &this->mFormatsFF [ XYUVC ];
		
		format->DeclareVertexArray ( GL_FLOAT, 2 );
		format->DeclareTexCoordArray ( GL_FLOAT, 2 );
		format->DeclareColorArray ( GL_UNSIGNED_BYTE );

	// programmable pipeline
	
	format = &this->mFormatsPP [ XYC ];
		
		format->ReserveAttributes ( XYC_SIZE );
		format->DeclareAttribute ( XYC_POSITION, 2, GL_FLOAT, false );
		format->DeclareAttribute ( XYC_COLOR, 4, GL_UNSIGNED_BYTE, true );
	
	format = &this->mFormatsPP [ XYUVC ];

		format->ReserveAttributes ( XYUVC_SIZE );
		format->DeclareAttribute ( XYUVC_POSITION, 2, GL_FLOAT, false );
		format->DeclareAttribute ( XYUVC_TEXCOORD, 2, GL_FLOAT, false );
		format->DeclareAttribute ( XYUVC_COLOR, 4, GL_UNSIGNED_BYTE, true );
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::~MOAIVertexFormatMgr () {
}
