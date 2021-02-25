// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLMUTEX_H
#define ZLMUTEX_H

class ZLMutexImpl;

//================================================================//
// ZLMutex
//================================================================//
class ZLMutex {
protected:

	ZLMutexImpl*	mMutexImpl;

	//----------------------------------------------------------------//
	ZLMutex& operator = ( const ZLMutex& ) {
		return *this;
	}

	//----------------------------------------------------------------//
	ZLMutex ( const ZLMutex& ) {
	}

public:

	//----------------------------------------------------------------//
	void			Lock				();
					ZLMutex				();
	virtual			~ZLMutex			();
	void			Unlock				();
};

//================================================================//
// MOAIAutoLock
//================================================================//
// TODO: rename to MOAIScopedLock?
class MOAIAutoLock {
private:

	ZLMutex&		mMutex;
	
public:
	
	//----------------------------------------------------------------//
					MOAIAutoLock		( ZLMutex& );
					~MOAIAutoLock		();
};

#endif
