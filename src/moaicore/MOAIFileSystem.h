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
	public USGlobalClass < MOAIFileSystem, USLuaObject > {
private:

	STLString					mWorkingDirectory;
	STLString					mWriteDirectory;
	STLList < STLString >		mArchives;

	//----------------------------------------------------------------//
	static int		_addArchive				( lua_State* L );
	static int		_checkFileExists		( lua_State* L );
	static int		_checkPathExists		( lua_State* L );
	static int		_delete					( lua_State* L );
	static int		_getBaseDirectory		( lua_State* L );
	static int		_getDirSeparator		( lua_State* L );
	static int		_getRealPath			( lua_State* L );
	static int		_getWorkingDirectory	( lua_State* L );
	static int		_loadAndRunLuaFile		( lua_State* L );
	static int		_loadLuaFile			( lua_State* L );
	static int		_printSearchPath		( lua_State* L );
	static int		_setWorkingDirectory	( lua_State* L );
	static int		_setWriteDirectory		( lua_State* L );
	static int		_removeArchive			( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIFileSystem )
	
	//----------------------------------------------------------------//
						MOAIFileSystem			();
						~MOAIFileSystem			();
	void				RegisterLuaClass		( USLuaState& state );
	void				SwapPhysFSPaths			();
};

#endif
