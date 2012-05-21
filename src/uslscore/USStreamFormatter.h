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
	size_t		mInputCursor;

	USStream*	mOutStream;

	size_t		mBlockCount;

	//----------------------------------------------------------------//
	virtual void	AffirmInit			() = 0;
	size_t			AppendInput			( const void* buffer, size_t size );
	virtual void	Finish				( u32 state ) = 0;
	virtual u32		Format				( bool more ) = 0;
	int				SetCursor			( long offset );
	size_t			WriteBytes			( const void* buffer, size_t size, bool more );

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
	void		Flush					();
	u32			GetCaps					();
	size_t		GetCursor				();
	size_t		GetLength				();
	size_t		ReadBytes				( void* buffer, size_t size );
				USStreamFormatter		();
				~USStreamFormatter		();
	size_t		WriteBytes				( const void* buffer, size_t size );
};

#endif
