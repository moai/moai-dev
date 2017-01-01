// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLWEAKPTR_H
#define	ZLWEAKPTR_H

#include <zl-util/ZLRefCountedObject.h>

//================================================================//
// ZLWeakPtr
//================================================================//
template < typename TYPE >
class ZLWeakPtr {
protected:

	TYPE*							mObject;
	ZLRefCountedObjectHandle*		mHandle;
	
	//----------------------------------------------------------------//
	inline TYPE* Get () {
	
		if ( this->mHandle ) {
			if ( this->mHandle->IsValid ()) {
				return this->mObject;
			}
			this->Release ();
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	void Release () {
	
		if ( this->mHandle ) {
			this->mHandle->Release ();
			this->mObject = 0;
			this->mHandle = 0;
		}
	}
	
	//----------------------------------------------------------------//
	void Set ( TYPE* assign ) {

		if ( this->mObject != assign ) {

			this->Release ();

			if ( assign ) {
				this->mObject = assign;
				this->mHandle = assign->GetHandle ();
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
	ZLWeakPtr () :
		mObject ( 0 ),
		mHandle ( 0 ) {
	}

	//----------------------------------------------------------------//
	ZLWeakPtr ( TYPE* assign ) :
		mObject ( 0 ),
		mHandle ( 0 ) {
		this->Set ( assign );
	};
	
	//----------------------------------------------------------------//
	~ZLWeakPtr () {
		this->Release ();
	}
};

#endif
