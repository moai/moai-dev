// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAUDIOSAMPLERCOCOA_H
#define	MOAIAUDIOSAMPLERCOCOA_H

#include <AudioToolbox/AudioToolbox.h>
#include <AudioToolbox/AudioServices.h>

#include <moai-core/headers.h>
#include <moai-sim/headers.h>

//================================================================//
// MOAIAudioSamplerCocoa
//================================================================//
/**	@name	MOAIAudioSamplerCocoa
	@text	Audio sampler singleton
*/
class MOAIAudioSamplerCocoa :
	public virtual MOAIInstanceEventSource {
private:
		
	AudioStreamBasicDescription				mStreamDescription;
    AudioQueueRef							mQueue;
    ZLLeanArray < AudioQueueBufferRef >		mBuffers;

    bool									mIsRunning;
	bool									mIsPaused;
	
    void*									mSessionActiveObserver;
    void*									mSessionInactiveObserver;

	MOAILuaSharedPtr < MOAIStream >			mStream;
	u32										mStreamFormat;
	size_t									mSampleSize;
	size_t									mSamplesWritten;

	//----------------------------------------------------------------//
	static int		_getLevels				( lua_State* L );
	static int		_init					( lua_State* L );
	static int		_pause					( lua_State* L );
	static int		_resume					( lua_State* L );
	static int		_setStream				( lua_State* L );
	static int		_start					( lua_State* L );
    static int		_stop					( lua_State* L );

	//----------------------------------------------------------------//
	void			HandleInput				( AudioQueueRef inAQ, AudioQueueBufferRef inBuffer, UInt32 inNumPackets );
	static void		InputCallback			( void *inUserData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer, const AudioTimeStamp *inStartTime, UInt32 inNumPackets, const AudioStreamPacketDescription *inPacketDesc );
	void			Pause					();
    void			Resume					();
		
public:

	DECL_LUA_FACTORY ( MOAIAudioSamplerCocoa )

	enum {
		EVENT_BUFFER,
	};

	//----------------------------------------------------------------//
	void			Clear						();
	void			GetLevels					( double& average, double& peak );
	void			Init						( u32 sampleRate, u32 channels, u32 sampleSize, u32 totalBuffers );
					MOAIAudioSamplerCocoa		();
					~MOAIAudioSamplerCocoa		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			Start						();
	void			Stop						();
};

#endif
