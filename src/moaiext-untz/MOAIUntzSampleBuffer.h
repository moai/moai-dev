// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNTZSAMPLEBUFFER_H
#define	MOAIUNTZSAMPLEBUFFER_H

#include <moaicore/moaicore.h>
#include <UntzSound.h>

//================================================================//
// MOAIUntzSampleBuffer
//================================================================//
/**	@name	MOAIUntzSampleBuffer
	@text	Uncompressed WAV data held in memory. May be shared between
			multiple MOAIUntzSound objects.
*/
class MOAIUntzSampleBuffer :
	public virtual USLuaObject {
private:
	float* mBuffer;
	UNTZ::SoundInfo mInfo;

	//----------------------------------------------------------------//
	static int		_load						( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIUntzSampleBuffer )

	//----------------------------------------------------------------//
					MOAIUntzSampleBuffer		();
					~MOAIUntzSampleBuffer		();
	void			RegisterLuaClass			( USLuaState& state );
	void			RegisterLuaFuncs			( USLuaState& state );		

	UNTZ::SoundInfo GetSoundInfo() { return mInfo; }
	float* GetSampleBuffer() { return mBuffer; } 
};

#endif
