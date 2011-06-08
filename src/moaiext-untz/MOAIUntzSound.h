// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIUNTZSOUND_H
#define	MOAIUNTZSOUND_H

#include <moaicore/moaicore.h>
#include <moaiext-untz/MOAIUntzSound.h>
#include <UntzSound.h>

class MOAIDataBuffer;

//================================================================//
// MOAIUntzSound
//================================================================//
/**	@name	MOAIUntzSound
	@text	Untz sound object.
*/
class MOAIUntzSound :
	public virtual USLuaObject {
private:

	//----------------------------------------------------------------//
	//static int	_load				( lua_State* L );
	//static int	_loadBGM			( lua_State* L );
	//static int	_loadSFX			( lua_State* L );
	//static int	_release			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIUntzSound )

	//----------------------------------------------------------------//
				MOAIUntzSound			();
				~MOAIUntzSound			();
	void		RegisterLuaClass		( USLuaState& state );
	void		RegisterLuaFuncs		( USLuaState& state );		
	STLString	ToString				();
};

#endif
