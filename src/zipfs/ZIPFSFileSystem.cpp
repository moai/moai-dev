// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zipfs/zipfs_util.h>
#include <zipfs/ZIPFSFileSystem.h>
#include <zipfs/ZIPFSVirtualPath.h>

using namespace std;

//================================================================//
// ZIPFSFileSystem
//================================================================//

//----------------------------------------------------------------//
int ZIPFSFileSystem::AffirmPath ( const char* path ) {

	assert ( 0 ); // fix me

	if ( !path ) return -1;

	string abspath = this->GetAbsoluteDirPath ( path );
	if ( this->IsVirtualPath ( abspath.c_str ())) return -1;
	
	return this->MakeDir ( abspath.c_str ());
}

//----------------------------------------------------------------//
string ZIPFSFileSystem::BlessPath ( const char* path ) {

	size_t i = 0;
	size_t j = 0;
	
	for ( i = 0; path [ i ]; ++i ) {
		if ( IsSeparator ( path [ i ] )) {
			while ( IsSeparator ( path [ ++i ]));
			--i;
		}
		j++;
	}
	
	string blessed;
	blessed.reserve ( j );
	
	i = 0;
	
	for ( i = 0; path [ i ]; ++i ) {
		
		char c = path [ i ];
		
		if ( IsSeparator ( c )) {
		
			c = '/';
			while ( IsSeparator ( path [ ++i ]));
			--i;
		}
		
		blessed.push_back ( c );
	}
	
	return blessed;
}

//----------------------------------------------------------------//
int ZIPFSFileSystem::ChangeDir ( const char* path ) {

	int result = -1;
	ZIPFSVirtualPath* mount = 0;

	string absDirpath = this->GetAbsoluteDirPath ( path );
	path = absDirpath.c_str ();
	mount = this->FindBestVirtualPath ( path );
	
	if ( mount ) {
		const char* localpath = mount->GetLocalPath ( path );
		if ( localpath ) {
			result = 0;
		}
	}
	else {
		result = chdir ( path );
	}

	if ( result == 0 ) {
		this->mWorkingPath = path;
	}
	return result;
}

//----------------------------------------------------------------//
void ZIPFSFileSystem::Cleanup () {

	ZIPFSVirtualPath* cursor = this->mVirtualPaths;
	while ( cursor ) {
		ZIPFSVirtualPath* virtualPath = cursor;
		cursor = cursor->mNext;
		delete virtualPath;
	}

	zipfs_mutex_destroy ( this->mMutex );
}

//----------------------------------------------------------------//
size_t ZIPFSFileSystem::ComparePaths ( const char* p0, const char* p1 ) {

	size_t i;
	size_t same = 0;

	for ( i = 0; p0 [ i ] && p1 [ i ]; ++i ) {
		
		char h = ( char )tolower ( p0 [ i ]);
		char v = ( char )tolower ( p1 [ i ]);
		
		if ( h != v ) break;
		
		if ( h == '/' ) {
			same = i + 1;
		}
	}

	return same;
}

//----------------------------------------------------------------//
ZIPFSVirtualPath* ZIPFSFileSystem::FindBestVirtualPath ( char const* path ) {

	size_t len = 0;
	size_t bestlen = 0;
	ZIPFSVirtualPath* best = 0;
	ZIPFSVirtualPath* cursor = this->mVirtualPaths;
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		const char* test = cursor->mPath.c_str ();
		len = ComparePaths ( test, path );
	
		if (( !test [ len ]) && ( len > bestlen )) {
			best = cursor;
			bestlen = len;
		}		
	}
	return best;
}

//----------------------------------------------------------------//
ZIPFSVirtualPath* ZIPFSFileSystem::FindNextVirtualSubdir ( char const* path, ZIPFSVirtualPath* cursor ) {

	size_t len = 0;
	
	cursor = cursor ? cursor->mNext : this->mVirtualPaths;
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		const char* test = cursor->mPath.c_str ();
		len = ComparePaths ( test, path );
		
		if ( test [ len ] && ( !path [ len ])) {
			return cursor;
		}		
	}
	return 0;
}

