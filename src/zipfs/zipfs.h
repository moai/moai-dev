// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFS_H
#define ZIPFS_H

#include <zipfs/pch.h>

#ifdef  __cplusplus
	extern "C" {
#endif

//================================================================//
// zipfs_stat
//================================================================//
typedef struct zipfs_stat {

	int						mExists;
	int						mIsDir;
	
	unsigned long long		mTimeCreated;	// Time created (in seconds from cardinal date)
	unsigned long long		mTimeModified;	// Time last modified (in seconds from cardinal date)
	unsigned long long		mTimeViewed;	// Time last viewed (in seconds from cardinal date)
	size_t					mSize;			// Yeah, that

} zipfs_stat;

//================================================================//
// stdlib
//================================================================//

typedef void ZIPFS_TLSF_POOL;

//----------------------------------------------------------------//
extern void*				zipfs_calloc				( size_t num, size_t size );
extern void					zipfs_free					( void* ptr );
extern void*				zipfs_malloc				( size_t size );
extern void*				zipfs_realloc				( void* ptr, size_t size );
extern ZIPFS_TLSF_POOL*		zipfs_tlsf_create_pool		( size_t bytes );
extern void					zipfs_tlsf_destroy_pool		( ZIPFS_TLSF_POOL* opaque );
extern ZIPFS_TLSF_POOL*		zipfs_tlsf_get_pool			( void );
extern void					zipfs_tlsf_set_pool			( ZIPFS_TLSF_POOL* opaque );

//================================================================//
// stdio
//================================================================//

typedef void ZIPFSFILE;

extern ZIPFSFILE* zipfs_stderr;
extern ZIPFSFILE* zipfs_stdin;
extern ZIPFSFILE* zipfs_stdout;

//----------------------------------------------------------------//
extern void					zipfs_clearerr				( ZIPFSFILE* fp );
extern int					zipfs_fclose				( ZIPFSFILE* fp );
extern int					zipfs_feof					( ZIPFSFILE* fp );
extern int					zipfs_ferror				( ZIPFSFILE* fp );
extern int					zipfs_fflush				( ZIPFSFILE* fp );
extern int					zipfs_fgetc					( ZIPFSFILE* fp );
extern int					zipfs_fgetpos				( ZIPFSFILE* fp, fpos_t* position );
extern char* 				zipfs_fgets					( char* string, int length, ZIPFSFILE* fp );
extern int					zipfs_fileno				( ZIPFSFILE* fp );
extern ZIPFSFILE* 			zipfs_fopen 				( const char* filename, const char* mode );
extern int					zipfs_fprintf				( ZIPFSFILE* fp, const char * format, ... );
extern int 					zipfs_fputc					( int c, ZIPFSFILE* fp );
extern int					zipfs_fputs					( const char* string, ZIPFSFILE* fp );
extern size_t				zipfs_fread					( void* buffer, size_t size, size_t count, ZIPFSFILE* fp );
extern ZIPFSFILE*			zipfs_freopen				( const char* filename, const char* mode, ZIPFSFILE* fp );
extern int					zipfs_fscanf				( ZIPFSFILE* fp, const char* format, ... );
extern int					zipfs_fseek					( ZIPFSFILE* fp, long offset, int origin );
extern int					zipfs_fsetpos				( ZIPFSFILE* fp, const fpos_t * pos );
extern long					zipfs_ftell					( ZIPFSFILE* fp );
extern size_t				zipfs_fwrite				( const void* data, size_t size, size_t count, ZIPFSFILE* fp );
extern int					zipfs_getc					( ZIPFSFILE* fp );
extern int					zipfs_getwc					( ZIPFSFILE* fp );

extern int					zipfs_pclose				( ZIPFSFILE* fp );
extern ZIPFSFILE*			zipfs_popen					( const char *command, const char *mode );

extern int					zipfs_putc					( int character, ZIPFSFILE* fp );
extern int					zipfs_remove				( const char* path );
extern int					zipfs_rename				( const char* oldname, const char* newname );
extern void					zipfs_rewind				( ZIPFSFILE* fp );
extern void					zipfs_setbuf				( ZIPFSFILE* fp, char* buffer );
extern int					zipfs_setvbuf				( ZIPFSFILE* fp, char* buffer, int mode, size_t size );
extern ZIPFSFILE*			zipfs_tmpfile				( void );
extern char*				zipfs_tmpnam				( char* str );
extern int					zipfs_ungetc				( int character, ZIPFSFILE* fp );
extern int					zipfs_vfprintf				( ZIPFSFILE* fp, const char* format, va_list arg );
extern int					zipfs_vfscanf				( ZIPFSFILE* fp, const char* format, va_list arg );

#ifdef MOAI_COMPILER_MSVC
	extern errno_t	 			zipfs_fopen_s 				( ZIPFSFILE** fp, const char* filename, const char* mode );
	extern int					zipfs_fseeki64				( ZIPFSFILE* fp, __int64 offset, int origin );
#endif

//================================================================//
// extra
//================================================================//

typedef void ZIPFSDIR;

//----------------------------------------------------------------//
extern int					zipfs_affirm_path				( const char* path );
extern char*				zipfs_basename					( const char* filename );
extern char*				zipfs_bless_path				( const char* path );
extern int					zipfs_chdir						( const char* path );
extern void					zipfs_cleanup					( void );
extern void					zipfs_dir_close					( ZIPFSDIR* dir );
extern char const*			zipfs_dir_entry_name			( ZIPFSDIR* dir );
extern int					zipfs_dir_entry_is_subdir		( ZIPFSDIR* dir );
extern ZIPFSDIR*			zipfs_dir_open					( void );
extern int					zipfs_dir_read_entry			( ZIPFSDIR* dir );
extern char*				zipfs_get_abs_dirpath			( const char* path );
extern char*				zipfs_get_abs_filepath			( const char* path );
extern char*				zipfs_get_rel_path				( const char* path );
extern int					zipfs_get_stat					( char const* path, zipfs_stat* filestat );
extern char*				zipfs_get_working_path			( void );
extern char*				zipfs_getcwd					( char* buffer, size_t length );
extern void					zipfs_init						( void );
extern int					zipfs_is_virtual_path			( const char* path );
extern int					zipfs_mkdir						( const char* path );
extern int					zipfs_mount_virtual				( const char* path, const char* archive );
extern char*				zipfs_normalize_path			( const char* path );
extern int					zipfs_rmdir						( const char* path );

#ifdef  __cplusplus
	}
#endif

#endif