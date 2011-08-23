// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

//================================================================//
// MOAI IO
//================================================================//

//----------------------------------------------------------------//
int	MOAIfclose ( void* file ) {
	return fclose ( ( FILE* )file );
}

//----------------------------------------------------------------//
int	MOAIfeof ( void* file ) {
	return feof ( ( FILE* )file );
}

//----------------------------------------------------------------//
int	MOAIfflush ( void* file ) {
	return fflush ( ( FILE* )file );
}

//----------------------------------------------------------------//
int	MOAIfgetc ( void* file ) {
	return fgetc ( ( FILE* )file );
}

//----------------------------------------------------------------//
char* MOAIfgets	( char* string, int length, void* file ) {
	return fgets ( string, length, ( FILE* )file );
}

//----------------------------------------------------------------//
void* MOAIfopen ( const char* filename, const char* mode ) {
	return fopen ( filename, mode );
}

//----------------------------------------------------------------//
int MOAIfputc ( int c, void* file ) {
	return fputc ( c, ( FILE* )file );
}

//----------------------------------------------------------------//
int	MOAIfputs ( const char* string, void* file ) {
	return fputs ( string, ( FILE* )file );
}

//----------------------------------------------------------------//
size_t MOAIfread ( void* buffer, size_t size, size_t count, void* file ) {
	return fread ( buffer, size, count, ( FILE* )file );
}

//----------------------------------------------------------------//
int	MOAIfseek ( void* file, long offset, int origin ) {
	return fseek ( ( FILE* )file, offset, origin );
}

//----------------------------------------------------------------//
long MOAIftell ( void* file ) {
	return ftell ( ( FILE* )file );
}

//----------------------------------------------------------------//
size_t MOAIfwrite ( const void* data, size_t size, size_t count, void* file ) {	
	return fwrite ( data, size, count, ( FILE* )file );
}