//----------------------------------------------------------------//
ZIPFSVirtualPath* ZIPFSFileSystem::FindVirtualPath ( char const* path ) {

	size_t len = 0;

	ZIPFSVirtualPath* cursor = this->mVirtualPaths;
	for ( ; cursor; cursor = cursor->mNext ) {
	
		const char* test = cursor->mPath.c_str ();
		len = ComparePaths ( test, path );
		
		if ( !( test [ len ] || path [ len ])) break;
	}
	return cursor;
}

//----------------------------------------------------------------//
ZIPFSFileSystem& ZIPFSFileSystem::Get () {

	static ZIPFSFileSystem zfs;
	return zfs;
}

//----------------------------------------------------------------//
string ZIPFSFileSystem::GetAbsoluteDirPath ( const char* path ) {

	string buffer = this->GetAbsoluteFilePath ( path );
	
	if ( buffer [ buffer.length () - 1 ] != '/' ) {
		buffer.push_back ( '/' );
	}
	return buffer;
}

//----------------------------------------------------------------//
string ZIPFSFileSystem::GetAbsoluteFilePath ( const char* path ) {

	// handle absolute paths
	if ( !path ) return ( char* )"/";
	
	if (( path [ 0 ] == '\\' ) || ( path [ 0 ] == '/' ) || ( path [ 1 ] == ':' )) {
		return BlessPath ( path );
	}
	
	string buffer = this->mWorkingPath;
	buffer.append ( path );
	return NormalizePath ( buffer.c_str ());
}

//----------------------------------------------------------------//
string ZIPFSFileSystem::GetBasename ( const char* filename ) {

	char* token;
	char* lastToken = 0;
	
	char* file = ( char* )filename;
	char delim = '/';
	
	for ( token = strtok ( file, &delim ); token; token = strtok ( NULL, &delim )) {
		lastToken = token;
	}

	return lastToken;
}

//----------------------------------------------------------------//
string ZIPFSFileSystem::GetRelativePath ( char const* path ) {

	int depth = 0;
	int same;

	if ( !path ) return 0;
	
	if ( path [ 0 ] == '.' ) {
		return BlessPath ( path );
	}
	
	if ( !(( path [ 0 ] == '/' ) || ( path [ 0 ] == '\\' ) || ( path [ 0 ] == ':' ))) {
		string buffer = "./";
		buffer.append ( path );
		return BlessPath ( buffer.c_str ());
	}

	string absPath = this->GetAbsoluteFilePath ( path );

	same = ComparePaths ( absPath.c_str (), this->mWorkingPath.c_str ());
	if ( same == 0 ) {
		return BlessPath ( path );
	}

	// count the number of steps up in the current directory
	for ( int i = same; this->mWorkingPath [ i ]; ++i ) {
		if (  this->mWorkingPath [ i ] == '/' ) {
			depth++;
		}
	}

	string relPath;

	for ( int i = 0; i < depth; ++i ) {
		relPath.append ( "../" );
	}
	
	return relPath.append ( absPath.substr ( same ));
}

//----------------------------------------------------------------//
std::string ZIPFSFileSystem::GetWorkingPath () {

	return this->mWorkingPath;
}

//----------------------------------------------------------------//
void ZIPFSFileSystem::Init () {

	this->mMutex = zipfs_mutex_create ();;

	char buffer [ FILENAME_MAX ];
	char* result = getcwd ( buffer, FILENAME_MAX );
	assert ( result );
	
	this->mWorkingPath = BlessPath ( buffer );
	
	if ( this->mWorkingPath [ this->mWorkingPath.length () - 1 ] != '/' ) {
		this->mWorkingPath.push_back ( '/' );
	}
}

//----------------------------------------------------------------//
bool ZIPFSFileSystem::IsSeparator ( char c ) {

	return ( c == '/' ) || ( c == '\\' ); 
}

//----------------------------------------------------------------//
bool ZIPFSFileSystem::IsVirtualPath ( char const* path ) {

	size_t len = 0;
	ZIPFSVirtualPath* cursor = this->mVirtualPaths;
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		const char* test = cursor->mPath.c_str ();
		len = ComparePaths ( test, path );
		
		if ( !test [ len ]) return true;
	}
	return false;
}

