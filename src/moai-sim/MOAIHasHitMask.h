// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASHITMASK_H
#define	MOAIHASHITMASK_H

#include <moai-sim/MOAIAbstractHasHitMask.h>

//================================================================//
// MOAIHasHitMask
//================================================================//
class MOAIHasHitMask :
	public virtual MOAIAbstractHasHitMask {
protected:

	ZLStrongPtr < MOAIAbstractHitMask >	mHitMask;

	//----------------------------------------------------------------//
	MOAIAbstractHitMask&	MOAIAbstractHasHitMask_AffirmHitMask		();

public:

	//----------------------------------------------------------------//
					MOAIHasHitMask			();
					~MOAIHasHitMask			();
	void			SetHitMash				( MOAIAbstractHitMask* hitMask );
};

#endif
