// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "ZLAbstractFinalizable.h"
#include "ZLAbstractFinalizable_Internal.h"

//================================================================//
// ZLAbstractFinalizable_Internal
//================================================================//

//----------------------------------------------------------------//
void ZLAbstractFinalizable_Internal::AddClient ( ZLAbstractFinalizable& client ) {
	this->mClients.insert ( &client );
}

//----------------------------------------------------------------//
void ZLAbstractFinalizable_Internal::AddProvider ( ZLAbstractFinalizable& provider ) {
	this->mProviders.insert ( &provider );
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
void ZLAbstractFinalizable_Internal::RemoveProvider ( ZLAbstractFinalizable& provider ) {
	this->mProviders.erase ( &provider );
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
	STLSet < ZLAbstractFinalizable* >::iterator providerIt = this->mProviders.begin ();
	for ( ; providerIt != this->mProviders.end (); ++providerIt ) {
		ZLAbstractFinalizable* provider = *providerIt;
		provider->GetMembership ()->RemoveClient ( *this->mOwner );
	}
}
