// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIFileSystem.h>
#include <zl-core/ZLVfsFileSystem.h>
#include <zl-core/ZLVfsVirtualPath.h>
#include <zl-core/ZLVfsZipArchive.h>

// TODO: these are getting reintroduced somewhere; find them and kill them

#ifdef DeleteFile
	#undef DeleteFile
#endif

#ifdef RemoveDirectory
	#undef RemoveDirectory
#endif

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	affirmPath
	@text	Creates a folder at 'path' if none exists.

	@in		string path
	@out	nil
*/
int MOAIFileSystem::_affirmPath ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	ZLFileSys::AffirmPath ( path );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	checkFileExists
	@text	Check for the existence of a file.

	@in		string filename
	@out	boolean exists
*/
int MOAIFileSystem::_checkFileExists ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	bool result = ZLFileSys::CheckFileExists ( filename );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	checkPathExists
	@text	Check for the existence of a path.

	@in		string path
	@out	boolean exists
*/
int MOAIFileSystem::_checkPathExists ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	bool result = ZLFileSys::CheckPathExists ( path );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	copy
	@text	Copy a file or directory to a new location.

	@in		string srcPath
	@in		string destPath
	@out	boolean result
*/
int MOAIFileSystem::_copy ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* srcPath	= state.GetValue < cc8* >( 1, "" );
	cc8* destPath	= state.GetValue < cc8* >( 2, "" );
	
	bool result = ZLFileSys::Copy ( srcPath, destPath );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	deleteDirectory
	@text	Deletes a directory and all of its contents.

	@in		string path
	@opt	boolean recursive		If true, the directory and all contents beneath it will be purged.
									Otherwise, the directory will only be removed if empty.
	@out	boolean success
*/
int MOAIFileSystem::_deleteDirectory ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	bool recursive = state.GetValue < bool >( 2, false );
	bool result = ZLFileSys::DeleteDirectory ( path, recursive, recursive );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	deleteFile
	@text	Deletes a file.

	@in		string filename
	@out	boolean success
*/
int MOAIFileSystem::_deleteFile ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	bool result = ZLFileSys::DeleteFile ( filename );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getAbsoluteDirectoryPath
	@text	Returns the absolute path given a relative path.

	@in		string path
	@out	string absolute
*/
int MOAIFileSystem::_getAbsoluteDirectoryPath ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	STLString result = ZLFileSys::GetAbsoluteDirPath ( path );
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getAbsoluteFilePath
	@text	Returns the absolute path to a file. Result includes the
			file name.

	@in		string filename
	@out	string absolute
*/
int MOAIFileSystem::_getAbsoluteFilePath ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* filename = state.GetValue < cc8* >( 1, "" );
	STLString result = ZLFileSys::GetAbsoluteFilePath ( filename );
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getRelativePath
	@text	Given an absolute path returns the relative path
			in relation to the current working directory or a
			user supplied 'base' directory.

	@in		string path
	@opt	string base
	@out	string path
*/
int MOAIFileSystem::_getRelativePath ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	cc8* base = state.GetValue < cc8* >( 2, 0 );
	
	STLString result = ZLFileSys::GetRelativePath ( path, base );
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getWorkingDirectory
	@text	Returns the path to current working directory.

	@out	string path
*/
int MOAIFileSystem::_getWorkingDirectory ( lua_State* L ) {
	MOAILuaState state ( L );
	
	STLString result = ZLFileSys::GetCurrentPath ();
	
	lua_pushstring ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getVirtualPathInfo
	@text	Given a virtual path, return the path to the archive and, if
			the path is to a file, the offset to the file record within
			the archive.
 
	@in		string path					A virtual path.
	@out	string pathToArchive		The archive to which the path is mapped.
	@out	string localPath			The relative portion of the path.
	@opt	string offsetToHeader		Offset to the file record header in the archive, or nil if not a file path.
	@opt	number uncompressedSize		Uncompressed size of the file or nil if not a file path.
	@opt	number compressedSize		Compressed size of the file or nil if not compressed or not a file path.
*/
int MOAIFileSystem::_getVirtualPathInfo ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* path = state.GetValue < cc8* >( 1, "" );

	ZLVfsVirtualPathInfo info = ZLVfsFileSystem::Get ().GetVirtualPathInfo ( path );

	int top = state.GetTop ();

	if ( info.mIsVirtual ) {
	
		state.Push ( info.mPathToArchive.c_str ());
		state.Push ( info.mLocalPath.c_str ());
		
		if ( info.mIsFile ) {
			state.Push (( u32 )info.mOffsetToHeader );
			state.Push (( u32 )info.mUncompressedSize );
			
			if ( info.mIsCompressed ) {
				state.Push (( u32 )info.mCompressedSize );
			}
		}
	}
	return state.GetTop () - top;
}

