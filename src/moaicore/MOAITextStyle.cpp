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
void MOAITextStyleState::AffirmGlyph ( u32 c ) {

	assert ( this->mFont );
	this->mFont->AffirmGlyph ( this->mSize, c );
}

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
/**	@name	setColor
	@text	Initialize the style's color.
	
	@in		MOAITextStyle self
	@in		number r	Default value is 0.
	@in		number g	Default value is 0.
	@in		number b	Default value is 0.
	@opt	number a	Default value is 1.
	@out	nil
*/
int MOAITextStyle::_setColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "UNNN" )
	self->mColor = state.GetColor32 ( 2, 0.0f, 0.0f, 0.0f, 1.0f );
	return 0;
}

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
		{ "setColor",				_setColor },
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
	
		this->LuaRetain ( font );
		this->LuaRelease ( this->mFont );
		this->mFont = font;
	}
}
