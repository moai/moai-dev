// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlib.h>
#include <uslscore/USInflater.h>
#include <uslscore/USZip.h>

//----------------------------------------------------------------//
void USInflater::AffirmInit () {

	if ( !this->mZStream ) {
		
		z_stream* stream = ( z_stream* )malloc ( sizeof ( z_stream ));
		memset ( stream, 0, sizeof ( z_stream ));
		
		this->mZStream = stream;
		this->mState = READY;
	}
}

//----------------------------------------------------------------//
void USInflater::Finish ( u32 state ) {

	z_stream* stream = ( z_stream* )this->mZStream;
	this->mZStream = 0;
	this->mState = state;

	if ( stream ) {
		inflateEnd ( stream );
		free ( stream );
	}
}

//----------------------------------------------------------------//
u32 USInflater::Format ( bool more ) {
	UNUSED ( more );
	
	u32 total = 0;
	u8 out [ USZip::CHUNKSIZE ];
	
	z_stream* stream = ( z_stream* )this->mZStream;

	stream->next_in		= this->mInputChunk;
	stream->avail_in	= this->mInputCursor;
	stream->next_out	= out;
	stream->avail_out	= USZip::CHUNKSIZE;
	
	this->mInputCursor = 0;

	if ( this->mState == READY ) {
		int result = inflateInit2 ( stream, this->mWindowBits );
		if ( result != Z_OK ) {
			this->Finish ( ERROR );
			return total;
		}
		this->mState = WORKING;
	}

	do {
		
		stream->next_out = out;
		stream->avail_out = USZip::CHUNKSIZE;
		
		int result = inflate ( stream, Z_NO_FLUSH );
		assert ( result != Z_STREAM_ERROR );

		if (( result != Z_OK ) && ( result != Z_STREAM_END )) {
		
			printf ( "%s\n", USZip::GetErrMsg ( result ));
		
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
USInflater::USInflater () :
	mZStream ( 0 ),
	mWindowBits ( DEFAULT_WBITS ) {
}

//----------------------------------------------------------------//
USInflater::~USInflater () {

	this->Finish ( DONE );
}
