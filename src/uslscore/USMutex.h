// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MUTEX_H
#define MUTEX_H

#include <uslscore/USTypedPtr.h>

class USMutexImpl;

//================================================================//
// USMutex
//================================================================//
class USMutex {
private:

	USMutexImpl*	mImpl;

	//----------------------------------------------------------------//
	void			Affirm			();
	void			Clear			();
					USMutex			( const USMutex& ) {}
	USMutex&		operator =		( const USMutex& ) { return *this; }

public:

	//----------------------------------------------------------------//
	void			Lock				();
	void			Unlock				();
					USMutex				();
					~USMutex			();
};

#endif
