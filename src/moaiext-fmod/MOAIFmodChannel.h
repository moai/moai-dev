// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODCHANNEL_H
#define	MOAIFMODCHANNEL_H

#include <moaicore/moaicore.h>

class MOAIFmodSound;

//================================================================//
// MOAIFmodChannel
//================================================================//
/**	@name	MOAIFmodChannel
	@text	FMOD singleton. Unsupported, legacy.
*/
class MOAIFmodChannel :
	public virtual MOAINode {
private:

	USRef < MOAIFmodSound > mSound;
	FMOD::Channel* mChannel;
	
	float	mVolume;
	bool	mPaused;

	//----------------------------------------------------------------//
	static int	_moveVolume			( lua_State* L );
	static int	_play				( lua_State* L );
	static int	_seekVolume			( lua_State* L );
	static int	_setPaused			( lua_State* L );
	static int	_setVolume			( lua_State* L );
	static int	_stop				( lua_State* L );

public:

	friend class MOAIFmod;

	DECL_LUA_FACTORY ( MOAIFmodChannel )
	DECL_ATTR_HELPER ( MOAIFmodChannel )

	enum {
		ATTR_VOLUME,
		TOTAL_ATTR,
	};

	//----------------------------------------------------------------//
	bool		ApplyAttrOp			( u32 attrID, USAttrOp& attrOp );
				MOAIFmodChannel		();
				~MOAIFmodChannel	();
	void		Play				( MOAIFmodSound* sound, int loopCount );
	void		RegisterLuaClass	( USLuaState& state );
	void		RegisterLuaFuncs	( USLuaState& state );
	void		SetPaused			( bool paused );
	void		SetVolume			( float volume );
	void		Stop				();
	STLString	ToString			();
};

#endif
