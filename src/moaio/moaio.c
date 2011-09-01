// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaio\moaio.h>
#include <moaio\moaio_util.h>
#include <moaio\MOAIOString.h>
#include <moaio\MOAIOVirtualPath.h>

//================================================================//
// MOAIOFile
//================================================================//
typedef struct MOAIOFile {

	int		mIsVirtual;
	void*	mPtr;

} MOAIOFile;

//================================================================//
// local
//================================================================//

static MOAIOString* sWorkingPath;
static MOAIOString* sBuffer;

static MOAIOVirtualPath* sVirtualPaths = 0;
static MOAIOVirtualPath* sVirtual = 0;

//----------------------------------------------------------------//
size_t compare_paths ( const char* p0, const char* p1 ) {

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
MOAIOVirtualPath* find_best_virtual_path ( char const* path ) {

	size_t len = 0;
	size_t bestlen = 0;
	MOAIOVirtualPath* best = 0;
	MOAIOVirtualPath* cursor = sVirtualPaths;
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		char* test = cursor->mPath;
		for ( len = 0; test [ len ] && ( path [ len ] == test [ len ]); ++len );
	
		if (( !test [ len ]) && ( len > bestlen )) {
			best = cursor;
			bestlen = len;
		}		
	}
	return best;
}

//----------------------------------------------------------------//
MOAIOVirtualPath* find_virtual_path ( char const* path ) {

	MOAIOVirtualPath* cursor = sVirtualPaths;
	for ( ; cursor; cursor = cursor->mNext ) {
		if ( strcmp ( cursor->mPath, path ) == 0 ) break;
	}
	return cursor;
}

//----------------------------------------------------------------//
char* resolve_working_path () {

	if ( sVirtual ) {
	
		size_t baselen;
	
		// try to mount the virtual path using physfs
		
		PHYSFS_mount ( sVirtual->mArchive, 0, 0 );
		
		baselen = strlen ( sVirtual->mPath );
		return &sWorkingPath->mMem [ baselen ];
	}
	
	// fall back on the regular file system
	chdir ( sWorkingPath->mMem );
	return sWorkingPath->mMem;
}

//================================================================//
// moaio
//================================================================//

//----------------------------------------------------------------//
int moaio_affirm_path ( const char* path ) {

	int result = 0;
	char* cursor;

	if ( !path ) return -1;

	moaio_get_abs_dirpath ( path );

	if ( find_best_virtual_path ( path )) return -1;
	
	cursor = sBuffer->mMem;
	if ( *cursor == '/' ) {
		++cursor;
	}
	
	while ( *cursor ) {
		
		// Advance to end of current directory name
		while (( *cursor ) && ( *cursor != '/' )) ++cursor;
		if ( !( *cursor )) break;
	
		*cursor = 0;

		result = mkdir ( sBuffer->mMem );
		
		if ( result && ( errno != EEXIST )) break;
		
		*cursor = '/';
		++cursor;
	}
	
	return result;
}

//----------------------------------------------------------------//
char* moaio_basename ( const char* filename ) {

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
char* moaio_bless_path ( const char* path ) {

	size_t i;
	size_t length = strlen ( path );
	
	MOAIOString_Grow ( sBuffer, length );
	
	for ( i = 0; i < length; ++i ) {
		char c = path [ i ];
		sBuffer->mMem [ i ] = ( c == '\\' ) ? '/' : c;
	}
	sBuffer->mMem [ length ] = 0;
	sBuffer->mStrLen = length;

	return sBuffer->mMem;
}

//----------------------------------------------------------------//
int moaio_chdir ( char* path ) {

	int result = 0;

	path = moaio_get_abs_dirpath ( path );
	sVirtual = find_best_virtual_path ( path );
	
	if ( !sVirtual ) {
		result = chdir ( path );
	}
	
	if ( result == 0 ) {
		MOAIOString_Set ( sWorkingPath, path );
	}

	return result;
}

//----------------------------------------------------------------//
void moaio_cleanup () {

	MOAIOVirtualPath* cursor = sVirtualPaths;
	while ( cursor ) {
		MOAIOVirtualPath* virtualPath = cursor;
		cursor = cursor->mNext;
		MOAIOVirtualPath_Delete ( virtualPath );
	}
	sVirtualPaths = 0;
	
	if ( sBuffer ) {
		free ( sBuffer );
		sBuffer = 0;
	}
	
	PHYSFS_deinit ();
}

//----------------------------------------------------------------//
int	moaio_fclose ( MOAIFILE fp ) {

	int result = 0;

	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		result = ( file->mIsVirtual ) ? PHYSFS_close (( PHYSFS_File* )file->mPtr ) : fclose (( FILE* )file->mPtr );
		free ( file );
	}
	return result;
}

//----------------------------------------------------------------//
int	moaio_fflush ( MOAIFILE fp ) {
	
	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		return ( file->mIsVirtual ) ? PHYSFS_flush (( PHYSFS_File* )file->mPtr ) : fflush (( FILE* )file->mPtr );
	}
	return 0;
}

