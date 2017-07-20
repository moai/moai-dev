// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIOFILESYSTEM_H
#define	MOAIOFILESYSTEM_H

//================================================================//
// MOAIFileSystem
//================================================================//
/**	@lua	MOAIFileSystem
	@text	Functions for manipulating the file system.
*/
class MOAIFileSystem :
	public MOAIGlobalClass < MOAIFileSystem, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int		_affirmPath					( lua_State* L );
	static int		_checkFileExists			( lua_State* L );
	static int		_checkPathExists			( lua_State* L );
	static int		_copy						( lua_State* L );
	static int		_deleteDirectory			( lua_State* L );
	static int		_deleteFile					( lua_State* L );
	static int		_getAbsoluteDirectoryPath	( lua_State* L );
	static int		_getAbsoluteFilePath		( lua_State* L );
	static int		_getRelativePath			( lua_State* L );
	static int		_getWorkingDirectory		( lua_State* L );
	static int		_getVirtualPathInfo			( lua_State* L );
	static int		_listDirectories			( lua_State* L );
	static int		_listFiles					( lua_State* L );
	static int		_loadFile					( lua_State* L );
	static int		_mountVirtualDirectory		( lua_State* L );
	static int		_pathFromRef				( lua_State* L );
	static int		_pathToRef					( lua_State* L );
	static int		_rename						( lua_State* L );
	static int		_saveFile					( lua_State* L );
	static int		_setPathRef					( lua_State* L );
	static int		_setWorkingDirectory		( lua_State* L );
	static int		_stripPKZipTimestamps		( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIFileSystem )
	
	//----------------------------------------------------------------//
	void				RegisterLuaClass		( MOAILuaState& state );
};

#endif

