// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAITableViewLayer.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>

//================================================================//
// lua
//==========================================================s======//

//----------------------------------------------------------------//

//================================================================//
// MOAITableViewLayer
//================================================================//

//----------------------------------------------------------------//
MOAITableViewLayer::MOAITableViewLayer () {
	
	RTTI_BEGIN ( MOAITableViewLayer )
		RTTI_EXTEND ( MOAIAbstractViewLayer )
		RTTI_EXTEND ( MOAIRenderNode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITableViewLayer::~MOAITableViewLayer () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAITableViewLayer::MOAIAbstractRenderNode_LoadGfxState ( MOAIRenderPhaseEnum::_ renderPhase ) {

	if ( !this->MOAIAbstractViewLayer::MOAIAbstractRenderNode_LoadGfxState ( renderPhase )) return false;
	if ( !this->MOAIRenderNode::MOAIAbstractRenderNode_LoadGfxState ( renderPhase )) return false;
	
	return true;
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAIAbstractRenderNode_Render ( MOAIRenderPhaseEnum::_ renderPhase ) {

	MOAIRenderNode::MOAIAbstractRenderNode_Render ( renderPhase );
	
	if ( MOAIDebugLinesMgr::Get ().IsVisible () && this->mShowDebugLines ) {
		MOAIRenderNode::MOAIAbstractRenderNode_Render ( MOAIRenderPhaseEnum::RENDER_PHASE_DRAW_DEBUG );
	}
}
