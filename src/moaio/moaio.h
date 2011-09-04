// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIO_H
#define MOAIO_H

//================================================================//
// moaio_stat
//================================================================//
typedef struct moaio_stat {

	int						mExists;
	int						mIsDir;
	
	unsigned long long		mTimeCreated;	// Time created (in seconds from cardinal date)
	unsigned long long		mTimeModified;	// Time last modified (in seconds from cardinal date)
	unsigned long long		mTimeViewed;	// Time last viewed (in seconds from cardinal date)
	size_t					mSize;			// Yeah, that

} moaio_stat;

//================================================================//
// MOAI IO
//================================================================//

typedef void* MOAIODIR;
typedef void* MOAIOFILE;

//----------------------------------------------------------------//
extern int					moaio_affirm_path				( const char* path );
extern char*				moaio_basename					( const char* filename );
extern char*				moaio_bless_path				( const char* path );
extern int					moaio_chdir						( const char* path );
extern void					moaio_cleanup					();
extern void					moaio_dir_close					( MOAIODIR dir );
extern char const*			moaio_dir_entry_name			( MOAIODIR dir );
extern int					moaio_dir_entry_is_subdir		( MOAIODIR dir );
extern MOAIODIR				moaio_dir_open					();
extern int					moaio_dir_read_entry			( MOAIODIR dir );
extern int					moaio_fclose					( MOAIOFILE fp );
extern int					moaio_feof						( MOAIOFILE fp );
extern int					moaio_fflush					( MOAIOFILE fp );
extern int					moaio_fgetc						( MOAIOFILE fp );
extern char* 				moaio_fgets						( char* string, int length, MOAIOFILE fp );
extern MOAIOFILE 			moaio_fopen 					( const char* filename, const char* mode );
extern int 					moaio_fputc						( int c, MOAIOFILE fp );
extern int					moaio_fputs						( const char* string, MOAIOFILE fp );
extern size_t				moaio_fread						( const void* buffer, size_t size, size_t count, MOAIOFILE fp );
extern int					moaio_freopen					( const char* filename, const char* mode, MOAIOFILE fp );
extern int					moaio_fseek						( MOAIOFILE fp, long offset, int origin );
extern long					moaio_ftell						( MOAIOFILE fp );
extern size_t				moaio_fwrite					( const void* data, size_t size, size_t count, MOAIOFILE fp );
extern char*				moaio_get_abs_dirpath			( const char* path );
extern char*				moaio_get_abs_filepath			( const char* path );
extern char*				moaio_get_rel_path				( const char* path );
extern int					moaio_get_stat					( char const* path, moaio_stat* filestat );
extern char*				moaio_get_working_path			();
extern char*				moaio_getcwd					( char* buffer, size_t length );
extern void					moaio_init						();
extern int					moaio_is_virtual_path			( const char* path );
extern int					moaio_mkdir						( const char* path );
extern char*				moaio_normalize_path			( const char* path );
extern int					moaio_remove					( const char* path );
extern int					moaio_rename					( const char* oldname, const char* newname );
extern int					moaio_rmdir						( const char* path );
extern int					moaio_set_virtual_path			( const char* path, const char* archive );

#endif