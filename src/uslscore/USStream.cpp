// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFileSys.h>
#include <uslscore/USDirectoryItr.h>
#include <uslscore/USStream.h>
#include <stdio.h>

//----------------------------------------------------------------//
bool USStream::Done () {

	return ( this->GetCursor () == this->GetLength ());
}

//----------------------------------------------------------------//
void USStream::Flush () {
}

//----------------------------------------------------------------//
u32 USStream::Pipe ( USStream& source ) {

	u8 buffer [ LOCAL_BUFFER ];

	u32 readSize = 0;
	u32 writeSize = 0;
	u32 total = 0;
	
	do {
	
		readSize = source.ReadBytes ( buffer, LOCAL_BUFFER );
		if ( readSize ) {
			writeSize = this->WriteBytes ( buffer, readSize );
			total += writeSize;
			if ( writeSize != readSize ) break;
		}
		
	} while ( readSize == LOCAL_BUFFER );

	return total;
}

//----------------------------------------------------------------//
u32 USStream::Pipe ( USStream& source, u32 size ) {

	u8 buffer [ LOCAL_BUFFER ];

	u32 readSize = 0;
	u32 total = 0;
	
	do {
	
		if (( total + LOCAL_BUFFER ) > size ) {
			readSize = source.ReadBytes ( buffer, size - total );
		}
		else {
			readSize = source.ReadBytes ( buffer, LOCAL_BUFFER );
		}
		
		if ( readSize ) {
			total += this->WriteBytes ( buffer, readSize );
		}
	
	} while ( readSize == LOCAL_BUFFER );
	
	return total;
}

//----------------------------------------------------------------//
u32 USStream::Print ( cc8* format, ... ) {

	va_list args;
	va_start ( args, format );
	
	u32 size = this->Print ( format, args );
	
	va_end ( args );
	
	return size;
}

//----------------------------------------------------------------//
u32 USStream::Print ( cc8* format, va_list args ) {

	static const u32 BUFFER_SIZE = 1024;
	char stackBuffer [ BUFFER_SIZE ];
	char* buffer = stackBuffer;
	int buffSize = BUFFER_SIZE;
	
	int result;
	
	for ( ;; ) {
	
		result = vsnprintf ( buffer, buffSize, format, args );

		// thanks to http://perfec.to/vsnprintf/ for a discussion of vsnprintf portability issues
		if (( result == buffSize ) || ( result == -1 ) || ( result == buffSize - 1 ))  {
			buffSize = buffSize << 1;
		}
		else if ( result > buffSize ) {
			buffSize = result;
		}
		else {
			break;
		}
		
		if ( buffer == stackBuffer ) {
			buffer = 0;
		}
		
		if ( buffer ) {
			buffer = ( char* )realloc ( buffer, buffSize );
		}
		else {
			buffer = ( char* )malloc ( buffSize );
		}
	}
	
	if ( result > 0 ) {
		this->WriteBytes ( buffer, ( u32 )result );
	}
	
	if ( buffer != stackBuffer ) {
		free ( buffer );
	}
	return ( u32 )result;
}

//----------------------------------------------------------------//
STLString USStream::ReadStr ( u32 size ) {

	STLString str;

	if ( size ) {
		char* buffer = ( char* )malloc ( size + 1 );
		this->ReadBytes ( buffer, size );
		buffer [ size ] = 0;
		str = buffer;
		free ( buffer );
	}
	return str;
}

//----------------------------------------------------------------//
template <> bool USStream::Read < bool >() {
    u8 value = this->Read < u8 >();
    return ( value == 0xff );
}

//----------------------------------------------------------------//
template <> string USStream::Read < string >() {

    string str;

    u32 size = this->Read < u32 >();

    if ( size > 0 ) {

        str.reserve ( size );

        if ( size < LOCAL_BUFFER ) {
            char buffer [ LOCAL_BUFFER ];
            this->ReadBytes ( buffer, size );
            str.insert ( 0, buffer );
        }
        else {
            char* buffer = ( char* )malloc ( size );
            this->ReadBytes ( buffer, size );
            str.insert ( 0, buffer );
            free ( buffer );
        }
    }
    return str;
}

//----------------------------------------------------------------//
template <> STLString USStream::Read < STLString >() {
    return this->Read < string >();
}

//----------------------------------------------------------------//
USStream::USStream () {
}

//----------------------------------------------------------------//
USStream::~USStream () {
}

//----------------------------------------------------------------//
template <> void USStream::Write < bool >( bool value ) {

    u8 boolByte = ( value ) ? 0xff : 0x00;
    this->Write < u8 >( boolByte );
}

//----------------------------------------------------------------//
template <> void USStream::Write < string >( string value ) {

    u32 size = ( u32 )value.length () + 1;
    this->Write < u32 >( size );

    if ( size ) {
        this->WriteBytes ( value.c_str (), size );
    }
}

//----------------------------------------------------------------//
template <> void USStream::Write < STLString >( STLString value ) {
    this->Write < string >( value );
}
