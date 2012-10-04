// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODEXSOUND_H
#define	MOAIFMODEXSOUND_H

#include <moaicore/moaicore.h>
#include <moaiext-fmod-ex/MOAIFmodEx.h>

class MOAIDataBuffer;

//================================================================//
// MOAIFmodExSound
//================================================================//
/**	@name	MOAIFmodExSound
	@text	FMOD singleton.
*/
class MOAIFmodExSound :
	public virtual MOAILuaObject {
private:

	FMOD::Sound* mSound;
	int mLoopCount;
	char mFileName[128];

	//----------------------------------------------------------------//
	static int	_load				( lua_State* L );
	static int	_loadBGM			( lua_State* L );
	static int	_loadSFX			( lua_State* L );
	static int	_release			( lua_State* L );

public:

	friend class MOAIFmodExChannel;
	friend class MOAIFmodEx;

	DECL_LUA_FACTORY ( MOAIFmodExSound )

	//----------------------------------------------------------------//
				MOAIFmodExSound			();
				~MOAIFmodExSound		();
	char		*GetFileName			() { return mFileName; }
	void		Load					( MOAIDataBuffer& data, bool streaming );
	void		Load					( cc8* filename, bool streaming, bool async );
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		Release					();			
	STLString	ToString				();
};

#endif
