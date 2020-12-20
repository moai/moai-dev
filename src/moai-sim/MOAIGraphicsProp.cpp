// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAISurfaceSampler2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIGraphicsProp
//================================================================//

//----------------------------------------------------------------//
MOAIGraphicsProp::MOAIGraphicsProp () {
	
	RTTI_BEGIN ( MOAIGraphicsProp )
		RTTI_EXTEND ( MOAIPropWithDeckAndIndex )
		RTTI_EXTEND ( MOAIAbstractGraphicsProp )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGraphicsProp::~MOAIGraphicsProp () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIGraphicsProp::MOAIAbstractRenderNode_LoadGfxState ( u32 renderPhase ) {
	if ( renderPhase == MOAIAbstractRenderNode::RENDER_PHASE_DRAW_DEBUG ) return true;

	if ( this->mDeck && MOAIAbstractGraphicsProp::MOAIAbstractRenderNode_LoadGfxState ( renderPhase )) {
	
		this->LoadVertexTransform ();
		this->LoadUVTransform ();
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIGraphicsProp::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( MOAIPropWithDeckAndIndex::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIAbstractGraphicsProp::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAINode_Update () {
	
	MOAIAbstractGraphicsProp::MOAINode_Update ();
}
