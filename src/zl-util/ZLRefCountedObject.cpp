// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>

#include <zl-util/ZLRefCountedObject.h>
#include <zl-util/ZLTransmigrationCache.h>

//================================================================//
// ZLRefCountedObjectSnapshot
//================================================================//
class ZLRefCountedObjectSnapshot {
public:

	u32								mRefCount;
	ZLRefCountedObjectHandle*		mHandle;
};

//================================================================//
// ZLRefCountedObjectHandle
//================================================================//

//----------------------------------------------------------------//
ZLRefCountedObject* ZLRefCountedObjectHandle::GetObject () {

	return this->mObject;
}

//----------------------------------------------------------------//
bool ZLRefCountedObjectHandle::IsValid () {

	return ( this->mObject != 0 );
}

//----------------------------------------------------------------//
ZLRefCountedObjectHandle::ZLRefCountedObjectHandle () :
	mObject ( 0 ) {
}

//----------------------------------------------------------------//
ZLRefCountedObjectHandle::~ZLRefCountedObjectHandle () {

	if ( this->mObject ) {
		this->mObject->mHandle = 0;
	}
}

//================================================================//
// ZLRefCountedObject
//================================================================//

//----------------------------------------------------------------//
void ZLRefCountedObject::Abandon () {

	if ( this->mHandle ) {
		this->mHandle->mObject = 0;
	}
}

//----------------------------------------------------------------//
ZLRefCountedObjectHandle* ZLRefCountedObject::GetHandle () {

	if ( !this->mHandle ) {
		this->mHandle = new ZLRefCountedObjectHandle ();
		this->mHandle->mObject = this;
	}
	else {
		this->mHandle->Retain ();
	}
	return this->mHandle;
}

//----------------------------------------------------------------//
ZLRefCountedObject::ZLRefCountedObject () :
	mHandle ( NULL ) {
	
	ZLTransmigrationCache& transmigrationCache = ZLTransmigrationCache::Get ();
	if ( transmigrationCache.IsActive ()) {
		
		const ZLRefCountedObjectSnapshot& snapshot = transmigrationCache.Memo < ZLRefCountedObjectSnapshot >( this );
		this->mRefCount = snapshot.mRefCount;
		this->mHandle = snapshot.mHandle;
	}
}

//----------------------------------------------------------------//
ZLRefCountedObject::~ZLRefCountedObject () {

	ZLTransmigrationCache& transmigrationCache = ZLTransmigrationCache::Get ();
	if ( transmigrationCache.IsActive ()) {
	
		ZLRefCountedObjectSnapshot snapshot;
		snapshot.mRefCount = this->mRefCount;
		snapshot.mHandle = this->mHandle;
		transmigrationCache.Memo < ZLRefCountedObjectSnapshot >( this, snapshot );
	}
	else {
		this->Abandon ();
	}
}
