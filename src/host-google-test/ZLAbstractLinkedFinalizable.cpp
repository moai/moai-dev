// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "ZLAbstractLinkedFinalizable.h"

//================================================================//
// ZLAbstractLinkedFinalizable
//================================================================//

//----------------------------------------------------------------//
ZLAbstractLinkedFinalizable::ZLAbstractLinkedFinalizable () :
	mMembership ( NULL ) {
}

//----------------------------------------------------------------//
ZLAbstractLinkedFinalizable::~ZLAbstractLinkedFinalizable () {
	if ( this->mMembership ) {
		delete this->mMembership;
		this->mMembership = NULL;
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
ZLLinkedFinalizableMembership& ZLAbstractLinkedFinalizable::ZLAbstractHasLinkedFinalizableMembership_Affirm () {
	if ( !this->mMembership ) {
		this->mMembership = new ZLLinkedFinalizableMembership ( *this );
	}
	return *this->mMembership;
}

//----------------------------------------------------------------//
ZLLinkedFinalizableMembership* ZLAbstractLinkedFinalizable::ZLAbstractHasLinkedFinalizableMembership_Get () {
	return this->mMembership;
}
