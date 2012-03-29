// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zipfs/zipfs.h>
#include <zipfs/zipfs_util.h>
#include <zipfs/ZIPFSString.h>
#include <zipfs/ZIPFSVirtualPath.h>
#include <zipfs/ZIPFSZipFile.h>

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

#ifdef _WIN32
	#define S_ISDIR(B) (((B)&_S_IFDIR)!=0)
#endif

#ifdef NACL
#include "NaClFile.h"
#endif

//================================================================//
// ZIPFSFile
//================================================================//
typedef struct ZIPFSFile {

	int		mIsArchive;
	
	union {
		FILE*				mFile;
		ZIPFSZipStream*		mZip;
	} mPtr;

} ZIPFSFile;

//================================================================//
// ZIPFSDir
//================================================================//
typedef struct ZIPFSDir {

	char*					mDirName;
	size_t					mDirNameLen;

	ZIPFSZipFileDir*		mZipFileDir; // this is a ref to the directory being interated
	
	ZIPFSZipFileDir*		mZipFileSubDir; // this is the sub directory iterator
	ZIPFSZipFileEntry*		mZipFileEntry; // this is the file entry iterator
	ZIPFSVirtualPath*		mVirtualSubDir;

	char const*				mName;
	int						mIsDir;

	#ifdef _WIN32
		struct _finddata_t	mFileInfo;
		intptr_t			mHandle;
	#else
		DIR*				mHandle;
	#endif

} ZIPFSDir;


//----------------------------------------------------------------//
int ZIPFSDir_ReadZipEntry ( ZIPFSDir* self );
int ZIPFSDir_ReadZipEntry ( ZIPFSDir* self ) {

	// only perform iteration if we have a valid directory
	// directory will be set to nil when iteration is finished
	if ( self->mZipFileDir ) {

		// iterate through the sub directories first
		// then the file entries

		if ( self->mZipFileSubDir ) {
			
			// we have a valid sub dir iterator, so iterate through
			// when iteration ends, move on to files
			
			self->mZipFileSubDir = self->mZipFileSubDir->mNext;
			if ( !self->mZipFileSubDir ) {
			
				// no more sub directories; move on to files
				self->mZipFileEntry = self->mZipFileDir->mChildFiles;
				if ( !self->mZipFileEntry ) {
				
					// no files, so end all iteration
					self->mZipFileDir = 0;
				}
			}
		}
		else if ( self->mZipFileEntry  ) {
			
			// try the next file in the iteration
			self->mZipFileEntry = self->mZipFileEntry->mNext;
			if ( !self->mZipFileEntry ) {
			
				// no more files, so end all iteration
				self->mZipFileDir = 0;
			}
		}
		else  {
			
			// there is no valid directory or file iterator, so begin iteration
			
			// first see if we have an valid sub directories
			self->mZipFileSubDir = self->mZipFileDir->mChildDirs;
			if ( !self->mZipFileSubDir ) {
		
				// no valid sub directories, so try files
				self->mZipFileEntry = self->mZipFileDir->mChildFiles;
				if ( !self->mZipFileEntry ) {
				
					// no valid files, so we're done
					self->mZipFileDir = 0;
				}
			}
		}
	}
	
	// if we have a valid dir at this point, it means we are still iterating
	if ( self->mZipFileDir ) {
		
		// get the name of the current directory or file and set the isDir flag accordingly
		if ( self->mZipFileSubDir ) {
			self->mName = self->mZipFileSubDir->mName;
			self->mIsDir = 1;
		}
		else if ( self->mZipFileEntry  ) {
			self->mName = self->mZipFileEntry->mName;
			self->mIsDir = 0;
		}
		return 1;
	}
	return 0;
}

//================================================================//
// ZIPFSTlsfPool
//================================================================//
typedef struct ZIPFSTlsfPool {

	tlsf_pool	mPool;
	void*		mBuffer;

} ZIPFSTlsfPool;

//================================================================//
// local
//================================================================//

static ZIPFSString* sWorkingPath;
static ZIPFSString* sBuffer;

static ZIPFSVirtualPath* sVirtualPaths = 0;

ZIPFSFILE* zipfs_stderr	= 0;
ZIPFSFILE* zipfs_stdin	= 0;
ZIPFSFILE* zipfs_stdout	= 0;

//----------------------------------------------------------------//
ZIPFSVirtualPath* find_best_virtual_path ( char const* path );
ZIPFSVirtualPath* find_best_virtual_path ( char const* path ) {

	size_t len = 0;
	size_t bestlen = 0;
	ZIPFSVirtualPath* best = 0;
	ZIPFSVirtualPath* cursor = sVirtualPaths;
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		char* test = cursor->mPath;
		len = compare_paths ( test, path );
	
		if (( !test [ len ]) && ( len > bestlen )) {
			best = cursor;
			bestlen = len;
		}		
	}
	return best;
}

