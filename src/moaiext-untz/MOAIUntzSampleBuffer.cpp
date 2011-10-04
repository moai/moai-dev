// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-untz/MOAIUntzSampleBuffer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	load
	@text	Loads a sound from disk.
	
	@in		MOAIUntzSampleBuffer self
	@in		string filename
	@out	nil
*/
int MOAIUntzSampleBuffer::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSampleBuffer, "US" )
	
	return 0;
}

//================================================================//
// MOAIUntzSampleBuffer
//================================================================//

//----------------------------------------------------------------//
MOAIUntzSampleBuffer::MOAIUntzSampleBuffer () {

	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIUntzSampleBuffer::~MOAIUntzSampleBuffer () {
}
//----------------------------------------------------------------//
void MOAIUntzSampleBuffer::RegisterLuaClass ( USLuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIUntzSampleBuffer::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "load",				_load },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
