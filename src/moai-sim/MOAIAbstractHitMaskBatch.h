// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTHITMASKBATCH_H
#define	MOAIABSTRACTHITMASKBATCH_H

class MOAIAbstractHitMask;

//================================================================//
// MOAIAbstractHitMaskBatch
//================================================================//
class MOAIAbstractHitMaskBatch :
	public virtual MOAIAbstractHasHitMask {
protected:

	friend class MOAIAbstractHasHitMaskBatch;

	//----------------------------------------------------------------//
	virtual MOAIAbstractHitMask&	MOAIAbstractHitMaskBatch_AffirmHitMask		( ZLIndex index ) = 0;
	virtual MOAIAbstractHitMask*	MOAIAbstractHitMaskBatch_GetHitMask			( ZLIndex index ) = 0;
	virtual void					MOAIAbstractHitMaskBatch_ReserveHitMasks	( ZLSize size ) = 0;
	virtual void					MOAIAbstractHitMaskBatch_SetHitMask			( ZLIndex index, MOAIAbstractHitMask* hitMask ) = 0;

public:

	//----------------------------------------------------------------//
					MOAIAbstractHitMaskBatch		( ZLContext& context );
					~MOAIAbstractHitMaskBatch		();
};

#endif
