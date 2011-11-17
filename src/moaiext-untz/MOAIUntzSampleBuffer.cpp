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
	
	cc8* filename = state.GetValue < cc8* >( 2, "" );

	if(!UNTZ::Sound::decode(filename, self->mInfo, (float**)&self->mBuffer))
		printf ( "error creating sample buffer\n" );

	return 0;
}

//================================================================//
// MOAIUntzSampleBuffer
//================================================================//

//----------------------------------------------------------------//
MOAIUntzSampleBuffer::MOAIUntzSampleBuffer () : mBuffer(0) {

	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIUntzSampleBuffer::~MOAIUntzSampleBuffer () {
	if(mBuffer)
		delete [] mBuffer;
}
//----------------------------------------------------------------//
void MOAIUntzSampleBuffer::RegisterLuaClass ( MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIUntzSampleBuffer::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "load",				_load },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
