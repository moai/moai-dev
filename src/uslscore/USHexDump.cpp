// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USHexDump.h>
#include <uslscore/USStream.h>

//================================================================//
// USHexDump
//================================================================//

//----------------------------------------------------------------//
void USHexDump::DumpAsCPPHeader ( USStream& stream, cc8* name, const void* data, size_t size, u32 columns ) {

	size_t count;
	u8 buffer;
	
	stream.Print ( "#ifndef _%s_H\n", name );
	stream.Print ( "#define _%s_H\n", name );
	
	stream.Print ( "\n" );
	stream.Print ( "#define %s_SIZE 0x%08X\n", name, ( uint )size );
	stream.Print ( "\n" );

	stream.Print ( "unsigned char %s [] = {\n\t", name );

	for ( count = 0; count < size; count ++ ) {
		if ( count ) {
			if ( !( count % columns )) {
				stream.Print ( "\n\t" );
			}
			else {
				stream.Print ( " " );
			}
		}
		buffer = (( u8* )data )[ count ];
		stream.Print ( "0x%02X,", buffer );	
	};

	if ((( count - 1 ) % columns )) {
		stream.Print ( "\n" );
	}
	
	stream.Print ( "};\n" );
	stream.Print ( "\n" );
	stream.Print ( "#endif\n" );
}