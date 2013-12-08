// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLBase64Writer.h>

//================================================================//
// ZLBase64Writer
//================================================================//

//----------------------------------------------------------------//
void ZLBase64Writer::Close () {
	
	if ( this->mOutputStream ) {
		
		// space remaining in current block
		u32 blockCursor = this->mCursor % ZLBase64Encoder::PLAIN_BLOCK_SIZE;

		if ( blockCursor ) {
			u8 crypt [ ZLBase64Encoder::CRYPT_BLOCK_SIZE ];
			this->mEncoder.Encode ( crypt, this->mPlainBlock, blockCursor );
			size_t write = this->mOutputStream->WriteBytes ( crypt, ZLBase64Encoder::CRYPT_BLOCK_SIZE );
			if ( write != ZLBase64Encoder::CRYPT_BLOCK_SIZE ) {
				// TODO: report errors
			}
		}
		
		this->mCursor = 0;
		this->mOutputStream = 0;
	}
}

//----------------------------------------------------------------//
u32 ZLBase64Writer::GetCaps () {

	return this->mOutputStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
size_t ZLBase64Writer::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t ZLBase64Writer::GetLength () {

	return this->mCursor;
}

//----------------------------------------------------------------//
bool ZLBase64Writer::Open ( ZLStream& stream ) {

	this->Close ();

	this->mOutputStream = &stream;
	return true;
}

//----------------------------------------------------------------//
ZLBase64Writer::ZLBase64Writer () :
	mOutputStream ( 0 ),
	mCursor ( 0 ) {
}

//----------------------------------------------------------------//
ZLBase64Writer::~ZLBase64Writer () {

	this->Close ();
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
			size_t write = this->mOutputStream->WriteBytes ( crypt, ZLBase64Encoder::CRYPT_BLOCK_SIZE );
			if ( write != ZLBase64Encoder::CRYPT_BLOCK_SIZE ) {
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
