// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHITMASKBATCH_H
#define	MOAIHITMASKBATCH_H

#include <moai-sim/MOAIAbstractHasHitMaskBatch.h>
#include <moai-sim/MOAIAbstractHitMaskBatch.h>

//================================================================//
// MOAIHitMaskBatch
//================================================================//
class MOAIHitMaskBatch :
	public virtual MOAIAbstractHasHitMaskBatch,
	public virtual MOAIAbstractHitMaskBatch {
protected:

	ZLLeanArray < ZLStrongPtr < MOAIAbstractHitMask > >	mHitMasks;

	//----------------------------------------------------------------//
	MOAIAbstractHitMaskBatch&		MOAIAbstractHasHitMaskBatch_AffirmHitMaskBatch		();
	MOAIAbstractHitMask&			MOAIAbstractHitMaskBatch_AffirmHitMask				( ZLIndex index );
	MOAIAbstractHitMaskBatch&		MOAIAbstractHitMaskBatch_AffirmHitMaskBatch			();
	MOAIAbstractHitMask*			MOAIAbstractHitMaskBatch_GetHitMask					( ZLIndex index );
	void							MOAIAbstractHitMaskBatch_ReserveHitMasks			( ZLSize size );
	void							MOAIAbstractHitMaskBatch_SetHitMask					( ZLIndex index, MOAIAbstractHitMask* hitMask );

public:

	DECL_LUA_FACTORY ( MOAIHitMaskBatch )

	//----------------------------------------------------------------//
							MOAIHitMaskBatch		();
							~MOAIHitMaskBatch		();
};

#endif
