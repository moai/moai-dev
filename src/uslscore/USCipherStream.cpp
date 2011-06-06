// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFilename.h>
#include <uslscore/USFileSys.h>
#include <uslscore/USCipher.h>
#include <uslscore/USCipherStream.h>
#include <stdio.h>

//================================================================//
// USCipherStream
//================================================================//

//----------------------------------------------------------------//
void USCipherStream::Clear () {

	if ( this->mPlaintext ) {
		free ( this->mPlaintext );
		this->mPlaintext = 0;
	}
	
	if ( this->mCrypttext ) {
		free ( this->mCrypttext );
		this->mCrypttext = 0;
	}
	
	this->mStream = 0;
	this->mCipher = 0;
}

//----------------------------------------------------------------//
void USCipherStream::CloseCipher () {

	this->Flush ();
}

//----------------------------------------------------------------//
void USCipherStream::Flush () {
	this->FlushBlock ();
}

//----------------------------------------------------------------//
void USCipherStream::FlushBlock () {

	if ( this->mIsDirty ) {
	
		// Update the the crypto
		this->WriteBlock ();
		
		// Huzzah!
		this->mIsDirty = false;
	}
}

//----------------------------------------------------------------//
u32 USCipherStream::GetCursor () {

	return this->mPlainCursor;
}

//----------------------------------------------------------------//
u32 USCipherStream::GetLength () {

	return this->mStream->GetLength () - this->mCryptBaseAddr;
}

//----------------------------------------------------------------//
void USCipherStream::OpenCipher ( USStream& stream, USCipher& cipher ) {

	assert ( &stream != this );

	this->Clear ();

	this->mStream = &stream;
	this->mCipher = &cipher;
	
	this->mPlainBlockSize = cipher.GetPlainBlockSize ();
	
	this->mCryptBlockSize = cipher.GetCryptBlockSize ();
	this->mCryptBaseAddr = stream.GetCursor ();
	
	this->mPlaintext = ( u8* )calloc ( 1, this->mPlainBlockSize );
	this->mCrypttext = ( u8* )calloc ( 1, this->mCryptBlockSize );

	this->mIsDirty = false;
	
	this->mPlainCursor = 0;
	this->mBlockID = 0xffffffff;
	this->SyncBlock ( true );
}

//----------------------------------------------------------------//
void USCipherStream::ReadBlock () {

	u32 blockSize = this->mCryptBlockSize;
	u32 blockAddr = ( this->mBlockID * blockSize ) + this->mCryptBaseAddr;

	// Seek to the current chunk
	this->mStream->Seek ( blockAddr, SEEK_SET );
	
	// Clamp the read size to the top of the file
	u32 remainder = this->mStream->GetLength () - blockAddr;
	
	if ( remainder < blockSize ) {
		
		this->mCipher->FormatCryptBlock ( this->mCrypttext, this->mBlockID );
		
		// Read part of the chunk and pad the rest...
		if ( remainder > 0 ) {
			this->mStream->ReadBytes ( this->mCrypttext, remainder );
		}
	}
	else {
		// Read the whole chunk
		this->mStream->ReadBytes ( this->mCrypttext, blockSize );
	}
	
	this->mBlockTop = this->mCipher->Decode ( this->mCrypttext, this->mPlaintext, this->mBlockID );
}

//----------------------------------------------------------------//
u32 USCipherStream::ReadBytes ( void* buffer, u32 size ) {

	u32 blockSize = this->mPlainBlockSize;
	
	u32 remainder = size;
	while ( remainder ) {

		this->SyncBlock ( true );

		// space remaining in current block
		u32 blockCursor = this->mPlainCursor % blockSize;
		u32 blockRemainder = this->mBlockTop - blockCursor;

		u32 copySize = remainder;
		if ( copySize > blockRemainder ) {
			copySize = blockRemainder;
		}
		
		memcpy ( buffer, &this->mPlaintext [ blockCursor ], copySize );
		
		buffer = &(( u8* )buffer )[ copySize ];
		remainder -= copySize;
		this->mPlainCursor += copySize;
		
		if ( this->mBlockTop < blockSize ) {
			break;
		}
	}
	
	return size - remainder;
}

//----------------------------------------------------------------//
void USCipherStream::Seek ( long offset, int origin ) {

	this->FlushBlock ();

	u32 length = this->GetLength ();

	switch ( origin ) {
		case SEEK_CUR: {
			this->mPlainCursor = this->mPlainCursor + offset;
			break;
		}
		case SEEK_END: {
			this->mPlainCursor = length;
			break;
		}
		case SEEK_SET: {
			this->mPlainCursor = offset;
			break;
		}
	}
	
	if ( this->mPlainCursor > length ) {
		this->mPlainCursor = length;
	}
	
	this->SyncBlock ( true );
}

//----------------------------------------------------------------//
void USCipherStream::SyncBlock ( bool reload ) {

	u32 blockID = ( u32 )( this->mPlainCursor / this->mPlainBlockSize );
	
	if ( blockID != this->mBlockID ) {
		
		// Just in case...
		this->FlushBlock ();
		
		// Update the chunk cursor
		this->mBlockID = blockID;
		
		if ( reload ) {
			// Read in the crypto
			this->ReadBlock ();
		}
	}
}

//----------------------------------------------------------------//
USCipherStream::USCipherStream () :
	mStream ( 0 ),
	mCipher ( 0 ),
	mPlainBlockSize ( 0 ),
	mPlainCursor ( 0 ),
	mBlockTop ( 0 ),
	mCryptBlockSize ( 0 ),
	mCryptBaseAddr ( 0 ),
	mBlockID ( 0 ),
	mPlaintext ( 0 ),
	mCrypttext ( 0 ),
	mIsDirty ( false ) {
}

//----------------------------------------------------------------//
USCipherStream::~USCipherStream () {

	this->Clear ();
}

//----------------------------------------------------------------//
void USCipherStream::WriteBlock () {

	this->mCipher->Encode ( this->mPlaintext, this->mCrypttext, this->mBlockID, this->mBlockTop );

	u32 blockSize = this->mCryptBlockSize;
	u32 blockAddr = ( this->mBlockID * blockSize ) + this->mCryptBaseAddr;

	// Seek to the current block
	this->mStream->Seek ( blockAddr, SEEK_SET );
	
	// Write the block
	this->mStream->WriteBytes ( this->mCrypttext, blockSize );
	this->mStream->Flush ();
}

//----------------------------------------------------------------//
u32 USCipherStream::WriteBytes ( const void* buffer, u32 size ) {

	u32 blockSize = this->mPlainBlockSize;

	u32 remainder = size;
	while ( remainder ) {

		// space remaining in current block
		u32 blockCursor = this->mPlainCursor % blockSize;
		u32 blockRemainder = blockSize - blockCursor;

		u32 copySize = remainder;
		if ( copySize > blockRemainder ) {
			copySize = blockRemainder;
		}
		
		this->SyncBlock ( copySize < blockSize );
		
		memcpy ( &this->mPlaintext [ blockCursor ], buffer, copySize );
		this->mIsDirty = true;
		
		blockCursor += copySize;
		
		if ( blockCursor > this->mBlockTop ) {
			this->mBlockTop = blockCursor;
		}
		
		buffer = &(( u8* )buffer )[ copySize ];
		remainder -= copySize;
		this->mPlainCursor += copySize;
	}
	
	return size;
}
