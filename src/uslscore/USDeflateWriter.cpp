// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USDeflateWriter.h>
#include <uslscore/USZip.h>

//================================================================//
// USDeflateWriter
//================================================================//

//----------------------------------------------------------------//
void USDeflateWriter::Close () {
	
	if ( this->mOutputStream ) {
	
		this->Deflate ( 0, 0 );
	
		deflateEnd ( &this->mZStream );
		memset ( &this->mZStream, 0, sizeof ( z_stream ));
		
		this->mUncompressedCursor = 0;
		this->mOutputStream = 0;
	}
}

//----------------------------------------------------------------//
u32 USDeflateWriter::GetCaps () {

	return this->mOutputStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
size_t USDeflateWriter::GetCursor () {

	return this->mUncompressedCursor;
}

//----------------------------------------------------------------//
size_t USDeflateWriter::GetLength () {

	return this->mUncompressedCursor;
}

//----------------------------------------------------------------//
size_t USDeflateWriter::Deflate ( const void* src, size_t size ) {
    
	char buffer [ US_DEFLATE_WRITER_CHUNK_SIZE ];
	size_t bufferSize = US_DEFLATE_WRITER_CHUNK_SIZE;
    
    z_stream* stream = &this->mZStream;
    stream->next_in = ( Bytef* )src;
	stream->avail_in = size;

	int flush = size ? Z_NO_FLUSH : Z_FINISH;

    do {
		
		stream->next_out = ( Bytef* )buffer;
		stream->avail_out = bufferSize;
		
		int result = deflate ( stream, flush );
		
		size_t have = US_DEFLATE_WRITER_CHUNK_SIZE - stream->avail_out;
		if ( have ) {
			size_t write = this->mOutputStream->WriteBytes ( buffer, have );
			if ( write != have ) {
				// TODO: report error
				break;
			}
		}
		
		if ( result != Z_OK ) {
			if ( result != Z_STREAM_END ) {
				printf ( "%s\n", USZip::GetErrMsg ( result ));
			}
			break;
		}
	}
	while ( stream->avail_out == 0 );
	
    return size - stream->avail_in;
}

//----------------------------------------------------------------//
bool USDeflateWriter::Open ( USStream* stream ) {

	this->Close ();

	if ( stream ) {
	
		memset ( &this->mZStream, 0, sizeof ( z_stream ));
		int result = deflateInit2 ( &this->mZStream, this->mCompressionLevel, Z_DEFLATED, this->mWindowBits, 7, Z_DEFAULT_STRATEGY );

		if ( result != Z_OK ) return false;

		this->mOutputStream = stream;
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
USDeflateWriter::USDeflateWriter () :
	mOutputStream ( 0 ),
	mUncompressedCursor ( 0 ),
	mCompressionLevel ( DEFAULT_LEVEL ),
	mWindowBits ( DEFAULT_WBITS ) {
	
	memset ( &this->mZStream, 0, sizeof ( z_stream ));
}

//----------------------------------------------------------------//
USDeflateWriter::~USDeflateWriter () {

	this->Close ();
}

//----------------------------------------------------------------//
size_t USDeflateWriter::WriteBytes ( const void* buffer, size_t size ) {

	if ( size ) {
		return this->Deflate ( buffer, size );
	}
	return 0;
}
