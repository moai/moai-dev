// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLBase64Writer.h>

//================================================================//
// ZLBase64Writer
//================================================================//

//----------------------------------------------------------------//
// returns an approx. len no smaller than actual encoded size
size_t ZLBase64Writer::EstimateEncodedLength ( size_t plainLength ) {

	return 4 * ( size_t )ceilf (( double )plainLength / 3.0 );
}

//----------------------------------------------------------------//
u32 ZLBase64Writer::GetCaps () {

	return this->mProxiedStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
void ZLBase64Writer::OnClose () {
	
	if ( this->mProxiedStream ) {
		
		// space remaining in current block
		u32 blockCursor = this->mCursor % ZLBase64Encoder::PLAIN_BLOCK_SIZE;

		if ( blockCursor ) {
			u8 crypt [ ZLBase64Encoder::CRYPT_BLOCK_SIZE ];
			this->mEncoder.Encode ( crypt, this->mPlainBlock, blockCursor );
			size_t write = this->mProxiedStream->WriteBytes ( crypt, ZLBase64Encoder::CRYPT_BLOCK_SIZE );
			if ( write != ZLBase64Encoder::CRYPT_BLOCK_SIZE ) {
				// TODO: report errors
			}
		}
	}
}

//----------------------------------------------------------------//
bool ZLBase64Writer::OnOpen () {

	this->mEncoder.FormatPlainBlock ( this->mPlainBlock );
	return true;
}

//----------------------------------------------------------------//
size_t ZLBase64Writer::WriteBytes ( const void* buffer, size_t size ) {
	
	u8 crypt [ ZLBase64Encoder::CRYPT_BLOCK_SIZE ];

	size_t remainder = size;
	while ( remainder ) {

		// space remaining in current block
		u32 blockCursor = this->mCursor % ZLBase64Encoder::PLAIN_BLOCK_SIZE;
		u32 blockRemainder = ZLBase64Encoder::PLAIN_BLOCK_SIZE - blockCursor;

		size_t copySize = remainder;
		if ( copySize > blockRemainder ) {
			copySize = blockRemainder;
		}

		memcpy ( &this->mPlainBlock [ blockCursor ], buffer, copySize );
		blockCursor += copySize;
		
		if ( ZLBase64Encoder::PLAIN_BLOCK_SIZE <= blockCursor ) {
			// encode and write
			this->mEncoder.Encode ( crypt, this->mPlainBlock, ZLBase64Encoder::PLAIN_BLOCK_SIZE );
			size_t write = this->mProxiedStream->WriteBytes ( crypt, ZLBase64Encoder::CRYPT_BLOCK_SIZE );
			this->mEncoder.FormatPlainBlock ( this->mPlainBlock );
			
			if ( write != ZLBase64Encoder::CRYPT_BLOCK_SIZE ) {
				// TODO: report errors
				break;
			}
		}
		
		buffer = ( const void* )(( size_t )buffer + copySize );
		remainder -= copySize;
		this->mCursor += copySize;
	}
	
	if ( this->mLength < this->mCursor ) {
		this->mLength = this->mCursor;
	}
	return size - remainder;
}

//----------------------------------------------------------------//
ZLBase64Writer::ZLBase64Writer () {
}

//----------------------------------------------------------------//
ZLBase64Writer::~ZLBase64Writer () {

	this->Close ();
}
