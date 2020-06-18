// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHASHITMASKBATCH_H
#define	MOAIHASHITMASKBATCH_H

#include <moai-sim/MOAIAbstractHasHitMaskBatch.h>

//================================================================//
// MOAIHasHitMaskBatch
//================================================================//
class MOAIHasHitMaskBatch :
	public virtual MOAIAbstractHasHitMaskBatch {
protected:

	ZLStrongPtr < MOAIAbstractHitMaskBatch >	mHitMaskBatch;

	//----------------------------------------------------------------//
	MOAIAbstractHitMask&			MOAIAbstractHasHitMask_AffirmHitMask				();
	MOAIAbstractHitMaskBatch&		MOAIAbstractHasHitMaskBatch_AffirmHitMaskBatch		();

public:

	//----------------------------------------------------------------//
					MOAIHasHitMaskBatch			();
					~MOAIHasHitMaskBatch		();
	void			SetHitMaskBatch				( MOAIAbstractHitMaskBatch* hitMaskBatch );
};

#endif
