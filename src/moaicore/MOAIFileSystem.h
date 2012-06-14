// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIOFILESYSTEM_H
#define	MOAIOFILESYSTEM_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIFileSystem
//================================================================//
/**	@name	MOAIFileSystem
	@text	Functions for manipulating the file system.
*/
class MOAIFileSystem :
	public MOAILuaObject {
private:

	//----------------------------------------------------------------//
	static int		_affirmPath					( lua_State* L );
	static int		_checkFileExists			( lua_State* L );
	static int		_checkPathExists			( lua_State* L );
	static int		_copy						( lua_State* L );
	static int		_deleteDirectory			( lua_State* L );
	static int		_deleteFile					( lua_State* L );
	static int		_getAbsoluteFilePath		( lua_State* L );
	static int		_getAbsoluteDirectoryPath	( lua_State* L );
	static int		_getRelativePath			( lua_State* L );
	static int		_getWorkingDirectory		( lua_State* L );
	static int		_listDirectories			( lua_State* L );
	static int		_listFiles					( lua_State* L );
	static int		_mountVirtualDirectory		( lua_State* L );
	static int		_rename						( lua_State* L );
	static int		_setWorkingDirectory		( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIFileSystem )
	
	//----------------------------------------------------------------//
	void				RegisterLuaClass		( MOAILuaState& state );
};

#endif