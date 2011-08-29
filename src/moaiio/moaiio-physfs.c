// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaiio\moaiio.h>

#ifdef MOAI_IO_PHYSFS

//================================================================//
// MOAI IO PhysFS wrapper
//================================================================//


//----------------------------------------------------------------//
char* moai_basename ( const char* filename ) {

	char* token;
	char* lastToken = 0;
	
	char* file = ( char* )filename;
	char delim = '/';
	
	for ( token = strtok ( file, &delim ); token; token = strtok ( NULL, &delim )) {
		lastToken = token;
	}

	return lastToken;
}

//----------------------------------------------------------------//
int	moai_fclose ( MOAIFILE* file ) {

	if ( PHYSFS_isInit () == 0 )
		return fclose ( ( FILE* )file );
	
	if ( file ) {

		if ( PHYSFS_close ( ( PHYSFS_File* )file ) == 0 ) {
			return fclose ( ( FILE* )file );
		}
		return 0;
	}	
	
	return 0;
}

//----------------------------------------------------------------//
int	moai_fflush ( MOAIFILE* file ) {
	
	if ( PHYSFS_isInit () == 0 )
		return fflush ( ( FILE* )file );

	if ( file ) {

		if ( PHYSFS_flush ( ( PHYSFS_File* )file ) == 0 ) {
			return fflush ( ( FILE* )file );
		}
		return 0;
	}	
	
	return 0;
}

//----------------------------------------------------------------//
int moai_fgetc ( MOAIFILE* file ) {
	
	char c;
	int result;

	if ( PHYSFS_isInit () == 0 )
		return fgetc ( ( FILE* )file );

	result =  ( int )PHYSFS_read ( ( PHYSFS_File* )file, &c, 1, 1 );
	if ( result == -1 ) {
		return fgetc ( ( FILE* )file );
	}
	else if ( result != 1 ) {
		return EOF;
	}
	
	return c;
}

//----------------------------------------------------------------//
char* moai_fgets ( char* string, int length, MOAIFILE* file ) {
	
	int i = 0;
	int c = 0;

	if ( PHYSFS_isInit () == 0 )
		return fgets ( string, length, ( FILE* )file );

	if ( length <= 1 )
		return 0;

	do {
		c = moai_fgetc ( file );
		if ( c == EOF ) break;
		string [ i++ ] = ( char )c;
	} while ( c != '\0' && c != -1 && c != '\n' );

	string [ i ] = '\0';
	return string;
}

//----------------------------------------------------------------//
MOAIFILE* moai_fopen ( const char* filename, const char* mode ) {
	
	MOAIFILE* outFile;

	if ( PHYSFS_isInit () == 0 || PHYSFS_exists ( filename ) == 0 )
		return fopen ( filename, mode );

	switch ( mode[0] ) {

	case 'r':
		outFile = PHYSFS_openRead ( filename );
		break;
	case 'w':
		outFile = PHYSFS_openWrite ( filename );
		break;
	case 'a':
		outFile = PHYSFS_openAppend ( filename );
		break;
	default:
		outFile = NULL;
		break;
	}

	return outFile;
}

//----------------------------------------------------------------//
size_t moai_fread ( void* buffer, size_t size, size_t count, MOAIFILE* file ) {
	
	size_t result;

	if ( PHYSFS_isInit () == 0 )
		return fread ( buffer, size, count, ( FILE* )file );

	result = ( size_t )PHYSFS_read ( ( PHYSFS_File* )file, buffer, size, count );
	if ( result == -1 ) {
		return fread ( buffer, size, count, ( FILE* )file );
	}

	return result;
}

//----------------------------------------------------------------//
int	moai_fseek ( MOAIFILE* file, long offset, int origin ) {
	
	int length;
	int result;

	if ( PHYSFS_isInit () == 0 )
		return fseek ( ( FILE* )file, offset, origin );

	length = ( int )PHYSFS_fileLength ( ( PHYSFS_File* )file );
	if ( origin == SEEK_CUR )
		result = PHYSFS_seek ( ( PHYSFS_File* )file, PHYSFS_tell ( ( PHYSFS_File* )file ) + offset );
	else if ( origin == SEEK_END )
		result = PHYSFS_seek ( ( PHYSFS_File* )file, length);
	else if ( origin == SEEK_SET )
		result = PHYSFS_seek ( ( PHYSFS_File* )file, 0 );
	else
		result = PHYSFS_seek ( ( PHYSFS_File* )file, origin + offset );
	
	if ( result == 0 )
		return fseek ( ( FILE* )file, offset, origin );
	
	return result;
}

//----------------------------------------------------------------//
long moai_ftell ( MOAIFILE* file ) {
	
	long result;

	if ( PHYSFS_isInit () == 0 )
		return ftell ( ( FILE* )file );

	result = ( long )PHYSFS_tell ( ( PHYSFS_File* )file );
	if ( result == -1 )
		return ftell ( ( FILE* )file );
	
	return result;
}

//----------------------------------------------------------------//
size_t moai_fwrite ( const void* data, size_t size, size_t count, MOAIFILE* file ) {
	
	size_t result;

	if ( PHYSFS_isInit () == 0 )
		return fwrite ( data, size, count, ( FILE* )file );
	
	result = ( size_t )PHYSFS_write ( ( PHYSFS_File* )file, data, size, count );
	if ( result == -1 )
		return fwrite ( data, size, count, ( FILE* )file );

	return result;
}

#endif