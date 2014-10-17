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
	void			Affirm			();
	void			Clear			();
					MOAIMutex		( const MOAIMutex& ) {}
	
	//----------------------------------------------------------------//
	MOAIMutex& operator = ( const MOAIMutex& ) {
		return *this;
	}

public:

	//----------------------------------------------------------------//
	void			Lock				();
					MOAIMutex			();
					~MOAIMutex			();
	void			Unlock				();
};

//================================================================//
// MOAIAutoLock
//================================================================//
// TODO: rename to MOAIScopedLock?
class MOAIAutoLock {
private:
	MOAIMutex&		mMutex;
	
public:
	
	//----------------------------------------------------------------//
	MOAIAutoLock					( MOAIMutex& );
	~MOAIAutoLock					();
};

#endif
