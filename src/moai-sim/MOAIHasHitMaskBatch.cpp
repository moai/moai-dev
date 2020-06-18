// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIHasHitMaskBatch.h>

//================================================================//
// MOAIHasHitMaskBatch
//================================================================//

//----------------------------------------------------------------//
MOAIHasHitMaskBatch::MOAIHasHitMaskBatch () {

	RTTI_BEGIN ( MOAIHasHitMaskBatch )
		RTTI_EXTEND ( MOAIAbstractHasHitMaskBatch )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasHitMaskBatch::~MOAIHasHitMaskBatch () {
}

//----------------------------------------------------------------//
void MOAIHasHitMaskBatch::SetHitMaskBatch ( MOAIAbstractHitMaskBatch* hitMaskBatch ) {

	this->mHitMaskBatch = hitMaskBatch;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractHitMask& MOAIHasHitMaskBatch::MOAIAbstractHasHitMask_AffirmHitMask () {

	return this->AffirmHitMaskBatch ().AffirmHitMask ();
}

//----------------------------------------------------------------//
MOAIAbstractHitMaskBatch& MOAIHasHitMaskBatch::MOAIAbstractHasHitMaskBatch_AffirmHitMaskBatch () {

	if ( !this->mHitMaskBatch ) {
		this->mHitMaskBatch = new MOAIHitMaskBatch ();
	}
	return *this->mHitMaskBatch;
}
