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
	public virtual MOAINode {

private:
    void pause();
    void resume();

	//----------------------------------------------------------------//
	static int		_setFrequency			( lua_State* L );
    static int		_setNumChannels			( lua_State* L );
    static int		_prepareBuffer			( lua_State* L );
    static int		_start					( lua_State* L );
    static int		_read					( lua_State* L );
    static int		_stop					( lua_State* L );
    static int		_pause					( lua_State* L );
    static int		_resume					( lua_State* L );        
    static int		_getLevels				( lua_State* L );

    u32 mNumFrequency;
    u32 mNumChannels;
    size_t mMaxBufferSizeInBytes;
    size_t mBufferAryLen;
    short **mBufferAry;
    size_t *mBufferReadSizeInBytes;

    AudioStreamBasicDescription recFmt;
    AudioQueueRef queue;
    AudioQueueBufferRef *buffers;
    AudioQueueLevelMeterState *levels;
    u32 currentWriteIndex;
    u32 currentReadIndex;

    bool isActive;
    bool isQueueInitialized;
    bool isQueuePrimed;
    void* mSessionActiveObserver;
    void* mSessionInactiveObserver;

    static void globalCallback( void *inUserData,
                                AudioQueueRef inAQ,
                                AudioQueueBufferRef inBuffer,
                                const AudioTimeStamp *inStartTime,
                                UInt32 inNumPackets,
                                const AudioStreamPacketDescription *inPacketDesc );
public:

	DECL_LUA_FACTORY ( MOAIAudioSamplerCocoa )
        //	DECL_ATTR_HELPER ( MOAIUntzSound )
        //	enum {
        //		ATTR_VOLUME,
        //		TOTAL_ATTR,
        //	};



	//----------------------------------------------------------------//
					MOAIAudioSamplerCocoa	();
					~MOAIAudioSamplerCocoa	();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );

    
};

#endif
