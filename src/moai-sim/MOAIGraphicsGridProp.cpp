// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIFancyGrid.h>
#include <moai-sim/MOAIGraphicsGridProp.h>
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
// MOAIGraphicsGridProp
//================================================================//

//----------------------------------------------------------------//
MOAIGraphicsGridProp::MOAIGraphicsGridProp () {
	
	RTTI_BEGIN ( MOAIGraphicsGridProp )
		RTTI_EXTEND ( MOAIPropWithDeckAndGrid )
		RTTI_EXTEND ( MOAIAbstractGraphicsProp )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGraphicsGridProp::~MOAIGraphicsGridProp () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIGraphicsGridProp::MOAIAbstractRenderNode_LoadGfxState ( u32 renderPhase ) {
	if ( renderPhase == MOAIAbstractRenderNode::RENDER_PHASE_DRAW_DEBUG ) return true;

	if ( this->mDeck && MOAIAbstractGraphicsProp::MOAIAbstractRenderNode_LoadGfxState ( renderPhase )) {
		this->LoadUVTransform ();
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIGraphicsGridProp::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( MOAIAbstractGraphicsProp::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsGridProp::MOAINode_Update () {
	
	MOAIAbstractGraphicsProp::MOAINode_Update ();
}
