// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFILECACHE_H
#define MOAIFILECACHE_H

//================================================================//
// MOAIFileCache
//================================================================//
/**	@brief Cache for downloaded files.
*/
class MOAIFileCache :
	public virtual USLuaData,
	public USFileCache {
private:
	
	//----------------------------------------------------------------//
	static int		_affirmPartition	( lua_State* L );
	static int		_close				( lua_State* L );
	static int		_open				( lua_State* L );

public:
	
	DECL_LUA_DATA ( MOAIFileCache )

	//----------------------------------------------------------------//
					MOAIFileCache		();
					~MOAIFileCache		();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	STLString		ToString			();
};

#endif
