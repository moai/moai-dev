// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAI_IO_H
#define MOAI_IO_H

//================================================================//
// MOAI IO
//================================================================//

typedef		void		MOAIFILE;

//----------------------------------------------------------------//
char*				moai_basename		( const char* filename );

//----------------------------------------------------------------//
int					moai_fclose			( MOAIFILE* file );

//----------------------------------------------------------------//
int					moai_feof			( MOAIFILE* file );

//----------------------------------------------------------------//
int					moai_fflush			( MOAIFILE* file );

//----------------------------------------------------------------//
int					moai_fgetc			( MOAIFILE* file );

//----------------------------------------------------------------//
char* 				moai_fgets			( char* string, int length, MOAIFILE* file );

//----------------------------------------------------------------//
MOAIFILE* 			moai_fopen 			( const char* filename, const char* mode );

//----------------------------------------------------------------//
int 				moai_fputc			( int c, MOAIFILE* file );

//----------------------------------------------------------------//
int					moai_fputs			( const char* string, MOAIFILE* file );

//----------------------------------------------------------------//
size_t				moai_fread			( const void* buffer, size_t size, size_t count, MOAIFILE* file );

//----------------------------------------------------------------//
int					moai_freopen		( const char* filename, const char* mode, MOAIFILE* file );

//----------------------------------------------------------------//
int					moai_fseek			( MOAIFILE* file, long offset, int origin );

//----------------------------------------------------------------//
long				moai_ftell			( MOAIFILE* file );

//----------------------------------------------------------------//
size_t				moai_fwrite			( const void* data, size_t size, size_t count, MOAIFILE* file );


#endif