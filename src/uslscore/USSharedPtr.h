// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USREF_H
#define	USREF_H

#include <uslscore/USCanary.h>

//================================================================//
// USSharedPtr
//================================================================//
template < typename TYPE >
class USSharedPtr {
protected:

	TYPE*		mObject;
	
	//----------------------------------------------------------------//
	inline TYPE* Get () {
		return this->mObject;
	}
	
	//----------------------------------------------------------------//
	void Set ( TYPE* assign ) {

		if ( this->mObject != assign ) {

			if ( this->mObject ) {
				this->mObject->Release ();
			}

			if ( assign ) {
				assign->Retain ();
			}
			
			this->mObject = assign;
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
	inline void operator = ( const USSharedPtr < TYPE >& assign ) {
		this->Set ( assign.mObject );
	};

	//----------------------------------------------------------------//
	USSharedPtr () :
		mObject ( 0 ) {
	}

	//----------------------------------------------------------------//
	USSharedPtr ( TYPE* assign ) :
		mObject ( 0 ) {
		this->Set ( assign );
	};

	//----------------------------------------------------------------//
	USSharedPtr ( const USSharedPtr < TYPE >& assign ) :
		mObject ( 0 ) {
		this->Set ( assign.mObject );
	};
	
	//----------------------------------------------------------------//
	~USSharedPtr () {
		this->Release ();
	}
};

#endif
