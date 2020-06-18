// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHITMASK_H
#define	MOAIHITMASK_H

#include <moai-sim/MOAIAbstractHasHitMask.h>
#include <moai-sim/MOAIAbstractHitMask.h>

//================================================================//
// MOAIHitMask
//================================================================//
class MOAIHitMask :
	public virtual MOAIAbstractHasHitMask,
	public virtual MOAIAbstractHitMask {
protected:

	ZLStrongPtr < MOAIImage > mImage;

	u32				mHitColorScalar;
	u32				mHitColorThreshold;

	//----------------------------------------------------------------//
	MOAIAbstractHitMask&	MOAIAbstractHasHitMask_AffirmHitMask	();
	void					MOAIAbstractHitMask_SetImage			( MOAIImage* image );
	void					MOAIAbstractHitMask_SetScalar			( u32 scalar );
	void					MOAIAbstractHitMask_SetThreshold		( u32 threshold );
	bool					MOAIAbstractHitMask_TestHit				( float x, float y );
	bool					MOAIAbstractHitMask_TestHit				( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y );

public:

	DECL_LUA_FACTORY ( MOAIHitMask )

	//----------------------------------------------------------------//
					MOAIHitMask				();
					~MOAIHitMask			();
};

#endif
