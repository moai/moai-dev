// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLAbstractFinalizable.h>

//================================================================//
// ZLAbstractFinalizable
//================================================================//

//----------------------------------------------------------------//
ZLAbstractFinalizable::ZLAbstractFinalizable () :
	mInternal ( NULL ) {
}

//----------------------------------------------------------------//
ZLAbstractFinalizable::~ZLAbstractFinalizable () {
	if ( this->mInternal ) {
		delete this->mInternal;
		this->mInternal = NULL;
	}
}

//----------------------------------------------------------------//
void ZLAbstractFinalizable::Finalize () {
	this->ZLAbstractFinalizable_Finalize ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
ZLAbstractFinalizable_Internal& ZLAbstractFinalizable::ZLAbstractFinalizable_HasInternal_AffirmInternal () {
	if ( !this->mInternal ) {
		this->mInternal = new ZLAbstractFinalizable_Internal ( *this );
	}
	return *this->mInternal;
}

//----------------------------------------------------------------//
ZLAbstractFinalizable_Internal* ZLAbstractFinalizable::ZLAbstractFinalizable_HasInternal_GetInternal () {
	return this->mInternal;
}
