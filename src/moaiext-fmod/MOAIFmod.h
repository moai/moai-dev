// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMOD_H
#define	MOAIFMOD_H

#include <moaicore/moaicore.h>

namespace FMOD {
	class System;
	class Sound;
	class Channel;
	class ChannelGroup;
};

//================================================================//
// MOAIFmod
//================================================================//
/**	@name	MOAIFmod
	@text	FMOD singleton. Unsupported, legacy.
*/
class MOAIFmod :
	public USGlobalClass < MOAIFmod, USLuaObject > {
private:

	FMOD::System* mSoundSys;
	FMOD::ChannelGroup* mMainChannelGroup;

	//----------------------------------------------------------------//
	static int	_init				( lua_State* L );
	static int	_getMemoryStats		( lua_State* L );
public:

	DECL_LUA_SINGLETON ( MOAIFmod )

	GET ( FMOD::System*, SoundSys, mSoundSys );
	GET ( FMOD::ChannelGroup*, MainChannelGroup, mMainChannelGroup);

	//----------------------------------------------------------------//
	void			CloseSoundSystem	();
					MOAIFmod			();
					~MOAIFmod			();
	void			MuteChannels		( bool mute );
	void			OpenSoundSystem		();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	void			Update				();
	STLString		ToString			();
};

#endif