//----------------------------------------------------------------//
int moaio_fgetc ( MOAIFILE fp ) {
	
	int result = EOF;

	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		
		if ( file->mIsVirtual ) {
			char c;
			result = ( int )PHYSFS_read (( PHYSFS_File* )file->mPtr, &c, 1, 1 );
			
			if ( result == 1 ) {
				result = c;
			}
		}
		else {
			result = fgetc (( FILE* )file->mPtr );
		}
	}
	return result;
}

//----------------------------------------------------------------//
char* moaio_fgets ( char* string, int length, MOAIFILE fp ) {
	
	int i = 0;
	int c = 0;

	if ( fp ) {
		
		MOAIOFile* file = ( MOAIOFile* )fp;
		
		if ( file->mIsVirtual ) {
		
			if ( length <= 1 ) return 0;

			do {
				c = moaio_fgetc ( fp );
				if ( c == EOF ) break;
				
				string [ i++ ] = ( char )c;
				if ( i >= length ) return 0;
				
			} while ( c && ( c != '\n' ));

			string [ i ] = 0;
			return string;
		}
		else {
			return fgets ( string, length, ( FILE* )file->mPtr );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIFILE moaio_fopen ( const char* filename, const char* mode ) {
	
	MOAIOFile* file = 0;

	if ( sVirtual ) {
		
		PHYSFS_File* physFile = 0;
		
		char* path = resolve_working_path ();
		MOAIOString_Set ( sBuffer, path );
		filename = MOAIOString_Append ( sBuffer, filename );
		
		if ( mode [ 0 ] == 'r' ) {
			physFile = PHYSFS_openRead ( filename );
		}
		
		if ( physFile ) {
			file = ( MOAIOFile* )malloc ( sizeof ( MOAIOFile ));
			file->mIsVirtual = TRUE;
			file->mPtr = physFile;
		}
	}
	else {
		
		FILE* stdFile = fopen ( filename, mode );
		
		if ( stdFile ) {
			file = ( MOAIOFile* )malloc ( sizeof ( MOAIOFile ));
			file->mIsVirtual = FALSE;
			file->mPtr = stdFile;
		}
	}

	return ( MOAIFILE )file;
}

//----------------------------------------------------------------//
size_t moaio_fread ( void* buffer, size_t size, size_t count, MOAIFILE fp ) {
	
	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		return ( file->mIsVirtual ) ? ( size_t )PHYSFS_read (( PHYSFS_File* )file->mPtr, buffer, size, count ) : fread ( buffer, size, count, ( FILE* )file->mPtr );
	}
	return 0;
}

//----------------------------------------------------------------//
int	moaio_fseek ( MOAIFILE fp, long offset, int origin ) {
	
	if ( fp ) {
	
		MOAIOFile* file = ( MOAIOFile* )fp;
		
		if ( file->mIsVirtual ) {
		
			int result;
			int length;
		
			length = ( int )PHYSFS_fileLength (( PHYSFS_File* )fp );
			
			switch ( origin ) {
				case SEEK_CUR:
					result = PHYSFS_seek (( PHYSFS_File* )fp, PHYSFS_tell (( PHYSFS_File* )file->mPtr ) + offset );
					break;
				case SEEK_END:
					result = PHYSFS_seek (( PHYSFS_File* )file->mPtr, length );
					break;
				case SEEK_SET:
					result = PHYSFS_seek (( PHYSFS_File* )file->mPtr, 0 );
					break;
				default:
					return -1;
			};
			if ( result == 0 ) return -1;
		}
		else {
			return fseek (( FILE* )fp, offset, origin );
		}
	}	
	return -1;
}

//----------------------------------------------------------------//
long moaio_ftell ( MOAIFILE fp ) {

	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		return ( file->mIsVirtual ) ? ( long )PHYSFS_tell (( PHYSFS_File* )file->mPtr ) : ftell (( FILE* )file->mPtr );
	}
	return -1L;
}

//----------------------------------------------------------------//
size_t moaio_fwrite ( const void* data, size_t size, size_t count, MOAIFILE fp ) {
	
	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		if ( file->mPtr ) {
			fwrite ( data, size, count, ( FILE* )file->mPtr );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
char* moaio_get_abs_filepath ( const char* path ) {

	char* buffer;
	
	// handle absolute paths
	if ( !path ) return ( char* )"/";
	
	if (( path [ 0 ] == '\\' ) || ( path [ 0 ] == '/' ) || ( path [ 1 ] == ':' )) {
		return moaio_bless_path ( path );
	}
	
	buffer = moaio_get_working_path ();
	buffer = MOAIOString_Append ( sBuffer, path );

	return moaio_normalize_path ( buffer );
}
//----------------------------------------------------------------//
char* moaio_get_abs_dirpath ( const char* path ) {

	moaio_get_abs_filepath ( path );
	
	if ( sBuffer->mMem [ sBuffer->mStrLen ] != '/' ) {
		MOAIOString_Append ( sBuffer, "/" );
	}
	return sBuffer->mMem;
}

//----------------------------------------------------------------//
char* moaio_get_rel_path ( const char* path ) {

	size_t depth = 0;
	size_t i = 0;
	size_t same;

	if ( !path ) return 0;
	
	if ( path [ 0 ] == '.' ) {
		return moaio_bless_path ( path );
	}
	
	if ( !(( path [ 0 ] == '/' ) || ( path [ 0 ] == '\\' ) || ( path [ 0 ] == ':' ))) {
		MOAIOString_Set ( sBuffer, "./" );
		MOAIOString_Append ( sBuffer, path );
		moaio_bless_path ( sBuffer->mMem );
		return sBuffer->mMem;
	}

	// store the expanded path in sBuffer
	moaio_get_abs_filepath ( path );

	same = compare_paths ( path, sWorkingPath->mMem );
	if ( same == 0 ) {
		return moaio_bless_path ( path );
	}

	// count the number of steps up in the current directory
	for ( i = same; sWorkingPath->mMem [ i ]; ++i ) {
		if (  sWorkingPath->mMem [ i ] == '/' ) {
			depth++;
		}
	}

	MOAIOString_Shift ( sBuffer, same, sBuffer->mStrLen - same, depth * 3 );

	sBuffer->mStrLen = sBuffer->mStrLen - same + ( depth * 3 );
	sBuffer->mMem [ sBuffer->mStrLen ] = 0;

	for ( i = 0; i < depth; ++i ) {
	
		size_t j = i * 3;
	
		sBuffer->mMem [ j + 0 ] = '.';
		sBuffer->mMem [ j + 1 ] = '.';
		sBuffer->mMem [ j + 2 ] = '/';
	}
	
	return sBuffer->mMem;
}

//----------------------------------------------------------------//
char* moaio_get_working_path () {

	char* result = 0;
	
	if ( !sWorkingPath->mStrLen ) {
	
		MOAIOString_Grow ( sBuffer, MOAIO_STRING_BLOCK_SIZE );
		
		while ( !result ) {
			result = getcwd ( sBuffer->mMem, sBuffer->mSize );
			if ( !result ) {
				MOAIOString_Grow ( sBuffer, sBuffer->mSize + MOAIO_STRING_BLOCK_SIZE );
			}
			else {
				size_t pathlen = strlen ( result );
				sBuffer->mStrLen = pathlen;
				
				if ( result [ pathlen ] != '/' ) {
					MOAIOString_Append ( sBuffer, "/" );
				}
			}
		}
		moaio_bless_path ( result );
		
		MOAIOString_Set ( sWorkingPath, result );
	}
	
	MOAIOString_Set ( sBuffer, sWorkingPath->mMem );
	return sBuffer->mMem;
}

//----------------------------------------------------------------//
char* moaio_getcwd ( char* buffer, size_t length ) {

	if ( sWorkingPath->mSize < length ) return 0;
	strcpy ( buffer, sWorkingPath->mMem );
	return buffer;
}

//----------------------------------------------------------------//
void moaio_init () {

	sWorkingPath = MOAIOString_New ();
	sBuffer = MOAIOString_New ();
	
	moaio_get_working_path ();
	
	PHYSFS_init ( "moaio" );
}

//----------------------------------------------------------------//
int moaio_mkdir ( const char* path ) {

	if ( !path ) return -1;

	moaio_get_abs_dirpath ( path );

	if ( find_best_virtual_path ( path )) return -1;
	
	return mkdir ( sBuffer->mMem );
}

//----------------------------------------------------------------//
char* moaio_normalize_path ( const char* path ) {

	size_t i = 0;
	size_t top = 0;

	size_t length = strlen ( path );
	char* buffer = MOAIOString_Grow ( sBuffer, length );

	buffer = moaio_bless_path ( path );

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
int moaio_rmdir ( const char* path ) {

	return 0;
}

//----------------------------------------------------------------//
void moaio_set_virtual_path ( const char* path, const char* archive ) {

	if ( !path ) return;

	path = moaio_get_abs_dirpath ( path );

	if ( archive ) {
		// affirm the path
		
		MOAIOVirtualPath* virtualPath = find_virtual_path ( path );
		
		if ( !virtualPath ) {
			virtualPath = MOAIOVirtualPath_New ();
			sVirtualPaths = MOAIOVirtualPath_PushFront ( virtualPath, sVirtualPaths );
		}
		
		MOAIOVirtualPath_SetPath ( virtualPath, path );
		MOAIOVirtualPath_SetArchive ( virtualPath, moaio_get_abs_filepath ( archive ));
	}
	else {
		// delete the path
		
		MOAIOVirtualPath* cursor = sVirtualPaths;
		MOAIOVirtualPath* virtualPath;
		MOAIOVirtualPath* list = 0;
		
		while ( cursor ) {
			virtualPath = cursor;
			cursor = cursor->mNext;
			
			if ( strcmp ( virtualPath->mPath, path ) == 0 ) {
				MOAIOVirtualPath_Delete ( virtualPath );
			}
			else {
				list = MOAIOVirtualPath_PushFront ( virtualPath, list );
			}
		}
		
		cursor = list;
		while ( cursor ) {
			virtualPath = cursor;
			cursor = cursor->mNext;
			
			sVirtualPaths = MOAIOVirtualPath_PushFront ( virtualPath, sVirtualPaths );
		}
	}
	return;
}
