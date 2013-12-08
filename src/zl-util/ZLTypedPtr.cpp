// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLTypedPtr.h>

//================================================================//
// ZLTypedPtr
//================================================================//

//----------------------------------------------------------------//
bool ZLTypedPtr::IsMatch ( ZLTypedPtr& src ) {
	return ( this->mTypeID == src.mTypeID );
}

//----------------------------------------------------------------//
void ZLTypedPtr::Nullify () {
	this->mData = 0;
}

//----------------------------------------------------------------//
ZLTypedPtr::ZLTypedPtr () :
	mData ( 0 ) {
}
