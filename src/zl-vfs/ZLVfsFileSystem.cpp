// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_config.h>
#ifdef NACL
	#include "NaClFile.h"
#endif
#include <zl-vfs/zl_util.h>
#include <zl-vfs/ZLVfsFileSystem.h>
#include <zl-vfs/ZLVfsVirtualPath.h>

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
// ZLVfsFileSystem
//================================================================//

//----------------------------------------------------------------//
int ZLVfsFileSystem::AffirmPath ( const char* path ) {

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
	errno = 0;
	while ( *cursor ) {

		// Advance to end of current directory name
		while (( *cursor ) && ( *cursor != '/' )) ++cursor;
		if ( !( *cursor )) break;

		*cursor = 0;
		
		#ifdef _WIN32
			result = _mkdir ( buffer );
		#else
			result = mkdir ( buffer, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
		#endif
		
		if ( ( result == 0 ) && ( errno != EEXIST )) break;
		result = 0;

		*cursor = '/';
		++cursor;
	}

	return result;
}

//----------------------------------------------------------------//
string ZLVfsFileSystem::BlessPath ( const char* path ) {

	string blessed;

	for ( size_t i = 0; path [ i ]; ++i ) {
		
		char c = path [ i ];
		
		if ( IsSeparator ( c )) {
		
			c = '/';
			while ( IsSeparator ( path [ ++i ]));
			--i;
		}
		
		blessed += c;
	}
	
	return blessed;
}

//----------------------------------------------------------------//
int ZLVfsFileSystem::ChangeDir ( const char* path ) {

	int result = -1;
	ZLVfsVirtualPath* mount = 0;

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
void ZLVfsFileSystem::Cleanup () {

	ZLVfsVirtualPath* cursor = this->mVirtualPaths;
	while ( cursor ) {
		ZLVfsVirtualPath* virtualPath = cursor;
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
size_t ZLVfsFileSystem::ComparePaths ( const char* p0, const char* p1 ) {

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
ZLVfsVirtualPath* ZLVfsFileSystem::FindBestVirtualPath ( char const* path ) {
	
	size_t len = 0;
	size_t bestlen = 0;
	ZLVfsVirtualPath* best = 0;
	ZLVfsVirtualPath* cursor = this->mVirtualPaths;
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		const char* test = cursor->mPath.c_str ();
		len = ComparePaths ( test, path );
	
		if (( test [ len ] == 0 ) && ( len > bestlen )) {
			best = cursor;
			bestlen = len;
		}		
	}
	return best;
}

//----------------------------------------------------------------//
ZLVfsVirtualPath* ZLVfsFileSystem::FindNextVirtualSubdir ( char const* path, ZLVfsVirtualPath* cursor ) {

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
ZLVfsVirtualPath* ZLVfsFileSystem::FindVirtualPath ( char const* path ) {

	size_t len = 0;

	ZLVfsVirtualPath* cursor = this->mVirtualPaths;
	for ( ; cursor; cursor = cursor->mNext ) {
	
		const char* test = cursor->mPath.c_str ();
		len = ComparePaths ( test, path );
		
		if ( !( test [ len ] || path [ len ])) break;
	}
	return cursor;
}

//----------------------------------------------------------------//
ZLVfsFileSystem& ZLVfsFileSystem::Get () {

	static ZLVfsFileSystem zfs;
	return zfs;
}

//----------------------------------------------------------------//
string ZLVfsFileSystem::GetAbsoluteDirPath ( const char* path ) {

	if ( !path ) return ( char* )"/";
	string abspath = this->GetAbsolutePath ( path );
	if ( abspath [ abspath.length () - 1 ] != '/' ) {
		abspath.push_back ( '/' );
	}
	return abspath;
}

//----------------------------------------------------------------//
string ZLVfsFileSystem::GetAbsoluteFilePath ( const char* path ) {

	if ( !path ) return ( char* )"";
	return this->GetAbsolutePath ( path );
}

//----------------------------------------------------------------//
std::string ZLVfsFileSystem::GetAbsolutePath ( const char* path ) {

	if ( !path ) return ( char* )"";
	
	string norm = this->PathFromRef ( path );
	
	if ( this->mFileRemapCallback ) {
		norm = this->mFileRemapCallback ( norm.c_str ());
	}
	
	if (( norm [ 0 ] == '\\' ) || ( norm [ 0 ] == '/' ) || ( norm [ 0 ] && ( norm [ 1 ] == ':' ))) {
		return this->NormalizeFilePath ( norm.c_str ());
	}
	
	string buffer = this->GetWorkingPath (); // use accessor for thread safety
	buffer.append ( norm );
	return this->NormalizeFilePath ( buffer.c_str ());
}

//----------------------------------------------------------------//
string ZLVfsFileSystem::GetBasename ( const char* filename ) {

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
std::string ZLVfsFileSystem::GetRelativePath ( const char* path, const char* base ) {

	if ( !path ) return 0;

	string workpath = base ? base : this->GetWorkingPath ();

	int depth = 0;
	int same;

	string abspath = this->GetAbsoluteFilePath ( path );
	
	same = ComparePaths ( abspath.c_str (), workpath.c_str ());
	if ( same == 0 ) {
		return BlessPath ( path );
	}

	// count the number of steps up in the current directory
	for ( int i = same; workpath [ i ]; ++i ) {
		if ( base [ i ] == '/' ) {
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
std::string ZLVfsFileSystem::GetWorkingPath () {

	// PCM: I'm not entirely sure about stl string's thread safety due to
	// copy-on-write. Going to explicitely force a copy here.

	zl_mutex_lock ( this->mMutex );
	std::string buffer = this->mWorkingPath.c_str ();
	zl_mutex_unlock ( this->mMutex );

	return buffer;
}

//----------------------------------------------------------------//
void ZLVfsFileSystem::Init () {

	this->mMutex = zl_mutex_create ();;
	
	#ifdef ANDROID
		this->mWorkingPath = this->NormalizeDirPath ( "/" );
	#else
		char buffer [ FILENAME_MAX ];
	
		char* result = getcwd ( buffer, FILENAME_MAX );
		assert ( result );
		
		this->mWorkingPath = this->NormalizeDirPath ( buffer );
	#endif
}

//----------------------------------------------------------------//
bool ZLVfsFileSystem::IsSeparator ( char c ) {

	return ( c == '/' ) || ( c == '\\' ); 
}

//----------------------------------------------------------------//
bool ZLVfsFileSystem::IsVirtualPath ( char const* path ) {

	size_t len = 0;
	ZLVfsVirtualPath* cursor = this->mVirtualPaths;
	
	string abspath = this->GetAbsoluteFilePath ( path );
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		const char* test = cursor->mPath.c_str ();
		len = ComparePaths ( test, abspath.c_str ());
		
		if ( !test [ len ]) return true;
	}
	return false;
}

//----------------------------------------------------------------//
int ZLVfsFileSystem::MakeDir ( char const* path ) {

	if ( !path ) return -1;
	
	string abspath = this->GetAbsoluteFilePath ( path );
	if ( this->IsVirtualPath ( abspath.c_str ())) return -1;
	
	#ifdef _WIN32
		return mkdir ( abspath.c_str ());
	#else
		return mkdir ( abspath.c_str (), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
	#endif
}

//----------------------------------------------------------------//
int ZLVfsFileSystem::MountVirtual ( const char* path, const char* archive ) {

	// delete the path if it exists
	int result;
	ZLVfsVirtualPath* cursor = this->mVirtualPaths;
	ZLVfsVirtualPath* virtualPath;
	ZLVfsVirtualPath* list = 0;
	
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

	virtualPath = new ZLVfsVirtualPath ();
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
string ZLVfsFileSystem::NormalizeDirPath ( const char* path ) {

	string result = NormalizeFilePath ( path );
	if ( result [ result.length () - 1 ] != '/' ) {
		result.push_back ( '/' );
	}
	return result;
}

//----------------------------------------------------------------//
string ZLVfsFileSystem::NormalizeFilePath ( const char* path ) {

	size_t i = 0;
	size_t top = 0;

	string buffer = BlessPath ( path );
	buffer.push_back ( 0 );

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
string ZLVfsFileSystem::PathFromRef ( const char* path ) {

	if ( path ) {
		if ( path [ 0 ] != '<' ) return path;
		
		for ( size_t i = 0; path [ i ]; ++i ) {
		
			if ( path [ i ] == '>' ) {
			
				string fullpath = path;
				string ref = fullpath.substr ( 0, i + 1 );
				
				if ( this->mReferenceMap.find ( ref ) != this->mReferenceMap.end ()) {
					return this->mReferenceMap [ ref ] + fullpath.substr ( i + 1 );
				}
				
				break;
			}
		}
	}
	return "";
}

//----------------------------------------------------------------//
string ZLVfsFileSystem::PathToRef ( const char* path ) {

	if ( this->mReferenceMap.size ()) {
	
		string abspath = this->GetAbsolutePath ( path );

		ReferenceMapIt refIt = this->mReferenceMap.begin ();
		for ( ; refIt != this->mReferenceMap.end (); ++refIt ) {
		
			string refPath = refIt->second;
			size_t refPathLen = refPath.size ();
		
			if (( refPathLen <= abspath.size ()) && ( refPath.compare ( 0, string::npos, abspath, 0, refPathLen ) == 0 )) {
				return refIt->first + abspath.substr ( refPathLen );
			}
		}
	}
	return path;
}

//----------------------------------------------------------------//
int ZLVfsFileSystem::Remove ( const char* path ) {

	string abspath = this->GetAbsolutePath ( path );

	if ( this->IsVirtualPath ( abspath.c_str ())) return -1;
	return remove ( abspath.c_str ());
}

//----------------------------------------------------------------//
int ZLVfsFileSystem::RemoveDir ( const char* path ) {

	string abspath = this->GetAbsolutePath ( path );

	if ( this->IsVirtualPath ( abspath.c_str ())) return -1;
	return rmdir ( abspath.c_str ());
}

//----------------------------------------------------------------//
int ZLVfsFileSystem::Rename ( const char* oldname, const char* newname ) {

	string oldnameAbs = this->GetAbsolutePath ( oldname );
	string newnameAbs = this->GetAbsolutePath ( newname );

	if ( this->IsVirtualPath ( oldnameAbs.c_str ())) return -1;
	if ( this->IsVirtualPath ( newnameAbs.c_str ())) return -1;
	
	return rename ( oldnameAbs.c_str (), newnameAbs.c_str ());
}

//----------------------------------------------------------------//
void ZLVfsFileSystem::SetFileRemapCallback ( FileRemapCallback callbackFct ) {

	this->mFileRemapCallback = callbackFct;
}

//----------------------------------------------------------------//
void ZLVfsFileSystem::SetPathRef ( const char* referenceName, const char* path ) {

	string key = "<";
	key.append ( referenceName );
	key.append ( ">" );

	if ( path ) {
		this->mReferenceMap [ key ] = this->GetAbsolutePath ( path );
	}
	else if ( this->mReferenceMap.find ( key ) != this->mReferenceMap.end ()) {
		this->mReferenceMap.erase ( key );
	}
}

//----------------------------------------------------------------//
string ZLVfsFileSystem::TruncateFilename ( const char* filename ) {

	string buffer = ZLVfsFileSystem::BlessPath ( filename );
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
ZLVfsFileSystem::ZLVfsFileSystem () :
	mMutex ( 0 ),
	mVirtualPaths ( 0 ),
	mFileRemapCallback ( 0 ) {
}

//----------------------------------------------------------------//
ZLVfsFileSystem::~ZLVfsFileSystem () {

	this->Cleanup ();
}
