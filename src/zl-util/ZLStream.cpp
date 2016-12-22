// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLFileSys.h>
#include <zl-util/ZLDirectoryItr.h>
#include <zl-util/ZLMemStream.h>
#include <zl-util/ZLSample.h>
#include <zl-util/ZLStream.h>
#include <stdio.h>

//----------------------------------------------------------------//
bool ZLStream::CheckCaps ( u32 flags ) {

	return ( this->GetCaps () & flags ) != 0;
}

//----------------------------------------------------------------//
ZLSizeResult ZLStream::Collapse ( size_t clipBase, size_t clipSize, size_t chunkSize, size_t size, bool invert ) {

	return this->Collapse ( *this, clipBase, clipSize, chunkSize, size, invert );
}

//----------------------------------------------------------------//
ZLSizeResult ZLStream::Collapse ( ZLStream& source, size_t clipBase, size_t clipSize, size_t chunkSize, size_t size, bool invert ) {

	if (( clipBase + clipSize ) > chunkSize ) {
		clipSize = ( clipBase < chunkSize ) ? ( clipBase + clipSize ) - chunkSize : 0;
	}

	if ( clipSize == 0 ) {
		return this->WriteStream ( source, size );
	}
	
	static const size_t BUFFER_SIZE = 1024;
	char stackBuffer [ BUFFER_SIZE ];
	char* buffer = stackBuffer;
	
	char* memBuffer = 0;
	if ( BUFFER_SIZE < chunkSize ) {
		memBuffer = ( char* )malloc ( chunkSize );
		buffer = memBuffer;
	}
	
	size_t dstStart = this->GetCursor ();
	
	// if the streams are the same we need to hop back and forth between two cursors
	size_t srcCursor = source.GetCursor ();
	size_t dstCursor = this->GetCursor ();
	
	// read one chunk at a time, until no more chunks
	size_t readSize = 0;
	size_t writeSize = 0;
	size_t totalRead = 0;
	
	do {
	
		source.Seek (( long )srcCursor, SEEK_SET );
		readSize = source.ReadBytes ( buffer, (( totalRead + chunkSize ) > size ) ? size - totalRead : chunkSize ).mValue;
	
		srcCursor += readSize;
		totalRead += readSize;
		
		if ( invert ) {
		
			if ( clipBase < readSize ) {
			
				size_t segSize = clipBase + clipSize;
				segSize = segSize > readSize ? ( readSize - clipBase ) : segSize;
				
				memmove ( buffer, ( char* )(( size_t )buffer + clipBase ), segSize );
				
				writeSize = segSize;
			}
		}
		else {
		
			writeSize = clipBase;
		
			size_t segBase = clipBase + clipSize;
			if ( segBase < readSize ) {
			
				size_t segSize = readSize - segBase;
				memmove (( char* )(( size_t )buffer + clipBase ), ( char* )(( size_t )buffer + segBase ), segSize );
				
				writeSize += segSize;
			}
		}
		
		if ( writeSize == 0 ) break;
		
		this->Seek (( long )dstCursor, SEEK_SET );
		if ( this->WriteBytes ( buffer, writeSize ).mValue < writeSize ) break;
		dstCursor += writeSize;
	}
	while ( readSize == chunkSize ); // if we've read less than the buffer size (for any reason), then we're done
	
	if ( memBuffer ) {
		free ( memBuffer );
	}
	
	ZL_RETURN_SIZE_RESULT ( this->GetCursor () - dstStart, ZL_OK );
}

//----------------------------------------------------------------//
void ZLStream::Compact () {
}

//----------------------------------------------------------------//
void ZLStream::Flush () {
}

//----------------------------------------------------------------//
size_t ZLStream::GetLength () {
	return UNKNOWN_SIZE;
}

//----------------------------------------------------------------//
bool ZLStream::IsAtEnd () {

	return ( this->GetCursor () == this->GetLength ());
}

//----------------------------------------------------------------//
ZLSizeResult ZLStream::PeekBytes ( void* buffer, size_t size  ) {

	if ( !( this->GetCaps () & ( CAN_READ | CAN_SEEK ))) ZL_RETURN_SIZE_RESULT ( 0, ZL_ERROR );

	ZLSizeResult result;

	size_t cursor = this->GetCursor ();
	result = this->ReadBytes ( buffer, size );
	this->Seek (( long )cursor, SEEK_SET );
	return result;
}

