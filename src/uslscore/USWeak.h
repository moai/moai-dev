// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USWEAK_H
#define	USWEAK_H

#include <uslscore/USCanary.h>

//================================================================//
// USWeak
//================================================================//
template < typename TYPE >
class USWeak {
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
			this->mCanary->Release ( false );
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
				this->mCanary->Retain ( false );
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
	USWeak () :
		mObject ( 0 ),
		mCanary ( 0 ) {
	}

	//----------------------------------------------------------------//
	USWeak ( TYPE* assign ) :
		mObject ( 0 ),
		mCanary ( 0 ) {
		this->Set ( assign );
	};
	
	//----------------------------------------------------------------//
	~USWeak () {
		this->Release ();
	}
};

#endif
