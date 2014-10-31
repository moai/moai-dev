// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLDeflateWriter.h>
#include <zl-util/ZLZip.h>

//================================================================//
// ZLDeflateWriter
//================================================================//

//----------------------------------------------------------------//
u32 ZLDeflateWriter::GetCaps () {

	return this->mProxiedStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
size_t ZLDeflateWriter::Deflate ( const void* src, size_t size ) {
    
	char buffer [ ZL_DEFLATE_WRITER_CHUNK_SIZE ];
	size_t bufferSize = ZL_DEFLATE_WRITER_CHUNK_SIZE;
    
    z_stream* stream = &this->mZStream;
    stream->next_in = ( Bytef* )src;
	stream->avail_in = size;

	int flush = size ? Z_NO_FLUSH : Z_FINISH;

    do {
		
		stream->next_out = ( Bytef* )buffer;
		stream->avail_out = bufferSize;
		
		int result = deflate ( stream, flush );
		
		size_t have = ZL_DEFLATE_WRITER_CHUNK_SIZE - stream->avail_out;
		if ( have ) {
			size_t write = this->mProxiedStream->WriteBytes ( buffer, have );
			if ( write != have ) {
				// TODO: report error
				break;
			}
		}
		
		if ( result != Z_OK ) {
			if ( result != Z_STREAM_END ) {
				printf ( "%s\n", ZLZip::GetErrMsg ( result ));
			}
			break;
		}
	}
	while ( stream->avail_out == 0 );
	
    return size - stream->avail_in;
}

//----------------------------------------------------------------//
void ZLDeflateWriter::OnClose () {
	
	if ( this->mProxiedStream ) {
	
		this->Deflate ( 0, 0 );
	
		deflateEnd ( &this->mZStream );
		memset ( &this->mZStream, 0, sizeof ( z_stream ));
		
		this->mProxiedStream = 0;
	}
}

//----------------------------------------------------------------//
bool ZLDeflateWriter::OnOpen () {

	memset ( &this->mZStream, 0, sizeof ( z_stream ));
	int result = deflateInit2 ( &this->mZStream, this->mCompressionLevel, Z_DEFLATED, this->mWindowBits, 7, Z_DEFAULT_STRATEGY );

	if ( result != Z_OK ) return false;

	return true;
}

//----------------------------------------------------------------//
size_t ZLDeflateWriter::WriteBytes ( const void* buffer, size_t size ) {

	if ( size ) {
	
		size = this->Deflate ( buffer, size );
		
		this->mCursor += size;
		if ( this->mLength < this->mCursor ) {
			this->mLength = this->mCursor;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
ZLDeflateWriter::ZLDeflateWriter () :
	mCompressionLevel ( DEFAULT_LEVEL ),
	mWindowBits ( DEFAULT_WBITS ) {
	
	memset ( &this->mZStream, 0, sizeof ( z_stream ));
}

//----------------------------------------------------------------//
ZLDeflateWriter::~ZLDeflateWriter () {

	this->Close ();
}
