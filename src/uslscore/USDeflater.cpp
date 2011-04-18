// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlib.h>
#include <uslscore/USDeflater.h>
#include <uslscore/USZip.h>

//----------------------------------------------------------------//
void USDeflater::AffirmInit () {

	if ( !this->mZStream ) {
		
		z_stream* stream = ( z_stream* )malloc ( sizeof ( z_stream ));
		memset ( stream, 0, sizeof ( z_stream ));
		
		this->mZStream = stream;
		this->mState = READY;
	}
}

//----------------------------------------------------------------//
u32 USDeflater::Format ( bool more ) {

	u32 total = 0;
	u8 out [ USZip::CHUNKSIZE ];
	
	z_stream* stream = ( z_stream* )this->mZStream;

	stream->next_in		= this->mInputChunk;
	stream->avail_in	= this->mInputCursor;
	stream->next_out	= out;
	stream->avail_out	= USZip::CHUNKSIZE;
	
	this->mInputCursor = 0;

	if ( this->mState == READY ) {
		int result = deflateInit2 ( stream, this->mCompressionLevel, Z_DEFLATED, this->mWindowBits, 7, Z_DEFAULT_STRATEGY );
		if ( result != Z_OK ) {
			this->Finish ( ERROR );
			return total;
		}
		this->mState = WORKING;
	}

	int flush = more ? Z_NO_FLUSH : Z_FINISH;

	do {
		
		stream->next_out = out;
		stream->avail_out = USZip::CHUNKSIZE;
		
		int result = deflate ( stream, flush );
		assert ( result != Z_STREAM_ERROR );

		if (( result != Z_OK ) && ( result != Z_STREAM_END )) {
			this->Finish ( ERROR );
			return total;
		}
		
		u32 have = USZip::CHUNKSIZE - stream->avail_out;
		u32 write = this->mOutStream->WriteBytes ( out, have );
		total += write;
		
		if ( write != have ) {
			this->Finish ( ERROR );
			return total;
		}

	} while ( stream->avail_out == 0 );

	return total;
}

//----------------------------------------------------------------//
void USDeflater::Finish ( u32 state ) {

	z_stream* stream = ( z_stream* )this->mZStream;
	this->mZStream = 0;
	this->mState = state;

	if ( stream ) {
		deflateEnd ( stream );
		free ( stream );
	}
}

//----------------------------------------------------------------//
USDeflater::USDeflater () :
	mZStream ( 0 ),
	mCompressionLevel ( DEFAULT_LEVEL ),
	mWindowBits ( DEFAULT_WBITS ) {
}

//----------------------------------------------------------------//
USDeflater::~USDeflater () {

	this->Finish ( DONE );
}
