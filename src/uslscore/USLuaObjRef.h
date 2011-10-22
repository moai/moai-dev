// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USLUAOBREF_H
#define	USLUAOBREF_H

#include <uslscore/USCanary.h>

//================================================================//
// USLuaObjRef
//================================================================//
template < typename TYPE >
class USLuaObjRef {
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
	USLuaObjRef () :
		mObject ( 0 ),
		mOwner ( 0 ) {
	}

	//----------------------------------------------------------------//
	USLuaObjRef ( const USLuaObjRef < TYPE >& assign ) :
		mObject ( 0 ),
		mOwner ( 0 ) {
		
		UNUSED ( assign );
		
		assert ( false ); // unsupported
	};
	
	//----------------------------------------------------------------//
	~USLuaObjRef () {
		this->Release ();
	}
};

#endif