//----------------------------------------------------------------//
ZIPFSVirtualPath* find_next_virtual_subdir ( char const* path, ZIPFSVirtualPath* cursor ) ;
ZIPFSVirtualPath* find_next_virtual_subdir ( char const* path, ZIPFSVirtualPath* cursor ) {

	size_t len = 0;
	
	cursor = cursor ? cursor->mNext : sVirtualPaths;
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		char* test = cursor->mPath;
		len = compare_paths ( test, path );
		
		if ( test [ len ] && ( !path [ len ])) {
			return cursor;
		}		
	}
	return 0;
}

//----------------------------------------------------------------//
ZIPFSVirtualPath* find_virtual_path ( char const* path );
ZIPFSVirtualPath* find_virtual_path ( char const* path ) {

	size_t len = 0;

	ZIPFSVirtualPath* cursor = sVirtualPaths;
	for ( ; cursor; cursor = cursor->mNext ) {
	
		char* test = cursor->mPath;
		len = compare_paths ( test, path );
		
		if ( !( test [ len ] || path [ len ])) break;
	}
	return cursor;
}

//----------------------------------------------------------------//
ZIPFSZipStream* is_archive ( ZIPFSFILE* opaque );
ZIPFSZipStream* is_archive ( ZIPFSFILE* opaque ) {

	if ( opaque ) { 
		ZIPFSFile* self = ( ZIPFSFile* )opaque;
		return self->mIsArchive ? self->mPtr.mZip : 0;
	}
	return 0;
}

//----------------------------------------------------------------//
FILE* is_file ( ZIPFSFILE* opaque );
FILE* is_file ( ZIPFSFILE* opaque ) {

	if ( opaque ) { 
		ZIPFSFile* self = ( ZIPFSFile* )opaque;
		return self->mIsArchive ? 0 : self->mPtr.mFile;
	}
	return 0;
}

//----------------------------------------------------------------//
int is_separator ( char c );
int is_separator ( char c ) {

	return ( c == '/' ) || ( c == '\\' ) ? 1 : 0; 

}

//----------------------------------------------------------------//
int is_virtual_path ( char const* path );
int is_virtual_path ( char const* path ) {

	size_t len = 0;
	ZIPFSVirtualPath* cursor = sVirtualPaths;
	
	for ( ; cursor; cursor = cursor->mNext ) {
	
		char* test = cursor->mPath;
		len = compare_paths ( test, path );
		
		if ( !test [ len ]) return 1;
	}
	return 0;
}

//================================================================//
// stdlib
//================================================================//

static ZIPFSTlsfPool* sTlsfPool = 0;

//----------------------------------------------------------------//
void* zipfs_calloc ( size_t num, size_t size ) {

	if ( sTlsfPool ) {
		void* ptr = tlsf_malloc ( sTlsfPool->mPool, num * size );
		if ( ptr ) {
			memset ( ptr, 0, num * size );
		}
		return ptr;
	}
	return calloc ( num, size );
}

//----------------------------------------------------------------//
void zipfs_free ( void* ptr ) {

	if ( sTlsfPool ) {
		tlsf_free ( sTlsfPool->mPool, ptr );
	}
	else {
		free ( ptr );
	}
}

//----------------------------------------------------------------//
void* zipfs_malloc ( size_t size ) {

	if ( sTlsfPool ) {
		return tlsf_malloc ( sTlsfPool->mPool, size );
	}
	return malloc ( size );
}

//----------------------------------------------------------------//
void* zipfs_realloc ( void* ptr, size_t size ) {

	if ( sTlsfPool ) {
		return tlsf_realloc ( sTlsfPool->mPool, ptr, size );
	}
	return realloc ( ptr, size );
}

//----------------------------------------------------------------//
ZIPFS_TLSF_POOL* zipfs_tlsf_create_pool ( size_t bytes ) {
	
	ZIPFSTlsfPool* pool = ( ZIPFSTlsfPool* )malloc ( sizeof ( ZIPFSTlsfPool ));
	
	pool->mBuffer = malloc ( bytes );
	pool->mPool = tlsf_create ( pool->mBuffer, bytes );
	
	return ( ZIPFS_TLSF_POOL* )pool;
}

//----------------------------------------------------------------//
void zipfs_tlsf_destroy_pool ( ZIPFS_TLSF_POOL* opaque ) {

	ZIPFSTlsfPool* pool = ( ZIPFSTlsfPool* )opaque;

	if ( pool ) {
		tlsf_destroy ( pool->mPool );
		free ( pool->mBuffer );
		free ( pool );
	}
}

//----------------------------------------------------------------//
ZIPFS_TLSF_POOL* zipfs_tlsf_get_pool ( void ) {

	return ( ZIPFS_TLSF_POOL* )sTlsfPool;
}

