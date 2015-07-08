// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLRefCountedObject.h>

//================================================================//
// ZLRefCountedObject
//================================================================//

//----------------------------------------------------------------//
void ZLRefCountedObject::Release () {

	if ( this->mRefCount > 0 ) {
		this->mRefCount--;
	}
	
	if ( this->mRefCount == 0 ) {
		delete ( this );
	}
}

//----------------------------------------------------------------//
void ZLRefCountedObject::Retain () {

	this->mRefCount++;
}

//----------------------------------------------------------------//
ZLRefCountedObject::ZLRefCountedObject () :
	mRefCount ( 0 ) {
}

//----------------------------------------------------------------//
ZLRefCountedObject::~ZLRefCountedObject () {
}
