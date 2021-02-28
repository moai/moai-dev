// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIHitMaskBatch.h>

//================================================================//
// MOAIHitMaskBatch
//================================================================//

//----------------------------------------------------------------//
MOAIHitMaskBatch::MOAIHitMaskBatch ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIAbstractHasHitMask ( context ),
	MOAIAbstractHasHitMaskBatch ( context ),
	MOAIAbstractHitMaskBatch ( context ) {

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
MOAIAbstractHitMask& MOAIHitMaskBatch::MOAIAbstractHitMaskBatch_AffirmHitMask ( ZLIndex index ) {

	this->mHitMasks.Grow (( ZLSize )index + 1 );
	MOAIAbstractHitMask* hitMask = this->mHitMasks [ index ];
	
	if ( !hitMask ) {
		hitMask = new MOAIHitMask ( this->GetContext ());
		this->mHitMasks [ index ] = hitMask;
	}
	return *hitMask;
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
