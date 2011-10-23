// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USLUASHAREDPTR_H
#define	USLUASHAREDPTR_H

#include <uslscore/USCanary.h>

//================================================================//
// USLuaSharedPtr
//================================================================//
template < typename TYPE >
class USLuaSharedPtr {
protected:

	TYPE*			mObject;
	USLuaObject*	mOwner;
	
	//----------------------------------------------------------------//
	inline TYPE* Get () {
	
		return this->mObject;
	}
	
	//----------------------------------------------------------------//
	inline void Release () {
	
		assert ( this->mOwner ); // owner must be set!
	
		if ( this->mObject ) {

			this->mOwner->LuaRelease ( *this->mObject );
			this->mObject = 0;
		}
	}
	
	//----------------------------------------------------------------//
	inline void Set ( TYPE* assign ) {

		assert ( this->mOwner ); // owner must be set!

		if ( this->mObject != assign ) {

			this->Release ();

			if ( assign ) {
			
				this->mObject = assign;
				this->mOwner->LuaRetain ( *this->mObject );
			}
		}
	}

public:

	//----------------------------------------------------------------//
	inline operator bool () {
		return this->Get () != 0;
	};

	//----------------------------------------------------------------//
	inline TYPE& operator * () {
		return *this->Get ();
	};

	//----------------------------------------------------------------//
	inline TYPE* operator -> () {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline operator TYPE* () {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline void operator = ( TYPE* assign ) {
		this->Set ( assign );
	};

	//----------------------------------------------------------------//
	void InitWithOwner ( USLuaObject& owner ) {
		this->mOwner = &owner;
	}

	//----------------------------------------------------------------//
	USLuaSharedPtr () :
		mObject ( 0 ),
		mOwner ( 0 ) {
	}

	//----------------------------------------------------------------//
	USLuaSharedPtr ( const USLuaSharedPtr < TYPE >& assign ) :
		mObject ( 0 ),
		mOwner ( 0 ) {
		assert ( false ); // unsupported
	};
	
	//----------------------------------------------------------------//
	~USLuaSharedPtr () {
		this->Release ();
	}
};

#endif
