// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTFINALIZABLE_HASINTERNAL_H
#define	ZLABSTRACTFINALIZABLE_HASINTERNAL_H

class ZLAbstractFinalizable_Internal;

//================================================================//
// ZLAbstractFinalizable_HasInternal
//================================================================//
class ZLAbstractFinalizable_HasInternal {
private:
	
	friend class ZLAbstractFinalizable;
	friend class ZLAbstractFinalizable_Internal;
	template < typename DEPENDENCY_TYPE > friend class ZLAbstractFinalizable_HasDependencyOn;

	//----------------------------------------------------------------//
	ZLAbstractFinalizable_Internal& AffirmInternal () {
		return this->ZLAbstractFinalizable_HasInternal_AffirmInternal ();
	}
	
	//----------------------------------------------------------------//
	ZLAbstractFinalizable_Internal* GetInternal () {
		return this->ZLAbstractFinalizable_HasInternal_GetInternal ();
	}
	
	//----------------------------------------------------------------//
	virtual ZLAbstractFinalizable_Internal&		ZLAbstractFinalizable_HasInternal_AffirmInternal		() = 0;
	virtual ZLAbstractFinalizable_Internal*		ZLAbstractFinalizable_HasInternal_GetInternal			() = 0;
	
public:

	//----------------------------------------------------------------//
	ZLAbstractFinalizable_HasInternal () {
	}

	//----------------------------------------------------------------//
	virtual ~ZLAbstractFinalizable_HasInternal () {
	}
};

#endif
