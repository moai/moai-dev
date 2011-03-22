// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USREF_H
#define	USREF_H

#include <uslscore/USCanary.h>

//================================================================//
// USRef
//================================================================//
template < typename TYPE >
class USRef {
protected:

	TYPE*		mObject;
	USCanary*	mCanary;
	
	//----------------------------------------------------------------//
	inline TYPE* Get () {
	
		if ( this->mCanary ) {	
			if ( this->mCanary->IsValid ()) {
				return this->mObject;
			}
			this->Release ();
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	void Release () {
	
		if ( this->mCanary ) {
			
			this->mCanary->Release ( true );
			this->mObject = 0;
			this->mCanary = 0;
		}
	}
	
	//----------------------------------------------------------------//
	void Set ( TYPE* assign ) {

		if ( this->mObject != assign ) {

			this->Release ();

			if ( assign ) {
				this->mObject = assign;
				this->mCanary = assign->AffirmCanary ();
				this->mCanary->Retain ( true );
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
	inline void operator = ( const USRef < TYPE >& assign ) {
		this->Set ( assign.mObject );
	};

	//----------------------------------------------------------------//
	USRef () :
		mObject ( 0 ),
		mCanary ( 0 ) {
	}

	//----------------------------------------------------------------//
	USRef ( TYPE* assign ) :
		mObject ( 0 ),
		mCanary ( 0 ) {
		this->Set ( assign );
	};

	//----------------------------------------------------------------//
	USRef ( const USRef < TYPE >& assign ) :
		mObject ( 0 ),
		mCanary ( 0 ) {
		this->Set ( assign.mObject );
	};
	
	//----------------------------------------------------------------//
	~USRef () {
		this->Release ();
	}
};

#endif
