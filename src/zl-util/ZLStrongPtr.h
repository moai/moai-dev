// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLSTRONGPTR_H
#define	ZLSTRONGPTR_H

#include <zl-util/ZLRefCountedObject.h>

//================================================================//
// ZLStrongPtr
//================================================================//
template < typename TYPE >
class ZLStrongPtr {
protected:

	TYPE*	mObject;
	
	//----------------------------------------------------------------//
	inline TYPE* Get () {
		return this->mObject;
	}
	
	//----------------------------------------------------------------//
	inline const TYPE* Get () const {
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
	inline operator bool () const {
		return this->Get () != 0;
	}

	//----------------------------------------------------------------//
	inline TYPE& operator * () {
		return *this->Get ();
	}

	//----------------------------------------------------------------//
	inline const TYPE& operator * () const {
		return *this->Get ();
	}

	//----------------------------------------------------------------//
	inline TYPE* operator -> () {
		return this->Get ();
	}
	
	//----------------------------------------------------------------//
	inline const TYPE* operator -> () const {
		return this->Get ();
	}

	//----------------------------------------------------------------//
	inline operator TYPE* () {
		return this->Get ();
	}

	//----------------------------------------------------------------//
	inline operator const TYPE* () const {
		return this->Get ();
	}

	//----------------------------------------------------------------//
	inline void operator = ( TYPE* assign ) {
		this->Set ( assign );
	}

	//----------------------------------------------------------------//
	inline void operator = ( const ZLStrongPtr < TYPE >& assign ) {
		this->Set ( assign.mObject );
	}

	//----------------------------------------------------------------//
	ZLStrongPtr () :
		mObject ( 0 ) {
	}

	//----------------------------------------------------------------//
	ZLStrongPtr ( TYPE* assign ) :
		mObject ( 0 ) {
		this->Set ( assign );
	}

	//----------------------------------------------------------------//
	ZLStrongPtr ( const ZLStrongPtr < TYPE >& assign ) :
		mObject ( 0 ) {
		this->Set ( assign.mObject );
	}
	
	//----------------------------------------------------------------//
	~ZLStrongPtr () {
		this->Set ( 0 );
	}
};

#endif
