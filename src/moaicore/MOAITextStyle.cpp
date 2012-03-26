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
MOAITextStyleState::MOAITextStyleState () :
	mFont ( 0 ),
	mSize ( 0.0f ),
	mColor ( 0xffffffff ) {
}

//----------------------------------------------------------------//
MOAITextStyleState::~MOAITextStyleState () {
}

//----------------------------------------------------------------//
bool MOAITextStyleState::NeedsLayout ( const MOAITextStyleState& compare ) const {

	if ( this->mFont != compare.mFont ) return true;
	if ( this->mSize != compare.mSize ) return true;
	
	return false;
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
/**	@name	setFont
	@text	Sets or clears the style's font.
	
	@in		MOAITextStyle self
	@opt	MOAIFont font		Default value is nil.
	@out	nil
*/
int MOAITextStyle::_setFont ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "U" )
	MOAIFont* font = state.GetLuaObject < MOAIFont >( 2 );
	self->SetFont ( font );
	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSize
	@text	Sets or clears the style's size.
	
	@in		MOAITextStyle self
	@in		number points			The point size to be used by the style.
	@opt	number dpi				The device DPI (dots per inch of device screen). Default value is 72 (points same as pixels).
	@out	nil
*/
int MOAITextStyle::_setSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "UN" )
	
	float points	= state.GetValue < float >( 2, 0.0f );
	float dpi		= state.GetValue < float >( 3, DPI );
	
	self->SetSize ( POINTS_TO_PIXELS ( points, dpi ));
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAITextStyle
//================================================================//

//----------------------------------------------------------------//
void MOAITextStyle::AffirmGlyph ( u32 c ) {

	assert ( this->mFont );
	this->mFont->AffirmGlyph ( this->mSize, c );
}

//----------------------------------------------------------------//
void MOAITextStyle::Init ( MOAITextStyle& style ) {

	this->SetFont ( style.mFont );
	this->mSize = style.mSize;
	this->mColor = style.mColor;
}

//----------------------------------------------------------------//
MOAITextStyle::MOAITextStyle () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
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

//----------------------------------------------------------------//
void MOAITextStyle::SetSize ( float size ) {

	if ( this->mSize != size ) {
		this->mSize = size;
	}
}