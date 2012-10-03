// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifdef NACL
	#include "NaClFile.h"
#endif
#include <zlcore/zl_util.h>
#include <zlcore/ZLFileSystem.h>
#include <zlcore/ZLVirtualPath.h>

#ifdef _WIN32
	#include <direct.h>
	#include <io.h>
#else
	#include <sys/types.h>
	#include <dirent.h>
	#include <unistd.h>
#endif

#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <tlsf.h>

using namespace std;

//================================================================//
// ZLFileSystem
//================================================================//

//----------------------------------------------------------------//
int ZLFileSystem::AffirmPath ( const char* path ) {

	if ( !path ) return -1;

	string abspath = this->GetAbsoluteDirPath ( path );
	if ( this->IsVirtualPath ( abspath.c_str ())) return -1;
	
	if ( abspath.length () > FILENAME_MAX ) return -1;
	
	char buffer [ FILENAME_MAX ];
	strcpy ( buffer, abspath.c_str ());
	char* cursor = buffer;
	
	if ( cursor [ 0 ] && ( cursor [ 1 ] == ':' )) {
		cursor = &cursor [ 2 ];
	}
	
	if ( *cursor == '/' ) {
		++cursor;
	}

	int result = 0;
	while ( *cursor ) {

		// Advance to end of current directory name
		while (( *cursor ) && ( *cursor != '/' )) ++cursor;
		if ( !( *cursor )) break;

		*cursor = 0;
		
		#ifdef _WIN32
			result = mkdir ( buffer );
		#else
			result = mkdir ( buffer, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
		#endif
		
		if ( result && ( errno != EEXIST )) break;
		result = 0;

		*cursor = '/';
		++cursor;
	}

	return result;
}

//----------------------------------------------------------------//
string ZLFileSystem::BlessPath ( const char* path ) {

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
int ZLFileSystem::ChangeDir ( const char* path ) {

	int result = -1;
	ZLVirtualPath* mount = 0;

	string absDirpath = this->GetAbsoluteDirPath ( path );
	path = absDirpath.c_str ();
	
	// engaging mutex lock early; want to protect entire operation
	// but especially the changes to mWorkingPath
	zl_mutex_lock ( this->mMutex );
	
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
	
	// done w/ the lock
	zl_mutex_unlock ( this->mMutex );
	
	return result;
}

//----------------------------------------------------------------//
void ZLFileSystem::Cleanup () {

	ZLVirtualPath* cursor = this->mVirtualPaths;
	while ( cursor ) {
		ZLVirtualPath* virtualPath = cursor;
		cursor = cursor->mNext;
		delete virtualPath;
	}

	if ( this->mMutex ) {
		zl_mutex_destroy ( this->mMutex );
		this->mMutex = 0;
	}
	
	this->mVirtualPaths = 0;
	this->mMutex = 0;
}

//----------------------------------------------------------------//
size_t ZLFileSystem::ComparePaths ( const char* p0, const char* p1 ) {

	size_t i;
	size_t same = 0;

	for ( i = 0; p0 [ i ]; ++i ) {
		
		char h = ( char )tolower ( p0 [ i ]);
		char v = ( char )tolower ( p1 [ i ]);
		
		if (( h != v ) && ( v != 0 )) break;
		
		if ( h == '/' ) {
			same = i + 1;
		}
		
		if ( v == 0 ) break;
	}

	return same;
}

//----------------------------------------------------------------//
ZLVirtualPath* ZLFileSystem::FindBestVirtualPath ( char const* path ) {

	size_t len = 0;
	size_t bestlen = 0;
	ZLVirtualPath* best = 0;
	ZLVirtualPath* cursor = this->mVirtualPaths;
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		const char* test = cursor->mPath.c_str ();
		len = ComparePaths ( test, path );
	
		if ((( test [ len ] == 0 )) && ( len > bestlen )) {
			best = cursor;
			bestlen = len;
		}		
	}
	return best;
}

//----------------------------------------------------------------//
ZLVirtualPath* ZLFileSystem::FindNextVirtualSubdir ( char const* path, ZLVirtualPath* cursor ) {

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
ZLVirtualPath* ZLFileSystem::FindVirtualPath ( char const* path ) {

	size_t len = 0;

	ZLVirtualPath* cursor = this->mVirtualPaths;
	for ( ; cursor; cursor = cursor->mNext ) {
	
		const char* test = cursor->mPath.c_str ();
		len = ComparePaths ( test, path );
		
		if ( !( test [ len ] || path [ len ])) break;
	}
	return cursor;
}

//----------------------------------------------------------------//
ZLFileSystem& ZLFileSystem::Get () {

	static ZLFileSystem zfs;
	return zfs;
}

//----------------------------------------------------------------//
string ZLFileSystem::GetAbsoluteDirPath ( const char* path ) {

	if ( !path ) return ( char* )"/";
	
	if (( path [ 0 ] == '\\' ) || ( path [ 0 ] == '/' ) || ( path [ 0 ] && ( path [ 1 ] == ':' ))) {
		return NormalizeDirPath ( path );
	}

	string buffer = this->GetWorkingPath (); // use accessor for thread safety
	buffer.append ( path );
	buffer = NormalizeDirPath ( buffer.c_str ());
	
	return buffer;
}

//----------------------------------------------------------------//
string ZLFileSystem::GetAbsoluteFilePath ( const char* path ) {

	if ( !path ) return ( char* )"/";
	
	if (( path [ 0 ] == '\\' ) || ( path [ 0 ] == '/' ) || ( path [ 0 ] && ( path [ 1 ] == ':' ))) {
		return NormalizeFilePath ( path );
	}
	
	string buffer = this->GetWorkingPath (); // use accessor for thread safety
	buffer.append ( path );
	buffer = NormalizeFilePath ( buffer.c_str ());
	
	return buffer;
}

//----------------------------------------------------------------//
string ZLFileSystem::GetBasename ( const char* filename ) {

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
string ZLFileSystem::GetRelativePath ( char const* path ) {

	int depth = 0;
	int same;

	if ( !path ) return 0;

	string abspath = this->GetAbsoluteFilePath ( path );
	string workpath = this->GetWorkingPath ();
	
	same = ComparePaths ( abspath.c_str (), workpath.c_str ());
	if ( same == 0 ) {
		return BlessPath ( path );
	}

	// count the number of steps up in the current directory
	for ( int i = same; workpath [ i ]; ++i ) {
		if ( workpath [ i ] == '/' ) {
			depth++;
		}
	}

	string relPath;

	for ( int i = 0; i < depth; ++i ) {
		relPath.append ( "../" );
	}
	
	return relPath.append ( abspath.substr ( same ));
}

//----------------------------------------------------------------//
std::string ZLFileSystem::GetWorkingPath () {

	// PCM: I'm not entirely sure about stl string's thread safety due to
	// copy-on-write. Going to explicitely force a copy here.

	zl_mutex_lock ( this->mMutex );
	std::string buffer = this->mWorkingPath.c_str ();
	zl_mutex_unlock ( this->mMutex );

	return buffer;
}

//----------------------------------------------------------------//
void ZLFileSystem::Init () {

	this->mMutex = zl_mutex_create ();;

	char buffer [ FILENAME_MAX ];

	char* result = getcwd ( buffer, FILENAME_MAX );
	assert ( result );
	
	this->mWorkingPath = this->NormalizeDirPath ( buffer );
}

//----------------------------------------------------------------//
bool ZLFileSystem::IsSeparator ( char c ) {

	return ( c == '/' ) || ( c == '\\' ); 
}

//----------------------------------------------------------------//
bool ZLFileSystem::IsVirtualPath ( char const* path ) {

	size_t len = 0;
	ZLVirtualPath* cursor = this->mVirtualPaths;
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		const char* test = cursor->mPath.c_str ();
		len = ComparePaths ( test, path );
		
		if ( !test [ len ]) return true;
	}
	return false;
}

//----------------------------------------------------------------//
int ZLFileSystem::MakeDir ( char const* path ) {

	if ( !path ) return -1;
	if ( this->IsVirtualPath ( path )) return -1;
	
	#ifdef _WIN32
		return mkdir ( path );
	#else
		return mkdir ( path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
	#endif
}

//----------------------------------------------------------------//
int ZLFileSystem::MountVirtual ( const char* path, const char* archive ) {

	// delete the path if it exists
	int result;
	ZLVirtualPath* cursor = this->mVirtualPaths;
	ZLVirtualPath* virtualPath;
	ZLVirtualPath* list = 0;
	
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

	virtualPath = new ZLVirtualPath ();
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
string ZLFileSystem::NormalizeDirPath ( const char* path ) {

	string result = NormalizeFilePath ( path );
	if ( result [ result.length () - 1 ] != '/' ) {
		result.push_back ( '/' );
	}
	return result;
}

//----------------------------------------------------------------//
string ZLFileSystem::NormalizeFilePath ( const char* path ) {

	size_t i = 0;
	size_t top = 0;

	string buffer = BlessPath ( path );
	buffer.resize ( buffer.length() + 1 );

	// normalize the path
	for ( ; buffer [ i ]; ++i ) {
		
		if ( buffer [ i ] == '.' ) {
		
			if ( buffer [ i + 1 ] == '/' || buffer [ i + 1 ] == 0 ) {
				if ( buffer [ i + 1 ] != 0 ) i += 1;
				continue;
			}
			
			if (( buffer [ i + 1 ] == '.' ) && (( buffer [ i + 2 ] == '/' ) || ( buffer [ i + 2 ] == 0 ))) {

				size_t j = top;
				for ( ; j > 0; --j ) {
					if ( buffer [ j ] == '/' ) {
					
						size_t k = j - 1;
						for ( ; k >= 0; --k ) {
							
							if ( buffer [ k ] == '/' ) {
								top = k + 1;
								buffer [ top ] = 0;
								break;
							}
						}
						break;
					}
				}
				
				if ( buffer [ i + 2 ] != 0 ) {
					i += 2;
				}
				else {
					i += 1;
				}
				continue;
			}
		}
		
		buffer [ top++ ] = buffer [ i ];
	}
	
	buffer [ top ] = 0;
	string result = buffer.c_str ();
	return result;
}

//----------------------------------------------------------------//
int ZLFileSystem::Remove ( const char* path ) {

	if ( this->IsVirtualPath ( path )) return -1;
	return remove ( path );
}

//----------------------------------------------------------------//
int ZLFileSystem::RemoveDir ( const char* path ) {

	if ( this->IsVirtualPath ( path )) return -1;
	return rmdir ( path );
}

//----------------------------------------------------------------//
int ZLFileSystem::Rename ( const char* oldname, const char* newname ) {

	if ( this->IsVirtualPath ( oldname )) return -1;
	if ( this->IsVirtualPath ( newname )) return -1;
	
	return rename ( oldname, newname );
}

//----------------------------------------------------------------//
string ZLFileSystem::TruncateFilename ( const char* filename ) {

	string buffer = ZLFileSystem::BlessPath ( filename );
	filename = buffer.c_str ();
	
	int len = 0;
	for ( int i = 0; filename [ i ]; ++i ) {
		if ( filename [ i ] == '/' ) {
			len = i + 1;
		}
	}

	return buffer.substr ( 0, len );
}

//----------------------------------------------------------------//
ZLFileSystem::ZLFileSystem () :
	mMutex ( 0 ),
	mVirtualPaths ( 0 ) {
}

//----------------------------------------------------------------//
ZLFileSystem::~ZLFileSystem () {

	this->Cleanup ();
}