//----------------------------------------------------------------//
int ZIPFSFileSystem::MakeDir ( char const* path ) {

	if ( !path ) return -1;
	if ( this->IsVirtualPath ( path )) return -1;
	
	#ifdef _WIN32
		return mkdir ( path );
	#else
		return mkdir ( path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
	#endif
}

//----------------------------------------------------------------//
int ZIPFSFileSystem::MountVirtual ( const char* path, const char* archive ) {

	// delete the path if it exists
	int result;
	ZIPFSVirtualPath* cursor = this->mVirtualPaths;
	ZIPFSVirtualPath* virtualPath;
	ZIPFSVirtualPath* list = 0;
	
	if ( !path ) return -1;
	string abspath = this->GetAbsoluteDirPath ( path );
	path = abspath.c_str ();
	
	while ( cursor ) {
		virtualPath = cursor;
		cursor = cursor->mNext;
		
		if ( strcmp_ignore_case ( virtualPath->mPath.c_str (), path ) == 0 ) {
			delete virtualPath;
		}
		else {
			list = virtualPath->PushFront ( list );
		}
	}
	
	cursor = list;
	this->mVirtualPaths = 0;
	
	while ( cursor ) {
		virtualPath = cursor;
		cursor = cursor->mNext;
		
		this->mVirtualPaths = virtualPath->PushFront ( this->mVirtualPaths );
	}

	if ( !archive ) return 0;

	virtualPath = new ZIPFSVirtualPath ();
	if ( !virtualPath ) goto error;

	result = virtualPath->SetPath ( path );
	if ( result ) goto error;
	
	result = virtualPath->SetArchive ( this->GetAbsoluteFilePath ( archive ).c_str ());
	if ( result ) goto error;

	this->mVirtualPaths = virtualPath->PushFront ( this->mVirtualPaths );

	return 0;

error:

	if ( virtualPath ) {
		delete virtualPath;
	}
	return -1;
}

//----------------------------------------------------------------//
string ZIPFSFileSystem::NormalizePath ( const char* path ) {

	size_t i = 0;
	size_t top = 0;

	string buffer = BlessPath ( path );

	// normalize the path
	for ( ; buffer [ i ]; ++i ) {
		
		if ( buffer [ i ] == '.' ) {
		
			if ( buffer [ i + 1 ] == '/' ) {
				i += 1;
				continue;
			}
			
			if (( buffer [ i + 1 ] == '.' ) && ( buffer [ i + 2 ] == '/' )) {

				size_t j = top;
				for ( ; j > 0; --j ) {
					if ( buffer [ j ] == '/' ) {
					
						size_t k = j - 1;
						for ( ; k > 0; --k ) {
							
							if ( buffer [ k ] == '/' ) {
								top = k + 1;
								buffer [ top ] = 0;
								break;
							}
						}
						break;
					}
				}
				i += 2;
				continue;
			}
		}
		
		buffer [ top++ ] = buffer [ i ];
	}
	
	buffer [ top ] = 0;
	return buffer;
}

//----------------------------------------------------------------//
int ZIPFSFileSystem::Remove ( const char* path ) {

	if ( this->IsVirtualPath ( path )) return -1;
	return remove ( path );
}

//----------------------------------------------------------------//
int ZIPFSFileSystem::RemoveDir ( const char* path ) {

	if ( this->IsVirtualPath ( path )) return -1;
	return rmdir ( path );
}

//----------------------------------------------------------------//
int ZIPFSFileSystem::Rename ( const char* oldname, const char* newname ) {

	if ( this->IsVirtualPath ( oldname )) return -1;
	if ( this->IsVirtualPath ( newname )) return -1;
	
	return rename ( oldname, newname );
}

//----------------------------------------------------------------//
string ZIPFSFileSystem::TruncateFilename ( const char* filename ) {

	string buffer = ZIPFSFileSystem::BlessPath ( filename );
	filename = buffer.c_str ();
	
	int len = 0;
	for ( int i = 0; filename [ i ]; ++i ) {
		if ( filename [ i ] == '/' ) {
			len = i + 1;
		}
	}

	return buffer.substr ( 0, len );
}