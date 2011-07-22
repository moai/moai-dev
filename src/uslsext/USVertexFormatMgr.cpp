// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USVertexFormatMgr.h>

//================================================================//
// USVertexFormatMgr
//================================================================//

//----------------------------------------------------------------//
const USVertexFormat& USVertexFormatMgr::GetPreset ( u32 presetID ) {

	assert ( presetID < TOTAL_PRESETS );
	return this->mFormats [ presetID ];
}

//----------------------------------------------------------------//
USVertexFormatMgr::USVertexFormatMgr () {

	USVertexFormat* format;
	
	format = &this->mFormats [ FF_XYC ];

	format->DeclareVertexArray ( GL_FLOAT, 2 );
	format->DeclareColorArray ( GL_UNSIGNED_BYTE );
	
	format = &this->mFormats [ FF_XYUVC ];

	format->DeclareVertexArray ( GL_FLOAT, 2 );
	format->DeclareTexCoordArray ( GL_FLOAT, 2 );
	format->DeclareColorArray ( GL_UNSIGNED_BYTE );
	
	format = &this->mFormats [ PP_XYUVC ];

	format->ReserveAttributes ( PP_XYUVC_SIZE );
	format->DeclareAttribute ( PP_XYUVC_POSITION, 2, GL_FLOAT, false );
	format->DeclareAttribute ( PP_XYUVC_TEXCOORD, 2, GL_FLOAT, false );
	format->DeclareAttribute ( PP_XYUVC_COLOR, 4, GL_UNSIGNED_BYTE, true );
}

//----------------------------------------------------------------//
USVertexFormatMgr::~USVertexFormatMgr () {
}