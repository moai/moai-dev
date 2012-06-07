// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFileSys.h>
#include <uslscore/USDirectoryItr.h>
#include <uslscore/USStream.h>
#include <stdio.h>

//----------------------------------------------------------------//
void USStream::Flush () {
}

//----------------------------------------------------------------//
size_t USStream::GetLength () {
	return UNKNOWN_SIZE;
}

//----------------------------------------------------------------//
bool USStream::IsAtEnd () {

	return ( this->GetCursor () == this->GetLength ());
}

//----------------------------------------------------------------//
size_t USStream::PeekBytes ( void* buffer, size_t size  ) {

	if ( !( this->GetCaps () & ( CAN_READ | CAN_SEEK ))) return 0;

	size_t cursor = this->GetCursor ();
	size = this->ReadBytes ( buffer, size );
	this->Seek ( cursor, SEEK_SET );
	return size;
}

//----------------------------------------------------------------//
size_t USStream::Print ( cc8* format, ... ) {

	va_list args;
	va_start ( args, format );
	
	size_t size = this->Print ( format, args );
	
	va_end ( args );
	
	return size;
}

//----------------------------------------------------------------//
size_t USStream::Print ( cc8* format, va_list args ) {

	if ( !( this->GetCaps () & CAN_WRITE )) return 0;

	static const size_t BUFFER_SIZE = 1024;
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
			buffSize = ( size_t )result;
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
	
	size_t size = 0;
	if ( result > 0 ) {
		size = this->WriteBytes ( buffer, ( size_t )result );
	}
	
	if ( buffer != stackBuffer ) {
		free ( buffer );
	}
	
	return size;
}

//----------------------------------------------------------------//
STLString USStream::ReadStr ( size_t size ) {

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
size_t USStream::ReadBytes ( void* buffer, size_t size ) {
	UNUSED ( buffer );
	UNUSED ( size );
	return 0;
}

//----------------------------------------------------------------//
int USStream::Seek ( long offset, int origin ) {

	if ( !( this->GetCaps () & CAN_SEEK )) return -1;

	size_t cursor = this->GetCursor ();
	size_t length = this->GetLength ();
	size_t absCursor = 0;

	switch ( origin ) {
		case SEEK_CUR: {
			absCursor = cursor + offset;
			break;
		}
		case SEEK_END: {
			if ( length == UNKNOWN_SIZE ) return -1;
			absCursor = length + offset;
			break;
		}
		case SEEK_SET: {
			absCursor = offset;
			break;
		}
	}
	
	if ( absCursor > cursor ) {
		if (( length == UNKNOWN_SIZE ) || ( absCursor > length )) return -1;
	}
	return this->SetCursor ( absCursor );
}

//----------------------------------------------------------------//
int USStream::SetCursor ( long offset ) {
	UNUSED ( offset );
	return -1;
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

//----------------------------------------------------------------//
size_t USStream::WriteBytes ( const void* buffer, size_t size ) {
	UNUSED ( buffer );
	UNUSED ( size );
	return 0;
}

//----------------------------------------------------------------//
size_t USStream::WriteStream ( USStream& source ) {

	if ( !( source.GetCaps () & CAN_READ )) return 0;
	if ( !( this->GetCaps () & CAN_WRITE )) return 0;

	u8 buffer [ LOCAL_BUFFER ];

	size_t readSize = 0;
	size_t writeSize = 0;
	size_t total = 0;
	
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
size_t USStream::WriteStream ( USStream& source, size_t size ) {

	if ( !( source.GetCaps () & CAN_READ )) return 0;
	if ( !( this->GetCaps () & CAN_WRITE )) return 0;

	u8 buffer [ LOCAL_BUFFER ];

	size_t readSize = 0;
	size_t total = 0;
	
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