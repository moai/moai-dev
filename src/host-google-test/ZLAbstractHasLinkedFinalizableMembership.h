// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTHASLINKEDFINALIZABLEMEMBERSHIP_H
#define	ZLABSTRACTHASLINKEDFINALIZABLEMEMBERSHIP_H

#include <moai-sim/headers.h>

class ZLLinkedFinalizableMembership;

//================================================================//
// ZLAbstractHasLinkedFinalizableMembership
//================================================================//
class ZLAbstractHasLinkedFinalizableMembership {
protected:
	
	//----------------------------------------------------------------//
	virtual ZLLinkedFinalizableMembership&		ZLAbstractHasLinkedFinalizableMembership_Affirm		() = 0;
	virtual ZLLinkedFinalizableMembership*		ZLAbstractHasLinkedFinalizableMembership_Get		() = 0;
	
public:

	//----------------------------------------------------------------//
	ZLLinkedFinalizableMembership& AffirmMembership () {
		return this->ZLAbstractHasLinkedFinalizableMembership_Affirm ();
	}

	//----------------------------------------------------------------//
	ZLLinkedFinalizableMembership* GetMembership () {
		return this->ZLAbstractHasLinkedFinalizableMembership_Get ();
	}

	//----------------------------------------------------------------//
	ZLAbstractHasLinkedFinalizableMembership () {
	}
	
	//----------------------------------------------------------------//
	virtual ~ZLAbstractHasLinkedFinalizableMembership () {
	}
};

#endif
