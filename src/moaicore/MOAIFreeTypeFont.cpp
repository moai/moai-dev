// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com



#include "pch.h"
#include <contrib/utf8.h>
#include <moaicore/MOAIFreeTypeFont.h>

//================================================================//
// local
//================================================================//
int	MOAIFreeTypeFont::_load(lua_State *L){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "US" );
	
	return 0;
}

int	MOAIFreeTypeFont::_setReader	( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	
	return 0;
}

//================================================================//
// MOAIFreeTypeFont
//================================================================//

void MOAIFreeTypeFont::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED(state);
}

void MOAIFreeTypeFont::RegisterLuaFuncs(MOAILuaState &state){
	UNUSED(state);
}
