// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTFINALIZABLE_INTERNAL_H
#define	ZLABSTRACTFINALIZABLE_INTERNAL_H

class ZLAbstractFinalizable;

//================================================================//
// ZLAbstractFinalizable_Internal
//================================================================//
class ZLAbstractFinalizable_Internal {
private:

	friend class ZLAbstractFinalizable;
	template < typename DEPENDENCY_TYPE > friend class ZLAbstractFinalizable_HasDependencyOn;

	ZLAbstractFinalizable*					mOwner;
	STLSet < ZLAbstractFinalizable* >		mClients;
	STLSet < ZLAbstractFinalizable* >		mDependencies;

	//----------------------------------------------------------------//
	void						AddClient							( ZLAbstractFinalizable& client );
	void						AddDependency						( ZLAbstractFinalizable& dependency );
	ZLAbstractFinalizable&		GetOwner							();
	void						RemoveClient						( ZLAbstractFinalizable& client );
	void						RemoveDependency					( ZLAbstractFinalizable& dependency );
								ZLAbstractFinalizable_Internal		();
								ZLAbstractFinalizable_Internal		( ZLAbstractFinalizable& owner );
								~ZLAbstractFinalizable_Internal		();
};

#endif
