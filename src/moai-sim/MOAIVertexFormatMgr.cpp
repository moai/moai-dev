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
MOAIVertexFormat& MOAIVertexFormatMgr::GetPreset ( u32 presetID ) {
	
	assert ( presetID < TOTAL_PRESETS );
	MOAIVertexFormat* format = this->mFormats [ presetID ];
	
	if ( !format ) {

		format = new MOAIVertexFormat ();
		this->LuaRetain ( format );
		
		switch ( presetID ) {
			
			case XYZC:
				format->DeclareAttribute ( XYZC_POSITION, ZGL_TYPE_FLOAT, 3, MOAIVertexFormat::ARRAY_VERTEX, false );
				format->DeclareAttribute ( XYZC_COLOR, ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormat::ARRAY_COLOR, true );
				break;
			
			case XYZWC:
				format->DeclareAttribute ( XYZWC_POSITION, ZGL_TYPE_FLOAT, 4, MOAIVertexFormat::ARRAY_VERTEX, false );
				format->DeclareAttribute ( XYZWC_COLOR, ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormat::ARRAY_COLOR, true );
				break;
			
			case XYZWUVC:
				format->DeclareAttribute ( XYZWUVC_POSITION, ZGL_TYPE_FLOAT, 4, MOAIVertexFormat::ARRAY_VERTEX, false );
				format->DeclareAttribute ( XYZWUVC_TEXCOORD, ZGL_TYPE_FLOAT, 2, MOAIVertexFormat::ARRAY_TEX_COORD, false );
				format->DeclareAttribute ( XYZWUVC_COLOR, ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormat::ARRAY_COLOR, true );
				break;
		}
		
		this->mFormats [ presetID ] = format;
	}
	
	return *format;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormatMgr::GetVertexSize ( u32 presetID ) {

	const MOAIVertexFormat& format = this->GetPreset ( presetID );
	return format.GetVertexSize ();
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::MOAIVertexFormatMgr () {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	for ( u32 i = 0; i < TOTAL_PRESETS; ++i ) {
		this->mFormats [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::~MOAIVertexFormatMgr () {

	for ( u32 i = 0; i < TOTAL_PRESETS; ++i ) {
		if ( this->mFormats [ i ]) {
			this->LuaRelease ( this->mFormats [ i ]);
		}
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormatMgr::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIVertexFormatMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}
