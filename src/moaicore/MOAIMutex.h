// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMUTEX_H
#define MOAIMUTEX_H

class MOAIMutexImpl;

//================================================================//
// MOAIMutex
//================================================================//
class MOAIMutex {
private:

	MOAIMutexImpl*	mImpl;

	//----------------------------------------------------------------//
	MOAIMutex&		operator =			( const MOAIMutex& ) { return *this; }
	void			Affirm				();
	void			Clear				();
					MOAIMutex			( const MOAIMutex& ) {}

public:

	//----------------------------------------------------------------//
	void			Lock				();
					MOAIMutex			();
					~MOAIMutex			();
	void			Unlock				();
};

#endif
