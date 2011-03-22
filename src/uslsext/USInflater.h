// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USINFLATER_H
#define USINFLATER_H

class USStream;

//================================================================//
// USInflater
//================================================================//
class USInflater :
	public USStreamFormatter {
private:

	void*		mZStream;

	//----------------------------------------------------------------//
	void		AffirmInit				();
	void		Finish					( u32 state );
	u32			Format					( bool more );

public:

	//----------------------------------------------------------------//
				USInflater				();
				~USInflater				();
};

#endif
