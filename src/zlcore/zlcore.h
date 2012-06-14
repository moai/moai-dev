// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_H
#define ZL_H

#include <zlcore/pch.h>
#include <zlcore/zl_mutex.h>

#ifdef  __cplusplus
	extern "C" {
#endif

//================================================================//
// zl_stat
//================================================================//
typedef struct zl_stat {

	int						mExists;
	int						mIsDir;
	
	unsigned long long		mTimeCreated;	// Time created (in seconds from cardinal date)
	unsigned long long		mTimeModified;	// Time last modified (in seconds from cardinal date)
	unsigned long long		mTimeViewed;	// Time last viewed (in seconds from cardinal date)
	size_t					mSize;			// Yeah, that

} zl_stat;

//================================================================//
// zlcore
//================================================================//

typedef void ZLDIR;
typedef void ZLFILE;
typedef void ZL_TLSF_POOL;

extern ZLFILE* zl_stderr;
extern ZLFILE* zl_stdin;
extern ZLFILE* zl_stdout;

//----------------------------------------------------------------//
extern int					zl_affirm_path				( const char* path );
extern int					zl_chdir					( const char* path );
extern void					zl_cleanup					( void );
extern void					zl_dir_close				( ZLDIR* dir );
extern char const*			zl_dir_entry_name			( ZLDIR* dir );
extern int					zl_dir_entry_is_subdir		( ZLDIR* dir );
extern ZLDIR*				zl_dir_open					( void );
extern int					zl_dir_read_entry			( ZLDIR* dir );
extern int					zl_get_stat					( char const* path, zl_stat* filestat );
extern char*				zl_getcwd					( char* buffer, size_t length );
extern void					zl_init						( void );
extern int					zl_mkdir					( const char* path );
extern int					zl_mount_virtual			( const char* virtualPath, const char* archive );
extern int					zl_rmdir					( const char* path );

//================================================================//
// stdlib
//================================================================//

//----------------------------------------------------------------//
extern void*				zl_calloc					( size_t num, size_t size );
extern void					zl_free						( void* ptr );
extern void*				zl_malloc					( size_t size );
extern void*				zl_realloc					( void* ptr, size_t size );
extern ZL_TLSF_POOL*		zl_tlsf_create_pool			( size_t bytes );
extern void					zl_tlsf_destroy_pool		( ZL_TLSF_POOL* opaque );
extern ZL_TLSF_POOL*		zl_tlsf_get_pool			( void );
extern void					zl_tlsf_set_pool			( ZL_TLSF_POOL* opaque );

//================================================================//
// stdio
//================================================================//

//----------------------------------------------------------------//
extern void					zl_clearerr				( ZLFILE* fp );
extern int					zl_fclose				( ZLFILE* fp );
extern int					zl_feof					( ZLFILE* fp );
extern int					zl_ferror				( ZLFILE* fp );
extern int					zl_fflush				( ZLFILE* fp );
extern int					zl_fgetc				( ZLFILE* fp );
extern int					zl_fgetpos				( ZLFILE* fp, fpos_t* position );
extern char* 				zl_fgets				( char* string, int length, ZLFILE* fp );
extern int					zl_fileno				( ZLFILE* fp );
extern ZLFILE* 				zl_fopen 				( const char* filename, const char* mode );
extern int					zl_fprintf				( ZLFILE* fp, const char * format, ... );
extern int 					zl_fputc				( int c, ZLFILE* fp );
extern int					zl_fputs				( const char* string, ZLFILE* fp );
extern size_t				zl_fread				( void* buffer, size_t size, size_t count, ZLFILE* fp );
extern ZLFILE*				zl_freopen				( const char* filename, const char* mode, ZLFILE* fp );
extern int					zl_fscanf				( ZLFILE* fp, const char* format, ... );
extern int					zl_fseek				( ZLFILE* fp, long offset, int origin );
extern int					zl_fsetpos				( ZLFILE* fp, const fpos_t * pos );
extern long					zl_ftell				( ZLFILE* fp );
extern size_t				zl_fwrite				( const void* data, size_t size, size_t count, ZLFILE* fp );
extern int					zl_getc					( ZLFILE* fp );
extern int					zl_getwc				( ZLFILE* fp );
extern int					zl_pclose				( ZLFILE* fp );
extern ZLFILE*				zl_popen				( const char *command, const char *mode );
extern int					zl_putc					( int character, ZLFILE* fp );
extern int					zl_remove				( const char* path );
extern int					zl_rename				( const char* oldname, const char* newname );
extern void					zl_rewind				( ZLFILE* fp );
extern void					zl_setbuf				( ZLFILE* fp, char* buffer );
extern int					zl_setvbuf				( ZLFILE* fp, char* buffer, int mode, size_t size );
extern ZLFILE*				zl_tmpfile				( void );
extern char*				zl_tmpnam				( char* str );
extern int					zl_ungetc				( int character, ZLFILE* fp );
extern int					zl_vfprintf				( ZLFILE* fp, const char* format, va_list arg );
extern int					zl_vfscanf				( ZLFILE* fp, const char* format, va_list arg );

#ifdef MOAI_COMPILER_MSVC
	extern errno_t	 		zl_fopen_s 				( ZLFILE** fp, const char* filename, const char* mode );
	extern int				zl_fseeki64				( ZLFILE* fp, __int64 offset, int origin );
#endif

#ifdef  __cplusplus
	}
#endif

#endif