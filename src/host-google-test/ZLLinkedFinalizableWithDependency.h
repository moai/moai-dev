// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLINKEDFINALIZABLEWITHDEPENDENCY_H
#define	ZLLINKEDFINALIZABLEWITHDEPENDENCY_H

#include <moai-sim/headers.h>

#include "ZLLinkedFinalizableMembership.h"
#include "ZLAbstractLinkedFinalizable.h"

//================================================================//
// ZLLinkedFinalizableWithDependency
//================================================================//
template < typename PROVIDER_TYPE >
class ZLLinkedFinalizableWithDependency :
	public virtual ZLAbstractHasLinkedFinalizableMembership {
protected:

	friend class AbstractFinalizeableDAG;
	friend class ZLLinkedFinalizableProviderGetter;

	PROVIDER_TYPE*		mProvider;

	//----------------------------------------------------------------//
	PROVIDER_TYPE& GetProvider () {
		assert ( this->mProvider );
		return *this->mProvider;
	}

public:

	//----------------------------------------------------------------//
	void SetProvider ( PROVIDER_TYPE& provider ) {
		
		assert ( !this->mProvider );
		
		ZLLinkedFinalizableMembership& clientMembership		= this->AffirmMembership ();
		ZLLinkedFinalizableMembership& providerMembership	= provider.AffirmMembership ();
		
		clientMembership.AddProvider ( provider );
		providerMembership.AddClient ( clientMembership.GetOwner ());
		
		this->mProvider = &provider;
	}

	//----------------------------------------------------------------//
	ZLLinkedFinalizableWithDependency () :
		mProvider ( NULL ) {
		( void )static_cast < ZLAbstractLinkedFinalizable* >(( PROVIDER_TYPE* )NULL );
	}

	//----------------------------------------------------------------//
	~ZLLinkedFinalizableWithDependency () {
		
		this->mProvider = NULL;
	}
};

#endif
