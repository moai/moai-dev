// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODEXCHANNEL_H
#define	MOAIFMODEXCHANNEL_H

#include <moaicore/moaicore.h>

class MOAIFmodExSound;

//================================================================//
// MOAIFmodExChannel
//================================================================//
/**	@name	MOAIFmodExChannel
	@text	FMOD singleton.
*/
class MOAIFmodExChannel :
	public virtual MOAINode {
private:

	MOAISharedPtr < MOAIFmodExSound > mSound;
	FMOD::Channel* mChannel;
	
	typedef enum {
		STOPPED = 0,
		PLAYING,
		PAUSED,
	} PlayState;

	float	mVolume;
	bool	mPaused;
	bool	mLooping;
	PlayState mPlayState;

	//----------------------------------------------------------------//'
	static int	_getVolume			( lua_State* L );
	static int  _isPlaying			( lua_State* L );
	static int	_moveVolume			( lua_State* L );
	static int	_play				( lua_State* L );
	static int	_seekVolume			( lua_State* L );
	static int	_setPaused			( lua_State* L );
	static int	_setVolume			( lua_State* L );
	static int  _setLooping			( lua_State* L );
	static int	_stop				( lua_State* L );

public:

	friend class MOAIFmodEx;

	DECL_LUA_FACTORY ( MOAIFmodExChannel )
	DECL_ATTR_HELPER ( MOAIFmodExChannel )

	enum {
		ATTR_VOLUME,
		TOTAL_ATTR,
	};

	//----------------------------------------------------------------//
	bool		ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	float		GetVolume			();
				MOAIFmodExChannel	();
				~MOAIFmodExChannel	();
	void		Play				( MOAIFmodExSound* sound, int loopCount );
	void		RegisterLuaClass	( MOAILuaState& state );
	void		RegisterLuaFuncs	( MOAILuaState& state );
	void		SetPaused			( bool paused );
	void		SetVolume			( float volume );
	void		Stop				();
	STLString	ToString			();
};

#endif
