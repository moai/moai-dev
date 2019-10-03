// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLLINKEDFINALIZABLEMEMBERSHIP_H
#define	ZLLINKEDFINALIZABLEMEMBERSHIP_H

#include <moai-sim/headers.h>

class ZLAbstractFinalizable;
class ZLAbstractLinkedFinalizable;

//================================================================//
// ZLLinkedFinalizableMembership
//================================================================//
class ZLLinkedFinalizableMembership {
private:

	ZLAbstractLinkedFinalizable*				mOwner;
	STLSet < ZLAbstractFinalizable* >			mClients;
	STLSet < ZLAbstractLinkedFinalizable* >		mProviders;

public:

	//----------------------------------------------------------------//
	void								AddClient							( ZLAbstractFinalizable& client );
	void								AddProvider							( ZLAbstractLinkedFinalizable& provider );
	ZLAbstractLinkedFinalizable&		GetOwner							();
	void								RemoveClient						( ZLAbstractFinalizable& client );
	void								RemoveProvider						( ZLAbstractLinkedFinalizable& provider );
										ZLLinkedFinalizableMembership		();
										ZLLinkedFinalizableMembership		( ZLAbstractLinkedFinalizable& owner );
										~ZLLinkedFinalizableMembership		();
};

#endif
