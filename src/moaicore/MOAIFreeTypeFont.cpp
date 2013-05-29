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
	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	self->Init ( filename );
	return 0;
}

int	MOAIFreeTypeFont::_setReader	( lua_State* L ){
	MOAI_LUA_SETUP ( MOAIFreeTypeFont, "U" );
	self->mReader.Set ( *self, state.GetLuaObject < MOAIFontReader >( 2, true ));
	return 0;
}

//================================================================//
// MOAIFreeTypeFont
//================================================================//


void MOAIFreeTypeFont::Init(cc8 *filename){
	if ( USFileSys::CheckFileExists ( filename, true )) {
		this->mFilename = USFileSys::GetAbsoluteFilePath ( filename );
	}
}

void MOAIFreeTypeFont::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED(state);
}

void MOAIFreeTypeFont::RegisterLuaFuncs(MOAILuaState &state){
	luaL_Reg regTable [] = {
		{ "load",						_load },
		{ "setReader",					_setReader },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