//----------------------------------------------------------------//
void zipfs_tlsf_set_pool ( ZIPFS_TLSF_POOL* opaque ) {

	sTlsfPool = ( ZIPFSTlsfPool* )opaque;
}

//================================================================//
// stdio
//================================================================//

//----------------------------------------------------------------//
void zipfs_clearerr ( ZIPFSFILE* fp ) {

	FILE* file = is_file ( fp );
	if ( file ) {
		clearerr ( file );
	}
}

//----------------------------------------------------------------//
int	zipfs_fclose ( ZIPFSFILE* fp ) {

	int result = 0;

	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		result = ( file->mIsArchive ) ? ZIPFSZipStream_Close ( file->mPtr.mZip ) : fclose ( file->mPtr.mFile );
		memset ( fp, 0, sizeof ( ZIPFSFile ));
		free ( file );
	}
	return result;
}

//----------------------------------------------------------------//
int zipfs_feof ( ZIPFSFILE* fp ) {

	int result = 0;

	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		result = ( file->mIsArchive ) ? ZIPFSZipStream_IsAtEnd ( file->mPtr.mZip ) : feof ( file->mPtr.mFile );
	}
	return result;
}

//----------------------------------------------------------------//
int zipfs_ferror ( ZIPFSFILE* fp ) {

	int result = 0;

	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		result = ( file->mIsArchive ) ? 0 : ferror ( file->mPtr.mFile ); // TODO: error flag for zip file
	}
	return result;
}

