#ifndef	MOAIAUDIOSAMPLER_H
#define	MOAIAUDIOSAMPLER_H

#include <AudioToolbox/AudioToolbox.h>
#include <AudioToolbox/AudioServices.h>

#include <moaicore/moaicore.h>

//================================================================//
// MOAIAudioSampler
//================================================================//
/**	@name	MOAIAudioSampler
	@text	Audio sampler singleton
*/
class MOAIAudioSampler :
	public virtual MOAINode {

private:

	//----------------------------------------------------------------//
	static int		_setFrequency			( lua_State* L );
    static int		_setNumChannels			( lua_State* L );
    static int		_prepareBuffer			( lua_State* L );
    static int		_start					( lua_State* L );
    static int		_read					( lua_State* L );
    static int		_stop					( lua_State* L );
    static int		_pause					( lua_State* L );
    static int		_resume					( lua_State* L );        

    u32 mNumFrequency;
    u32 mNumChannels;
    size_t mMaxBufferSizeInBytes;
    size_t mBufferAryLen;
    short **mBufferAry;
    size_t *mBufferReadSizeInBytes;

    AudioStreamBasicDescription recFmt;
    AudioQueueRef queue;
    u32 currentWriteIndex;
    u32 currentReadIndex;

    bool isActive;
    bool isQueueInitialized;

    static void globalCallback( void *inUserData,
                                AudioQueueRef inAQ,
                                AudioQueueBufferRef inBuffer,
                                const AudioTimeStamp *inStartTime,
                                UInt32 inNumPackets,
                                const AudioStreamPacketDescription *inPacketDesc );
public:

	DECL_LUA_FACTORY ( MOAIAudioSampler )
        //	DECL_ATTR_HELPER ( MOAIUntzSound )
        //	enum {
        //		ATTR_VOLUME,
        //		TOTAL_ATTR,
        //	};



	//----------------------------------------------------------------//
					MOAIAudioSampler	();
					~MOAIAudioSampler	();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );

    
};

#endif
