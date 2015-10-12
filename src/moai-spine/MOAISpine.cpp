// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-spine/MOAISpine.h>

//================================================================//
// lua
//================================================================//


//----------------------------------------------------------------//
/**	@name	setCreateTexture
 @text	Create texture for spine atlas page.
 
		signature: MOAITexture createTexture ( string path, min_filter, mag_filter )
 
 @out	nil
 */
int MOAISpine::_setCreateTexture ( lua_State* L ) {
	
	MOAILuaState state ( L );
	if ( state.IsType ( 1, LUA_TFUNCTION )) {
		MOAISpine::Get ().GetCreateTextureRef().SetRef ( state, 1 );
	}
	
	return 0;
}


//----------------------------------------------------------------//
/**	@name	setReadFile
	@text	When spine needs to load a file it will call this function
			to get the actual filename. Can be used to switch assets by
			resolution/environment.
			
			signature: string readFile ( string path )
 
	@out	nil
*/
int MOAISpine::_setReadFile ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( state.IsType ( 1, LUA_TFUNCTION )) {
		MOAISpine::Get ().GetReadFileRef().SetRef ( state, 1 );
	}
		
	return 0;
}

//================================================================//
// MOAISpine
//================================================================//

//----------------------------------------------------------------//
MOAISpine::MOAISpine () {
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
		
	RTTI_END
}

//----------------------------------------------------------------//
MOAISpine::~MOAISpine () {
}

//----------------------------------------------------------------//
void MOAISpine::RegisterLuaClass ( MOAILuaState& state ) {

	// here are the class methods:
	luaL_Reg regTable [] = {
		{ "setCreateTexture",		_setCreateTexture },
		{ "setReadFile",			_setReadFile },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
