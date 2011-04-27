// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USINFLATER_H
#define USINFLATER_H

#include <uslscore/USStreamFormatter.h>

//================================================================//
// USInflater
//================================================================//
class USInflater :
	public USStreamFormatter {
private:

	void*		mZStream;
	int			mWindowBits;

	//----------------------------------------------------------------//
	void		AffirmInit				();
	void		Finish					( u32 state );
	u32			Format					( bool more );

public:

	static const int DEFAULT_WBITS = -15;

	GET_SET ( int, WindowBits, mWindowBits )

	//----------------------------------------------------------------//
				USInflater				();
				~USInflater				();
};

#endif
