// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "ZLAbstractFinalizable.h"

//================================================================//
// ZLAbstractFinalizable
//================================================================//

//----------------------------------------------------------------//
ZLAbstractFinalizable::ZLAbstractFinalizable () :
	mMembership ( NULL ) {
}

//----------------------------------------------------------------//
ZLAbstractFinalizable::~ZLAbstractFinalizable () {
	if ( this->mMembership ) {
		delete this->mMembership;
		this->mMembership = NULL;
	}
}

//----------------------------------------------------------------//
void ZLAbstractFinalizable::Finalize () {
	void* self = this->ZLAbstractFinalizable_GetSelf ();
	this->ZLAbstractFinalizable_Finalize ( self );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
ZLAbstractFinalizable_Internal& ZLAbstractFinalizable::ZLAbstractFinalizable_HasInternal_AffirmMembership () {
	if ( !this->mMembership ) {
		this->mMembership = new ZLAbstractFinalizable_Internal ( *this );
	}
	return *this->mMembership;
}

//----------------------------------------------------------------//
ZLAbstractFinalizable_Internal* ZLAbstractFinalizable::ZLAbstractFinalizable_HasInternal_GetMembership () {
	return this->mMembership;
}
