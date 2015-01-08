// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_config.h>
#include <zl-vfs/headers.h>
#include <zl-vfs/zl_util.h>
#include <zl-vfs/ZLVfsDirectoryItr.h>
#include <zl-vfs/ZLVfsVirtualPath.h>
#include <zl-vfs/ZLVfsZipArchive.h>
#include <zl-vfs/ZLVfsZipStream.h>

#ifdef _WIN32
	#include <direct.h>
	#include <io.h>
	#include <windows.h>
#else
	#include <sys/types.h>
	#include <dirent.h>
	#include <unistd.h>
#endif

#include <errno.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <time.h>
#include <tlsf.h>

#ifdef _WIN32
	#define S_ISDIR(B) (((B)&_S_IFDIR)!=0)
#endif

#ifdef NACL
	#include "NaClFile.h"
#endif

#ifdef ANDROID
	#include <android/log.h>
#endif

#include <zl-vfs/ZLVfsFile.h>
#include <zl-vfs/ZLVfsFileSystem.h>

using namespace std;

//================================================================//
// ZLTlsfPool
//================================================================//
typedef struct ZLTlsfPool {

	tlsf_pool	mPool;
	void*		mBuffer;

} ZLTlsfPool;

//================================================================//
// zlcore
//================================================================//

ZLFILE* zl_stderr = 0;
ZLFILE* zl_stdin = 0;
ZLFILE* zl_stdout = 0;

//----------------------------------------------------------------//
int zl_affirm_path ( const char* path ) {

	return ZLVfsFileSystem::Get ().AffirmPath ( path );
}

//----------------------------------------------------------------//
int zl_chdir ( const char* path ) {

	return ZLVfsFileSystem::Get ().ChangeDir ( path );
}

//----------------------------------------------------------------//
void zl_cleanup ( void ) {

	ZLVfsFileSystem::Get ().Cleanup ();

	ZLVfsFile* file;
	
	file = ( ZLVfsFile* )zl_stderr;
	file->SetFile ( 0 );
	delete file;

	file = ( ZLVfsFile* )zl_stdin;
	file->SetFile ( 0 );
	delete file;

	file = ( ZLVfsFile* )zl_stdout;
	file->SetFile ( 0 );
	delete file;
	
	zl_stderr = 0;
	zl_stdin = 0;
	zl_stdout = 0;
}

//----------------------------------------------------------------//
void zl_dir_close ( ZLDIR* dir ) {

	ZLVfsDirectoryItr* itr = ( ZLVfsDirectoryItr* )dir;
	if ( itr ) {
		delete itr;
	}
}

//----------------------------------------------------------------//
char const* zl_dir_entry_name ( ZLDIR* dir ) {

	ZLVfsDirectoryItr* itr = ( ZLVfsDirectoryItr* )dir;
	if ( itr ) {
		return itr->GetEntryName ();
	}
	return 0;
}

//----------------------------------------------------------------//
int zl_dir_entry_is_subdir ( ZLDIR* dir ) {

	ZLVfsDirectoryItr* itr = ( ZLVfsDirectoryItr* )dir;
	if ( itr ) {
		return itr->IsSubdir () ? 1 : 0;
	}
	return 0;
}

//----------------------------------------------------------------//
ZLDIR* zl_dir_open ( void ) {

	ZLVfsDirectoryItr* itr = new ZLVfsDirectoryItr ();
	int result = itr->Open ();
	
	if ( result ) {
		delete itr;
		return 0;
	}
	return ( ZLDIR* )itr;
}

//----------------------------------------------------------------//
int zl_dir_read_entry ( ZLDIR* dir ) {

	ZLVfsDirectoryItr* itr = ( ZLVfsDirectoryItr* )dir;
	if ( itr ) {
		return itr->ReadEntry ();
	}
	return 0;
}

