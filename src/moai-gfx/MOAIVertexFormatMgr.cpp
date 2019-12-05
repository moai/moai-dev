// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxPresetEnum.h>
#include <moai-gfx/MOAIVertexFormatMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVertexFormatMgr::_getFormat ( lua_State* L ) {
	MOAILuaState state ( L );

	u32 formatID = ( u32 )state.GetValue < u32 >( 1, ( u32 )MOAIVertexFormatPresetEnum::UNKNOWN_FORMAT );
	
	state.Push ( MOAIVertexFormatMgr::Get ().GetFormat (( MOAIVertexFormatPresetEnum )formatID ));
	return 1;
}

//================================================================//
// MOAIVertexFormatMgr
//================================================================//

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIVertexFormatMgr::GetFormat ( MOAIVertexFormatPresetEnum formatID ) {
	
	MOAIVertexFormat* format = 0;
	
	if ( formatID < MOAIVertexFormatPresetEnum::TOTAL_FORMATS ) {
	
		format = this->mFormats [ formatID ];
		
		if ( !format ) {

			format = MOAIGfxMgr::Get ().CreateVertexFormat ();
			this->LuaRetain ( format );
			
			switch ( formatID ) {
				
				case MOAIVertexFormatPresetEnum::XYZC:
					format->DeclareAttribute ( ZLIndexCast ( XYZC_POSITION ), MOAIGfxTypeEnum::FLOAT, 3, MOAIVertexFormat::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZC_COLOR ), MOAIGfxTypeEnum::UNSIGNED_BYTE, 4, MOAIVertexFormat::ATTRIBUTE_COLOR, true );
					break;
				
				case MOAIVertexFormatPresetEnum::XYZWC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWC_POSITION ), MOAIGfxTypeEnum::FLOAT, 4, MOAIVertexFormat::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWC_COLOR ), MOAIGfxTypeEnum::UNSIGNED_BYTE, 4, MOAIVertexFormat::ATTRIBUTE_COLOR, true );
					break;
				
				case MOAIVertexFormatPresetEnum::XYZWUVC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWUVC_POSITION ), MOAIGfxTypeEnum::FLOAT, 4, MOAIVertexFormat::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWUVC_TEXCOORD ), MOAIGfxTypeEnum::FLOAT, 2, MOAIVertexFormat::ATTRIBUTE_TEX_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWUVC_COLOR ), MOAIGfxTypeEnum::UNSIGNED_BYTE, 4, MOAIVertexFormat::ATTRIBUTE_COLOR, true );
					break;
				
				case MOAIVertexFormatPresetEnum::XYZWNNNC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNC_POSITION ), MOAIGfxTypeEnum::FLOAT, 4, MOAIVertexFormat::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNC_NORMAL ), MOAIGfxTypeEnum::FLOAT, 3, MOAIVertexFormat::ATTRIBUTE_NORMAL, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNC_COLOR ), MOAIGfxTypeEnum::UNSIGNED_BYTE, 4, MOAIVertexFormat::ATTRIBUTE_COLOR, true );
					break;
				
				case MOAIVertexFormatPresetEnum::XYZWNNNUVC:
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNUVC_POSITION ), MOAIGfxTypeEnum::FLOAT, 4, MOAIVertexFormat::ATTRIBUTE_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNUVC_NORMAL ), MOAIGfxTypeEnum::FLOAT, 3, MOAIVertexFormat::ATTRIBUTE_NORMAL, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNUVC_TEXCOORD ), MOAIGfxTypeEnum::FLOAT, 2, MOAIVertexFormat::ATTRIBUTE_TEX_COORD, false );
					format->DeclareAttribute ( ZLIndexCast ( XYZWNNNUVC_COLOR ), MOAIGfxTypeEnum::UNSIGNED_BYTE, 4, MOAIVertexFormat::ATTRIBUTE_COLOR, true );
					break;
			}
			
			this->mFormats [ formatID ] = format;
		}
	}
	return format;
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIVertexFormatMgr::GetFormat ( lua_State* L, int idx ) {

	MOAILuaState state ( L );
	MOAIVertexFormat* format = NULL;

	if ( state.IsType ( idx, LUA_TNUMBER )) {
		format = this->GetFormat (( MOAIVertexFormatPresetEnum )state.GetValue < u32 >( idx, ( u32 )MOAIVertexFormatPresetEnum::UNKNOWN_FORMAT ));
	}
	else {
		format = state.GetLuaObject < MOAIVertexFormat >( idx, true );
	}
	return format;
}

//----------------------------------------------------------------//
u32 MOAIVertexFormatMgr::GetVertexSize ( MOAIVertexFormatPresetEnum formatID ) {

	const MOAIVertexFormat* format = this->GetFormat ( formatID );
	return format ? format->GetVertexSize () : 0;
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::MOAIVertexFormatMgr () {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	for ( u32 i = 0; i < MOAIVertexFormatPresetEnum::TOTAL_FORMATS; ++i ) {
		this->mFormats [ i ] = 0;
	}
}

//----------------------------------------------------------------//
MOAIVertexFormatMgr::~MOAIVertexFormatMgr () {

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
void MOAIVertexFormatMgr::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

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
void MOAIVertexFormatMgr::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );
}
