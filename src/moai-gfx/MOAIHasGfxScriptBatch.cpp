// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxScriptBatch.h>
#include <moai-gfx/MOAIHasGfxScriptBatch.h>

//================================================================//
// MOAIHasGfxScriptBatch
//================================================================//

//----------------------------------------------------------------//
MOAIHasGfxScriptBatch::MOAIHasGfxScriptBatch () {

	RTTI_BEGIN ( MOAIHasGfxScriptBatch )
		RTTI_EXTEND ( MOAIAbstractHasGfxScriptBatch )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasGfxScriptBatch::~MOAIHasGfxScriptBatch () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxScriptBatch& MOAIHasGfxScriptBatch::MOAIAbstractHasGfxScriptBatch_AffirmGfxScriptBatch () {

	if ( !this->mGfxScriptBatch ) {
		this->mGfxScriptBatch = new MOAIGfxScriptBatch ();
	}
	return *this->mGfxScriptBatch;
}

