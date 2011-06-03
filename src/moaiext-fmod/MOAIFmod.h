// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMOD_H
#define	MOAIFMOD_H

#include <moaicore/moaicore.h>

namespace FMOD {
	class System;
	class Sound;
	class Channel;
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

	//----------------------------------------------------------------//
	static int	_init				( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIFmod )

	GET ( FMOD::System*, SoundSys, mSoundSys );

	//----------------------------------------------------------------//
	void			CloseSoundSystem	();
					MOAIFmod			();
					~MOAIFmod			();
	void			OpenSoundSystem		();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	void			Update				();
	STLString		ToString			();
};

#endif