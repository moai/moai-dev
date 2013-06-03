// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include "MOAIFreeTypeFont.h"
#include "MOAIFreeTypeTextBox.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


int MOAIFreeTypeTextBox::_getAutoFit( lua_State *L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "U" )
	state.Push ( self->mAutoFit );
	return 1;
}

int MOAIFreeTypeTextBox::_getGlyphScale( lua_State *L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "U" )
	state.Push ( self->mGlyphScale );
	return 1;
}

int MOAIFreeTypeTextBox::_getRect( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "U" )
	
	USRect rect = self->mFrame;
	rect.Bless ();
	
	lua_pushnumber ( state, rect.mXMin );
	lua_pushnumber ( state, rect.mYMin );
	lua_pushnumber ( state, rect.mXMax );
	lua_pushnumber ( state, rect.mYMax );
	
	return 4;
}


int	MOAIFreeTypeTextBox::_setAutoFit( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "UB" )
	self->mAutoFit = state.GetValue < bool >( 2, false );
	return 0;
}

/**	@name	setFont
	@text	Sets or clears the style's font.
 
	@in		MOAIFreeTypeTextBox self
	@opt	MOAIFreeTypeFont font		Default value is nil.
	@out	nil
 */

int	MOAIFreeTypeTextBox::_setFont( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "U" )
	MOAIFreeTypeFont *font = state.GetLuaObject< MOAIFreeTypeFont >(2, true);
	self->SetFont(font);
	return 0;
}

int	MOAIFreeTypeTextBox::_setGlyphScale( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "UN" )
	self->mGlyphScale = state.GetValue < float >( 2, 1.0f );
	return 0;
}


int	MOAIFreeTypeTextBox::_setRect( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeTextBox, "UNNNN" )
	
	float left		= state.GetValue < float >( 2, 0.0f );
	float top		= state.GetValue < float >( 3, 0.0f );
	float right		= state.GetValue < float >( 4, 0.0f );
	float bottom	= state.GetValue < float >( 5, 0.0f );
	
	// TODO: implement ScheduleLayout ();
	self->SetRect ( left, top, right, bottom );
	//self->ScheduleLayout ();
	return 0;
}

void MOAIFreeTypeTextBox::BuildLayout(){
	// retrieve the glyph slot.
	FT_GlyphSlot  slot = NULL; //face->glyph;
	int	pen_x, pen_y;
	int n;
	int num_chars = 1; // strlen(this-mText);
	
	FT_Error error;
	
	// initialize library
	
	// create face object
	FT_Face face = NULL;
	// set character size
	
	
	// initialize pen position
	pen_x = 300;
	pen_y = 200;
	
	for (n = 0; n < num_chars; ) {
		FT_UInt glyph_index;
		
		// retrieve the next Unicode character value and update the integer n
		u32 c = u8_nextchar(this->mText, &n);
		
		// retrieve glyph index from character code
		glyph_index = FT_Get_Char_Index(face, c);
		
		// load glyph index into slot
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
			continue;
		
		// convert to anti-aliased bitmap
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error) {
			continue;
		}
		
		// draw to target surface
		/*
		my_draw_bitmap ( &slot->bitmap,
						pen_x + slot->bitmap_left,
						pen_y - slot->bitmap_top);
		*/
		
		// increment pen position; 
		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6;
		
	}
}

void MOAIFreeTypeTextBox::Draw(int subPrimID){
	UNUSED(subPrimID);
	
	if ( !( this->mFlags & FLAGS_VISIBLE )) return;
	// TODO: implement Draw()
	
}


void MOAIFreeTypeTextBox::Layout(){
	// TODO: implement Layout()
	
	// text is NULL
	if ( !this->mText ){
		// reset layout
		
	}
	else if (this->mNeedsLayout) {
		
		// build style map if necessary
		
		// reset layout
		
		
		// build layout (Done with MOAITextDesigner in MOAITextBox)
		this->BuildLayout();
		
		// apply highlights
		
	}
	
	this->mNeedsLayout = false;
}


MOAIFreeTypeTextBox::MOAIFreeTypeTextBox() :
mLineSpacing ( 0.0f ),
mText ( "" ),
mTextLength ( 0 ),
mHAlign ( LEFT_JUSTIFY ),
mVAlign ( LEFT_JUSTIFY ),
mYFlip ( false ),
mGlyphScale ( 1.0f ),
mWordBreak ( WORD_BREAK_NONE ),
mAutoFit( false ),
mFont( NULL ),
mFontSize(0.0f),
mNeedsLayout ( false ){
	RTTI_BEGIN
		RTTI_EXTEND( MOAIProp )
	RTTI_END
	
	this->mFrame.Init( 0.0f, 0.0f, 0.0f, 0.0f);
}


MOAIFreeTypeTextBox::~MOAIFreeTypeTextBox(){
	
}

void MOAIFreeTypeTextBox::OnDepNodeUpdate(){
	
	MOAIProp::OnDepNodeUpdate ();
	
	// implement style changed?
	
	this->Layout ();
	if ( this->mYFlip ) {
		
		USAffine3D mtx;
		
		mtx.ScRoTr ( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, ( this->mFrame.mYMin + this->mFrame.mYMax ), 0.0f );
		this->mLocalToWorldMtx.Prepend ( mtx );
		
		this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
	}
}


void MOAIFreeTypeTextBox::RegisterLuaClass( MOAILuaState &state ){
	MOAIProp::RegisterLuaClass ( state );
	
	state.SetField( -1, "LEFT_JUSTIFY", ( u32 )LEFT_JUSTIFY );
	state.SetField ( -1, "CENTER_JUSTIFY", ( u32 )CENTER_JUSTIFY );
	state.SetField ( -1, "RIGHT_JUSTIFY", ( u32 )RIGHT_JUSTIFY );
	
	state.SetField ( -1, "WORD_BREAK_NONE", ( u32 )WORD_BREAK_NONE );
	state.SetField ( -1, "WORD_BREAK_CHAR", ( u32 )WORD_BREAK_CHAR );
}

void MOAIFreeTypeTextBox::RegisterLuaFuncs( MOAILuaState &state ){
	
	luaL_Reg regTable [] = {
		{ "getAutoFit",				_getAutoFit },
		{ "getGlyphScale",			_getGlyphScale },
		{ "getRect",				_getRect },
		{ "setAutoFit",				_setAutoFit },
		{ "setFont",				_setFont },
		{ "setGlyphScale",			_setGlyphScale },
		{ "setRect",				_setRect },
		{ NULL, NULL }
	};
	
	luaL_register(state, 0, regTable );
}

void MOAIFreeTypeTextBox::ScheduleLayout(){
	//this->mNeedsLayout = true;
	this->ScheduleUpdate();
}

void MOAIFreeTypeTextBox::SetFont( MOAIFreeTypeFont* font ){
	if (this->mFont != font) {
		this->LuaRetain( font );
		this->LuaRelease( this->mFont );
		this->mFont = font;
	}
}


void MOAIFreeTypeTextBox::SetRect(float left, float top, float right, float bottom){
	this->mFrame.Init(left, top, right, bottom);
}
