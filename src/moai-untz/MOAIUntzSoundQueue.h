// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNTZSOUNDQUEUE_H
#define	MOAIUNTZSOUNDQUEUE_H

#include <moai-core/host.h>
#include <moai-untz/MOAIUntzSoundQueue.h>
#include <moai-untz/MOAIUntzSound.h>
#include <UntzSound.h>
#include <vector>

//================================================================//
// MOAIUntzSoundQueue
//================================================================//
/**	@name	MOAIUntzSoundQueue
	@text	Untz sound queue object.
	
	@attr	ATTR_VOLUME
*/
class MOAIUntzSoundQueue :
	public virtual MOAINode {
private:

	UNTZ::Sound* mSound;
	std::vector<MOAIUntzSound*> mQueue;
	MOAIUntzSound* mCurrent;
	float mVolume;
		
	void  SetVolume(float volume);
	float GetVolume() const;
		
	//----------------------------------------------------------------//
	static int		_getVolume			( lua_State* L );
	static int		_moveVolume			( lua_State* L );
	static int		_seekVolume			( lua_State* L );
	static int		_setVolume			( lua_State* L );
	static int		_addSound			( lua_State* L );
	static int		_isPaused			( lua_State* L );
	static int		_isPlaying			( lua_State* L );
	static int		_pause				( lua_State* L );
	static int		_play				( lua_State* L );
	static int		_stop				( lua_State* L );

public:
	DECL_LUA_FACTORY ( MOAIUntzSoundQueue )
	DECL_ATTR_HELPER ( MOAIUntzSoundQueue )

	enum {
		ATTR_VOLUME,
		TOTAL_ATTR,
	};

	static Int64 StreamCallback(float* buffers, UInt32 numChannels, UInt32 numFrames, void* userdata);

	//----------------------------------------------------------------//
	bool			ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
					MOAIUntzSoundQueue	();
					~MOAIUntzSoundQueue	();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );		
};

#endif
