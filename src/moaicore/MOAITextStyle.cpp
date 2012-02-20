// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAINodeMgr.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAITextStyle.h>

//================================================================//
// MOAITextStyleState
//================================================================//
	
//----------------------------------------------------------------//
bool MOAITextStyleState::IsMatch ( const MOAITextStyleState& compare ) const {

	if ( this->mFont != compare.mFont ) return false;
	if ( this->mColor != compare.mColor ) return false;
	if ( this->mSize != compare.mSize ) return false;
	
	return true;
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITextStyle::_setFont ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "U" )
	MOAIFont* font = state.GetLuaObject < MOAIFont >( 2 );
	self->SetFont ( font );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITextStyle::_setSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "UN" )
	
	float points	= state.GetValue < float >( 2, 0.0f );
	float dpi		= state.GetValue < float >( 3, DPI );
	
	self->mSize = POINTS_TO_PIXELS ( points, dpi );
	return 0;
}

//================================================================//
// MOAITextStyle
//================================================================//

//----------------------------------------------------------------//
MOAITextStyle::MOAITextStyle () {
	
	this->mFont = 0;
	this->mSize = 0.0f;
	this->mColor = 0xffffffff;
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextStyle::~MOAITextStyle () {

	this->SetFont ( 0 );
}

//----------------------------------------------------------------//
void MOAITextStyle::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITextStyle::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
	
	luaL_Reg regTable [] = {
		{ "setFont",				_setFont },
		{ "setSize",				_setSize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITextStyle::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAITextStyle::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAITextStyle::SetFont ( MOAIFont* font ) {

	if ( this->mFont != font ) {
	
		if ( font ) {
			this->LuaRetain ( *font );
		}
		
		if ( this->mFont ) {
			this->LuaRelease ( *this->mFont );
		}
		
		this->mFont = font;
	}
}
