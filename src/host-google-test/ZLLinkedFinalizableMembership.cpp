// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "ZLAbstractLinkedFinalizable.h"
#include "ZLLinkedFinalizableMembership.h"

//================================================================//
// ZLLinkedFinalizableMembership
//================================================================//

//----------------------------------------------------------------//
void ZLLinkedFinalizableMembership::AddClient ( ZLAbstractFinalizable& client ) {
	this->mClients.insert ( &client );
}

//----------------------------------------------------------------//
void ZLLinkedFinalizableMembership::AddProvider ( ZLAbstractLinkedFinalizable& provider ) {
	this->mProviders.insert ( &provider );
}

//----------------------------------------------------------------//
ZLAbstractLinkedFinalizable& ZLLinkedFinalizableMembership::GetOwner () {
	assert ( this->mOwner );
	return *this->mOwner;
}

//----------------------------------------------------------------//
void ZLLinkedFinalizableMembership::RemoveClient ( ZLAbstractFinalizable& client ) {
	this->mClients.erase ( &client );
}

//----------------------------------------------------------------//
void ZLLinkedFinalizableMembership::RemoveProvider ( ZLAbstractLinkedFinalizable& provider ) {
	this->mProviders.erase ( &provider );
}


//----------------------------------------------------------------//
ZLLinkedFinalizableMembership::ZLLinkedFinalizableMembership () :
	mOwner ( NULL ) {
}
//----------------------------------------------------------------//
ZLLinkedFinalizableMembership::ZLLinkedFinalizableMembership ( ZLAbstractLinkedFinalizable& owner ) :
	mOwner ( &owner ) {
}

//----------------------------------------------------------------//
ZLLinkedFinalizableMembership::~ZLLinkedFinalizableMembership () {

	// finalize all our clients; they will remove themselves fromt this membership
	// when their own memberships are destroyed.
	while ( this->mClients.size ()) {
		( *this->mClients.begin ())->Finalize ();
	}

	// remove self from providers.
	STLSet < ZLAbstractLinkedFinalizable* >::iterator providerIt = this->mProviders.begin ();
	for ( ; providerIt != this->mProviders.end (); ++providerIt ) {
		ZLAbstractLinkedFinalizable* provider = *providerIt;
		provider->GetMembership ()->RemoveClient ( *this->mOwner );
	}
}
