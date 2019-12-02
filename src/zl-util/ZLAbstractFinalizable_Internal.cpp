// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLAbstractFinalizable.h>
#include <zl-util/ZLAbstractFinalizable_Internal.h>

//================================================================//
// ZLAbstractFinalizable_Internal
//================================================================//

//----------------------------------------------------------------//
void ZLAbstractFinalizable_Internal::AddClient ( ZLAbstractFinalizable& client ) {
	this->mClients.insert ( &client );
}

//----------------------------------------------------------------//
void ZLAbstractFinalizable_Internal::AddDependency ( ZLAbstractFinalizable& dependency ) {
	this->mDependencies.insert ( &dependency );
}

//----------------------------------------------------------------//
ZLAbstractFinalizable& ZLAbstractFinalizable_Internal::GetOwner () {
	assert ( this->mOwner );
	return *this->mOwner;
}

//----------------------------------------------------------------//
void ZLAbstractFinalizable_Internal::RemoveClient ( ZLAbstractFinalizable& client ) {
	this->mClients.erase ( &client );
}

//----------------------------------------------------------------//
void ZLAbstractFinalizable_Internal::RemoveDependency ( ZLAbstractFinalizable& dependency ) {
	this->mDependencies.erase ( &dependency );
}


//----------------------------------------------------------------//
ZLAbstractFinalizable_Internal::ZLAbstractFinalizable_Internal () :
	mOwner ( NULL ) {
}
//----------------------------------------------------------------//
ZLAbstractFinalizable_Internal::ZLAbstractFinalizable_Internal ( ZLAbstractFinalizable& owner ) :
	mOwner ( &owner ) {
}

//----------------------------------------------------------------//
ZLAbstractFinalizable_Internal::~ZLAbstractFinalizable_Internal () {

	// finalize all our clients; they will remove themselves fromt this membership
	// when their own memberships are destroyed.
	while ( this->mClients.size ()) {
		( *this->mClients.begin ())->Finalize ();
	}

	// remove self from providers.
	STLSet < ZLAbstractFinalizable* >::iterator dependencyIt = this->mDependencies.begin ();
	for ( ; dependencyIt != this->mDependencies.end (); ++dependencyIt ) {
		ZLAbstractFinalizable* dependency = *dependencyIt;
		dependency->GetInternal ()->RemoveClient ( *this->mOwner );
	}
}
