// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef CIPHERSTREAM_H
#define CIPHERSTREAM_H

#include <uslscore/USStream.h>

class USCipher;

//================================================================//
// USCipherStream
//================================================================//
class USCipherStream :
	public USStream {
private:
	
	USStream*	mStream;
	USCipher*	mCipher;

	u32			mPlainBlockSize;
	u32			mPlainCursor;		// cursor in the *plain text* file
	u32			mBlockTop;			// top of valid data in block

	u32			mCryptBlockSize;
	u32			mCryptBaseAddr;		// base addr of crypt in crypt stream
	
	u32			mBlockID;			// block cursor
	
	u8*			mPlaintext;
	u8*			mCrypttext;
	bool		mIsDirty;
	
	//----------------------------------------------------------------//
	void		DecodeBlock			();
	void		EncodeBlock			();
	void		FlushBlock			();
	void		ReadBlock			();
	void		SyncBlock			( bool reload );
	void		WriteBlock			();

public:

	//----------------------------------------------------------------//
	void		Clear				();
	void		CloseCipher			();
	void		Flush				();
	u32			GetCursor			();
	u32			GetLength			();
	void		OpenCipher			( USStream& stream, USCipher& cipher );
	u32			ReadBytes			( void* buffer, u32 size );
	void		Seek				( long offset, int origin );
				USCipherStream		();
				~USCipherStream		();
	u32			WriteBytes			( const void* buffer, u32 size );
};

#endif
