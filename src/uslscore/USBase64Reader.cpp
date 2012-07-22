// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USBase64Reader.h>

//================================================================//
// USBase64Reader
//================================================================//

//----------------------------------------------------------------//
void USBase64Reader::Close () {
	
	this->mInputStream = 0;
	this->mInputBase = 0;
	
	this->mCursor = 0;
	this->mSize = 0;
	this->mLength = 0;
	
	this->mBlockID = ( size_t )-1;
	this->mBlockTop = 0;
}

//----------------------------------------------------------------//
u32 USBase64Reader::GetCaps () {

	return this->mInputStream ? CAN_READ | CAN_SEEK : 0;
}

//----------------------------------------------------------------//
size_t USBase64Reader::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t USBase64Reader::GetLength () {

	return this->mSize > this->mLength ? this->mSize : this->mLength;
}

//----------------------------------------------------------------//
bool USBase64Reader::IsAtEnd () {

	return this->mSize && ( this->mCursor >= this->mSize ? 1 : 0 );
}

//----------------------------------------------------------------//
bool USBase64Reader::Open ( USStream* stream ) {

	this->Close ();

	if ( stream ) {
	
		this->mInputStream = stream;
		this->mInputBase = stream->GetCursor ();
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool USBase64Reader::Open ( USStream* stream, size_t size ) {

	this->Close ();

	if ( stream ) {
	
		this->mInputStream = stream;
		this->mInputBase = stream->GetCursor ();
		
		this->mSize = size;
		this->mLength = size;
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
size_t USBase64Reader::ReadBytes ( void* buffer, size_t size ) {

	size_t remainder = size;
	while ( remainder ) {
		
		this->SyncBlock ();
		
		// space remaining in current block
		u32 blockCursor = this->mCursor % USBase64Encoder::PLAIN_BLOCK_SIZE;
		u32 blockRemainder = USBase64Encoder::PLAIN_BLOCK_SIZE - blockCursor;

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
	
		if ( this->mBlockTop < USBase64Encoder::PLAIN_BLOCK_SIZE ) {
			break;
		}
	}
	return size - remainder;
}

//----------------------------------------------------------------//
int USBase64Reader::SetCursor ( long offset ) {

	this->mCursor = offset;
	return 0;
}

//----------------------------------------------------------------//
void USBase64Reader::SyncBlock () {

	u32 blockID = ( u32 )( this->mCursor / USBase64Encoder::PLAIN_BLOCK_SIZE );
	
	if ( blockID != this->mBlockID ) {
	
		this->mBlockID = blockID;
		
		size_t cryptBlockAddr = ( blockID * USBase64Encoder::CRYPT_BLOCK_SIZE ) + this->mInputBase;
		this->mInputStream->Seek ( cryptBlockAddr, SEEK_SET );
		
		u8 cryptBlock [ USBase64Encoder::CRYPT_BLOCK_SIZE ];
		this->mEncoder.FormatCryptBlock ( cryptBlock );
		this->mInputStream->ReadBytes ( cryptBlock, USBase64Encoder::CRYPT_BLOCK_SIZE );
		
		this->mBlockTop = this->mEncoder.Decode ( this->mPlainBlock, cryptBlock );
	}
}

//----------------------------------------------------------------//
USBase64Reader::USBase64Reader () :
	mInputStream ( 0 ),
	mInputBase ( 0 ),
	mCursor ( 0 ),
	mSize ( 0 ),
	mLength ( 0 ),
	mBlockID (( size_t )-1 ),
	mBlockTop ( 0 ) {
}

//----------------------------------------------------------------//
USBase64Reader::~USBase64Reader () {

	this->Close ();
}