//----------------------------------------------------------------//
int zl_get_stat ( char const* path, zl_stat* filestat ) {

	ZLVfsFileSystem& filesys = ZLVfsFileSystem::Get ();

	#ifdef NACL
		#define stat stat
	#endif
	
	struct stat s;
	
	#ifdef NACL
		#define stat nacl_stat
	#endif

	//struct stat s;
	int result;
	ZLVfsVirtualPath* mount;
	string abspath;

	filestat->mExists = 0;

	abspath = filesys.GetAbsoluteFilePath ( path );
	mount = filesys.FindBestVirtualPath ( abspath.c_str ());

	if ( mount ) {

		const char* localpath = mount->GetLocalPath ( abspath.c_str ());
		
		if ( abspath.size () && localpath ) {
		
			ZLVfsZipFileDir* parentDir = mount->mArchive->FindDir ( localpath );
			ZLVfsZipFileDir* dir;
			ZLVfsZipFileEntry* entry;

			const char *filename = localpath;
			int i = strlen ( filename ) - 2;

			result = stat ( mount->mArchive->mFilename.c_str (), &s );

			if ( result ) return -1;

			for ( ; i >= 0; --i ) {
				if ( filename [ i ] == '/' ) {
					filename = &filename [ i + 1 ];
					break;
				}
			}

			entry = parentDir->mChildFiles;
			for ( ; entry; entry = entry->mNext ) {
				if ( strcmp_ignore_case ( entry->mName.c_str (), filename ) == 0 ) break;
			}

			if ( entry ) {
				// Entry
				filestat->mIsDir		= 0;
				filestat->mSize			= entry->mUncompressedSize;
			}
			else {
				 // No entries found, check for directories
				dir = parentDir;
				std::string dirname = filename;
				if ( dirname.length () && dirname [ dirname.length () - 1 ] != '/' ) {
					dir = dir->mChildDirs;
					dirname.append ( "/" );
				}
				for ( ; dir; dir = dir->mNext ) {
					if ( strcmp_ignore_case ( dir->mName.c_str (), dirname.c_str ()) == 0 ) break;
				}

				if ( dir ) {
					filestat->mIsDir			= 1;
					filestat->mSize				= 0;
				}
				else {
					return 0;
				}
			}
			
			filestat->mExists		    = 1;
			filestat->mTimeCreated		= s.st_ctime;
			filestat->mTimeModified		= s.st_mtime;
			filestat->mTimeViewed		= s.st_atime;
		}
	}
	else {
		
		while ( abspath.size () && ( abspath [ abspath.length () - 1 ] == '/' )) {
			abspath [ abspath.length () - 1 ] = 0;
		}
		
		result = stat ( abspath.c_str (), &s );
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
char* zl_getcwd ( char* buffer, size_t length ) {

	string path = ZLVfsFileSystem::Get ().GetWorkingPath ();
	if ( path.length () < length ) {
		strcpy ( buffer, path.c_str ());
		return buffer;
	}
	return 0;
}

//----------------------------------------------------------------//
void zl_init () {

	ZLVfsFileSystem::Get ().Init ();

	ZLVfsFile* file;

	file = new ZLVfsFile ();
	file->SetFile ( stderr );
	zl_stderr = ( ZLFILE* )file;
	
	file = new ZLVfsFile ();
	file->SetFile ( stdin );
	zl_stdin = ( ZLFILE* )file;
	
	file = new ZLVfsFile ();
	file->SetFile ( stdout );
	zl_stdout = ( ZLFILE* )file;
}

//----------------------------------------------------------------//
int zl_mkdir ( const char* path ) {

	return ZLVfsFileSystem::Get ().MakeDir ( path );
}

//----------------------------------------------------------------//
int zl_mount_virtual ( const char* virtualPath, const char* archive ) {
	return ZLVfsFileSystem::Get ().MountVirtual ( virtualPath, archive );
}

//----------------------------------------------------------------//
int zl_rmdir ( const char* path ) {
	return ZLVfsFileSystem::Get ().RemoveDir ( path );
}

//================================================================//
// stdlib
//================================================================//

static ZLTlsfPool* sTlsfPool = 0;

//----------------------------------------------------------------//
void* zl_calloc ( size_t num, size_t size ) {

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
void zl_free ( void* ptr ) {

	if ( sTlsfPool ) {
		tlsf_free ( sTlsfPool->mPool, ptr );
	}
	else {
		free ( ptr );
	}
}

//----------------------------------------------------------------//
void* zl_malloc ( size_t size ) {

	if ( sTlsfPool ) {
		return tlsf_malloc ( sTlsfPool->mPool, size );
	}
	return malloc ( size );
}

//----------------------------------------------------------------//
void* zl_realloc ( void* ptr, size_t size ) {

	if ( sTlsfPool ) {
		return tlsf_realloc ( sTlsfPool->mPool, ptr, size );
	}
	return realloc ( ptr, size );
}

//----------------------------------------------------------------//
ZL_TLSF_POOL* zl_tlsf_create_pool ( size_t bytes ) {
	
	ZLTlsfPool* pool = ( ZLTlsfPool* )malloc ( sizeof ( ZLTlsfPool ));
	
	pool->mBuffer = malloc ( bytes );
	pool->mPool = tlsf_create ( pool->mBuffer, bytes );
	
	return ( ZL_TLSF_POOL* )pool;
}

//----------------------------------------------------------------//
void zl_tlsf_destroy_pool ( ZL_TLSF_POOL* opaque ) {

	ZLTlsfPool* pool = ( ZLTlsfPool* )opaque;

	if ( pool ) {
		tlsf_destroy ( pool->mPool );
		free ( pool->mBuffer );
		free ( pool );
	}
}

//----------------------------------------------------------------//
ZL_TLSF_POOL* zl_tlsf_get_pool ( void ) {

	return ( ZL_TLSF_POOL* )sTlsfPool;
}

//----------------------------------------------------------------//
void zl_tlsf_set_pool ( ZL_TLSF_POOL* opaque ) {

	sTlsfPool = ( ZLTlsfPool* )opaque;
}

//================================================================//
// stdio
//================================================================//

//----------------------------------------------------------------//
void zl_clearerr ( ZLFILE* fp ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		file->ClearError ();
	}
}

//----------------------------------------------------------------//
int	zl_fclose ( ZLFILE* fp ) {

	int result = 0;

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		result = file->Close ();
		delete file;
	}
	return result;
}

//----------------------------------------------------------------//
int zl_feof ( ZLFILE* fp ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->IsEOF ();
	}
	return 0;
}

