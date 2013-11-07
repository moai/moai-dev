//
//  MOAITextRenderer.cpp
//  libmoai
//
//  Created by Isaac Barrett on 11/5/13.
//
//

#include "pch.h"
#include <moaicore/MOAITextRenderer.h>
#include <moaicore/MOAIFreeTypeFont.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAITextBox.h>

//================================================================//
// local
//================================================================//
//----------------------------------------------------------------//
/** @name	render
	@text	Renders the string with all current settings and returns the texture.
 
	@in		MOAITextRenderer	self
	@in		string				text
	@out	MOAITexture			texture
	@out	table				glyphTable
 
 */
int	MOAITextRenderer::_render ( lua_State *L ){
	MOAI_LUA_SETUP ( MOAITextRenderer, "US" );
	
	if (!self->mFont) {
		return 0;
	}
	
	cc8* text = state.GetValue < cc8* > (2, "");
	MOAITexture *texture = self->mFont->RenderTexture(text, self->mFontSize, self->mWidth,
													  self->mHeight, self->mHorizontalAlignment,
													  self->mVerticalAlignment, self->mWordBreak,
													  false, self->mReturnGlyphBounds, state);
	
	state.Push( texture );
	if (self->mReturnGlyphBounds) {
		state.MoveToTop(-2);
		return 2;
	}
	
	return 1;
}

//----------------------------------------------------------------//
/** @name	renderSingleLine
	@text	Renders the string with the current font and font size on a single line and returns the texture.
 
	@in		MOAITextRenderer	self
	@in		string				text
	@out	MOAITexture			texture
	@out	number				width
	@out	number				height
	@out	table				glyphTable
 */
int MOAITextRenderer::_renderSingleLine ( lua_State *L ){
	MOAI_LUA_SETUP ( MOAITextRenderer, "US" );
	
	if (!self->mFont) {
		return 0;
	}
	
	cc8* text = state.GetValue < cc8* > (2, "");
	
	USRect rect;
	MOAITexture *texture = self->mFont->RenderTextureSingleLine(text, self->mFontSize, &rect, self->mReturnGlyphBounds, state);
	state.Push(texture);
	state.Push(rect.Width());
	state.Push(rect.Height());
	if (self->mReturnGlyphBounds) {
		// return the glyph bound table after the height information
		state.MoveToTop(-4);
		return 4;
	}
	return 3;
}

//----------------------------------------------------------------//
/** @name	setAlignment
	@text	Set the horizontal and vertical alignment of the text to render.
 
	@in		MOAITextRenderer	self
	@opt	number	horizontalAlignment		default to MOAITextBox.LEFT_JUSTIFY
	@opt	number	verticalAlignment		default to MOAITextBox.LEFT_JUSTIFY
	@out	nil
 
 */
int MOAITextRenderer::_setAlignment ( lua_State *L ){
	MOAI_LUA_SETUP ( MOAITextRenderer, "U" );
	
	int horizontalAlignment = state.GetValue < int > ( 2, MOAITextBox::LEFT_JUSTIFY );
	int verticalAlignment = state.GetValue <int > ( 3, MOAITextBox::LEFT_JUSTIFY );
	
	self->mHorizontalAlignment = horizontalAlignment;
	self->mVerticalAlignment = verticalAlignment;
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	setDimensions
	@text	Set the dimensions of the text box to render.
 
 	@in		MOAITextRenderer	self
	@in		number	width
	@in		number	height
	@out	nil
	
 */
int MOAITextRenderer::_setDimensions( lua_State *L ){
	
	MOAI_LUA_SETUP ( MOAITextRenderer, "UNN" );
	
	float width = state.GetValue < float > ( 2, 0.0f );
	float height = state.GetValue < float > ( 3, 0.0f );
	
	self->mWidth = width;
	self->mHeight = height;
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	setFont
	@text	Set the font to use when rendering.
 
	@in		MOAITextRenderer self
	@in		MOAIFreeTypeFont font
	@out	nil
 
 */
int MOAITextRenderer::_setFont ( lua_State *L ){
	MOAI_LUA_SETUP ( MOAITextRenderer, "U" );
	self->mFont.Set( *self, state.GetLuaObject < MOAIFreeTypeFont >( 2, true ));
	return 0;
}

//----------------------------------------------------------------//
/** @name	setFontSize
	@text	Set the size of the font to use when rendering.
 
	@in		MOAITextRenderer self
	@in		number fontSize
	@out	nil
 */
int MOAITextRenderer::_setFontSize ( lua_State *L ){
	MOAI_LUA_SETUP ( MOAITextRenderer, "UN" );
	self->mFontSize = state.GetValue < float > ( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setHeight
	@text	Set the height of the text box to render.
 
	@in		MOAITextRenderer self
	@in		number height
	@out	nil
 
 */

int MOAITextRenderer::_setHeight ( lua_State *L ){
	MOAI_LUA_SETUP ( MOAITextRenderer, "UN" );
	self->mHeight = state.GetValue < float > ( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setReturnGlyphBounds
	@text	Set the flag to return an additional table containing 
			information about each glyph.
 
	@in		MOAITextRenderer self
	@opt	bool returnGlyphBounds	default true
	@out	nil
 
 */
int MOAITextRenderer::_setReturnGlyphBounds ( lua_State *L ){
	MOAI_LUA_SETUP ( MOAITextRenderer, "U" );
	self->mReturnGlyphBounds = state.GetValue < bool > ( 2, true );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setWidth
	@text	Set the width of the text box to render.
 
	@in		MOAITextRenderer self
	@in		number width
	@out	nil
 
 */
int MOAITextRenderer::_setWidth ( lua_State *L ){
	MOAI_LUA_SETUP ( MOAITextRenderer, "UN" );
	self->mWidth = state.GetValue < float > ( 2, 0.0f );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setWordBreak
	@text	Set the word break mode of the text to render.
 
	@in		MOAITextRenderer self
	@in		number height
	@out	nil
 
 */
int MOAITextRenderer::_setWordBreak ( lua_State *L ){
	MOAI_LUA_SETUP ( MOAITextRenderer, "U" );
	self->mWordBreak = state.GetValue < int > ( 2, MOAITextBox::WORD_BREAK_NONE );
	return 0;
}

//----------------------------------------------------------------//

MOAITextRenderer::MOAITextRenderer ( ):
	mFontSize(0.0f),
	mWidth(0.0f),
	mHeight(0.0f),
	mHorizontalAlignment(MOAITextBox::LEFT_JUSTIFY),
	mVerticalAlignment(MOAITextBox::LEFT_JUSTIFY),
	mWordBreak(MOAITextBox::WORD_BREAK_NONE),
	mReturnGlyphBounds(false)
{
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
	
}

//----------------------------------------------------------------//
MOAITextRenderer::~MOAITextRenderer () {
	
}

//----------------------------------------------------------------//
void MOAITextRenderer::RegisterLuaClass ( MOAILuaState &state ) {
	UNUSED( state );
}

//----------------------------------------------------------------//
void MOAITextRenderer::RegisterLuaFuncs ( MOAILuaState &state ) {
	luaL_Reg regTable [] = {
		{ "render",					_render },
		{ "renderSingleLine",		_renderSingleLine },
		{ "setAlignment",			_setAlignment },
		{ "setDimensions",			_setDimensions },
		{ "setFont",				_setFont },
		{ "setFontSize",			_setFontSize },
		{ "setHeight",				_setHeight },
		{ "setReturnGlyphBounds",	_setReturnGlyphBounds  },
		{ "setWidth",				_setWidth },
		{ "setWordBreak",			_setWordBreak },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

