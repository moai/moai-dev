// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTFINALIZABLE_HASINTERNAL_H
#define	ZLABSTRACTFINALIZABLE_HASINTERNAL_H

#include <moai-sim/headers.h>

class ZLAbstractFinalizable_Internal;

//================================================================//
// ZLAbstractFinalizable_HasInternal
//================================================================//
class ZLAbstractFinalizable_HasInternal {
private:
	
	friend class ZLAbstractFinalizable;
	friend class ZLAbstractFinalizable_Internal;
	template < typename PROVIDER_TYPE > friend class ZLAbstractFinalizable_HasDependency;

	//----------------------------------------------------------------//
	ZLAbstractFinalizable_Internal& AffirmMembership () {
		return this->ZLAbstractFinalizable_HasInternal_AffirmMembership ();
	}
	
	//----------------------------------------------------------------//
	ZLAbstractFinalizable_Internal* GetMembership () {
		return this->ZLAbstractFinalizable_HasInternal_GetMembership ();
	}
	
	//----------------------------------------------------------------//
	virtual ZLAbstractFinalizable_Internal&		ZLAbstractFinalizable_HasInternal_AffirmMembership		() = 0;
	virtual ZLAbstractFinalizable_Internal*		ZLAbstractFinalizable_HasInternal_GetMembership			() = 0;
	
public:

	//----------------------------------------------------------------//
	ZLAbstractFinalizable_HasInternal () {
	}

	//----------------------------------------------------------------//
	virtual ~ZLAbstractFinalizable_HasInternal () {
	}
};

#endif
