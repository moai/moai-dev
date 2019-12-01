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
	template < typename PROVIDER_TYPE > friend class ZLAbstractFinalizable_HasDependencyOn;

	ZLAbstractFinalizable*					mOwner;
	STLSet < ZLAbstractFinalizable* >		mClients;
	STLSet < ZLAbstractFinalizable* >		mProviders;

	//----------------------------------------------------------------//
	void						AddClient							( ZLAbstractFinalizable& client );
	void						AddProvider							( ZLAbstractFinalizable& provider );
	ZLAbstractFinalizable&		GetOwner							();
	void						RemoveClient						( ZLAbstractFinalizable& client );
	void						RemoveProvider						( ZLAbstractFinalizable& provider );
								ZLAbstractFinalizable_Internal		();
								ZLAbstractFinalizable_Internal		( ZLAbstractFinalizable& owner );
								~ZLAbstractFinalizable_Internal		();
};

#endif
