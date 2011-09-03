// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaio\moaio.h>
#include <moaio\moaio_util.h>
#include <moaio\MOAIOString.h>
#include <moaio\MOAIOVirtualPath.h>
#include <moaio\MOAIOZipFile.h>

//================================================================//
// MOAIOFile
//================================================================//
typedef struct MOAIOFile {

	int		mIsArchive;
	
	union {
		FILE*				mFile;
		MOAIOZipStream*		mZip;
	} mPtr;

} MOAIOFile;

//================================================================//
// MOAIODir
//================================================================//
typedef struct MOAIODir {

	MOAIOZipFileDir*		mZipFileDir;
	MOAIOZipFileDir*		mZipFileSubDir;
	MOAIOZipFileEntry*		mZipFileEntry;

	MOAIOString			mName;
	int					mIsDir;

} MOAIODir;

//----------------------------------------------------------------//
int MOAIODir_ReadZipEntry ( MOAIODir* self ) {

	if ( self->mZipFileSubDir ) {
		
		self->mZipFileSubDir = self->mZipFileSubDir->mNext;
		if ( !self->mZipFileSubDir ) {
		
			self->mZipFileEntry = self->mZipFileDir->mChildFiles;
			if ( !self->mZipFileEntry ) {
				self->mZipFileDir = 0;
			}
		}
	}
	else if ( self->mZipFileEntry  ) {
	
		self->mZipFileSubDir = self->mZipFileSubDir->mNext;
		if ( !self->mZipFileSubDir ) {
			self->mZipFileDir = 0;
		}
	}
	else {
		
		self->mZipFileSubDir = self->mZipFileDir->mChildDirs;
		if ( !self->mZipFileSubDir ) {
	
			self->mZipFileEntry = self->mZipFileDir->mChildFiles;
			if ( !self->mZipFileEntry ) {
				self->mZipFileDir = 0;
			}
		}
	}
	
	if ( self->mZipFileDir ) {
	
		if ( self->mZipFileSubDir ) {
			self->mIsDir = 1;
			MOAIOString_Set ( &self->mName, self->mZipFileSubDir->mName );
		}
		else if ( self->mZipFileEntry  ) {
			self->mIsDir = 0;
			MOAIOString_Set ( &self->mName, self->mZipFileEntry->mName );
		}
		return 1;
	}
	
	return 0;
}

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
		len = count_same ( test, path );
	
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
int is_separator ( char c ) {

	return ( c == '/' ) || ( c == '\\' ) ? 1 : 0; 

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

	size_t i = 0;
	size_t j = 0;
	
	for ( i = 0; path [ i ]; ++i ) {
		if ( is_separator ( path [ i ] )) {
			while ( is_separator ( path [ ++i ]));
			--i;
		}
		j++;
	}
	
	MOAIOString_Grow ( sBuffer, j );
	
	i = 0;
	j = 0;
	
	for ( i = 0; path [ i ]; ++i ) {
		
		char c = path [ i ];
		
		if ( is_separator ( c )) {
		
			c = '/';
			while ( is_separator ( path [ ++i ]));
			--i;
		}
		
		sBuffer->mMem [ j++ ] = c;
	}
	
	sBuffer->mMem [ j ] = 0;
	sBuffer->mStrLen = j;

	return sBuffer->mMem;
}

//----------------------------------------------------------------//
int moaio_chdir ( char* path ) {

	int result = -1;
	MOAIOVirtualPath* mount = 0;

	path = moaio_get_abs_dirpath ( path );
	mount = find_best_virtual_path ( path );
	
	if ( mount ) {
		const char* localpath = MOAIOVirtualPath_GetLocalPath ( mount, path );
		if ( localpath ) {
			sVirtual = mount;
			result = 0;
		}
	}
	else {
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
}

//----------------------------------------------------------------//
void moaio_dir_close ( MOAIODIR dir ) {

	MOAIODir* self = ( MOAIODir* )dir;
	MOAIOString_Clear ( &self->mName );
	memset ( self, 0, sizeof ( MOAIODir ));
	free ( self );
}

//----------------------------------------------------------------//
const int moaio_dir_entry_is_subdir ( MOAIODIR dir ) {

	MOAIODir* self = ( MOAIODir* )dir;
	return self->mIsDir;
}

//----------------------------------------------------------------//
const char* moaio_dir_entry_name ( MOAIODIR dir ) {

	MOAIODir* self = ( MOAIODir* )dir;
	return self->mName.mMem;
}

//----------------------------------------------------------------//
MOAIODIR moaio_dir_open () {

	MOAIODir* self = ( MOAIODir* )calloc ( 1, sizeof ( MOAIODir ));
	
	if ( sVirtual ) {
		char const* path = MOAIOVirtualPath_GetLocalPath ( sVirtual, sWorkingPath->mMem );
		self->mZipFileDir = MOAIOZipFile_FindDir ( sVirtual->mArchive, path );
	}
	return self;
}

//----------------------------------------------------------------//
int moaio_dir_read_entry ( MOAIODIR dir ) {

	MOAIODir* self = ( MOAIODir* )dir;

	if ( self->mZipFileDir ) {
		return MOAIODir_ReadZipEntry ( self );		
	}
	return 0;
}

//----------------------------------------------------------------//
int	moaio_fclose ( MOAIOFILE fp ) {

	int result = 0;

	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		result = ( file->mIsArchive ) ? MOAIOZipStream_Close ( file->mPtr.mZip ) : fclose ( file->mPtr.mFile );
		memset ( fp, 0, sizeof ( MOAIOFile ));
		free ( file );
	}
	return result;
}

