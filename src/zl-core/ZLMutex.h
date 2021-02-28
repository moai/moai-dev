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
// ZLScopedLock
//================================================================//
class ZLScopedLock {
private:

	ZLMutex*		mMutex;
	
public:
	
	//----------------------------------------------------------------//
					ZLScopedLock		( ZLMutex& mutex );
					ZLScopedLock		( const ZLScopedLock& other );
					~ZLScopedLock		();
};

//================================================================//
// ZLScopedRef
//================================================================//
template < typename TYPE >
class ZLScopedRef :
	public ZLScopedLock {
private:

	TYPE&			mRef;
	
public:
	
	//----------------------------------------------------------------//
	TYPE& operator * () {
		return this->mRef;
	}

	//----------------------------------------------------------------//
	const TYPE& operator * () const {
		return this->mRef;
	}

	//----------------------------------------------------------------//
	TYPE* operator -> () {
		return &this->mRef;
	}
	
	//----------------------------------------------------------------//
	const TYPE* operator -> () const {
		return &this->mRef;
	}

	//----------------------------------------------------------------//
	operator TYPE* () {
		return &this->mRef;
	}

	//----------------------------------------------------------------//
	operator const TYPE* () const {
		return &this->mRef;
	}
	
	//----------------------------------------------------------------//
	ZLScopedRef ( ZLMutex& mutex, TYPE& ref ) :
		ZLScopedLock ( mutex ),
		mRef ( ref ) {
	}
	
	//----------------------------------------------------------------//
	ZLScopedRef ( const ZLScopedRef < TYPE >& other ) :
		ZLScopedLock ( other ),
		mRef ( other.mRef ) {
	}
	
	//----------------------------------------------------------------//
	~ZLScopedRef () {
	}
};

#endif