//----------------------------------------------------------------//
int zl_ferror ( ZLFILE* fp ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->GetError ();
	}
	return 0;
}

//----------------------------------------------------------------//
int	zl_fflush ( ZLFILE* fp ) {
	
	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->Flush ();
	}
	return 0;
}

//----------------------------------------------------------------//
int zl_fgetc ( ZLFILE* fp ) {
	
	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->GetChar ();
	}
	return EOF;
}

//----------------------------------------------------------------//
int zl_fgetpos ( ZLFILE* fp, fpos_t* position ) {
	(( void )fp );
	(( void )position );

	assert ( 0 ); // not implemented
	return -1;
}

//----------------------------------------------------------------//
char* zl_fgets ( char* string, int length, ZLFILE* fp ) {
	
	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->GetString ( string, length );
	}
	return 0;
}

//----------------------------------------------------------------//
int	zl_fileno ( ZLFILE* fp ) {
	
	// TODO:
	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->GetFileNum ();
	}
	return -1;
}

//----------------------------------------------------------------//
ZLFILE* zl_fopen ( const char* filename, const char* mode ) {
	
	ZLVfsFile* file = new ZLVfsFile ();
	int result = file->Open ( filename, mode );
	
	if ( result ) {
		delete file;
		return 0;
	}
	return ( ZLFILE* )file;
}

//----------------------------------------------------------------//
#ifdef MOAI_COMPILER_MSVC
	errno_t zl_fopen_s ( ZLFILE** fp, const char* filename, const char* mode ) {

		*fp = zl_fopen ( filename, mode );
		return errno;
	}

	ZLFILE *zl_wfopen(const wchar_t *filename, const wchar_t *mode) {
		ZLVfsFile* file = new ZLVfsFile();

		//Convert to ansi for our zlvfsfile functions
		char filenameA[260];
		char modeA[20];
		char DefChar = ' ';
		WideCharToMultiByte(CP_ACP, 0, filename, -1, filenameA, 260, &DefChar, NULL);
		WideCharToMultiByte(CP_ACP, 0, mode, -1, modeA, 260, &DefChar, NULL);



		int result = file->Open(filenameA, modeA);

		if (result) {
			delete file;
			return 0;
		}
		return (ZLFILE*)file;
	}
#endif

//----------------------------------------------------------------//
int zl_fprintf ( ZLFILE * fp, const char * format, ... ) {

	int result;

	va_list args;
	va_start ( args, format );
	result = zl_vfprintf ( fp, format, args );
	va_end ( args );

	return result;
}

//----------------------------------------------------------------//
int zl_fputc ( int c, ZLFILE* fp ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->PutChar ( c );
	}
	return EOF;
}

//----------------------------------------------------------------//
int zl_fputs ( const char* string, ZLFILE* fp ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->PutString ( string );
	}
	return EOF;
}

//----------------------------------------------------------------//
size_t zl_fread ( void* buffer, size_t size, size_t count, ZLFILE* fp ) {
	
	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->Read ( buffer, size, count );
	}
	return 0;
}

