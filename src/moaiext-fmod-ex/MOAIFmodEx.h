// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODEX_H
#define	MOAIFMODEX_H

#include <moaicore/moaicore.h>

namespace FMOD {
	class System;
	class Sound;
	class Channel;
	class ChannelGroup;
};

//================================================================//
// MOAIFmodEx
//================================================================//
/**	@name	MOAIFmodEx
	@text	FMOD singleton.
*/
class MOAIFmodEx :
	public MOAIGlobalClass < MOAIFmodEx, MOAILuaObject > {
private:

	FMOD::System* mSoundSys;
	FMOD::ChannelGroup* mMainChannelGroup;

	//----------------------------------------------------------------//
	static int	_getMemoryStats		( lua_State* L );
	static int	_init				( lua_State* L );
	static int _mute				( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIFmodEx )

	GET ( FMOD::System*, SoundSys, mSoundSys );
	GET ( FMOD::ChannelGroup*, MainChannelGroup, mMainChannelGroup);

	//----------------------------------------------------------------//
	void			CloseSoundSystem	();
					MOAIFmodEx			();
					~MOAIFmodEx			();
	void			MuteChannels		( bool mute );
	void			OpenSoundSystem		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			Update				();
	STLString		ToString			();
};

#endif