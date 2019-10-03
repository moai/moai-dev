// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTLINKEDFINALIZABLE_H
#define	ZLABSTRACTLINKEDFINALIZABLE_H

#include <moai-sim/headers.h>
#include "ZLAbstractFinalizable.h"
#include "ZLAbstractHasLinkedFinalizableMembership.h"
#include "ZLLinkedFinalizableMembership.h"

#define IMPLEMENT_LINKED_FINALIZEABLE(TYPE,...)		\
													\
	IMPLEMENT_FINALIZEABLE ( TYPE, __VA_ARGS__ )	\
													\
	template < typename PROVIDER_TYPE >				\
	PROVIDER_TYPE& GetProvider () {					\
		return this->ZLLinkedFinalizableWithDependency < PROVIDER_TYPE >::GetProvider ();	\
	}

//================================================================//
// ZLAbstractLinkedFinalizable
//================================================================//
class ZLAbstractLinkedFinalizable :
	public virtual ZLAbstractFinalizable,
	public virtual ZLAbstractHasLinkedFinalizableMembership {
private:

	ZLLinkedFinalizableMembership*	mMembership;

	//----------------------------------------------------------------//
	ZLLinkedFinalizableMembership&		ZLAbstractHasLinkedFinalizableMembership_Affirm		();
	ZLLinkedFinalizableMembership*		ZLAbstractHasLinkedFinalizableMembership_Get		();

public:

	//----------------------------------------------------------------//
			ZLAbstractLinkedFinalizable			();
			~ZLAbstractLinkedFinalizable		();
};


#endif
