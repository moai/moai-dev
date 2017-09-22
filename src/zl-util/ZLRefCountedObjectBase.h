// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLREFCOUNTEDOBJECTBASE_H
#define ZLREFCOUNTEDOBJECTBASE_H

#include <zl-util/ZLAccessors.h>

//================================================================//
// ZLRefCountedObjectBase
//================================================================//
class ZLRefCountedObjectBase {
private:

	u32		mRefCount;

protected:

	//----------------------------------------------------------------//
	virtual void OnRelease ( u32 refCount ) {

		if ( refCount == 0 ) {
			delete ( this );
		}
	}

	//----------------------------------------------------------------//
	virtual void OnRetain ( u32 refCount ) {
		UNUSED ( refCount );
	}

public:

	GET ( u32, RefCount, mRefCount )

	//----------------------------------------------------------------//
	void Release () {

		if ( this->mRefCount > 0 ) {
			this->mRefCount--;
		}
		this->OnRelease ( this->mRefCount );
	}

	//----------------------------------------------------------------//
	void Retain () {

		this->mRefCount++;
		this->OnRetain ( this->mRefCount );
	}

	//----------------------------------------------------------------//
	ZLRefCountedObjectBase () :
		mRefCount ( 0 ) {
	}

	//----------------------------------------------------------------//
	virtual ~ZLRefCountedObjectBase () {
	}
};

//----------------------------------------------------------------//
void	ZLRelease		( ZLRefCountedObjectBase* object );
void	ZLRetain		( ZLRefCountedObjectBase* object );

#endif
