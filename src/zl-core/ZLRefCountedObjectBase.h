// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLREFCOUNTEDOBJECTBASE_H
#define ZLREFCOUNTEDOBJECTBASE_H

#include <zl-core/ZLAccessors.h>

//================================================================//
// ZLRefCountedObjectBase
//================================================================//
class ZLRefCountedObjectBase {
private:

	friend class ZLRefCountedObject;

	u32		mRefCount;

protected:

	//----------------------------------------------------------------//
	virtual void ZLRefCountedObjectBase_OnRelease ( u32 refCount ) {

		if ( refCount == 0 ) {
			delete ( this );
		}
	}

	//----------------------------------------------------------------//
	virtual void ZLRefCountedObjectBase_OnRetain ( u32 refCount ) {
		UNUSED ( refCount );
	}

public:

	GET ( u32, RefCount, mRefCount )

	//----------------------------------------------------------------//
	void Release () {

		if ( this->mRefCount > 0 ) {
			this->mRefCount--;
		}
		this->ZLRefCountedObjectBase_OnRelease ( this->mRefCount );
	}

	//----------------------------------------------------------------//
	void Retain () {

		this->mRefCount++;
		this->ZLRefCountedObjectBase_OnRetain ( this->mRefCount );
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
