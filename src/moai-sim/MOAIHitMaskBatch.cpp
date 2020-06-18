// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIHitMaskBatch.h>

//================================================================//
// MOAIHitMaskBatch
//================================================================//

//----------------------------------------------------------------//
MOAIHitMaskBatch::MOAIHitMaskBatch () {

	RTTI_BEGIN ( MOAIHitMaskBatch )
		RTTI_EXTEND ( MOAIAbstractHasHitMaskBatch )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHitMaskBatch::~MOAIHitMaskBatch () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractHitMask& MOAIHitMaskBatch::MOAIAbstractHasHitMask_AffirmHitMask () {

	this->mHitMasks.Grow ( 1 );
	if ( !this->mHitMasks [ 0 ]) {
		this->mHitMasks [ 0 ] = new MOAIHitMask ();
	}
	return *this->mHitMasks [ 0 ];
}

//----------------------------------------------------------------//
MOAIAbstractHitMaskBatch& MOAIHitMaskBatch::MOAIAbstractHasHitMaskBatch_AffirmHitMaskBatch () {

	return *this;
}

//----------------------------------------------------------------//
MOAIAbstractHitMask* MOAIHitMaskBatch::MOAIAbstractHitMaskBatch_GetHitMask ( ZLIndex index ) {

	if ( index < this->mHitMasks.Size ()) {
		return this->mHitMasks [ index ];
	}
	return NULL;
}

//----------------------------------------------------------------//
void MOAIHitMaskBatch::MOAIAbstractHitMaskBatch_ReserveHitMasks ( ZLSize size ) {

	this->mHitMasks.Init ( size );
}

//----------------------------------------------------------------//
void MOAIHitMaskBatch::MOAIAbstractHitMaskBatch_SetHitMask ( ZLIndex index, MOAIAbstractHitMask* hitMask ) {

	this->mHitMasks.Grow ( index + 1 );
	this->mHitMasks [ index ] = hitMask;
}
