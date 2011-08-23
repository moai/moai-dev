// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAI_IO_H
#define MOAI_IO_H

//================================================================//
// MOAI IO
//================================================================//

//----------------------------------------------------------------//
int					MOAIfclose			( void* file );

//----------------------------------------------------------------//
int					MOAIfeof			( void* file );

//----------------------------------------------------------------//
int					MOAIfflush			( void* file );

//----------------------------------------------------------------//
int					MOAIfgetc			( void* file );

//----------------------------------------------------------------//
char* 				MOAIfgets			( char* string, int length, void* file );

//----------------------------------------------------------------//
void* 				MOAIfopen 			( const char* filename, const char* mode );

//----------------------------------------------------------------//
int 				MOAIfputc			( int c, void* file );

//----------------------------------------------------------------//
int					MOAIfputs			( const char* string, void* file );

//----------------------------------------------------------------//
size_t				MOAIfread			( const void* buffer, size_t size, size_t count, void* file );

//----------------------------------------------------------------//
int					MOAIfreopen			( const char* filename, const char* mode, void* file );

//----------------------------------------------------------------//
int					MOAIfseek			( void* file, long offset, int origin );

//----------------------------------------------------------------//
long				MOAIftell			( void* file );

//----------------------------------------------------------------//
size_t				MOAIfwrite			( const void* data, size_t size, size_t count, void* file );


#endif