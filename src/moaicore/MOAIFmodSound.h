// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODSOUND_H
#define	MOAIFMODSOUND_H
#if USE_FMOD

#include <moaicore/MOAIFmod.h>

class MOAIData;

//================================================================//
// MOAIFmodSound
//================================================================//
/**	@brief FMOD sound.
*/
class MOAIFmodSound :
	public virtual USLuaData {
private:

	FMOD::Sound* mSound;
	int mLoopCount;

	//----------------------------------------------------------------//
	static int	_load				( lua_State* L );
	static int	_loadBGM			( lua_State* L );
	static int	_loadSFX			( lua_State* L );
	static int	_release			( lua_State* L );

public:

	friend class MOAIFmodChannel;
	friend class MOAIFmod;

	DECL_LUA_DATA ( MOAIFmodSound )

	//----------------------------------------------------------------//
				MOAIFmodSound			();
				~MOAIFmodSound			();
	void		Load				( MOAIData& data, bool streaming );
	void		Load				( cc8* filename, bool streaming, bool async );
	void		RegisterLuaClass	( USLuaState& state );
	void		RegisterLuaFuncs	( USLuaState& state );
	void		Release				();			
	STLString	ToString			();
};

#endif
#endif