//----------------------------------------------------------------//
/**	@lua	listDirectories
	@text	Lists the sub-directories contained in a directory, sorted alphabetically.
			Special directories '.' and '..' are omitted.
 
	@opt	string path				Path to search. Default is current directory.
	@out	table directories		A table of directory names (or nil if the path is invalid)
*/
int MOAIFileSystem::_listDirectories ( lua_State* L ) {
	UNUSED ( L );
	
	STLString oldPath = ZLFileSys::GetCurrentPath ();
	
	if ( lua_type ( L, 1 ) == LUA_TSTRING ) {
		cc8* dir = lua_tostring ( L, 1 );
		if ( !ZLFileSys::SetCurrentPath ( dir )) {
			return 0;
		}
	}
	
	STLSet < STLString > sortedSet;
	
	ZLDirectoryItr dirItr;
	dirItr.Start ();
	while ( dirItr.NextDirectory ()) {
		sortedSet.insert ( dirItr.Current ());
	}
	ZLFileSys::SetCurrentPath ( oldPath );
	
	lua_newtable ( L );
	
	STLSet < STLString >::iterator sortedSetIt = sortedSet.begin ();
	for ( int n = 1; sortedSetIt != sortedSet.end (); ++sortedSetIt, ++n ) {
	
		lua_pushstring ( L, sortedSetIt->c_str ());
		#ifdef luaL_setn
			luaL_setn ( L, -2, n );  // new size
		#endif
		lua_rawseti ( L, -2, n );  // t[pos] = v
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	listFiles
	@text	Lists the files contained in a directory, sorted alphabetically.
 
	@opt	string path		Path to search. Default is current directory.
	@out	table files		A table of filenames (or nil if the path is invalid)
*/
int MOAIFileSystem::_listFiles ( lua_State* L ) {
	UNUSED ( L );
	
	STLString oldPath = ZLFileSys::GetCurrentPath ();
	
	if ( lua_type ( L, 1 ) == LUA_TSTRING ) {
		cc8* dir = lua_tostring ( L, 1 );
		if( !ZLFileSys::SetCurrentPath ( dir )) {
			return 0;
		}
	}

	STLSet < STLString > sortedSet;
	
	ZLDirectoryItr dirItr;
	dirItr.Start ();
	while ( dirItr.NextFile ()) {
		sortedSet.insert ( dirItr.Current ());
	}
	ZLFileSys::SetCurrentPath ( oldPath );
	
	lua_newtable ( L );
	
	STLSet < STLString >::iterator sortedSetIt = sortedSet.begin ();
	for ( int n = 1; sortedSetIt != sortedSet.end (); ++sortedSetIt, ++n ) {
	
		lua_pushstring ( L, sortedSetIt->c_str ());
		#ifdef luaL_setn
			luaL_setn ( L, -2, n );  // new size
		#endif
		lua_rawseti ( L, -2, n );  // t[pos] = v
	}
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	loadFile
	@text	Open and read the entirity of a file into a string.

	@in		string filename
	@out	string contents		Returns empty string if file length is 0. Returns nil is no such file.
*/
int MOAIFileSystem::_loadFile ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFileSystem, "S" )

	cc8* filename = state.GetValue < cc8* >( 1, 0 );

	if ( filename && ZLFileSys::CheckFileExists ( filename )) {
	
		ZLFileStream stream;
		if ( stream.OpenRead ( filename )) {
		
			size_t len = stream.GetLength ();
			if ( len > 0 ) {
			
				void* buffer = malloc ( len );
				stream.ReadBytes ( buffer, len );
				lua_pushlstring ( state, ( cc8* )buffer, len );
				free ( buffer );
			}
			else {
				state.Push ( "" );
			}
			stream.Close ();
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	mountVirtualDirectory
	@text	Mount an archive as a virtual filesystem directory.

	@in		string path			Virtual path.
	@opt	string archive		Name of archive file to mount. Default value is nil.
	@out	boolean success
*/
int MOAIFileSystem::_mountVirtualDirectory ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path		= state.GetValue < cc8* >( 1, "" );
	cc8* archive	= state.GetValue < cc8* >( 2, 0 );
	
	bool result = ZLFileSys::MountVirtualDirectory ( path, archive );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
// TODO: deprecate
int MOAIFileSystem::_pathFromRef ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* ref = state.GetValue < cc8* >( 1, "" );
	state.Push ( ZLFileSys::PathFromRef ( ref ).c_str ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: deprecate
int MOAIFileSystem::_pathToRef ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* ref = state.GetValue < cc8* >( 1, "" );
	state.Push ( ZLFileSys::PathToRef ( ref ).c_str ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	rename
	@text	Renames a file or folder.

	@in		string oldPath
	@in		string newPath
	@out	boolean success
*/
int MOAIFileSystem::_rename ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* oldPath = state.GetValue < cc8* >( 1, "" );
	cc8* newPath = state.GetValue < cc8* >( 2, "" );
	
	bool result = ZLFileSys::Rename ( oldPath, newPath );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	saveFile
	@text	Create or overwrite a file to contain the contents of a given string.

	@in		string filename
	@opt	string contents
	@out	nil
*/
int MOAIFileSystem::_saveFile ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIFileSystem, "S" )

	cc8* filename = state.GetValue < cc8* >( 1, 0 );
	
	ZLFileStream stream;
	if ( filename && stream.Open ( filename, ZLFileStream::READ_WRITE_NEW )) {
	
		if ( state.IsType ( 2, LUA_TSTRING )) {
			size_t len;
			cc8* str = lua_tolstring ( state, 2, &len );
			stream.WriteBytes ( str, len );
		}
		stream.Close ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: deprecate
int MOAIFileSystem::_setPathRef ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* ref = state.GetValue < cc8* >( 1, "" );
	cc8* path = state.GetValue < cc8* >( 2, 0 );
	
	ZLFileSys::SetPathRef ( ref, path );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setWorkingDirectory
	@text	Sets the current working directory.

	@in		string path
	@out	boolean success
*/
int MOAIFileSystem::_setWorkingDirectory ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* path = state.GetValue < cc8* >( 1, "" );
	bool result = ZLFileSys::SetCurrentPath ( path );
	
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	stripPKZipTimestamps
	@text	Strip the timestamp from a pkzip file. Useful in systems
			where periodically generated pkzip files are compared against
			each other using hashes. Once timestamps are stripeed, two
			pkzip files with the same contents should produce identical
			hashes.

	@in		string infilename
	@in		string outfilename
	@out	boolean success
*/
int MOAIFileSystem::_stripPKZipTimestamps ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* infilename = state.GetValue < cc8* >( 1, "" );
	cc8* outfilename = state.GetValue < cc8* >( 2, "" );
	bool result = ZLFileSys::StripPKZipTimestamps ( infilename, outfilename );
	
	lua_pushboolean ( state, result );
	return 1;
}

//================================================================//
// MOAIFileSystem
//================================================================//

//----------------------------------------------------------------//
MOAIFileSystem::MOAIFileSystem () {
		
	RTTI_BEGIN ( MOAIFourier )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIFileSystem >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFileSystem::~MOAIFileSystem () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFileSystem::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

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
		{ "getVirtualPathInfo",			_getVirtualPathInfo },
		{ "listDirectories",			_listDirectories },
		{ "listFiles",					_listFiles },
		{ "loadFile",					_loadFile },
		{ "mountVirtualDirectory",		_mountVirtualDirectory },
		{ "pathFromRef",				_pathFromRef },
		{ "pathToRef",					_pathToRef },
		{ "rename",						_rename },
		{ "saveFile",					_saveFile },
		{ "setPathRef",					_setPathRef },
		{ "setWorkingDirectory",		_setWorkingDirectory },
		{ "stripPKZipTimestamps",		_stripPKZipTimestamps },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
