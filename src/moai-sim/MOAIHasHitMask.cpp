// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIHitMask.h>
#include <moai-sim/MOAIHasHitMask.h>

//================================================================//
// MOAIHasHitMask
//================================================================//

//----------------------------------------------------------------//
MOAIHasHitMask::MOAIHasHitMask () {

	RTTI_BEGIN ( MOAIHasHitMask )
		RTTI_EXTEND ( MOAIAbstractHasHitMask )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasHitMask::~MOAIHasHitMask () {
}

//----------------------------------------------------------------//
void MOAIHasHitMask::SetHitMash ( MOAIAbstractHitMask* hitMask ) {

	this->mHitMask = hitMask;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractHitMask& MOAIHasHitMask::MOAIAbstractHasHitMask_AffirmHitMask () {

	if ( !this->mHitMask ) {
		this->mHitMask = new MOAIHitMask ();
	}
	return *this->mHitMask;
}
