// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLHANDLE_H
#define ZLHANDLE_H

#include <zl-util/ZLRefCountedObject.h>

template < typename TYPE > class ZLSharedHandleTarget;

//================================================================//
// ZLSharedHandle
//================================================================//
template < typename TYPE >
class ZLSharedHandle :
	public ZLRefCountedObject {
private:

	friend class ZLSharedHandleTarget < TYPE >;

	TYPE* mTarget;

	//----------------------------------------------------------------//
	static TYPE* Deref ( ZLSharedHandle < TYPE >* handle ) {
	
		return handle ? handle->mTarget : 0;
	}
	
	//----------------------------------------------------------------//
	ZLSharedHandle () :
		mTarget ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~ZLSharedHandle () {
	}

public:

	GET ( TYPE*, Target, mTarget )

};

//================================================================//
// ZLSharedHandleTarget
//================================================================//
template < typename TYPE >
class ZLSharedHandleTarget {
private:

	ZLSharedHandle < TYPE >* mHandle;

protected:

	//----------------------------------------------------------------//
	void Affirm () {
	
		if ( !this->mHandle ) {
			this->mHandle = new ZLSharedHandle < TYPE > ();
			this->mHandle->Retain ();
			this->mHandle->mTarget = this->GetHandleTarget ();
			assert ( this->mHandle );
		}
	}

	//----------------------------------------------------------------//
	virtual TYPE* GetHandleTarget () = 0;

public:

	//----------------------------------------------------------------//
	void Abandon () {
	
		if ( this->mHandle ) {
			this->mHandle->mTarget = 0;
			this->mHandle->Release ();
			this->mHandle = 0;
		}
		
		this->Affirm ();
	}
	
	//----------------------------------------------------------------//
	ZLSharedHandle < TYPE >* GetRetainedHandle () {
	
		this->Affirm ();
		this->mHandle->Retain ();
		return this->mHandle;
	}
	
	//----------------------------------------------------------------//
	ZLSharedHandleTarget () :
		mHandle ( 0 ) {
	}

	//----------------------------------------------------------------//
	virtual ~ZLSharedHandleTarget () {

		if ( this->mHandle ) {
			this->mHandle->mTarget = 0;
			this->mHandle->Release ();
		}
	}
};

#endif
