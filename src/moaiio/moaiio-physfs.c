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

	if ( PHYSFS_isInit () == 0 )
		return 0;
	
	if ( file )
		return PHYSFS_close ( file );
	
#ifdef _DEBUG
	printf ( "%s\n", PHYSFS_getLastError ());
#endif

	return 0;
}

//----------------------------------------------------------------//
int	moai_fflush ( MOAIFILE* file ) {
	
	if ( PHYSFS_isInit () == 0 )
		return 0;

	if ( file )
		return PHYSFS_flush ( file );
	
#ifdef _DEBUG
	printf ( "%s\n", PHYSFS_getLastError ());
#endif

	return 0;
}

//----------------------------------------------------------------//
int moai_fgetc ( MOAIFILE* file ) {
	
	char c;

	if ( PHYSFS_isInit () == 0 )
		return 0;

	if ( PHYSFS_read ( file, &c, 1, 1 ) != 1 )
		return EOF;
	
#ifdef _DEBUG
	printf ( "%s\n", PHYSFS_getLastError ());
#endif

	return c;
}

//----------------------------------------------------------------//
char* moai_fgets ( char* string, int length, MOAIFILE* file ) {
	
	int i = 0;
	int c = 0;

	if ( PHYSFS_isInit () == 0 )
		return 0;

	if ( length <= 1 )
		return 0;

	do {
		c = moai_fgetc ( file );
		if ( c == EOF ) break;
		string [ i++ ] = c;
	} while ( c != '\0' && c != -1 && c != '\n' );

	string [ i ] = '\0';
	return string;
}

//----------------------------------------------------------------//
MOAIFILE* moai_fopen ( const char* filename, const char* mode ) {
	
	MOAIFILE* outFile;
	char* base;

	if ( PHYSFS_isInit () == 0 )
		return NULL;
	
	// PhysFS uses relative names - the whole working directory is on the search path, get the filename
	base = moai_basename ( filename );

	switch ( mode[0] ) {

	case 'r':
		outFile = PHYSFS_openRead ( base );
	case 'w':
		outFile = PHYSFS_openWrite ( base );
		break;
	case 'a':
		outFile = PHYSFS_openAppend ( base );
		break;
	default:
		outFile = NULL;
		break;
	}

#ifdef _DEBUG
	printf ( "%s\n", PHYSFS_getLastError ());
#endif

	return outFile;
}

//----------------------------------------------------------------//
size_t moai_fread ( void* buffer, size_t size, size_t count, MOAIFILE* file ) {
	
	if ( PHYSFS_isInit () == 0 )
		return 0;

	if ( file )
		return ( size_t )PHYSFS_read ( file, buffer, size, count );
	
#ifdef _DEBUG
	printf ( "%s\n", PHYSFS_getLastError ());
#endif

	return 0;
}

//----------------------------------------------------------------//
int	moai_fseek ( MOAIFILE* file, long offset, int origin ) {
	
	if ( PHYSFS_isInit () == 0 )
		return 0;

	if ( origin == SEEK_END )
		return PHYSFS_seek ( file, PHYSFS_fileLength ( file ));
	else if ( origin == SEEK_SET )
		return PHYSFS_seek ( file, 0 );
	else
		return PHYSFS_seek ( file, origin + offset );
	
#ifdef _DEBUG
	printf ( "%s\n", PHYSFS_getLastError ());
#endif

	return 0;
}

//----------------------------------------------------------------//
long moai_ftell ( MOAIFILE* file ) {
	
	if ( PHYSFS_isInit () == 0 )
		return 0;

	if ( file )
		return ( long )PHYSFS_tell ( file );
	
#ifdef _DEBUG
	printf ( "%s\n", PHYSFS_getLastError ());
#endif

	return 0;
}

//----------------------------------------------------------------//
size_t moai_fwrite ( const void* data, size_t size, size_t count, MOAIFILE* file ) {
	
	if ( PHYSFS_isInit () == 0 )
		return 0;
	
	if ( file )
		return ( size_t )PHYSFS_write ( file, data, size, count );
	
#ifdef _DEBUG
	printf ( "%s\n", PHYSFS_getLastError ());
#endif

	return 0;
}

#endif