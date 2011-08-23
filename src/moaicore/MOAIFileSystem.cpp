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
/**	@name	addArchive
	@text	Mounts a new archive or directory to the current PHYSFS file system.

	@in		string newDir
	@out	string error
*/
int MOAIFileSystem::_addArchive ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* newDir = state.GetValue < cc8* >( 1, "" );
	
	STLString workingDir = MOAIFileSystem::Get ().mWorkingDirectory;
	workingDir.append ( newDir );
	MOAIFileSystem::Get ().mArchives.push_back ( workingDir );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	checkFileExists
	@text	Check for the existence of a file.

	@in		string filename
	@out	boolean exists
*/
int MOAIFileSystem::_checkFileExists ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	
	MOAIFileSystem::Get ().SwapPhysFSPaths ();
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

	MOAIFileSystem::Get ().SwapPhysFSPaths ();
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

	MOAIFileSystem::Get ().SwapPhysFSPaths ();
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
	
	lua_pushstring ( state, USFileSys::GetCurrentPath ());

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
/**	@name	getRealPath
	@text	Returns a the real path of a file

	@in		string path
	@out	string real
*/
int MOAIFileSystem::_getRealPath ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	
	MOAIFileSystem::Get ().SwapPhysFSPaths ();
	STLString result = PHYSFS_getRealDir ( path );
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getWorkingDirectory
	@text	Returns the current working directory

	@in		nil
	@out	string currentDir
*/
int MOAIFileSystem::_getWorkingDirectory ( lua_State* L ) {
	USLuaState state ( L );
	
	lua_pushstring ( state, MOAIFileSystem::Get ().mWorkingDirectory.str ());
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

	MOAIFileSystem::Get ().SwapPhysFSPaths ();
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
	
	MOAIFileSystem::Get ().SwapPhysFSPaths ();
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
/**	@name	printSearchPath
	@text	Prints the archives and directories in the search path.

	@in		nil
	@out	nil
*/
int MOAIFileSystem::_printSearchPath ( lua_State* L ) {
	USLuaState state ( L );
	
	MOAIFileSystem::Get ().SwapPhysFSPaths ();
	char **list = PHYSFS_getSearchPath();
	for ( char** i = list; *i != NULL; i++ )
		printf ( "[%s] - ", *i );
	
	printf ( "[end]\n" );
	PHYSFS_freeList ( list );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	removeArchive
	@text	Removces a diretory or archive from the PHYFS file system.

	@in		string oldDir
	@out	string error
*/
int MOAIFileSystem::_removeArchive ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* oldDir = state.GetValue < cc8* >( 1, "" );	
	MOAIFileSystem::Get ().mArchives.remove ( oldDir );

	lua_pushstring ( state, PHYSFS_getLastError ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setWorkingDirectory
	@text	Sets the current working directory.

	@in		string path
	@out	string error
*/
int MOAIFileSystem::_setWorkingDirectory ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	MOAIFileSystem::Get ().mWorkingDirectory = path;
	
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
	MOAIFileSystem::Get ().mWriteDirectory = path;
	
	return 0;
}

//================================================================//
// MOAIFileSystem
//================================================================//

//----------------------------------------------------------------//
MOAIFileSystem::MOAIFileSystem () {

	// set up initial working directory
	mWorkingDirectory = USFileSys::GetCurrentPath ();
	
	// set up initial write directory
	mWriteDirectory = mWorkingDirectory;
	PHYSFS_setWriteDir ( mWriteDirectory );
}

//----------------------------------------------------------------//
MOAIFileSystem::~MOAIFileSystem () {
}

//----------------------------------------------------------------//
void MOAIFileSystem::RegisterLuaClass ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "addArchive",				_addArchive },
		{ "checkFileExists",		_checkFileExists },
		{ "checkPathExists",		_checkPathExists },
		{ "delete",					_delete },
		{ "getBaseDirectory",		_getBaseDirectory },
		{ "getDirSeparator",		_getDirSeparator },
		{ "getRealPath",			_getRealPath },
		{ "getWorkingDirectory",	_getWorkingDirectory },
		{ "loadAndRunLuaFile",		_loadAndRunLuaFile }, 
		{ "loadLuaFile",			_loadLuaFile }, 
		{ "printSearchPath",		_printSearchPath },
		{ "removeArchive",			_removeArchive },
		{ "setWorkingDirectory",	_setWorkingDirectory },
		{ "setWriteDirectory",		_setWriteDirectory },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFileSystem::SwapPhysFSPaths ( ) {
	
	// set the write dir
	PHYSFS_setWriteDir( mWriteDirectory.str ());
		
	// clear old working directory and archives
	char** oldSearchPath = PHYSFS_getSearchPath ();
	for ( int i = 0; oldSearchPath [ i ]; ++i ) {
		PHYSFS_removeFromSearchPath ( oldSearchPath [ i ] );
	}
	
	// add current directory
	PHYSFS_mount ( mWorkingDirectory.str (), "", 1 );
	
	// add current archives		
	for ( STLList < STLString >::iterator it = mArchives.begin (); it != mArchives.end (); ++it ) {
		PHYSFS_mount ( (*it).str (), "", 1 );
	}
}