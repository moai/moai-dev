// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USBase64Writer.h>

//================================================================//
// USBase64Writer
//================================================================//

//----------------------------------------------------------------//
void USBase64Writer::Close () {
	
	if ( this->mOutputStream ) {
		
		// space remaining in current block
		u32 blockCursor = this->mCursor % USBase64Encoder::PLAIN_BLOCK_SIZE;

		if ( blockCursor ) {
			u8 crypt [ USBase64Encoder::CRYPT_BLOCK_SIZE ];
			this->mEncoder.Encode ( crypt, this->mPlainBlock, blockCursor );
			size_t write = this->mOutputStream->WriteBytes ( crypt, USBase64Encoder::CRYPT_BLOCK_SIZE );
			if ( write != USBase64Encoder::CRYPT_BLOCK_SIZE ) {
				// TODO: report errors
			}
		}
		
		this->mCursor = 0;
		this->mOutputStream = 0;
	}
}

//----------------------------------------------------------------//
u32 USBase64Writer::GetCaps () {

	return this->mOutputStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
size_t USBase64Writer::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t USBase64Writer::GetLength () {

	return this->mCursor;
}

//----------------------------------------------------------------//
bool USBase64Writer::Open ( USStream* stream ) {

	this->Close ();

	if ( stream ) {
		this->mOutputStream = stream;
		this->mEncoder.FormatPlainBlock ( this->mPlainBlock );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
USBase64Writer::USBase64Writer () :
	mOutputStream ( 0 ),
	mCursor ( 0 ) {
}

//----------------------------------------------------------------//
USBase64Writer::~USBase64Writer () {

	this->Close ();
}

//----------------------------------------------------------------//
size_t USBase64Writer::WriteBytes ( const void* buffer, size_t size ) {
	
	u8 crypt [ USBase64Encoder::CRYPT_BLOCK_SIZE ];

	size_t remainder = size;
	while ( remainder ) {

		// space remaining in current block
		u32 blockCursor = this->mCursor % USBase64Encoder::PLAIN_BLOCK_SIZE;
		u32 blockRemainder = USBase64Encoder::PLAIN_BLOCK_SIZE - blockCursor;

		size_t copySize = remainder;
		if ( copySize > blockRemainder ) {
			copySize = blockRemainder;
		}

		memcpy ( &this->mPlainBlock [ blockCursor ], buffer, copySize );
		blockCursor += copySize;
		
		if ( USBase64Encoder::PLAIN_BLOCK_SIZE <= blockCursor ) {
			
			// encode and write
			this->mEncoder.Encode ( crypt, this->mPlainBlock, USBase64Encoder::PLAIN_BLOCK_SIZE );
			this->mEncoder.FormatPlainBlock ( this->mPlainBlock );
			
			size_t write = this->mOutputStream->WriteBytes ( crypt, USBase64Encoder::CRYPT_BLOCK_SIZE );
			if ( write != USBase64Encoder::CRYPT_BLOCK_SIZE ) {
				// TODO: report errors
				break;
			}
		}
		
		buffer = ( const void* )(( size_t )buffer + copySize );
		remainder -= copySize;
		this->mCursor += copySize;
	}
	return size - remainder;
}
