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
	static int		_checkFileExists		( lua_State* L );
	static int		_checkPathExists		( lua_State* L );
	static int		_delete					( lua_State* L );
	static int		_getBaseDirectory		( lua_State* L );
	static int		_getDirSeparator		( lua_State* L );
	static int		_getFileDirectory		( lua_State* L );
	static int		_getRealPath			( lua_State* L );
	static int		_loadAndRunLuaFile		( lua_State* L );
	static int		_loadLuaFile			( lua_State* L );
	static int		_mount					( lua_State* L );
	static int		_printSearchPath		( lua_State* L );
	static int		_setWriteDirectory		( lua_State* L );
	static int		_unmount				( lua_State* L );

	// TODO: harebrained
	static int		_getFiles				( lua_State* L );
	static int		_getDirectories			( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIFileSystem )
	
	//----------------------------------------------------------------//
						MOAIFileSystem			();
						~MOAIFileSystem			();
	void				RegisterLuaClass		( USLuaState& state );
};

#endif
