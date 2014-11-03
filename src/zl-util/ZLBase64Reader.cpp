// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLBase64Reader.h>

//================================================================//
// ZLBase64Reader
//================================================================//

//----------------------------------------------------------------//
// returns an approx. len no smaller than actual decoded size
size_t ZLBase64Reader::EstimateDecodedLength ( size_t encodedLength ) {

	return 3 * ( size_t )ceilf (( double )encodedLength / 4.0 ); // should not need ceil
}

//----------------------------------------------------------------//
u32 ZLBase64Reader::GetCaps () {

	return this->mProxiedStream ? CAN_READ | CAN_SEEK : 0;
}

//----------------------------------------------------------------//
void ZLBase64Reader::OnClose () {
	
	this->mBlockID = ( size_t )-1;
	this->mBlockTop = 0;
}

//----------------------------------------------------------------//
size_t ZLBase64Reader::ReadBytes ( void* buffer, size_t size ) {

	size_t remainder = size;
	while ( remainder ) {
		
		this->SyncBlock ();
		
		// space remaining in current block
		u32 blockCursor = this->mCursor % ZLBase64Encoder::PLAIN_BLOCK_SIZE;
		u32 blockRemainder = ZLBase64Encoder::PLAIN_BLOCK_SIZE - blockCursor;

		size_t copySize = remainder;
		if ( copySize > blockRemainder ) {
			copySize = blockRemainder;
		}
		
		if ( copySize > this->mBlockTop ) {
			copySize = this->mBlockTop;
		}
		
		if ( copySize ) {
			memcpy ( buffer, &this->mPlainBlock [ blockCursor ], copySize );
			buffer = ( void* )(( size_t )buffer + copySize );
			remainder -= copySize;
			this->mCursor += copySize;
		}
	
		if ( this->mBlockTop < ZLBase64Encoder::PLAIN_BLOCK_SIZE ) {
			break;
		}
	}
	
	return size - remainder;
}

//----------------------------------------------------------------//
int ZLBase64Reader::SetCursor ( long offset ) {

	this->mCursor = offset;
	return 0;
}

//----------------------------------------------------------------//
void ZLBase64Reader::SyncBlock () {

	u32 blockID = ( u32 )( this->mCursor / ZLBase64Encoder::PLAIN_BLOCK_SIZE );
	
	if ( blockID != this->mBlockID ) {
	
		this->mBlockID = blockID;
		
		size_t cryptBlockAddr = ( blockID * ZLBase64Encoder::CRYPT_BLOCK_SIZE ) + this->mBase;
		this->mProxiedStream->Seek ( cryptBlockAddr, SEEK_SET );
		
		u8 cryptBlock [ ZLBase64Encoder::CRYPT_BLOCK_SIZE ];
		this->mEncoder.FormatCryptBlock ( cryptBlock );
		this->mProxiedStream->ReadBytes ( cryptBlock, ZLBase64Encoder::CRYPT_BLOCK_SIZE );
		
		this->mBlockTop = this->mEncoder.Decode ( this->mPlainBlock, cryptBlock );
	}
}

//----------------------------------------------------------------//
ZLBase64Reader::ZLBase64Reader () :
	mBlockID (( size_t )-1 ),
	mBlockTop ( 0 ) {
}

//----------------------------------------------------------------//
ZLBase64Reader::~ZLBase64Reader () {

	this->Close ();
}