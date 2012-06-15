// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIFileSystem.h>
#include <moaicore/MOAILogMessages.h>

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
/**	@name	affirmPath
	@text	Creates a folder at 'path' if none exists.

	@in		string path
	@out	nil
*/
int MOAIFileSystem::_affirmPath ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	USFileSys::AffirmPath ( path );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	checkFileExists
	@text	Check for the existence of a file.

	@in		string filename
	@out	boolean exists
*/
int MOAIFileSystem::_checkFileExists ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	bool result = USFileSys::CheckFileExists ( filename );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	checkPathExists
	@text	Check for the existence of a path.

	@in		string path
	@out	boolean exists
*/
int MOAIFileSystem::_checkPathExists ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	bool result = USFileSys::CheckPathExists ( path );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	copy
	@text	Copy a file or directory to a new location.

	@in		string srcPath
	@in		string destPath
	@out	boolean result
*/
int MOAIFileSystem::_copy ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* srcPath	= state.GetValue < cc8* >( 1, "" );
	cc8* destPath	= state.GetValue < cc8* >( 2, "" );
	
	bool result = USFileSys::Copy ( srcPath, destPath );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	deleteDirectory
	@text	Deletes a directory and all of its contents.

	@in		string path
	@in		boolean (Optional) recursive If true, the directory and all contents beneath it will be purged. Otherwise, the directory will only be removed if empty.
	@out	boolean success
*/
int MOAIFileSystem::_deleteDirectory ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	bool recursive = state.GetValue < bool >( 2, false );
	bool result = USFileSys::DeleteDirectory ( path, recursive, recursive );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	deleteFile
	@text	Deletes a file.

	@in		string filename
	@out	boolean success
*/
int MOAIFileSystem::_deleteFile ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	bool result = USFileSys::DeleteFile ( filename );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAbsoluteDirectoryPath
	@text	Returns the absolute path given a relative path.

	@in		string path
	@out	string absolute
*/
int MOAIFileSystem::_getAbsoluteDirectoryPath ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	STLString result = USFileSys::GetAbsoluteDirPath ( path );
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAbsoluteFilePath
	@text	Returns the absolute path to a file. Result includes the
			file name.

	@in		string filename
	@out	string absolute
*/
int MOAIFileSystem::_getAbsoluteFilePath ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	STLString result = USFileSys::GetAbsoluteFilePath ( filename );
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getRelativePath
	@text	Given an absolute path returns the relative path
			in relation to the current working directory.

	@in		string path
	@out	string path
-*/
int MOAIFileSystem::_getRelativePath ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	STLString result = USFileSys::GetRelativePath ( path );
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getWorkingDirectory
	@text	Returns the path to current working directory.

	@out	string path
*/
int MOAIFileSystem::_getWorkingDirectory ( lua_State* L ) {
	MOAILuaState state ( L );
	
	STLString result = USFileSys::GetCurrentPath ();
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	listDirectories
	@text	Lists the sub-directories contained in a directory.
 
	@opt	string path				Path to search. Default is current directory.
	@out	table diresctories		A table of directory names (or nil if the path is invalid)
*/
int MOAIFileSystem::_listDirectories ( lua_State* L ) {
	UNUSED ( L );
	
	STLString oldPath = USFileSys::GetCurrentPath();
	
	//cc8* dir = NULL;
	if ( lua_type ( L, 1 ) == LUA_TSTRING ) {
		cc8* dir = lua_tostring ( L, 1 );
		if ( !USFileSys::SetCurrentPath ( dir )) {
			return 0;
		}
	}
	
	USDirectoryItr dirItr;
	
	lua_newtable ( L );
	int n = 0;
	dirItr.Start ();
	while ( dirItr.NextDirectory ()) {

		if( strcmp(dirItr.Current(), "..") == 0 ||
			strcmp(dirItr.Current(), ".") == 0 ) {	
			continue;	
		}
		
		lua_pushstring ( L, dirItr.Current ());
		n++;
		luaL_setn ( L, -2, n );  // new size
		lua_rawseti ( L, -2, n );  // t[pos] = v
	}
	
	USFileSys::SetCurrentPath ( oldPath );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	listFiles
	@text	Lists the files contained in a directory
 
	@opt	string path		Path to search. Default is current directory.
	@out	table files		A table of filenames (or nil if the path is invalid)
*/
int MOAIFileSystem::_listFiles ( lua_State* L ) {
	UNUSED ( L );
	
	STLString oldPath = USFileSys::GetCurrentPath ();
	
	//cc8* dir = NULL;
	if ( lua_type ( L, 1 ) == LUA_TSTRING ) {
		cc8* dir = lua_tostring ( L, 1 );
		if( !USFileSys::SetCurrentPath ( dir )) {
			return 0;
		}
	}

	USDirectoryItr dirItr;
	
	lua_newtable ( L );
	int n = 0;
	dirItr.Start ();
	while ( dirItr.NextFile ()) {
		lua_pushstring ( L, dirItr.Current ());
		n++;
		luaL_setn ( L, -2, n );  // new size
		lua_rawseti ( L, -2, n );  // t[pos] = v
	}
	
	USFileSys::SetCurrentPath ( oldPath );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	mountVirtualDirectory
	@text	Mount an archive as a virtual filesystem directory.

	@in		string path			Virtual path.
	@opt	string archive		Name of archive file to mount. Default value is nil.
	@out	boolean success
*/
int MOAIFileSystem::_mountVirtualDirectory ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path		= state.GetValue < cc8* >( 1, "" );
	cc8* archive	= state.GetValue < cc8* >( 2, 0 );
	
	bool result = USFileSys::MountVirtualDirectory ( path, archive );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	rename
	@text	Renames a file or folder.

	@in		string oldPath
	@in		string newPath
	@out	boolean success
*/
int MOAIFileSystem::_rename ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* oldPath = state.GetValue < cc8* >( 1, "" );
	cc8* newPath = state.GetValue < cc8* >( 2, "" );
	
	bool result = USFileSys::Rename ( oldPath, newPath );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setWorkingDirectory
	@text	Sets the current working directory.

	@in		string path
	@out	boolean success
*/
int MOAIFileSystem::_setWorkingDirectory ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	bool result = USFileSys::SetCurrentPath ( path );
	
	lua_pushboolean ( state, result );
	return 1;
}

//================================================================//
// MOAIFileSystem
//================================================================//

//----------------------------------------------------------------//
void MOAIFileSystem::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "affirmPath",					_affirmPath },
		{ "checkFileExists",			_checkFileExists },
		{ "checkPathExists",			_checkPathExists },
		{ "copy",						_copy },
		{ "deleteDirectory",			_deleteDirectory },
		{ "deleteFile",					_deleteFile },
		{ "getAbsoluteFilePath",		_getAbsoluteFilePath },
		{ "getAbsoluteDirectoryPath",	_getAbsoluteDirectoryPath },
		{ "getRelativePath",			_getRelativePath },
		{ "getWorkingDirectory",		_getWorkingDirectory },
		{ "listDirectories",			_listDirectories },
		{ "listFiles",					_listFiles },
		{ "mountVirtualDirectory",		_mountVirtualDirectory },
		{ "rename",						_rename },
		{ "setWorkingDirectory",		_setWorkingDirectory },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}
