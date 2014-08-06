// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAINodeMgr.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextStyle.h>

//================================================================//
// MOAITextStyleState
//================================================================//

//----------------------------------------------------------------//
MOAITextStyleState::MOAITextStyleState () :
	mFont ( 0 ),
	mSize ( 0.0f ),
	mScale ( 1.0f, 1.0f ),
	mColor ( 0xffffffff ) {
	
	this->mPadding.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
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
/**	@lua	getColor
	@text	Gets the color of the style.
	
	@in		MOAITextStyle self
	@out	number r
	@out	number g
	@out	number b
	@out	number a
*/
int MOAITextStyle::_getColor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "U" )

	ZLColorVec color = ZLColor::Set ( self->mColor );
	
	lua_pushnumber ( state, color.mR );
	lua_pushnumber ( state, color.mG );
	lua_pushnumber ( state, color.mB );
	lua_pushnumber ( state, color.mA );

	return 4;
}

//----------------------------------------------------------------//
/**	@lua	getFont
	@text	Gets the font of the style.
	
	@in		MOAITextStyle self
	@out	MOAIFont font
*/
int MOAITextStyle::_getFont ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "U" )

	MOAIFont* font = self->GetFont ();
	if ( font ) {
		font->PushLuaUserdata ( state );
		return 1;
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getScale
	@text	Gets the scale of the style.
	
	@in		MOAITextStyle self
	@out	number scale
*/
int MOAITextStyle::_getScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "U" )
	state.Push ( self->mScale.mX );
	state.Push ( self->mScale.mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getSize
	@text	Gets the size of the style.
	
	@in		MOAITextStyle self
	@out	number size
*/
int MOAITextStyle::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "U" )
	lua_pushnumber ( state, self->mSize );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setColor
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
/**	@lua	setFont
	@text	Sets or clears the style's font.
	
	@in		MOAITextStyle self
	@opt	MOAIFont font		Default value is nil.
	@out	nil
*/
int MOAITextStyle::_setFont ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "U" )
	MOAIFont* font = state.GetLuaObject < MOAIFont >( 2, true );
	
	self->SetFont ( font );
	self->ScheduleUpdate ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAITextStyle::_setPadding ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "U" )
	
	// TODO: pasted from MOAIDynamicGlyphCache; find some central place for this
	
	if ( state.CheckParams ( 2, "NNNN", false )) {
	
		self->mPadding.mXMin = state.GetValue < float >( 2, 0.0f );
		self->mPadding.mYMin = state.GetValue < float >( 3, 0.0f );
		
		self->mPadding.mXMax = state.GetValue < float >( 4, 0.0f );
		self->mPadding.mYMax = state.GetValue < float >( 5, 0.0f );
	}
	else {
	
		float hPad = state.GetValue < float >( 2, 0.0f );
		float vPad = state.GetValue < float >( 3, hPad );
	
		hPad *= 0.5f;
		vPad *= 0.5f;
		
		self->mPadding.mXMin = -hPad;
		self->mPadding.mYMin = -vPad;
		
		self->mPadding.mXMax = hPad;
		self->mPadding.mYMax = vPad;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setScale
	@text	Sets the scale of the style. The scale is applied to
			any glyphs drawn using the style after the glyph set
			has been selected by size.
	
	@in		MOAITextStyle self
	@opt	number scale		Default value is 1.
	@out	nil
*/
int MOAITextStyle::_setScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITextStyle, "U" )
	self->mScale.mX = state.GetValue < float >( 2, 1.0f );
	self->mScale.mY = state.GetValue < float >( 3, self->mScale.mX );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSize
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
		{ "getColor",				_getColor },
		{ "getFont",				_getFont },
		{ "getScale",				_getScale },
		{ "getSize",				_getSize },
		{ "setColor",				_setColor },
		{ "setFont",				_setFont },
		{ "setPadding",				_setPadding },
		{ "setScale",				_setScale },
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
		
		if ( font && ( this->mSize == 0.0f )) {
			this->mSize = font->GetDefaultSize ();
		}
	}
}

//----------------------------------------------------------------//
void MOAITextStyle::SetSize ( float size ) {

	if ( this->mSize != size ) {
		this->mSize = size;
	}
}