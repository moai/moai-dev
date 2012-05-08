// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zipfs/zipfs.h>
#include <zipfs/zipfs_util.h>
#include <zipfs/ZIPFSDirectoryItr.h>
#include <zipfs/ZIPFSFile.h>
#include <zipfs/ZIPFSFileSystem.h>
#include <zipfs/ZIPFSVirtualPath.h>
#include <zipfs/ZIPFSZipArchive.h>
#include <zipfs/ZIPFSZipStream.h>

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

using namespace std;

//================================================================//
// ZIPFSTlsfPool
//================================================================//
typedef struct ZIPFSTlsfPool {

	tlsf_pool	mPool;
	void*		mBuffer;

} ZIPFSTlsfPool;

//================================================================//
// zipfs
//================================================================//

ZIPFSFILE* zipfs_stderr = 0;
ZIPFSFILE* zipfs_stdin = 0;
ZIPFSFILE* zipfs_stdout = 0;

//----------------------------------------------------------------//
int zipfs_affirm_path ( const char* path ) {

	return ZIPFSFileSystem::Get ().AffirmPath ( path );
}

//----------------------------------------------------------------//
int zipfs_chdir ( const char* path ) {

	return ZIPFSFileSystem::Get ().ChangeDir ( path );
}

//----------------------------------------------------------------//
void zipfs_cleanup ( void ) {

	ZIPFSFileSystem::Get ().Cleanup ();

	ZIPFSFile* file;
	
	file = ( ZIPFSFile* )zipfs_stderr;
	file->SetFile ( 0 );
	delete file;

	file = ( ZIPFSFile* )zipfs_stdin;
	file->SetFile ( 0 );
	delete file;

	file = ( ZIPFSFile* )zipfs_stdout;
	file->SetFile ( 0 );
	delete file;
	
	zipfs_stderr = 0;
	zipfs_stdin = 0;
	zipfs_stdout = 0;
}

//----------------------------------------------------------------//
void zipfs_dir_close ( ZIPFSDIR* dir ) {

	ZIPFSDirectoryItr* itr = ( ZIPFSDirectoryItr* )dir;
	if ( itr ) {
		delete itr;
	}
}

//----------------------------------------------------------------//
char const* zipfs_dir_entry_name ( ZIPFSDIR* dir ) {

	ZIPFSDirectoryItr* itr = ( ZIPFSDirectoryItr* )dir;
	if ( itr ) {
		return itr->GetEntryName ();
	}
	return 0;
}

//----------------------------------------------------------------//
int zipfs_dir_entry_is_subdir ( ZIPFSDIR* dir ) {

	ZIPFSDirectoryItr* itr = ( ZIPFSDirectoryItr* )dir;
	if ( itr ) {
		return itr->IsSubdir () ? 1 : 0;
	}
	return 0;
}

//----------------------------------------------------------------//
ZIPFSDIR* zipfs_dir_open ( void ) {

	ZIPFSDirectoryItr* itr = new ZIPFSDirectoryItr ();
	int result = itr->Open ();
	
	if ( result ) {
		delete itr;
		return 0;
	}
	return ( ZIPFSDIR* )itr;
}

//----------------------------------------------------------------//
int zipfs_dir_read_entry ( ZIPFSDIR* dir ) {

	ZIPFSDirectoryItr* itr = ( ZIPFSDirectoryItr* )dir;
	if ( itr ) {
		return itr->ReadEntry ();
	}
	return 0;
}

//----------------------------------------------------------------//
int zipfs_get_stat ( char const* path, zipfs_stat* filestat ) {

	ZIPFSFileSystem& filesys = ZIPFSFileSystem::Get ();

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
	string abspath;

	filestat->mExists = 0;

	abspath = filesys.GetAbsoluteFilePath ( path );
	mount = filesys.FindBestVirtualPath ( abspath.c_str ());

	if ( mount ) {

		const char* localpath = mount->GetLocalPath ( abspath.c_str ());
		
		if ( abspath.size () && localpath ) {
		
			ZIPFSZipFileEntry* entry;
		
			ZIPFSZipFileDir* dir = mount->mArchive->FindDir ( localpath );
		
			const char *filename = localpath;
			int i = strlen ( filename ) - 1;

			result = stat ( mount->mArchive->mFilename.c_str (), &s );

			if ( result ) return -1;
			
			for ( ; i >= 0; --i ) {
				if ( filename [ i ] == '/' ) {
					filename = &filename [ i + 1 ];
					break;
				}
			}
			
			entry = mount->mArchive->FindEntry ( localpath );

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
char* zipfs_getcwd ( char* buffer, size_t length ) {

	string path = ZIPFSFileSystem::Get ().GetWorkingPath ();
	if ( path.length () < length ) {
		strcpy ( buffer, path.c_str ());
		return buffer;
	}
	return 0;
}

//----------------------------------------------------------------//
void zipfs_init () {

	ZIPFSFileSystem::Get ().Init ();

	ZIPFSFile* file;

	file = new ZIPFSFile ();
	file->SetFile ( stderr );
	zipfs_stderr = ( ZIPFSFILE* )file;
	
	file = new ZIPFSFile ();
	file->SetFile ( stdin );
	zipfs_stdin = ( ZIPFSFILE* )file;
	
	file = new ZIPFSFile ();
	file->SetFile ( stdout );
	zipfs_stdout = ( ZIPFSFILE* )file;
}

//----------------------------------------------------------------//
int zipfs_mkdir ( const char* path ) {

	return ZIPFSFileSystem::Get ().MakeDir ( path );
}

//----------------------------------------------------------------//
int zipfs_mount_virtual ( const char* virtualPath, const char* archive ) {
	return ZIPFSFileSystem::Get ().MountVirtual ( virtualPath, archive );
}

//----------------------------------------------------------------//
int zipfs_rmdir ( const char* path ) {
	return ZIPFSFileSystem::Get ().RemoveDir ( path );
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

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		file->ClearError ();
	}
}

//----------------------------------------------------------------//
int	zipfs_fclose ( ZIPFSFILE* fp ) {

	int result = 0;

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		result = file->Close ();
		delete file;
	}
	return result;
}

//----------------------------------------------------------------//
int zipfs_feof ( ZIPFSFILE* fp ) {

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->IsEOF ();
	}
	return 0;
}

