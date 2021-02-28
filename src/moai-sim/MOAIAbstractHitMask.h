// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTHITMASK_H
#define	MOAIABSTRACTHITMASK_H

#include <moai-sim/MOAIAbstractHasHitMask.h>

//================================================================//
// MOAIHitMask
//================================================================//
class MOAIAbstractHitMask :
	public virtual MOAIAbstractHasHitMask,
	public virtual MOAILuaObject {
protected:

	friend class MOAIAbstractHasHitMask;

	//----------------------------------------------------------------//
	virtual void	MOAIAbstractHitMask_SetImage		( MOAIImage* image ) = 0;
	virtual void	MOAIAbstractHitMask_SetScalar		( u32 scalar ) = 0;
	virtual void	MOAIAbstractHitMask_SetThreshold	( u32 threshold ) = 0;
	virtual bool	MOAIAbstractHitMask_TestHit			( float x, float y ) = 0;
	virtual bool	MOAIAbstractHitMask_TestHit			( const ZLQuad& modelQuad, const ZLQuad& uvQuad, float x, float y ) = 0;

public:

	//----------------------------------------------------------------//
	MOAIAbstractHitMask ( ZLContext& context ) {
	}

	//----------------------------------------------------------------//
	~MOAIAbstractHitMask () {
	}
};

#endif
