// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFILESYSTEM_H
#define	MOAIFILESYSTEM_H

//================================================================//
// MOAIFileSystem
//================================================================//
/**	@name	MOAIFileSystem
	@text	Functions for manipulating the file system.
*/
class MOAIFileSystem :
	public USLuaObject {
private:

	//----------------------------------------------------------------//
	static int		_affirmPath				( lua_State* L );
	static int		_checkFileExists		( lua_State* L );
	static int		_checkPathExists		( lua_State* L );
	static int		_deleteDirectory		( lua_State* L );
	static int		_deleteFile				( lua_State* L );
	static int		_expandFilename			( lua_State* L );
	static int		_expandPath				( lua_State* L );
	static int		_getRelativePath		( lua_State* L );
	static int		_getWorkingDirectory	( lua_State* L );
	static int		_rename					( lua_State* L );
	static int		_setWorkingDirectory	( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIFileSystem )
	
	//----------------------------------------------------------------//
	void				RegisterLuaClass		( USLuaState& state );
};

#endif
