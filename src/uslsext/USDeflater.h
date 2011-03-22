// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USDEFLATER_H
#define USDEFLATER_H

class USStream;

//================================================================//
// USDeflater
//================================================================//
class USDeflater :
	public USStreamFormatter {
private:

	enum {
		DEFAULT_LEVEL = 9,
	};

	void*		mZStream;
	u32			mCompressionLevel;

	//----------------------------------------------------------------//
	void		AffirmInit				();
	void		Finish					( u32 state );
	u32			Format					( bool more );

public:

	GET_SET ( u32, CompressionLevel, mCompressionLevel )

	//----------------------------------------------------------------//
				USDeflater				();
				~USDeflater				();
};

#endif
