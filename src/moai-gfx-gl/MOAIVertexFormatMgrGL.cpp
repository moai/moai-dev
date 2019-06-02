// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIVertexFormatGL.h>
#include <moai-gfx-gl/MOAIVertexFormatMgrGL.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormatMgrGL::_getFormat ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 formatID = ( u32 )state.GetValue < u32 >( 1, ( u32 )MOAIVertexFormatPresetEnum::UNKNOWN_FORMAT );
	
	state.Push ( MOAIVertexFormatMgrGL::Get ().GetFormat (( MOAIVertexFormatPresetEnum )formatID ));
	return 1;
}

//================================================================//
// MOAIVertexFormatMgrGL
//================================================================//

//----------------------------------------------------------------//
MOAIVertexFormatGL* MOAIVertexFormatMgrGL::GetFormat ( MOAIVertexFormatPresetEnum formatID ) {
	
	MOAIVertexFormatGL* format = 0;
	
	if ( formatID < MOAIVertexFormatPresetEnum::TOTAL_FORMATS ) {
	
		format = this->mFormats [ formatID ];
		
		if ( !format ) {

			format = new MOAIVertexFormatGL ();
			this->LuaRetain ( format );
			
			switch ( formatID ) {
				
				case MOAIVertexFormatPresetEnum::XYZC:
					format->DeclareAttribute ( ZLIndexCast ( XYZC_POSITION ), ZGL_TYPE_FLOAT, 3, MOAIVertexFormatGL::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZC_COLOR ), ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormatGL::ATTRIBUTE_COLOR, true );
					break;
				
				case MOAIVertexFormatPresetEnum::XYZWC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWC_POSITION ), ZGL_TYPE_FLOAT, 4, MOAIVertexFormatGL::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWC_COLOR ), ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormatGL::ATTRIBUTE_COLOR, true );
					break;
				
				case MOAIVertexFormatPresetEnum::XYZWUVC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWUVC_POSITION ), ZGL_TYPE_FLOAT, 4, MOAIVertexFormatGL::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWUVC_TEXCOORD ), ZGL_TYPE_FLOAT, 2, MOAIVertexFormatGL::ATTRIBUTE_TEX_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWUVC_COLOR ), ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormatGL::ATTRIBUTE_COLOR, true );
					break;
				
				case MOAIVertexFormatPresetEnum::XYZWNNNC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNC_POSITION ), ZGL_TYPE_FLOAT, 4, MOAIVertexFormatGL::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNC_NORMAL ), ZGL_TYPE_FLOAT, 3, MOAIVertexFormatGL::ATTRIBUTE_NORMAL, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNC_COLOR ), ZGL_TYPE_UNSIGNED_BYTE, 4, MOAIVertexFormatGL::ATTRIBUTE_COLOR, true );
					break;
				
				case MOAIVertexFormatPresetEnum::XYZWNNNUVC:
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
		format = this->GetFormat (( MOAIVertexFormatPresetEnum )state.GetValue < u32 >( idx, ( u32 )MOAIVertexFormatPresetEnum::UNKNOWN_FORMAT ));
	}
	else {
		format = state.GetLuaObject < MOAIVertexFormatGL >( idx, true );
	}
	return format;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormatMgrGL::GetVertexSize ( MOAIVertexFormatPresetEnum formatID ) {

	const MOAIVertexFormatGL* format = this->GetFormat ( formatID );
	return format ? format->GetVertexSize () : 0;
}

//----------------------------------------------------------------//
MOAIVertexFormatMgrGL::MOAIVertexFormatMgrGL () {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	for ( u32 i = 0; i < MOAIVertexFormatPresetEnum::TOTAL_FORMATS; ++i ) {
		this->mFormats [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIVertexFormatMgrGL::~MOAIVertexFormatMgrGL () {

	for ( u32 i = 0; i < MOAIVertexFormatPresetEnum::TOTAL_FORMATS; ++i ) {
		if ( this->mFormats [ i ]) {
			this->LuaRelease ( this->mFormats [ i ]);
		}
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexFormatMgrGL::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	state.SetField ( -1, "XYZC",			( u32 )MOAIVertexFormatPresetEnum::XYZC );
	state.SetField ( -1, "XYZWC",			( u32 )MOAIVertexFormatPresetEnum::XYZWC );
	state.SetField ( -1, "XYZWUVC",			( u32 )MOAIVertexFormatPresetEnum::XYZWUVC );
	state.SetField ( -1, "XYZWNNNC",		( u32 )MOAIVertexFormatPresetEnum::XYZWNNNC );
	state.SetField ( -1, "XYZWNNNUVC",		( u32 )MOAIVertexFormatPresetEnum::XYZWNNNUVC );
	
	luaL_Reg regTable [] = {
		{ "getFormat",				_getFormat },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIVertexFormatMgrGL::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );
}
