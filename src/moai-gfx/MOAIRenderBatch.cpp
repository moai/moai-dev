// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractDrawable.h>
#include <moai-gfx/MOAIColor.h>
#include <moai-gfx/MOAIGfxConsts.h>
#include <moai-gfx/MOAIRenderBatch.h>

//================================================================//
// MOAIRenderBatch
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderBatch::DrawRenderRoot () {

	if ( this->mRenderRoot ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mRenderRoot );
		MOAIAbstractDrawable::Draw ( state, -1 );
	}
}

//----------------------------------------------------------------//
MOAIRenderBatch::MOAIRenderBatch () {
}

//----------------------------------------------------------------//
MOAIRenderBatch::~MOAIRenderBatch () {
}

//----------------------------------------------------------------//
void MOAIRenderBatch::Render () {

	this->MOAIRenderBatch_Render ();
}
