// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaiio\moaiio.h>

#ifdef MOAI_IO_STD

//================================================================//
// MOAI IO Standard C wrapper
//================================================================//

//----------------------------------------------------------------//
char* moai_basename ( const char* filename ) {

	char* token;
	char* lastToken;
	
	char* file = ( char* )filename;
	char delim = '/';
	
	for ( token = strtok ( file, &delim ); token; token = strtok ( NULL, &delim )) {
		lastToken = token;
	}

	return lastToken;
}

//----------------------------------------------------------------//
int	moai_fclose ( MOAIFILE* file ) {
	return fclose ( file );
}

//----------------------------------------------------------------//
int	moai_feof ( MOAIFILE* file ) {
	return feof ( file );
}

//----------------------------------------------------------------//
int	moai_fflush ( MOAIFILE* file ) {
	return fflush ( file );
}

//----------------------------------------------------------------//
int	moai_fgetc ( MOAIFILE* file ) {
	return fgetc ( file );
}

//----------------------------------------------------------------//
char* moai_fgets ( char* string, int length, MOAIFILE* file ) {
	return fgets ( string, length, file );
}

//----------------------------------------------------------------//
MOAIFILE* moai_fopen ( const char* filename, const char* mode ) {
	return fopen ( filename, mode );
}

//----------------------------------------------------------------//
int moai_fputc ( int c, MOAIFILE* file ) {
	return fputc ( c, file );
}

//----------------------------------------------------------------//
int	moai_fputs ( const char* string, MOAIFILE* file ) {
	return fputs ( string, file );
}

//----------------------------------------------------------------//
size_t moai_fread ( void* buffer, size_t size, size_t count, MOAIFILE* file ) {
	size_t num = fread ( buffer, size, count, file );
	return num;
}

//----------------------------------------------------------------//
int	moai_fseek ( MOAIFILE* file, long offset, int origin ) {
	return fseek ( file, offset, origin );
}

//----------------------------------------------------------------//
long moai_ftell ( MOAIFILE* file ) {
	return ftell ( file );
}

//----------------------------------------------------------------//
size_t moai_fwrite ( const void* data, size_t size, size_t count, MOAIFILE* file ) {	
	return fwrite ( data, size, count, file );
}

#endif