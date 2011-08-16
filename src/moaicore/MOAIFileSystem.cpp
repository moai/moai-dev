// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIFileSystem.h>
#include <moaicore/MOAILogMessages.h>
#include <physfs.h>

// TODO: these are getting reintroduced somewhere; find them and kill them

#ifdef DeleteFile
	#undef DeleteFile
#endif

#ifdef RemoveDirectory
	#undef RemoveDirectory
#endif

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	checkFileExists
	@text	Check for the existence of a file.

	@in		string filename
	@out	boolean exists
*/
int MOAIFileSystem::_checkFileExists ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	int result = PHYSFS_exists ( filename );
	
	lua_pushboolean ( state, ( result == 0 ) ? false : true );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	checkPathExists
	@text	Check for the existence of a path.

	@in		string path
	@out	boolean exists
*/
int MOAIFileSystem::_checkPathExists ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	int result = PHYSFS_isDirectory ( path );
	
	lua_pushboolean ( state, ( result == 0 ) ? false : true );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	delete
	@text	Deletes a file or directory.

	@in		string file/directory
	@out	string error
*/
int MOAIFileSystem::_delete ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* name = state.GetValue < cc8* >( 1, "" );
	PHYSFS_delete ( name );
	
	lua_pushstring ( state, PHYSFS_getLastError ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getBaseDirectory
	@text	Returns the path where the application resides

	@in		nil
	@out	string applicationDir
*/
int MOAIFileSystem::_getBaseDirectory ( lua_State* L ) {
	USLuaState state ( L );
	
	lua_pushstring ( state, PHYSFS_getBaseDir ());

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDirSeparator
	@text	Returns a string that represents the platforms directory seprator.

	@in		nil
	@out	string dirSeperator
*/
int MOAIFileSystem::_getDirSeparator ( lua_State* L ) {
	USLuaState state ( L );
	
	lua_pushstring ( state, PHYSFS_getDirSeparator ());

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getFileDirectory
	@text	Returns the directory inside the PHYSFS search path.

	@in		string filename
	@out	string directroy
*/
int MOAIFileSystem::_getFileDirectory ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	lua_pushstring ( state, PHYSFS_getRealDir ( filename ));

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getRealPath
	@text	Returns a the real path of a file

	@in		string path
	@out	string real
*/
int MOAIFileSystem::_getRealPath ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	STLString result = PHYSFS_getRealDir ( path );
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	_loadAndRunLuaFile
	@text	Loads and runs a Lua file from the PHYSFS file system.

	@in		string filename
	@out	int error
*/

int MOAIFileSystem::_loadAndRunLuaFile ( lua_State* L ) {
	USLuaState state ( L );

	cc8* filename = state.GetValue < cc8* >( 1, "" );
	
	// Check that the lua file exists in the archive
	if ( !PHYSFS_exists ( filename )) {
		printf ( "File does not exist" );
		return 0;
	}

	// File exists, so read the data
	PHYSFS_File* file = PHYSFS_openRead ( filename );
	int fileSize = ( int )PHYSFS_fileLength ( file );
	char* data = new char[ fileSize ];
	PHYSFS_read ( file, data, 1, fileSize );

	// Load and run the chunk
	luaL_loadbuffer ( state, data, fileSize, filename );
	lua_pcall( state, 0, LUA_MULTRET, 0 );

	// Cleanup
	delete [] data;
	PHYSFS_close ( file );

	// Return 1 ( the return of the function/module ran in of pcall )
	return 1;
}

//----------------------------------------------------------------//
/**	@name	loadLuaFile
	@text	Loads a Lua file from the PHYSFS file system. 

	@in		string filename
	@out	int error
*/

int MOAIFileSystem::_loadLuaFile ( lua_State* L ) {
	USLuaState state ( L );

	cc8* filename = state.GetValue < cc8* >( 1, "" );
	
	// Check that the lua file exists in the archive
	if ( !PHYSFS_exists ( filename )) {
		printf ( "File does not exist" );
		return 0;
	}

	// File exists, so read the data
	PHYSFS_File* file = PHYSFS_openRead ( filename );
	int fileSize = ( int )PHYSFS_fileLength ( file );
	char* data = new char[ fileSize ];
	PHYSFS_read ( file, data, 1, fileSize );

	// Load and the chunk
	luaL_loadbuffer ( state, data, fileSize, filename );

	// Cleanup
	delete [] data;
	PHYSFS_close ( file );

	// Return 1 ( the file buffer )
	return 1;
}

//----------------------------------------------------------------//
/**	@name	mount
	@text	Mounts a new archive or directory to the current PHYSFS file system.

	@in		string newDir
	@in		string mountPoint
	@in		int appendToPath (Non-zero to append, zero to prepend)
	@out	string error
*/
int MOAIFileSystem::_mount ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* newDir = state.GetValue < cc8* >( 1, "" );
	cc8* mountPoint = state.GetValue < cc8* >( 2, "" );
	int appendToPath = state.GetValue < int >( 3, 1 ); 
	
	PHYSFS_mount ( newDir, mountPoint, appendToPath ); 

	lua_pushstring ( state, PHYSFS_getLastError ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	printSearchPath
	@text	Prints the archives and directories in the search path.

	@in		nil
	@out	nil
*/
int MOAIFileSystem::_printSearchPath ( lua_State* L ) {
	USLuaState state ( L );
	
	char **list = PHYSFS_getSearchPath();
	for ( char** i = list; *i != NULL; i++ )
		printf ( "[%s] - ", *i );
	
	printf ( "[end]\n" );
	PHYSFS_freeList ( list );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setWriteDirectory
	@text	Sets the current write directory.

	@in		string path
	@out	string error
*/
int MOAIFileSystem::_setWriteDirectory ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	PHYSFS_setWriteDir ( path );
	
	lua_pushstring ( state, PHYSFS_getLastError ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	unmount
	@text	Removces a diretory or archive from the PHYFS file system.

	@in		string oldDir
	@out	string error
*/
int MOAIFileSystem::_unmount ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* oldDir = state.GetValue < cc8* >( 1, "" );	
	PHYSFS_removeFromSearchPath ( oldDir ); 

	lua_pushstring ( state, PHYSFS_getLastError ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getFiles
 @text	Lists the files contained in a directory
 
 @in	(Optional) string path Path to search (or current directory if nil)
 @out	table A table of filenames (or nil if the path is invalid)
 */
int MOAIFileSystem::_getFiles ( lua_State* L ) {
	STLString oldPath = USFileSys::GetCurrentPath();
	
	cc8* dir = NULL;
	if( lua_type(L, 1) == LUA_TSTRING )
	{
		dir = lua_tostring(L, 1);
		if( !USFileSys::SetCurrentPath(dir) )
		{
			return 0;
		}
	}

	
	USDirectoryItr dirItr;
	
	lua_newtable(L);
	int n = 0;
	dirItr.Start ();
	while ( dirItr.NextFile() )
	{
		if( dir )
		{
			lua_pushstring(L, dir);
			lua_pushstring(L, "/");
			lua_pushstring(L, dirItr.Current());
			lua_concat(L, 3);
		}
		else
		{
			lua_pushstring(L, dirItr.Current());
		}

		n++;
		luaL_setn(L, -2, n);  // new size
		lua_rawseti(L, -2, n);  // t[pos] = v
	}
	
	USFileSys::SetCurrentPath(oldPath);
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getDirectories
 @text	Lists the sub-directories contained in a directory.
 
 @in	(Optional) string path Path to search (or current directory if nil)
 @out	table A table of directory names (or nil if the path is invalid)
 */
int MOAIFileSystem::_getDirectories ( lua_State* L ) {
	STLString oldPath = USFileSys::GetCurrentPath();
	
	cc8* dir = NULL;
	if( lua_type(L, 1) == LUA_TSTRING )
	{
		dir = lua_tostring(L, 1);
		if( !USFileSys::SetCurrentPath(dir) )
		{
			return 0;
		}
	}
	
	USDirectoryItr dirItr;
	
	lua_newtable(L);
	int n = 0;
	dirItr.Start ();
	while ( dirItr.NextDirectory() )
	{
		if( dir )
		{
			lua_pushstring(L, dir);
			lua_pushstring(L, "/");
			lua_pushstring(L, dirItr.Current());
			lua_concat(L, 3);
		}
		else
		{
			lua_pushstring(L, dirItr.Current());
		}
		n++;
		luaL_setn(L, -2, n);  // new size
		lua_rawseti(L, -2, n);  // t[pos] = v
	}
	
	USFileSys::SetCurrentPath(oldPath);
	
	return 1;
}


//================================================================//
// MOAIFileSystem
//================================================================//

//----------------------------------------------------------------//
MOAIFileSystem::MOAIFileSystem () {
	PHYSFS_init ( NULL );
}

//----------------------------------------------------------------//
MOAIFileSystem::~MOAIFileSystem () {
	PHYSFS_deinit ();
}

//----------------------------------------------------------------//
void MOAIFileSystem::RegisterLuaClass ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "checkFileExists",		_checkFileExists },
		{ "checkPathExists",		_checkPathExists },
		{ "delete",					_delete },
		{ "getBaseDirectory",		_getBaseDirectory },
		{ "getDirectories",			_getDirectories },
		{ "getDirSeparator",		_getDirSeparator },
		{ "getFileDirectory",		_getFileDirectory },
		{ "getFiles",				_getFiles },
		{ "getRealPath",			_getRealPath },
		{ "loadAndRunLuaFile",		_loadAndRunLuaFile }, 
		{ "loadLuaFile",			_loadLuaFile }, 
		{ "mount",					_mount },
		{ "printSearchPath",		_printSearchPath },
		{ "setWriteDirectory",		_setWriteDirectory },
		{ "unmount",				_unmount },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}
