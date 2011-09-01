// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIO_H
#define MOAIO_H

//================================================================//
// MOAI IO
//================================================================//

typedef void* MOAIFILE;

//----------------------------------------------------------------//
int				moaio_affirm_path			( const char* path );
char*			moaio_basename				( const char* filename );
char*			moaio_bless_path			( const char* path );
int				moaio_chdir					( char* path );
void			moaio_cleanup				();
int				moaio_fclose				( MOAIFILE fp );
int				moaio_feof					( MOAIFILE fp );
int				moaio_fflush				( MOAIFILE fp );
int				moaio_fgetc					( MOAIFILE fp );
char* 			moaio_fgets					( char* string, int length, MOAIFILE fp );
MOAIFILE 		moaio_fopen 				( const char* filename, const char* mode );
int 			moaio_fputc					( int c, MOAIFILE fp );
int				moaio_fputs					( const char* string, MOAIFILE fp );
size_t			moaio_fread					( const void* buffer, size_t size, size_t count, MOAIFILE fp );
int				moaio_freopen				( const char* filename, const char* mode, MOAIFILE fp );
int				moaio_fseek					( MOAIFILE fp, long offset, int origin );
long			moaio_ftell					( MOAIFILE fp );
size_t			moaio_fwrite				( const void* data, size_t size, size_t count, MOAIFILE fp );
char*			moaio_get_abs_dirpath		( const char* path );
char*			moaio_get_abs_filepath		( const char* path );
char*			moaio_get_rel_path			( const char* path );
char*			moaio_get_working_path		();
char*			moaio_getcwd				( char* buffer, size_t length );
void			moaio_init					();
int				moaio_mkdir					( const char* path );
char*			moaio_normalize_path		( const char* path );
int				moaio_rmdir					( const char* path );
void			moaio_set_virtual_path		( const char* path, const char* archive );

#endif