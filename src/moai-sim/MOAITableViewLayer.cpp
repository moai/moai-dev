// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAITableViewLayer.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>

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
void MOAITableViewLayer::MOAIAbstractViewLayer_Render ( u32 renderPhase ) {
	
	MOAIRenderNode::MOAIAbstractRenderNode_RenderInner ( renderPhase );
	
//	if ( MOAIDebugLinesMgr::Get ().IsVisible () && this->mShowDebugLines ) {
//		MOAIRenderNode::Draw ( this->mRenderTable, true );
//	}
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAIAbstractRenderNode_RenderInner ( u32 renderPhase ) {

	MOAIAbstractLayer::MOAIAbstractRenderNode_RenderInner ( renderPhase );
}