//----------------------------------------------------------------//
int	zipfs_fflush ( ZIPFSFILE* fp ) {
	
	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		if ( !file->mIsArchive ) {
			return fflush ( file->mPtr.mFile );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int zipfs_fgetc ( ZIPFSFILE* fp ) {
	
	int result = EOF;

	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		
		if ( file->mIsArchive ) {
			char c;
			result = ( int )ZIPFSZipStream_Read ( file->mPtr.mZip, &c, 1 );
			
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
int zipfs_fgetpos ( ZIPFSFILE* fp, fpos_t* position ) {
	(( void )fp );
	(( void )position );

	assert ( 0 ); // not implemented
	return -1;
}

//----------------------------------------------------------------//
char* zipfs_fgets ( char* string, int length, ZIPFSFILE* fp ) {
	
	int i = 0;
	int c = 0;

	if ( fp ) {
		
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		
		if ( file->mIsArchive ) {
		
			if ( length <= 1 ) return 0;

			do {
				c = zipfs_fgetc ( fp );
				 if ( c == EOF || c == NULL ) break;
				
				string [ i++ ] = ( char )c;
				if ( i >= length ) return 0;
				
			} while ( c && ( c != '\n' ));

			if ( i == 0 ) {
				return 0;
			}
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
int	zipfs_fileno ( ZIPFSFILE* fp ) {
	
	// TODO:
	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		if ( !file->mIsArchive ) {
			return fileno ( file->mPtr.mFile );
		}
	}
	return -1;
}

//----------------------------------------------------------------//
ZIPFSFILE* zipfs_fopen ( const char* filename, const char* mode ) {
	
	ZIPFSFile* file = 0;
	ZIPFSVirtualPath* mount;

	filename = zipfs_get_abs_filepath ( filename );
	mount = find_best_virtual_path ( filename );

	if ( mount ) {
		
		if ( mode [ 0 ] == 'r' ) {
		
			ZIPFSZipStream* zipStream = 0;
			
			filename = ZIPFSVirtualPath_GetLocalPath ( mount, filename );

			if ( filename ) {
				zipStream = ZIPFSZipStream_Open ( mount->mArchive, filename );
			}
			
			if ( zipStream ) {
				file = ( ZIPFSFile* )malloc ( sizeof ( ZIPFSFile ));
				file->mIsArchive = 1;
				file->mPtr.mZip = zipStream;
			}
		}
	}
	else {

		#ifdef MOAI_COMPILER_MSVC
			FILE* stdFile = 0;
			_set_errno ( fopen_s ( &stdFile, filename, mode ));
		#else
			FILE* stdFile = fopen ( filename, mode );
		#endif
		
		if ( stdFile ) {
			file = ( ZIPFSFile* )calloc ( 1, sizeof ( ZIPFSFile ));
			file->mPtr.mFile = stdFile;
		}
	}

	return ( ZIPFSFILE* )file;
}

//----------------------------------------------------------------//
#ifdef MOAI_COMPILER_MSVC
	errno_t zipfs_fopen_s ( ZIPFSFILE** fp, const char* filename, const char* mode ) {

		*fp = zipfs_fopen ( filename, mode );
		return errno;
	}
#endif

//----------------------------------------------------------------//
int zipfs_fprintf ( ZIPFSFILE * fp, const char * format, ... ) {

	int result;

	va_list args;
	va_start ( args, format );
	result = zipfs_vfprintf ( fp, format, args );
	va_end ( args );

	return result;
}

//----------------------------------------------------------------//
int zipfs_fputc ( int c, ZIPFSFILE* fp ) {

	FILE* file = is_file ( fp );
	if ( file ) {
		return fputc ( c, file );
	}
	return EOF;
}

//----------------------------------------------------------------//
int zipfs_fputs ( const char* string, ZIPFSFILE* fp ) {

	FILE* file = is_file ( fp );
	if ( file ) {
		return fputs ( string, file );
	}
	return EOF;
}

//----------------------------------------------------------------//
size_t zipfs_fread ( void* buffer, size_t size, size_t count, ZIPFSFILE* fp ) {
	
	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		if ( file->mIsArchive ) {
			size_t result = ( size_t )ZIPFSZipStream_Read ( file->mPtr.mZip, buffer, size * count );
			return ( size_t )( result / size );
		}
		return fread ( buffer, size, count, file->mPtr.mFile );
	}
	return 0;
}

//----------------------------------------------------------------//
ZIPFSFILE* zipfs_freopen ( const char* filename, const char* mode, ZIPFSFILE* fp ) {

	ZIPFSFile* result = 0;
	ZIPFSFile* file = ( ZIPFSFile* )fp;
	ZIPFSVirtualPath* mount;

	if ( !file ) return 0;

	if ( file->mIsArchive ){
		ZIPFSZipStream_Close ( file->mPtr.mZip );
	}
	else {
		fclose ( file->mPtr.mFile );
	}
	
	memset ( fp, 0, sizeof ( ZIPFSFile ));

	filename = zipfs_get_abs_filepath ( filename );
	mount = find_best_virtual_path ( filename );

	if ( mount ) {
		
		if ( mode [ 0 ] == 'r' ) {
			
			ZIPFSZipStream* zipStream;
			
			filename = ZIPFSVirtualPath_GetLocalPath ( mount, filename );
			zipStream = ZIPFSZipStream_Open ( mount->mArchive, filename );
			
			if ( zipStream ) {
				file->mIsArchive = 1;
				file->mPtr.mZip = zipStream;
				result = file;
			}
		}
	}
	else {
		
		FILE* stdFile = fopen ( filename, mode );
		
		if ( stdFile ) {
			file->mPtr.mFile = stdFile;
			result = file;
		}
	}

	return ( ZIPFSFILE* )result;
}

//----------------------------------------------------------------//
int zipfs_fscanf ( ZIPFSFILE* fp, const char * format, ... ) {
	
	int result;
	va_list args;
	
	va_start ( args, format );
	result = zipfs_vfscanf ( fp, format, args );
	va_end ( args );
	
	return result;
}

//----------------------------------------------------------------//
int	zipfs_fseek ( ZIPFSFILE* fp, long offset, int origin ) {
	
	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		return ( file->mIsArchive ) ? ZIPFSZipStream_Seek ( file->mPtr.mZip, offset, origin ) : fseek ( file->mPtr.mFile, offset, origin );
	}	
	return -1;
}

//----------------------------------------------------------------//
#ifdef MOAI_COMPILER_MSVC
	int zipfs_fseeki64 ( ZIPFSFILE* fp, __int64 offset, int origin ) {

		// TODO:
		return zipfs_fseek ( fp, ( long )offset, origin );
	}
#endif

//----------------------------------------------------------------//
int zipfs_fsetpos ( ZIPFSFILE* fp, const fpos_t * pos ) {
	(( void )fp );
	(( void )pos );

	assert ( 0 ); // not implemented
	return -1;
}

//----------------------------------------------------------------//
long zipfs_ftell ( ZIPFSFILE* fp ) {

	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		return ( file->mIsArchive ) ? ( long )ZIPFSZipStream_Tell ( file->mPtr.mZip ) : ftell ( file->mPtr.mFile );
	}
	return -1L;
}

//----------------------------------------------------------------//
size_t zipfs_fwrite ( const void* data, size_t size, size_t count, ZIPFSFILE* fp ) {
	
	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		if ( !file->mIsArchive ) {
			return fwrite ( data, size, count, file->mPtr.mFile );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int zipfs_getc ( ZIPFSFILE* fp ) {

	return zipfs_fgetc ( fp );
}

//----------------------------------------------------------------//
int zipfs_getwc ( ZIPFSFILE* fp ) {
	UNUSED ( fp );

	assert ( 0 ); // TODO:
	return -1;
}

//----------------------------------------------------------------//
int zipfs_pclose ( ZIPFSFILE* fp ) {

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	
	FILE* stdFile = 0;
	if ( file && !file->mIsArchive ) {
		stdFile = file->mPtr.mFile;
		memset ( fp, 0, sizeof ( ZIPFSFile ));
		free ( file );
	}

	#ifdef MOAI_OS_WINDOWS
		return _pclose ( stdFile );
	#else
		return pclose ( stdFile );
	#endif
}

//----------------------------------------------------------------//
ZIPFSFILE* zipfs_popen ( const char *command, const char *mode ) {

	ZIPFSFile* file = 0;
	FILE* stdFile = 0;

	#ifdef MOAI_OS_WINDOWS
		stdFile = _popen ( command, mode );	
	#else
		stdFile = popen ( command, mode );
	#endif

	if ( stdFile ) {
		file = ( ZIPFSFile* )calloc ( 1, sizeof ( ZIPFSFile ));
		file->mPtr.mFile = stdFile;
	}
	return ( ZIPFSFILE* )file;
}

//----------------------------------------------------------------//
int zipfs_putc ( int character, ZIPFSFILE* fp ) {

	return zipfs_fputc ( character, fp );
}

//----------------------------------------------------------------//
int zipfs_remove ( const char* path ) {

	if ( zipfs_is_virtual_path ( path )) return -1;
	return remove ( path );
}

//----------------------------------------------------------------//
int zipfs_rename ( const char* oldname, const char* newname ) {

	if ( zipfs_is_virtual_path ( oldname )) return -1;
	if ( zipfs_is_virtual_path ( newname )) return -1;
	
	return rename ( oldname, newname );
}

//----------------------------------------------------------------//
void zipfs_rewind ( ZIPFSFILE* fp ) {

	zipfs_fseek ( fp, 0, SEEK_SET );
}


//----------------------------------------------------------------//
void zipfs_setbuf ( ZIPFSFILE* fp, char* buffer ) {

	FILE* file = is_file ( fp );
	if ( file ) {
		setbuf ( file, buffer );
	}
}

//----------------------------------------------------------------//
int zipfs_setvbuf ( ZIPFSFILE* fp, char* buffer, int mode, size_t size ) {

	FILE* file = is_file ( fp );
	if ( file ) {
		setvbuf ( file, buffer, mode, size );
	}
	return 0;
}

//----------------------------------------------------------------//
ZIPFSFILE* zipfs_tmpfile ( void ) {

	FILE* fp = tmpfile ();
	if ( fp ) {
	
		ZIPFSFile* file = ( ZIPFSFile* )calloc ( 1, sizeof ( ZIPFSFile ));
		file->mPtr.mFile = fp;
		return ( ZIPFSFILE* )file;
	}
	return 0;
}

//----------------------------------------------------------------//
char* zipfs_tmpnam ( char* str ) {

	return tmpnam ( str );
}

//----------------------------------------------------------------//
int zipfs_ungetc ( int character, ZIPFSFILE* fp ) {
	
	if ( fp ) {
		ZIPFSFile* file = ( ZIPFSFile* )fp;
		if ( file->mIsArchive ) {
			int result = ZIPFSZipStream_UnGetChar ( file->mPtr.mZip, ( char )character );
			return result ? EOF : 0;
		}
		else {
			return ungetc ( character, file->mPtr.mFile );
		}
	}
	return EOF;
}

//----------------------------------------------------------------//
int zipfs_vfprintf ( ZIPFSFILE* fp, const char* format, va_list arg ) {

	FILE* file = is_file ( fp );
	if ( file ) {
		return vfprintf ( file, format, arg );
	}
	return -1;
}

//================================================================//
// extra
//================================================================//

//----------------------------------------------------------------//
int zipfs_affirm_path ( const char* path ) {

	int result = 0;
	char* cursor;

	if ( !path ) return -1;

	zipfs_get_abs_dirpath ( path );
	if ( is_virtual_path ( path )) return -1;
	
	cursor = sBuffer->mMem;
	if ( *cursor == '/' ) {
		++cursor;
	}
	
	while ( *cursor ) {
		
		// Advance to end of current directory name
		while (( *cursor ) && ( *cursor != '/' )) ++cursor;
		if ( !( *cursor )) break;
	
		*cursor = 0;

		result = zipfs_mkdir ( sBuffer->mMem );
		
		if ( result && ( errno != EEXIST )) break;
		result = 0;
		
		*cursor = '/';
		++cursor;
	}
	
	return result;
}

//----------------------------------------------------------------//
char* zipfs_basename ( const char* filename ) {

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
char* zipfs_bless_path ( const char* path ) {

	size_t i = 0;
	size_t j = 0;
	
	for ( i = 0; path [ i ]; ++i ) {
		if ( is_separator ( path [ i ] )) {
			while ( is_separator ( path [ ++i ]));
			--i;
		}
		j++;
	}
	
	ZIPFSString_Grow ( sBuffer, j );
	
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
int zipfs_chdir ( const char* path ) {

	int result = -1;
	ZIPFSVirtualPath* mount = 0;

	path = zipfs_get_abs_dirpath ( path );
	mount = find_best_virtual_path ( path );
	
	if ( mount ) {
		const char* localpath = ZIPFSVirtualPath_GetLocalPath ( mount, path );
		if ( localpath ) {
			result = 0;
		}
	}
	else {
		result = chdir ( path );
	}

	if ( result == 0 ) {
		ZIPFSString_Set ( sWorkingPath, path );
	}
	return result;
}

//----------------------------------------------------------------//
void zipfs_cleanup ( void ) {

	ZIPFSVirtualPath* cursor = sVirtualPaths;
	while ( cursor ) {
		ZIPFSVirtualPath* virtualPath = cursor;
		cursor = cursor->mNext;
		ZIPFSVirtualPath_Delete ( virtualPath );
	}
	
	ZIPFSString_Delete ( sBuffer );
	ZIPFSString_Delete ( sWorkingPath );
	
	memset ( zipfs_stderr, 0, sizeof ( ZIPFSFile ));
	free ( zipfs_stderr );

	memset ( zipfs_stdin, 0, sizeof ( ZIPFSFile ));
	free ( zipfs_stdin );

	memset ( zipfs_stdout, 0, sizeof ( ZIPFSFile ));
	free ( zipfs_stdout );

	sVirtualPaths = 0;
	sBuffer = 0;
	sWorkingPath = 0;
	zipfs_stderr = 0;
	zipfs_stdin = 0;
	zipfs_stdout = 0;
}

//----------------------------------------------------------------//
void zipfs_dir_close ( ZIPFSDIR* dir ) {

	ZIPFSDir* self = ( ZIPFSDir* )dir;

	#ifdef _WIN32
		if ( self->mHandle ) {
			_findclose( self->mHandle );
		}
	#else
		if ( self->mHandle ) {
			closedir ( self->mHandle );
		}
	#endif

	clear_string ( self->mDirName );
	
	memset ( self, 0, sizeof ( ZIPFSDir ));
	free ( self );
}

//----------------------------------------------------------------//
char const* zipfs_dir_entry_name ( ZIPFSDIR* dir ) {

	ZIPFSDir* self = ( ZIPFSDir* )dir;
	return self->mName;
}

//----------------------------------------------------------------//
int zipfs_dir_entry_is_subdir ( ZIPFSDIR* dir ) {

	ZIPFSDir* self = ( ZIPFSDir* )dir;
	return self->mIsDir;
}

//----------------------------------------------------------------//
ZIPFSDIR* zipfs_dir_open ( void ) {

	ZIPFSDir* self = ( ZIPFSDir* )calloc ( 1, sizeof ( ZIPFSDir ));
	ZIPFSVirtualPath* mount = find_best_virtual_path ( sWorkingPath->mMem );
	
	self->mDirName = copy_string ( sWorkingPath->mMem );
	self->mDirNameLen = sWorkingPath->mStrLen;
	
	if ( mount ) {
		char const* path = ZIPFSVirtualPath_GetLocalPath ( mount, sWorkingPath->mMem );
		self->mZipFileDir = ZIPFSZipFile_FindDir ( mount->mArchive, path );
	}
	else {
		self->mVirtualSubDir = find_next_virtual_subdir ( self->mDirName, 0 );
	}
	
	#ifndef _WIN32
		self->mHandle = opendir ( "." );
	#endif
	
	return ( ZIPFSDIR* )self;
}

//----------------------------------------------------------------//
int zipfs_dir_read_entry ( ZIPFSDIR* dir ) {

	ZIPFSDir* self = ( ZIPFSDir* )dir;
	if ( !self ) return 0;

	self->mName = 0;
	self->mIsDir = 0;

	if ( self->mZipFileDir ) {
		return ZIPFSDir_ReadZipEntry ( self );
	}
	
	if ( self->mVirtualSubDir ) {
	
		self->mName = self->mVirtualSubDir->mName;
		self->mIsDir = 1;
		self->mVirtualSubDir = find_next_virtual_subdir ( self->mDirName, self->mVirtualSubDir );
		
		return 1;
	}
	
	#ifdef _WIN32
	
		if ( self->mHandle ) {
			if ( _findnext( self->mHandle, &self->mFileInfo ) < 0 ) return 0;
		}
		else {
			self->mHandle = _findfirst ( "*", &self->mFileInfo );
		}
		
		if ( self->mHandle ) {
			self->mName = self->mFileInfo.name;
			self->mIsDir = ( self->mFileInfo.attrib & _A_SUBDIR ) ? 1 : 0;
			return 1;
		}
		
	#else
	{
		struct dirent* entry = readdir ( self->mHandle );
		if ( entry ) {
			self->mName = entry->d_name;
#ifndef NACL
			self->mIsDir = ( entry->d_type == DT_DIR ) ? 1 : 0;
#else
			self->mIsDir = 1;
#endif
			return 1;
		}
	}
	#endif
	
	return 0;
}

//----------------------------------------------------------------//
char* zipfs_get_abs_filepath ( const char* path ) {

	char* buffer;
	
	// handle absolute paths
	if ( !path ) return ( char* )"/";
	
	if (( path [ 0 ] == '\\' ) || ( path [ 0 ] == '/' ) || ( path [ 1 ] == ':' )) {
		return zipfs_bless_path ( path );
	}
	
	buffer = zipfs_get_working_path ();
	buffer = ZIPFSString_Append ( sBuffer, path );

	return zipfs_normalize_path ( buffer );
}
//----------------------------------------------------------------//
char* zipfs_get_abs_dirpath ( const char* path ) {

	zipfs_get_abs_filepath ( path );
	
	if ( sBuffer->mMem [ sBuffer->mStrLen - 1 ] != '/' ) {
		ZIPFSString_Append ( sBuffer, "/" );
	}
	return sBuffer->mMem;
}

//----------------------------------------------------------------//
char* zipfs_get_rel_path ( char const* path ) {

	size_t depth = 0;
	size_t i = 0;
	size_t same;

	if ( !path ) return 0;
	
	if ( path [ 0 ] == '.' ) {
		return zipfs_bless_path ( path );
	}
	
	if ( !(( path [ 0 ] == '/' ) || ( path [ 0 ] == '\\' ) || ( path [ 0 ] == ':' ))) {
		ZIPFSString_Set ( sBuffer, "./" );
		ZIPFSString_Append ( sBuffer, path );
		zipfs_bless_path ( sBuffer->mMem );
		return sBuffer->mMem;
	}

	// store the expanded path in sBuffer
	zipfs_get_abs_filepath ( path );

	same = compare_paths ( path, sWorkingPath->mMem );
	if ( same == 0 ) {
		return zipfs_bless_path ( path );
	}

	// count the number of steps up in the current directory
	for ( i = same; sWorkingPath->mMem [ i ]; ++i ) {
		if (  sWorkingPath->mMem [ i ] == '/' ) {
			depth++;
		}
	}

	ZIPFSString_Shift ( sBuffer, same, sBuffer->mStrLen - same, depth * 3 );

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
int zipfs_get_stat ( char const* path, zipfs_stat* filestat ) {

	#ifdef NACL
		#define stat stat
	#endif
	
	struct stat s;
	
	#ifdef NACL
		#define stat nacl_stat
	#endif

	//struct stat s;
	int result;
	ZIPFSVirtualPath* mount;
	char* abspath;

	filestat->mExists = 0;

	abspath = zipfs_get_abs_filepath ( path );
	mount = find_best_virtual_path ( abspath );

	if ( mount ) {

		const char* localpath = ZIPFSVirtualPath_GetLocalPath ( mount, abspath );
		
		if ( abspath && localpath ) {
		
			ZIPFSZipFileEntry* entry;
		
			ZIPFSZipFileDir* dir = ZIPFSZipFile_FindDir ( mount->mArchive, localpath );
		
			const char *filename = localpath;
			int i = strlen ( filename ) - 1;

			result = stat ( mount->mArchive->mFilename, &s );

			if ( result ) return -1;
			
			for ( ; i >= 0; --i ) {
				if ( filename [ i ] == '/' ) {
					filename = &filename [ i + 1 ];
					break;
				}
			}
			
			entry = ZIPFSZipFile_FindEntry ( mount->mArchive, localpath );

			if(filename == dir->mName) { // Directory		
				filestat->mIsDir	= 1;	
				filestat->mSize		= 0;	
			}
			else if ( entry ) { // Entry
					
				filestat->mIsDir	= 0;
				filestat->mSize		= entry->mUncompressedSize;
			}
			else {
				return 0;
			}

			filestat->mExists			= 1;	
			filestat->mTimeCreated		= s.st_ctime;	
			filestat->mTimeModified		= s.st_mtime;	
			filestat->mTimeViewed		= s.st_atime;
		}
	}
	else {
	
		result = stat ( path, &s );
		if ( result ) return -1;

		filestat->mExists = 1;
		
		filestat->mIsDir			= S_ISDIR ( s.st_mode );
		filestat->mSize				= ( size_t )s.st_size;
		filestat->mTimeCreated		= s.st_ctime;
		filestat->mTimeModified		= s.st_mtime;
		filestat->mTimeViewed		= s.st_atime;
	}
	return 0;
}

//----------------------------------------------------------------//
char* zipfs_get_working_path ( void ) {

	char* result = 0;
	
	if ( !sWorkingPath->mStrLen ) {
	
		ZIPFSString_Grow ( sBuffer, MOAIO_STRING_BLOCK_SIZE );
		
		while ( !result ) {
			result = getcwd ( sBuffer->mMem, sBuffer->mSize );
			if ( !result ) {
				ZIPFSString_Grow ( sBuffer, sBuffer->mSize + MOAIO_STRING_BLOCK_SIZE );
			}
			else {
				size_t pathlen = strlen ( result );
				sBuffer->mStrLen = pathlen;
				
				if ( result [ pathlen ] != '/' ) {
					ZIPFSString_Append ( sBuffer, "/" );
				}
			}
		}
		zipfs_bless_path ( result );
		
		ZIPFSString_Set ( sWorkingPath, result );
	}
	
	ZIPFSString_Set ( sBuffer, sWorkingPath->mMem );
	return sBuffer->mMem;
}

//----------------------------------------------------------------//
char* zipfs_getcwd ( char* buffer, size_t length ) {

	if ( sWorkingPath->mSize >= length ) return 0;
	strcpy ( buffer, sWorkingPath->mMem );
	return buffer;
}

//----------------------------------------------------------------//
void zipfs_init ( void ) {

	ZIPFSFile* file;

	sWorkingPath = ZIPFSString_New ();
	sBuffer = ZIPFSString_New ();

	zipfs_get_working_path ();
	
	file = ( ZIPFSFile* )calloc ( 1, sizeof ( ZIPFSFile ));
	file->mPtr.mFile = stderr;
	zipfs_stderr = ( ZIPFSFILE* )file;
	
	file = ( ZIPFSFile* )calloc ( 1, sizeof ( ZIPFSFile ));
	file->mPtr.mFile = stdin;
	zipfs_stdin = ( ZIPFSFILE* )file;
	
	file = ( ZIPFSFile* )calloc ( 1, sizeof ( ZIPFSFile ));
	file->mPtr.mFile = stdout;
	zipfs_stdout = ( ZIPFSFILE* )file;
}

//----------------------------------------------------------------//
int zipfs_is_virtual_path ( const char* path ) {

	if ( sVirtualPaths ) {
		path = zipfs_get_abs_dirpath ( path );
		return is_virtual_path ( path );
	}
	return 0;
}

//----------------------------------------------------------------//
int zipfs_mkdir ( const char* path ) {

	if ( !path ) return -1;
	if ( zipfs_is_virtual_path ( path )) return -1;
	
	#ifdef _WIN32
		return mkdir ( sBuffer->mMem );
	#else
		return mkdir ( path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
	#endif
}

//----------------------------------------------------------------//
int zipfs_mount_virtual ( const char* path, const char* archive ) {

	// delete the path if it exists
	int result;
	ZIPFSVirtualPath* cursor = sVirtualPaths;
	ZIPFSVirtualPath* virtualPath;
	ZIPFSVirtualPath* list = 0;
	
	if ( !path ) return -1;
	path = zipfs_get_abs_dirpath ( path );
	
	while ( cursor ) {
		virtualPath = cursor;
		cursor = cursor->mNext;
		
		if ( strcmp_ignore_case ( virtualPath->mPath, path ) == 0 ) {
			ZIPFSVirtualPath_Delete ( virtualPath );
		}
		else {
			list = ZIPFSVirtualPath_PushFront ( virtualPath, list );
		}
	}
	
	cursor = list;
	sVirtualPaths = 0;
	
	while ( cursor ) {
		virtualPath = cursor;
		cursor = cursor->mNext;
		
		sVirtualPaths = ZIPFSVirtualPath_PushFront ( virtualPath, sVirtualPaths );
	}

	if ( !archive ) return 0;

	virtualPath = ZIPFSVirtualPath_New ();
	if ( !virtualPath ) goto error;

	result = ZIPFSVirtualPath_SetPath ( virtualPath, path );
	if ( result ) goto error;
	
	result = ZIPFSVirtualPath_SetArchive ( virtualPath, zipfs_get_abs_filepath ( archive ));
	if ( result ) goto error;

	sVirtualPaths = ZIPFSVirtualPath_PushFront ( virtualPath, sVirtualPaths );

	return 0;

error:

	if ( virtualPath ) {
		ZIPFSVirtualPath_Delete ( virtualPath );
	}
	return -1;
}

//----------------------------------------------------------------//
char* zipfs_normalize_path ( const char* path ) {

	size_t i = 0;
	size_t top = 0;

	size_t length = strlen ( path );
	char* buffer = ZIPFSString_Grow ( sBuffer, length );

	buffer = zipfs_bless_path ( path );

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
	sBuffer->mStrLen = top;
	return buffer;
}

//----------------------------------------------------------------//
int zipfs_rmdir ( const char* path ) {

	if ( zipfs_is_virtual_path ( path )) return -1;
	return rmdir ( path );
}
