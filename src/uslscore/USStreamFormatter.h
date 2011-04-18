// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USSTREAMFORMATTER_H
#define USSTREAMFORMATTER_H

#include <uslscore/USAccessors.h>
#include <uslscore/USStream.h>

//================================================================//
// USStreamFormatter
//================================================================//
class USStreamFormatter :
	public USStream {
protected:

	enum {
		INPUT_CHUNKSIZE = 1024,
	};

	u32			mState;

	u8			mInputChunk [ INPUT_CHUNKSIZE ];
	u32			mInputCursor;

	USStream*	mOutStream;

	u32			mBlockCount;

	//----------------------------------------------------------------//
	virtual void	AffirmInit			() = 0;
	u32				AppendInput			( const void* buffer, u32 size );
	virtual void	Finish				( u32 state ) = 0;
	virtual u32		Format				( bool more ) = 0;
	u32				WriteBytes			( const void* buffer, u32 size, bool more );

public:

	enum {
		IDLE,
		READY,
		WORKING,
		ERROR,
		DONE,
		TOTAL_STATES,
	};

	GET ( u32, State, mState )
	GET_SET ( USStream*, Stream, mOutStream )

	//----------------------------------------------------------------//
	void	Flush					();
	u32		GetCursor				();
	u32		GetLength				();
	u32		ReadBytes				( void* buffer, u32 size );
	void	Seek					( long offset, int origin );
			USStreamFormatter		();
			~USStreamFormatter		();
	u32		WriteBytes				( const void* buffer, u32 size );
};

#endif
