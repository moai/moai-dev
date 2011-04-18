// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USDEFLATER_H
#define USDEFLATER_H

#include <uslscore/USStreamFormatter.h>

//================================================================//
// USDeflater
//================================================================//
class USDeflater :
	public USStreamFormatter {
private:

	void*		mZStream;
	u32			mCompressionLevel;
	int			mWindowBits;

	//----------------------------------------------------------------//
	void		AffirmInit				();
	void		Finish					( u32 state );
	u32			Format					( bool more );

public:

	static const u32 DEFAULT_LEVEL = 9;
	static const int DEFAULT_WBITS = -15;

	GET_SET ( u32, CompressionLevel, mCompressionLevel )
	GET_SET ( int, WindowBits, mWindowBits )

	//----------------------------------------------------------------//
				USDeflater				();
				~USDeflater				();
};

#endif
