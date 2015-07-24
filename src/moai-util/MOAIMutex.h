// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMUTEX_H
#define MOAIMUTEX_H

class MOAIMutexImpl;

//================================================================//
// MOAIMutex
//================================================================//
class MOAIMutex {
protected:

	MOAIMutexImpl*	mMutexImpl;

	//----------------------------------------------------------------//
	MOAIMutex& operator = ( const MOAIMutex& ) {
		return *this;
	}

	//----------------------------------------------------------------//
	MOAIMutex ( const MOAIMutex& ) {
	}

public:

	//----------------------------------------------------------------//
	void			Lock				();
					MOAIMutex			();
	virtual			~MOAIMutex			();
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
					MOAIAutoLock		( MOAIMutex& );
					~MOAIAutoLock		();
};

#endif
