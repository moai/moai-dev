// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>

#include <zl-util/ZLRefCountedObject.h>

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
	mHandle ( 0 ) {
}

//----------------------------------------------------------------//
ZLRefCountedObject::~ZLRefCountedObject () {

	this->Abandon ();
}
