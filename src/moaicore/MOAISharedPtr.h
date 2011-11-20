// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIREF_H
#define	MOAIREF_H

#include <moaicore/MOAICanary.h>

//================================================================//
// MOAISharedPtr
//================================================================//
template < typename TYPE >
class MOAISharedPtr {
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
	inline void operator = ( const MOAISharedPtr < TYPE >& assign ) {
		this->Set ( assign.mObject );
	};

	//----------------------------------------------------------------//
	MOAISharedPtr () :
		mObject ( 0 ) {
	}

	//----------------------------------------------------------------//
	MOAISharedPtr ( TYPE* assign ) :
		mObject ( 0 ) {
		this->Set ( assign );
	};

	//----------------------------------------------------------------//
	MOAISharedPtr ( const MOAISharedPtr < TYPE >& assign ) :
		mObject ( 0 ) {
		this->Set ( assign.mObject );
	};
	
	//----------------------------------------------------------------//
	~MOAISharedPtr () {
		this->Set ( 0 );
	}
};

#endif
