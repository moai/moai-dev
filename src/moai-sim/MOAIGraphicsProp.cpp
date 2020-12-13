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
		RTTI_EXTEND ( MOAIHasDeckAndIndex )
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
bool MOAIGraphicsProp::MOAIAbstractGraphicsProp_LoadGfxState () {

	if ( this->mDeck && MOAIAbstractGraphicsProp::MOAIAbstractGraphicsProp_LoadGfxState ()) {
	
		this->LoadVertexTransform ();
		this->LoadUVTransform ();
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAIAbstractGraphicsProp_Render ( u32 renderPhase ) {
	UNUSED ( renderPhase );

	this->mDeck->Draw ( this->mIndex );
}

//----------------------------------------------------------------//
bool MOAIGraphicsProp::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( MOAIHasDeckAndIndex::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIAbstractGraphicsProp::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}

//----------------------------------------------------------------//
void MOAIGraphicsProp::MOAINode_Update () {
	
	MOAIAbstractGraphicsProp::MOAINode_Update ();
}
