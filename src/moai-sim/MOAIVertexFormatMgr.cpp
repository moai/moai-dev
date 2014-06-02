// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormatMgr::_getFormat ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 formatID = state.GetValue < u32 >( 1, 0xffffffff );
	
	if ( formatID < TOTAL_FORMATS ) {
	
		MOAIVertexFormat& format = MOAIVertexFormatMgr::Get ().GetFormat ( formatID );
		format.PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIVertexFormatMgr
//================================================================//

//----------------------------------------------------------------//
MOAIVertexFormat& MOAIVertexFormatMgr::GetFormat ( u32 formatID ) {
	
	assert ( formatID < TOTAL_FORMATS );
	MOAIVertexFormat* format = this->mFormats [ formatID ];
	
	if ( !format ) {

		format = new MOAIVertexFormat ();
		this->LuaRetain ( format );
		
		switch ( formatID ) {
			
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
		
		this->mFormats [ formatID ] = format;
	}
	
	return *format;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormatMgr::GetVertexSize ( u32 formatID ) {

	const MOAIVertexFormat& format = this->GetFormat ( formatID );
	return format.GetVertexSize ();
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::MOAIVertexFormatMgr () {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	for ( u32 i = 0; i < TOTAL_FORMATS; ++i ) {
		this->mFormats [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::~MOAIVertexFormatMgr () {

	for ( u32 i = 0; i < TOTAL_FORMATS; ++i ) {
		if ( this->mFormats [ i ]) {
			this->LuaRelease ( this->mFormats [ i ]);
		}
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormatMgr::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "XYZC",			( u32 )XYZC );
	state.SetField ( -1, "XYZWC",			( u32 )XYZWC );
	state.SetField ( -1, "XYZWUVC",			( u32 )XYZWUVC );
	
	luaL_Reg regTable [] = {
		{ "getFormat",				_getFormat },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexFormatMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}