//----------------------------------------------------------------//
int zipfs_ferror ( ZIPFSFILE* fp ) {

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->GetError ();
	}
	return 0;
}

//----------------------------------------------------------------//
int	zipfs_fflush ( ZIPFSFILE* fp ) {
	
	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->Flush ();
	}
	return 0;
}

//----------------------------------------------------------------//
int zipfs_fgetc ( ZIPFSFILE* fp ) {
	
	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->GetChar ();
	}
	return EOF;
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
	
	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->GetString ( string, length );
	}
	return 0;
}

//----------------------------------------------------------------//
int	zipfs_fileno ( ZIPFSFILE* fp ) {
	
	// TODO:
	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->GetFileNum ();
	}
	return -1;
}

//----------------------------------------------------------------//
ZIPFSFILE* zipfs_fopen ( const char* filename, const char* mode ) {
	
	ZIPFSFile* file = new ZIPFSFile ();
	int result = file->Open ( filename, mode );
	
	if ( result ) {
		delete file;
		return 0;
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

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->PutChar ( c );
	}
	return EOF;
}

//----------------------------------------------------------------//
int zipfs_fputs ( const char* string, ZIPFSFILE* fp ) {

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->PutString ( string );
	}
	return EOF;
}

//----------------------------------------------------------------//
size_t zipfs_fread ( void* buffer, size_t size, size_t count, ZIPFSFILE* fp ) {
	
	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->Read ( buffer, size, count );
	}
	return 0;
}

//----------------------------------------------------------------//
ZIPFSFILE* zipfs_freopen ( const char* filename, const char* mode, ZIPFSFILE* fp ) {

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		delete file;
	}
	return zipfs_fopen ( filename, mode );
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
	
	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->Seek ( offset, origin );
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

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->Tell ();
	}
	return -1L;
}

//----------------------------------------------------------------//
size_t zipfs_fwrite ( const void* data, size_t size, size_t count, ZIPFSFILE* fp ) {
	
	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->Write ( data, size, count );
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
	if ( file ) {
		file->CloseProcess ();
	}
	return -1;
}

//----------------------------------------------------------------//
ZIPFSFILE* zipfs_popen ( const char *command, const char *mode ) {

	ZIPFSFile* file = new ZIPFSFile ();
	int result = file->OpenProcess ( command, mode );
	
	if ( result ) {
		delete file;
		return 0;
	}
	return ( ZIPFSFILE* )file;
}

//----------------------------------------------------------------//
int zipfs_putc ( int character, ZIPFSFILE* fp ) {

	return zipfs_fputc ( character, fp );
}

//----------------------------------------------------------------//
int zipfs_remove ( const char* path ) {

	return ZIPFSFileSystem::Get ().Remove ( path );
}

//----------------------------------------------------------------//
int zipfs_rename ( const char* oldname, const char* newname ) {

	return ZIPFSFileSystem::Get ().Rename ( oldname, newname );
}

//----------------------------------------------------------------//
void zipfs_rewind ( ZIPFSFILE* fp ) {

	zipfs_fseek ( fp, 0, SEEK_SET );
}


//----------------------------------------------------------------//
void zipfs_setbuf ( ZIPFSFILE* fp, char* buffer ) {

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		file->SetBuf ( buffer );
	}
}

//----------------------------------------------------------------//
int zipfs_setvbuf ( ZIPFSFILE* fp, char* buffer, int mode, size_t size ) {

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->SetVBuf ( buffer, mode, size );
	}
	return 0;
}

//----------------------------------------------------------------//
ZIPFSFILE* zipfs_tmpfile ( void ) {

	ZIPFSFile* file = new ZIPFSFile ();
	int result = file->OpenTemp ();
	
	if ( result ) {
		delete file;
		return 0;
	}
	return ( ZIPFSFILE* )file;
}

//----------------------------------------------------------------//
char* zipfs_tmpnam ( char* str ) {

	return tmpnam ( str );
}

//----------------------------------------------------------------//
int zipfs_ungetc ( int character, ZIPFSFILE* fp ) {
	
	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->UnGetChar ( character );
	}
	return EOF;
}

//----------------------------------------------------------------//
int zipfs_vfprintf ( ZIPFSFILE* fp, const char* format, va_list arg ) {

	ZIPFSFile* file = ( ZIPFSFile* )fp;
	if ( file ) {
		return file->VarPrintf ( format, arg );
	}
	return -1;
}