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

//================================================================//
// local
//================================================================//
//----------------------------------------------------------------//
int	MOAITextRenderer::_render ( lua_State *L ){
	return 0;
}

//----------------------------------------------------------------//
int MOAITextRenderer::_renderSingleLine ( lua_State *L ){
	return 0;
}

//----------------------------------------------------------------//
int MOAITextRenderer::_setAlignment ( lua_State *L ){
	return 0;
}

//----------------------------------------------------------------//
int MOAITextRenderer::_setDimensions( lua_State *L ){
	
}

//----------------------------------------------------------------//
int MOAITextRenderer::_setFont ( lua_State *L ){
	return 0;
}

//----------------------------------------------------------------//
int MOAITextRenderer::_setFontSize ( lua_State *L ){
	return 0;
}

//----------------------------------------------------------------//
int MOAITextRenderer::_setHeight ( lua_State *L ){
	return 0;
}

//----------------------------------------------------------------//
int MOAITextRenderer::_setReturnGlyphBounds ( lua_State *L ){
	return 0;
}

//----------------------------------------------------------------//
int MOAITextRenderer::_setWidth ( lua_State *L ){
	return 0;
}

//----------------------------------------------------------------//

int MOAITextRenderer::_setWordBreak ( lua_State *L ){
	return 0;
}

//----------------------------------------------------------------//

MOAITextRenderer::MOAITextRenderer ( ) {
	
}

//----------------------------------------------------------------//
MOAITextRenderer::~MOAITextRenderer () {
	
}

//----------------------------------------------------------------//
void MOAITextRenderer::RegisterLuaClass ( MOAILuaState &state ) {
	
}

//----------------------------------------------------------------//
void MOAITextRenderer::RegisterLuaFuncs ( MOAILuaState &state ) {
	luaL_Reg regTable [] = {
		{ "render",					_render },
		{ "renderSingleLine",		_renderSingleLine },
		{ "setAlignment",			_setAlignment },
		{ "setFont",				_setFont },
		{ "setFontSize",			_setFontSize },
		{ "setHeight",				_setHeight },
		{ NULL, NULL }
	};
	
	
}

