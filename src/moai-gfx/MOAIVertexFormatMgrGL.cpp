// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgrGL.h>
#include <moai-gfx/MOAIVertexFormatGL.h>
#include <moai-gfx/MOAIVertexFormatMgrGL.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormatMgrGL::_getFormat ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 formatID = state.GetValue < u32 >( 1, 0xffffffff );
	
	state.Push ( MOAIVertexFormatMgrGL::Get ().GetFormat ( formatID ));
	return 1;
}

//================================================================//
// MOAIVertexFormatMgrGL
//================================================================//

//----------------------------------------------------------------//
MOAIVertexFormatGL* MOAIVertexFormatMgrGL::GetFormat ( u32 formatID ) {
	
	MOAIVertexFormatGL* format = 0;
	
	if ( formatID < TOTAL_FORMATS ) {
	
		format = this->mFormats [ formatID ];
		
		if ( !format ) {

			format = new MOAIVertexFormatGL ();
			this->LuaRetain ( format );
			
			switch ( formatID ) {
				
				case XYZC:
					format->DeclareAttribute ( ZLIndexCast ( XYZC_POSITION ), ZGL_TYPE_FLOAT, 3, MOAIVertexFormatGL::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZC_COLOR ), ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormatGL::ATTRIBUTE_COLOR, true );
					break;
				
				case XYZWC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWC_POSITION ), ZGL_TYPE_FLOAT, 4, MOAIVertexFormatGL::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWC_COLOR ), ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormatGL::ATTRIBUTE_COLOR, true );
					break;
				
				case XYZWUVC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWUVC_POSITION ), ZGL_TYPE_FLOAT, 4, MOAIVertexFormatGL::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWUVC_TEXCOORD ), ZGL_TYPE_FLOAT, 2, MOAIVertexFormatGL::ATTRIBUTE_TEX_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWUVC_COLOR ), ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormatGL::ATTRIBUTE_COLOR, true );
					break;
				
				case XYZWNNNC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNC_POSITION ), ZGL_TYPE_FLOAT, 4, MOAIVertexFormatGL::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNC_NORMAL ), ZGL_TYPE_FLOAT, 3, MOAIVertexFormatGL::ATTRIBUTE_NORMAL, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNC_COLOR ), ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormatGL::ATTRIBUTE_COLOR, true );
					break;
				
				case XYZWNNNUVC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNUVC_POSITION ), ZGL_TYPE_FLOAT, 4, MOAIVertexFormatGL::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNUVC_NORMAL ), ZGL_TYPE_FLOAT, 3, MOAIVertexFormatGL::ATTRIBUTE_NORMAL, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNUVC_TEXCOORD ), ZGL_TYPE_FLOAT, 2, MOAIVertexFormatGL::ATTRIBUTE_TEX_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNUVC_COLOR ), ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormatGL::ATTRIBUTE_COLOR, true );
					break;
			}
			
			this->mFormats [ formatID ] = format;
		}
	}
	return format;
}

//----------------------------------------------------------------//
MOAIVertexFormatGL* MOAIVertexFormatMgrGL::GetFormat ( lua_State* L, int idx ) {

	MOAILuaState state ( L );
	MOAIVertexFormatGL* format = NULL;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		format = this->GetFormat ( state.GetValue < u32 >( idx, MOAIVertexFormatMgrGL::UNKNOWN_FORMAT ));
	}
	else {
		format = state.GetLuaObject < MOAIVertexFormatGL >( idx, true );
	}
	return format;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormatMgrGL::GetVertexSize ( u32 formatID ) {

	const MOAIVertexFormatGL* format = this->GetFormat ( formatID );
	return format ? format->GetVertexSize () : 0;
}

//----------------------------------------------------------------//
MOAIVertexFormatMgrGL::MOAIVertexFormatMgrGL () {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	for ( u32 i = 0; i < TOTAL_FORMATS; ++i ) {
		this->mFormats [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIVertexFormatMgrGL::~MOAIVertexFormatMgrGL () {

	for ( u32 i = 0; i < TOTAL_FORMATS; ++i ) {
		if ( this->mFormats [ i ]) {
			this->LuaRelease ( this->mFormats [ i ]);
		}
	}
}

//----------------------------------------------------------------//
void MOAIVertexFormatMgrGL::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "XYZC",			( u32 )XYZC );
	state.SetField ( -1, "XYZWC",			( u32 )XYZWC );
	state.SetField ( -1, "XYZWUVC",			( u32 )XYZWUVC );
	state.SetField ( -1, "XYZWNNNC",		( u32 )XYZWNNNC );
	state.SetField ( -1, "XYZWNNNUVC",		( u32 )XYZWNNNUVC );
	
	luaL_Reg regTable [] = {
		{ "getFormat",				_getFormat },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexFormatMgrGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}
