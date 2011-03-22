// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILUASCRIPT_H
#define MOAILUASCRIPT_H

class USData;

//================================================================//
// MOAILuaScript
//================================================================//
/**	@brief USLuaData subclass for loading and running Lua scripts.
*/
class MOAILuaScript
	: public virtual USLuaData {

private:

	const char*	mData;
	int			mDataLength;

	//----------------------------------------------------------------//
	static int	_execute	( lua_State* L );
	static int	_load		( lua_State* L );

public:

	DECL_LUA_DATA ( MOAILuaScript )

	//----------------------------------------------------------------//
				MOAILuaScript		();
				~MOAILuaScript		();

	void		Clear				();
	bool		Execute				( USLuaState& state );
	bool		Load				( const char *filename );
	bool		Load				( USData& data );
	void		RegisterLuaClass	( USLuaState& state );
	void		RegisterLuaFuncs	( USLuaState& state );
	STLString	ToString			();
};

#endif