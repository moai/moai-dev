// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-untz/MOAIUntzSampleBuffer.h>

//extern "C" {
//	extern void __cdecl lua_pushnumber (lua_State *L, lua_Number n);
//}

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
/**	@name	getInfo
 @text	Returns attributes of sample buffer.
 
 @in		MOAIUntzSampleBuffer self
 @out		number bits per sample 
 @out		number num of channels (mono=1, stereo=2)
 @out		number num of total frames contained
 @out		number sample rate (44100, 22050, etc )
 @out		number seconds of sound length 
 */
int MOAIUntzSampleBuffer::_getInfo( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSampleBuffer, "U" );

	lua_pushnumber(L, self->mInfo.mBitsPerSample);
	lua_pushnumber(L, self->mInfo.mChannels);
	lua_pushnumber(L, self->mInfo.mTotalFrames);
	lua_pushnumber(L, self->mInfo.mSampleRate);
	lua_pushnumber(L, self->mInfo.mLength);
	return 5;			
}

/**	@name	getData
 @text	Retrieve every sample data in buffer
 
 @in		MOAIUntzSampleBuffer self
 @out		table array of sample data number ( -1 ~ 1 as sample level)
 */
int MOAIUntzSampleBuffer::_getData( lua_State* L ) {
	MOAI_LUA_SETUP( MOAIUntzSampleBuffer, "U" );
	u32 arynum = self->mInfo.mTotalFrames * self->mInfo.mChannels;
	lua_createtable(L, arynum, 0);
	for(u32 i=0;i<arynum;i++ ) {
		lua_pushnumber(L, self->mBuffer[i]);
		lua_rawseti(L, -2, i+1);
	}
	return 1;
}
/**	@name	setData
 @text	Write sample data into buffer
 
 @in		MOAIUntzSampleBuffer self
 @in		table array of sample data number ( -1 ~ 1 as sample level )
 @in		number index of sample buffer start copying from (1 for the first sample)
 @out		nil
 */

int MOAIUntzSampleBuffer::_setData( lua_State* L ) {
	MOAI_LUA_SETUP( MOAIUntzSampleBuffer, "UTN" );
	u32 startDataIndex = state.GetValue<u32>(3,1);
	int itr = state.PushTableItr(2);
	int idx = 0;
	u32 total = 0;
	int maxindex = self->mInfo.mChannels * self->mInfo.mTotalFrames;

	idx = startDataIndex - 1;
	
	for(; state.TableItrNext(itr) && idx< maxindex; ++idx ) {
		float val = state.GetValue<float>(-1,0);
		self->mBuffer[idx] = val;		
//		fprintf(stderr, "value %d: %f\n", idx, val );
	}
	return 0;
}
/** name setRawData
 @text Write raw sample data (array of 16bit short value) into buffer
 @in		MOAIUntzSampleBuffer self
 @in		string raw binary data that contains array of network byte ordered 16bit short value
 @in		number of bytes to read
 @in		number index of sample buffer start copying from (1 for the first sample)
 @out nil
 */
int MOAIUntzSampleBuffer::_setRawData( lua_State* L ) {
	MOAI_LUA_SETUP( MOAIUntzSampleBuffer, "USNN" );
	
	cc8* s = state.GetValue < cc8* >( 2, "" );
	short *buf = (short*) s;
	u32 l = state.GetValue<u32>(3, 0);
	u32 elemnum = l / 2;
	int maxindex = self->mInfo.mChannels * self->mInfo.mTotalFrames;
	u32 startDataIndex = state.GetValue<u32>(3,1);
	
	for(int idx=0; (idx+(int)startDataIndex) < maxindex && idx < (int)elemnum; ++idx ) {
		float val = buf[idx] / 32767.0f;
		self->mBuffer[idx] = val;
	}	
	return 0;
}
/**	@name	preparBuffer
 @text	Allocate internal memory for sample buffer
 
 @in		MOAIUntzSampleBuffer self
 @in		number num of channels (mono=1, stereo=2)
 @in		number num of total frames of sample
 @in		number sample rate in Hz (44100 or else)
 @out		nil
 */
int MOAIUntzSampleBuffer::_prepareBuffer( lua_State* L ) {
	MOAI_LUA_SETUP(MOAIUntzSampleBuffer, "UNNN" );
	
	u32 bitsPerSample = 32;
	u32 channels = state.GetValue<u32>(2,1);
	u32 totalFrames = state.GetValue<u32>(3,0);
	u32 sampleRate = state.GetValue<u32>(4,44100);
	double length = (double) totalFrames / sampleRate;
	size_t memSize = totalFrames * channels * 4;	
	
	// Allocate space and copy the buffer
	self->mBuffer = (float*)new char[memSize];
	memset( self->mBuffer, 0, memSize );

	self->mInfo.mBitsPerSample = bitsPerSample;
	self->mInfo.mChannels = channels;
	self->mInfo.mTotalFrames = totalFrames;
	self->mInfo.mSampleRate = sampleRate;
	self->mInfo.mLength = length;
	
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
		{ "getInfo",  _getInfo },
		{ "getData",  _getData },
		{ "setData",  _setData },
		{ "setRawData", _setRawData },
		{ "prepareBuffer", _prepareBuffer },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
