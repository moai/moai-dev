// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIAbstractHitMaskBatch.h>

//================================================================//
// MOAIAbstractHitMaskBatch
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractHitMaskBatch::MOAIAbstractHitMaskBatch ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIAbstractHasHitMask ( context ) {
}

//----------------------------------------------------------------//
MOAIAbstractHitMaskBatch::~MOAIAbstractHitMaskBatch () {
}