//----------------------------------------------------------------//
int	moaio_fflush ( MOAIOFILE fp ) {
	
	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		if ( !file->mIsArchive ) {
			return fflush ( file->mPtr.mFile );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int moaio_fgetc ( MOAIOFILE fp ) {
	
	int result = EOF;

	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		
		if ( file->mIsArchive ) {
			char c;
			result = ( int )MOAIOZipStream_Read ( file->mPtr.mZip, &c, 1 );
			
			if ( result == 1 ) {
				result = c;
			}
		}
		else {
			result = fgetc ( file->mPtr.mFile );
		}
	}
	return result;
}

//----------------------------------------------------------------//
char* moaio_fgets ( char* string, int length, MOAIOFILE fp ) {
	
	int i = 0;
	int c = 0;

	if ( fp ) {
		
		MOAIOFile* file = ( MOAIOFile* )fp;
		
		if ( file->mIsArchive ) {
		
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
			return fgets ( string, length, file->mPtr.mFile );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIOFILE moaio_fopen ( const char* filename, const char* mode ) {
	
	MOAIOFile* file = 0;

	filename = moaio_get_abs_filepath ( filename );

	if ( sVirtual ) {
		
		if ( mode [ 0 ] == 'r' ) {
		
			MOAIOZipStream* zipStream;
			
			filename = MOAIOVirtualPath_GetLocalPath ( sVirtual, filename );
			zipStream = MOAIOZipStream_Open ( sVirtual->mArchive, filename );
			
			if ( zipStream ) {
				file = ( MOAIOFile* )malloc ( sizeof ( MOAIOFile ));
				file->mIsArchive = TRUE;
				file->mPtr.mZip = zipStream;
			}
		}
	}
	else {
		
		FILE* stdFile = fopen ( filename, mode );
		
		if ( stdFile ) {
			file = ( MOAIOFile* )malloc ( sizeof ( MOAIOFile ));
			file->mIsArchive = FALSE;
			file->mPtr.mFile = stdFile;
		}
	}

	return ( MOAIOFILE )file;
}

//----------------------------------------------------------------//
size_t moaio_fread ( void* buffer, size_t size, size_t count, MOAIOFILE fp ) {
	
	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		return ( file->mIsArchive ) ? ( size_t )MOAIOZipStream_Read ( file->mPtr.mZip, buffer, size * count ) : fread ( buffer, size, count, file->mPtr.mFile );
	}
	return 0;
}

//----------------------------------------------------------------//
int	moaio_fseek ( MOAIOFILE fp, long offset, int origin ) {
	
	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		return ( file->mIsArchive ) ? MOAIOZipStream_Seek ( file->mPtr.mZip, offset, origin ) : fseek ( file->mPtr.mFile, offset, origin );
	}	
	return -1;
}

//----------------------------------------------------------------//
long moaio_ftell ( MOAIOFILE fp ) {

	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		return ( file->mIsArchive ) ? ( long )MOAIOZipStream_Tell ( file->mPtr.mZip ) : ftell ( file->mPtr.mFile );
	}
	return -1L;
}

//----------------------------------------------------------------//
size_t moaio_fwrite ( const void* data, size_t size, size_t count, MOAIOFILE fp ) {
	
	if ( fp ) {
		MOAIOFile* file = ( MOAIOFile* )fp;
		if ( !file->mIsArchive ) {
			fwrite ( data, size, count, file->mPtr.mFile );
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
	
	if ( sBuffer->mMem [ sBuffer->mStrLen - 1 ] != '/' ) {
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
int moaio_set_virtual_path ( const char* path, const char* archive ) {

	// delete the path if it exists
	int result;
	MOAIOVirtualPath* cursor = sVirtualPaths;
	MOAIOVirtualPath* virtualPath;
	MOAIOVirtualPath* list = 0;
	
	if ( !path ) return -1;
	path = moaio_get_abs_dirpath ( path );
	
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

	if ( !archive ) return 0;

	virtualPath = MOAIOVirtualPath_New ();
	if ( !virtualPath ) goto error;

	result = MOAIOVirtualPath_SetPath ( virtualPath, path );
	if ( result ) goto error;
	
	result = MOAIOVirtualPath_SetArchive ( virtualPath, moaio_get_abs_filepath ( archive ));
	if ( result ) goto error;

	sVirtualPaths = MOAIOVirtualPath_PushFront ( virtualPath, sVirtualPaths );

	return 0;

error:

	if ( virtualPath ) {
		MOAIOVirtualPath_Delete ( virtualPath );
	}
	return -1;
}