//----------------------------------------------------------------//
ZLSizeResult ZLStream::Print ( cc8* format, ... ) {

	ZLSizeResult result;

	va_list args;
	va_start ( args, format );
	
	result = this->Print ( format, args );
	
	va_end ( args );
	
	return result;
}

//----------------------------------------------------------------//
ZLSizeResult ZLStream::Print ( cc8* format, va_list args ) {

	if ( !( this->GetCaps () & CAN_WRITE )) ZL_RETURN_SIZE_RESULT ( 0, ZL_ERROR );

	static const size_t BUFFER_SIZE = 1024;
	char stackBuffer [ BUFFER_SIZE ];
	char* buffer = stackBuffer;
	int buffSize = BUFFER_SIZE;
	
	int result;
	
	for ( ;; ) {
	
		va_list copy;
		va_copy ( copy, args );
		result = vsnprintf ( buffer, buffSize, format, copy );
		va_end ( copy );
		
		// thanks to http://perfec.to/vsnprintf/ for a discussion of vsnprintf portability issues
		if (( result == buffSize ) || ( result == -1 ) || ( result == buffSize - 1 ))  {
			buffSize = buffSize << 1;
		}
		else if ( result > buffSize ) {
			buffSize = ( size_t )result + 2;
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
		size = this->WriteBytes ( buffer, ( size_t )result ).mValue;
	}
	
	if ( buffer != stackBuffer ) {
		free ( buffer );
	}
	
	ZL_RETURN_SIZE_RESULT ( size, ZL_OK );
}

//----------------------------------------------------------------//
template <> ZLResult < bool > ZLStream::Read < bool >() {
	ZLResult < u8 > result = this->Read < u8 >( 0 );
	ZL_RETURN_BOOL_RESULT ( result.mValue != 0, result.mCode );
}

//----------------------------------------------------------------//
template <> ZLResult < bool > ZLStream::Read < bool >( bool value ) {
	ZLResult < u8 > result = this->Read < u8 >( value ? 1 : 0 );
	ZL_RETURN_BOOL_RESULT ( result.mValue != 0, result.mCode );
}

//----------------------------------------------------------------//
ZLSizeResult ZLStream::ReadBytes ( void* buffer, size_t size ) {
	UNUSED ( buffer );
	UNUSED ( size );
	
	ZL_RETURN_SIZE_RESULT ( 0, ZL_UNSUPPORTED );
}

//----------------------------------------------------------------//
ZLStringResult ZLStream::ReadString ( size_t size ) {

	STLString str;

	const unsigned int MAX_HEAP_ALLOC = 1024;
	
	if ( size ) {

		char* buffer;
		if ( size > MAX_HEAP_ALLOC ) {
			buffer = ( char* )malloc ( size + 1 );
		}
		else {
			buffer = ( char* )alloca ( size + 1 );
		}
		this->ReadBytes ( buffer, size );
		buffer [ size ] = 0;
		str = buffer;

		if ( size > MAX_HEAP_ALLOC ) {
			free ( buffer );
		}
	}
	ZL_RETURN_STRING_RESULT ( str, ZL_OK );
}

//----------------------------------------------------------------//
ZLStringResult ZLStream::ReadToken ( cc8* delimiters ) {

	STLString str;
	if ( this->IsAtEnd ()) ZL_RETURN_STRING_RESULT ( str, ZL_OK );

	char stackBuffer [ LOCAL_BUFFER ];
	
	ZLMemStream memStream;
	memStream.SetChunkSize ( LOCAL_BUFFER );
	memStream.SetGuestBuffer ( stackBuffer, LOCAL_BUFFER );
	
	char c = 0;
	size_t size = 0;
	
	do {
		
		c = this->Read < char >( 0 ).mValue;
		
		if ( delimiters && c ) {
		
			bool isDelimiter = false;
		
			for ( size_t i = 0; delimiters [ i ]; ++i ) {
				if ( delimiters [ i ] == c ) {
					isDelimiter = true;
					break;
				}
			}
			
			if ( isDelimiter ) {
				if ( size ) {
					c = 0;
				}
				else {
					continue;
				}
			}
		}
		
		memStream.Write < char >( c );
		size++;
	}
	while ( c );
	
	if ( size ) {
		
		str.reserve ( size + 1 );
		memStream.Seek ( 0, SEEK_SET );
		
		while ( size > 0 ) {
			
			char buffer [ LOCAL_BUFFER ];
			
			size_t readSize = size;
			if ( LOCAL_BUFFER < readSize ) {
				readSize = LOCAL_BUFFER;
			}
			
			memStream.ReadBytes ( buffer, readSize );
			str.append ( buffer, readSize );
			size -= readSize;
		}
	}
	ZL_RETURN_STRING_RESULT ( str, ZL_OK );
}

//----------------------------------------------------------------//
float ZLStream::Sample ( u32 streamType, size_t sampleSize ) {

	float accum = 0.0f;
	
	for ( size_t i = 0; i < sampleSize; ++i ) {
	
		float sample;
	
		size_t result = ZLSample::ReadSample ( *this, streamType, &sample, ZLSample::SAMPLE_FLOAT );
		UNUSED ( result ); // TODO: why isn't assert undef taking care of this?
		assert ( result );
		
		accum += sample / ( float )sampleSize;
	}
	return accum;
}

//----------------------------------------------------------------//
int ZLStream::Seek ( long offset, int origin ) {

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
	
//	if ( absCursor > cursor ) {
//		if (( length == UNKNOWN_SIZE ) || ( absCursor > length )) return -1;
//	}
	return ( absCursor != cursor ) ? this->SetCursor ( absCursor ) : 0;
}

//----------------------------------------------------------------//
ZLResultCode ZLStream::SetCursor ( size_t offset ) {
	UNUSED ( offset );
	return ZL_UNSUPPORTED;
}

//----------------------------------------------------------------//
ZLSizeResult ZLStream::SetLength ( size_t length ) {
	UNUSED ( length );
	ZL_RETURN_SIZE_RESULT ( 0, ZL_UNSUPPORTED );
}

//----------------------------------------------------------------//
template <> ZLSizeResult ZLStream::Write < bool >( bool value ) {

    u8 boolByte = ( value ) ? 1 : 0;
    return this->Write < u8 >( boolByte );
}

//----------------------------------------------------------------//
ZLSizeResult ZLStream::WriteBytes ( const void* buffer, size_t size ) {
	UNUSED ( buffer );
	UNUSED ( size );
	ZL_RETURN_SIZE_RESULT ( 0, ZL_UNSUPPORTED );
}

//----------------------------------------------------------------//
ZLSizeResult ZLStream::WriteStream ( ZLStream& source ) {

	if ( !(( source.GetCaps () & CAN_READ ) && ( this->GetCaps () & CAN_WRITE ))) ZL_RETURN_SIZE_RESULT ( 0, ZL_ERROR );

	u8 buffer [ LOCAL_BUFFER ];

	size_t readSize = 0;
	size_t writeSize = 0;
	size_t total = 0;
	
	do {
	
		readSize = source.ReadBytes ( buffer, LOCAL_BUFFER ).mValue;
		if ( readSize ) {
			writeSize = this->WriteBytes ( buffer, readSize ).mValue;
			total += writeSize;
			if ( writeSize != readSize ) break;
		}
		
	} while ( readSize == LOCAL_BUFFER );

	ZL_RETURN_SIZE_RESULT ( total, ZL_OK );
}

//----------------------------------------------------------------//
ZLSizeResult ZLStream::WriteStream ( ZLStream& source, size_t size ) {

	if ( !(( source.GetCaps () & CAN_READ ) && ( this->GetCaps () & CAN_WRITE ))) ZL_RETURN_SIZE_RESULT ( 0, ZL_ERROR );

	if ( this == &source ) {
		size_t cursor = this->GetCursor ();
		this->Seek (( long )( cursor + size ), SEEK_SET );
		ZL_RETURN_SIZE_RESULT ( this->GetCursor () - cursor, ZL_OK );
	}

	u8 buffer [ LOCAL_BUFFER ];

	size_t readSize = 0;
	size_t total = 0;
	
	do {
		
		readSize = source.ReadBytes ( buffer, (( total + LOCAL_BUFFER ) > size ) ? size - total : LOCAL_BUFFER ).mValue;
		
		if ( readSize ) {
			total += this->WriteBytes ( buffer, readSize ).mValue;
		}
	
	} while ( readSize == LOCAL_BUFFER );
	
	ZL_RETURN_SIZE_RESULT ( total, ZL_OK )
}

//----------------------------------------------------------------//
ZLStream::ZLStream () {
}

//----------------------------------------------------------------//
ZLStream::~ZLStream () {
}
