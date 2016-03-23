// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <zl-util/headers.h>

static char smPathBuffer [ PATH_MAX ];

//----------------------------------------------------------------//
void _dumpAsCPPHeader ( cc8* name, void* data, u32 size, u32 columns ) {

	FILE* file;
	u32 count;
	u8 buffer;

	sprintf ( smPathBuffer, "%s.h", name );	
	file = fopen ( smPathBuffer, "w" );
	assert ( file );
	
	fprintf ( file, "#ifndef _%s_H\n", name );
	fprintf ( file, "#define _%s_H\n", name );
	
	fprintf ( file, "\n" );
	fprintf ( file, "#define %s_SIZE 0x%08X\n", name, ( uint )size );
	fprintf ( file, "\n" );

	fprintf ( file, "unsigned char %s [] = {\n\t", name );

	for ( count = 0; count < size; count ++ ) {
		if ( count ) {
			if ( !( count % 12 )) fprintf ( file, "\n\t" );
			else fprintf ( file, " " );
		}

		buffer = (( u8* )data )[count];
		fprintf ( file, "0x%02X,", buffer );	
	};

	if ((( count - 1 ) % 12 )) fprintf ( file, "\n" );
	fprintf ( file, "};\n" );

	fprintf ( file, "\n" );
	
	fprintf ( file, "#endif\n" );

	fclose ( file );
}

//----------------------------------------------------------------//
int main ( int argc, char** argv ) {

	if ( argc < 3 ) return 0;
	
	cc8* infile = argv [ 1 ];
	cc8* outfile = argv [ 2 ];
	
	ZLFileStream file;
	file.OpenRead ( infile );
	
	size_t size = file.GetLength ();
	void* data = malloc ( size );
	file.ReadBytes ( data, size );
	
	_dumpAsCPPHeader ( outfile, data, size, 16 );
	
	free ( data );
	return 0;
}
