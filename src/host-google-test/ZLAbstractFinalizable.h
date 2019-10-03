// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTFINALIZABLE_H
#define	ZLABSTRACTFINALIZABLE_H

#include <moai-sim/headers.h>
#include "ZLAbstractFinalizable_HasInternal.h"
#include "ZLAbstractFinalizable_Internal.h"

#define IMPLEMENT_FINALIZABLE(TYPE,...)						\
															\
	void ZLAbstractFinalizable_Finalize ( void* self ) {	\
		(( TYPE* )self )->~TYPE ();							\
		new ( this ) TYPE ( __VA_ARGS__ );					\
	}														\
															\
	void* ZLAbstractFinalizable_GetSelf () {				\
		( void )static_cast < TYPE* >( this );				\
		return this;										\
	}														\
															\
	template < typename PROVIDER_TYPE >						\
	PROVIDER_TYPE& GetProvider () {							\
		return this->ZLAbstractFinalizable_HasDependency < PROVIDER_TYPE >::GetProvider ();	\
	}

class ZLAbstractFinalizable_Internal;

//================================================================//
// ZLAbstractFinalizable
//================================================================//
class ZLAbstractFinalizable :
	public virtual ZLAbstractFinalizable_HasInternal {
private:
	
	ZLAbstractFinalizable_Internal*		mMembership;
	
	//----------------------------------------------------------------//
	virtual void						ZLAbstractFinalizable_Finalize							( void* self ) = 0;
	virtual void*						ZLAbstractFinalizable_GetSelf							() = 0;
	ZLAbstractFinalizable_Internal&		ZLAbstractFinalizable_HasInternal_AffirmMembership		();
	ZLAbstractFinalizable_Internal*		ZLAbstractFinalizable_HasInternal_GetMembership			();

protected:

	enum ReconstructionFlag {
		RECONSTRUCTING,
	};

public:

	//----------------------------------------------------------------//
						ZLAbstractFinalizable			();
	virtual				~ZLAbstractFinalizable			();
	void				Finalize						();
};

//================================================================//
// ZLAbstractFinalizable_HasDependency
//================================================================//
template < typename PROVIDER_TYPE >
class ZLAbstractFinalizable_HasDependency :
	public virtual ZLAbstractFinalizable_HasInternal {
protected:

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
		
		ZLAbstractFinalizable_Internal& clientMembership	= this->AffirmMembership ();
		ZLAbstractFinalizable_Internal& providerMembership	= provider.AffirmMembership ();
		
		clientMembership.AddProvider ( provider );
		providerMembership.AddClient ( clientMembership.GetOwner ());
		
		this->mProvider = &provider;
	}

	//----------------------------------------------------------------//
	ZLAbstractFinalizable_HasDependency () :
		mProvider ( NULL ) {
		( void )static_cast < ZLAbstractFinalizable* >(( PROVIDER_TYPE* )NULL );
	}

	//----------------------------------------------------------------//
	~ZLAbstractFinalizable_HasDependency () {
		
		this->mProvider = NULL;
	}
};

#endif
