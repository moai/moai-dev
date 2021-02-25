// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-core/ZLAbstractFinalizable.h>

//================================================================//
// ZLAbstractFinalizable
//================================================================//

//----------------------------------------------------------------//
ZLAbstractFinalizableImpl& ZLAbstractFinalizable::AffirmImpl () {

	if ( !this->mImpl ) {
		this->mImpl = new ZLAbstractFinalizableImpl ( *this );
	}
	return *this->mImpl;
}

//----------------------------------------------------------------//
void ZLAbstractFinalizable::Destruct () {
	
	this->FinalizeDependencies ();
	
	const ZLAbstractFinalizationVisitor* visitor = this->GetVisitor < ZLAbstractFinalizationVisitor >();
	if ( visitor ) {
		visitor->Finalize ( *this );
	}
}

//----------------------------------------------------------------//
void ZLAbstractFinalizable::Finalize () {
	
	this->FinalizeDependencies ();
	
	RTTIVisitorIterator < ZLAbstractFinalizationVisitor > visitorIt = this->GetVisitors < ZLAbstractFinalizationVisitor >();
	for ( ; visitorIt; ++visitorIt ) {
		( *visitorIt ).Finalize ( *this );
	}
}

//----------------------------------------------------------------//
void ZLAbstractFinalizable::FinalizeDependencies () {

	if ( this->mImpl ) {
		delete this->mImpl;
		this->mImpl = NULL;
	}
}

//----------------------------------------------------------------//
ZLAbstractFinalizableImpl* ZLAbstractFinalizable::GetImpl () {

	return this->mImpl;
}

//----------------------------------------------------------------//
ZLAbstractFinalizable::ZLAbstractFinalizable () :
	mImpl ( NULL ) {
}

//----------------------------------------------------------------//
ZLAbstractFinalizable::~ZLAbstractFinalizable () {
	assert ( this->mImpl == NULL );
}

//================================================================//
// ZLAbstractFinalizableImpl
//================================================================//

//----------------------------------------------------------------//
void ZLAbstractFinalizableImpl::AddDownstream ( ZLAbstractFinalizable& downstream ) {
	this->mDownstream.insert ( &downstream );
}

//----------------------------------------------------------------//
void ZLAbstractFinalizableImpl::AddUpstream ( ZLAbstractFinalizable& upstream ) {
	this->mUpstream.insert ( &upstream );
}

//----------------------------------------------------------------//
ZLAbstractFinalizable& ZLAbstractFinalizableImpl::GetOwner () {
	assert ( this->mOwner );
	return *this->mOwner;
}

//----------------------------------------------------------------//
void ZLAbstractFinalizableImpl::RemoveDownstream ( ZLAbstractFinalizable& downstream ) {
	this->mDownstream.erase ( &downstream );
}

//----------------------------------------------------------------//
void ZLAbstractFinalizableImpl::RemoveUpstream ( ZLAbstractFinalizable& upstream ) {
	this->mUpstream.erase ( &upstream );
}


//----------------------------------------------------------------//
ZLAbstractFinalizableImpl::ZLAbstractFinalizableImpl () :
	mOwner ( NULL ) {
}
//----------------------------------------------------------------//
ZLAbstractFinalizableImpl::ZLAbstractFinalizableImpl ( ZLAbstractFinalizable& owner ) :
	mOwner ( &owner ) {
}

//----------------------------------------------------------------//
ZLAbstractFinalizableImpl::~ZLAbstractFinalizableImpl () {

	// finalize all our clients; they will remove themselves front this membership
	// when their own memberships are destroyed.
	while ( this->mDownstream.size ()) {
		( *this->mDownstream.begin ())->Finalize ();
	}

	// remove self from providers.
	STLSet < ZLAbstractFinalizable* >::iterator dependencyIt = this->mUpstream.begin ();
	for ( ; dependencyIt != this->mUpstream.end (); ++dependencyIt ) {
		ZLAbstractFinalizable* dependency = *dependencyIt;
		dependency->GetImpl ()->RemoveDownstream ( *this->mOwner );
	}
}
