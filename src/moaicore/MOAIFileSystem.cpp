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
	USLuaState state ( L );
	
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
	USLuaState state ( L );
	
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
	USLuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	bool result = USFileSys::CheckPathExists ( path );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	deleteDirectory
	@text	Deletes a directory and all of its contents.

	@in		string path
	@out	boolean success
*/
int MOAIFileSystem::_deleteDirectory ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	bool result = USFileSys::DeleteDirectory ( path );
	
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
	USLuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	bool result = USFileSys::DeleteFile ( filename );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	expandFilename
	@text	Returns the absolute path to a file. Result includes the
			file name.

	@in		string filename
	@out	string absolute
*/
int MOAIFileSystem::_expandFilename ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	STLString result = USFileSys::Expand ( filename );
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	expandPath
	@text	Returns the absolute path given a relative path.

	@in		string path
	@out	string absolute
*/
int MOAIFileSystem::_expandPath ( lua_State* L ) {
	USLuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 2, "" );
	STLString result = USFileSys::ExpandPath ( path );
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
int MOAIFileSystem::_getRelativePath ( lua_State* L ) {
	USLuaState state ( L );
	
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
	USLuaState state ( L );
	
	STLString result = USFileSys::GetCurrentPath ();
	
	lua_pushstring ( state, result );
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
	USLuaState state ( L );
	
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
	USLuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	bool result = USFileSys::SetCurrentPath ( path );
	
	lua_pushboolean ( state, result );
	return 1;
}

//================================================================//
// MOAIFileSystem
//================================================================//

//----------------------------------------------------------------//
void MOAIFileSystem::RegisterLuaClass ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "affirmPath",				_affirmPath },
		{ "checkFileExists",		_checkFileExists },
		{ "checkPathExists",		_checkPathExists },
		{ "deleteDirectory",		_deleteDirectory },
		{ "deleteFile",				_deleteFile },
		{ "expandFilename",			_expandFilename },
		{ "expandPath",				_expandPath },
		{ "getRelativePath",		_getRelativePath },
		{ "getWorkingDirectory",	_getWorkingDirectory },
		{ "rename",					_rename },
		{ "setWorkingDirectory",	_setWorkingDirectory },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}