//----------------------------------------------------------------//
ZLFILE* zl_freopen ( const char* filename, const char* mode, ZLFILE* fp ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {

		int result = file->Reopen ( filename, mode );

		if ( result == 0 ) {
			return ( ZLFILE* )file;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int zl_fscanf ( ZLFILE* fp, const char * format, ... ) {
	
	int result;
	va_list args;
	
	va_start ( args, format );
	result = zl_vfscanf ( fp, format, args );
	va_end ( args );
	
	return result;
}

//----------------------------------------------------------------//
int	zl_fseek ( ZLFILE* fp, long offset, int origin ) {
	
	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->Seek ( offset, origin );
	}
	return -1;
}

#if defined(__APPLE__) || defined(EMSCRIPTEN) || defined(__unix__) || defined(MOAI_COMPILER_MSVC)
	int zl_fseeko ( ZLFILE* fp, off_t offset, int origin ) {
		// TODO:
		return zl_fseek ( fp, ( long )offset, origin );
	}
#endif



#ifdef __MINGW32__
	int zl_fseeko64 ( ZLFILE* fp, __int64 offset, int origin ) {

		// TODO:
		return zl_fseek ( fp, ( long )offset, origin );
	}
#endif

//----------------------------------------------------------------//
#ifdef MOAI_COMPILER_MSVC
	int zl_fseeki64 ( ZLFILE* fp, __int64 offset, int origin ) {

		// TODO:
		return zl_fseek ( fp, ( long )offset, origin );
	}
#endif

//----------------------------------------------------------------//
int zl_fsetpos ( ZLFILE* fp, const fpos_t * pos ) {
	(( void )fp );
	(( void )pos );

	assert ( 0 ); // not implemented
	return -1;
}

//----------------------------------------------------------------//
long zl_ftell ( ZLFILE* fp ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->Tell ();
	}
	return -1L;
}

#if defined (__APPLE__) || defined (EMSCRIPTEN) || defined(__unix__)
off_t zl_ftello ( ZLFILE* fp ) {
	return (off_t) zl_ftell(fp);
}
#endif

//----------------------------------------------------------------//
size_t zl_fwrite ( const void* data, size_t size, size_t count, ZLFILE* fp ) {
	
	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->Write ( data, size, count );
	}
	return 0;
}

//----------------------------------------------------------------//
int zl_getc ( ZLFILE* fp ) {

	return zl_fgetc ( fp );
}

//----------------------------------------------------------------//
int zl_getwc ( ZLFILE* fp ) {
	UNUSED ( fp );

	assert ( 0 ); // TODO:
	return -1;
}

//----------------------------------------------------------------//
int zl_pclose ( ZLFILE* fp ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		file->CloseProcess ();
	}
	return -1;
}

//----------------------------------------------------------------//
ZLFILE* zl_popen ( const char* command, const char *mode ) {

	ZLVfsFile* file = new ZLVfsFile ();
	int result = file->OpenProcess ( command, mode );
	
	if ( result ) {
		delete file;
		return 0;
	}
	return ( ZLFILE* )file;
}

//----------------------------------------------------------------//
int zl_printf ( const char* format, ... ) {

	int result;

	va_list args;
	va_start ( args, format );
	result = zl_vprintf ( format, args );
	va_end ( args );

	return result;
}

//----------------------------------------------------------------//
int zl_putc ( int character, ZLFILE* fp ) {

	return zl_fputc ( character, fp );
}

//----------------------------------------------------------------//
int zl_remove ( const char* path ) {

	return ZLVfsFileSystem::Get ().Remove ( path );
}

//----------------------------------------------------------------//
int zl_rename ( const char* oldname, const char* newname ) {

	return ZLVfsFileSystem::Get ().Rename ( oldname, newname );
}

//----------------------------------------------------------------//
void zl_rewind ( ZLFILE* fp ) {

	zl_fseek ( fp, 0, SEEK_SET );
}


//----------------------------------------------------------------//
void zl_setbuf ( ZLFILE* fp, char* buffer ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		file->SetBuf ( buffer );
	}
}

//----------------------------------------------------------------//
int zl_setvbuf ( ZLFILE* fp, char* buffer, int mode, size_t size ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->SetVBuf ( buffer, mode, size );
	}
	return 0;
}

//----------------------------------------------------------------//
ZLFILE* zl_tmpfile ( void ) {

	ZLVfsFile* file = new ZLVfsFile ();
	int result = file->OpenTemp ();
	
	if ( result ) {
		delete file;
		return 0;
	}
	return ( ZLFILE* )file;
}

//----------------------------------------------------------------//
char* zl_tmpnam ( char* str ) {

#ifdef MOAI_OS_LINUX
    // The last six characters of template must be "XXXXXX"
    // Example:
    //      char ss[200] = "/tmp/moaiXXXXXX";
    //      zl_tmpnam(ss);
    mkstemp(str);
    return str;
#else
	return tmpnam ( str );
#endif 
}

//----------------------------------------------------------------//
int zl_ungetc ( int character, ZLFILE* fp ) {
	
	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->UnGetChar ( character );
	}
	return EOF;
}

//----------------------------------------------------------------//
int zl_vfprintf ( ZLFILE* fp, const char* format, va_list arg ) {

	ZLVfsFile* file = ( ZLVfsFile* )fp;
	if ( file ) {
		return file->VarPrintf ( format, arg );
	}
	return -1;
}

//----------------------------------------------------------------//
int zl_vprintf ( const char * format, va_list arg ) {

	int result;
	
	#ifdef ANDROID
		result = __android_log_vprint ( ANDROID_LOG_INFO, "MoaiLog", format, arg );
	#else
		result = vprintf ( format, arg );
	#endif

	return result;
}
