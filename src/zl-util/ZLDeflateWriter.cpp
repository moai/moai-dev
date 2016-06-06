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
ZLSizeResult ZLDeflateWriter::Deflate ( const void* src, size_t size ) {
    
	char buffer [ ZL_DEFLATE_WRITER_CHUNK_SIZE ];
	size_t bufferSize = ZL_DEFLATE_WRITER_CHUNK_SIZE;
    
    z_stream* stream = &this->mZStream;
    stream->next_in = ( Bytef* )src;
	stream->avail_in = ( uInt )size;

	int flush = size ? Z_NO_FLUSH : Z_FINISH;

    do {
		
		stream->next_out = ( Bytef* )buffer;
		stream->avail_out = ( uInt )bufferSize;
		
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
				ZL_RETURN_SIZE_RESULT ( 0, ZL_ERROR );
			}
			break;
		}
	}
	while ( stream->avail_out == 0 );
	
    ZL_RETURN_SIZE_RESULT ( size - stream->avail_in, stream->avail_in ? ZL_OK : ZL_ERROR );
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
ZLResultCode ZLDeflateWriter::OnOpen () {

	memset ( &this->mZStream, 0, sizeof ( z_stream ));
	int result = deflateInit2 ( &this->mZStream, this->mCompressionLevel, Z_DEFLATED, this->mWindowBits, 7, Z_DEFAULT_STRATEGY );

	if ( result != Z_OK ) return ZL_ERROR;

	return ZL_OK;
}

//----------------------------------------------------------------//
ZLSizeResult ZLDeflateWriter::WriteBytes ( const void* buffer, size_t size ) {

	if ( size ) {
	
		ZLSizeResult result = this->Deflate ( buffer, size );
		
		if ( result.mCode == ZL_OK ) {
		
			this->mCursor += size;
			if ( this->mLength < this->mCursor ) {
				this->mLength = this->mCursor;
			}
			return result;
		}
	}
	ZL_RETURN_SIZE_RESULT ( 0, ZL_OK );
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
