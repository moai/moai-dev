// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractPropWithDeckAndGrid.h>

//================================================================//
// MOAIAbstractPropWithDeckAndGrid
//================================================================//
	
//----------------------------------------------------------------//
MOAIAbstractPropWithDeckAndGrid::MOAIAbstractPropWithDeckAndGrid () {
}

//----------------------------------------------------------------//
MOAIAbstractPropWithDeckAndGrid::~MOAIAbstractPropWithDeckAndGrid () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPropWithDeckAndGrid::MOAIAbstractPickable_PickByPoint ( ZLVec3D loc ) {

	return this->MOAIAbstractProp::MOAIAbstractPickable_PickByPoint ( loc );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPropWithDeckAndGrid::MOAIAbstractPickable_PickByRay ( ZLVec3D loc, ZLVec3D normal ) {

	return this->MOAIAbstractProp::MOAIAbstractPickable_PickByRay ( loc, normal );
}

//----------------------------------------------------------------//
ZLBounds MOAIAbstractPropWithDeckAndGrid::MOAIAbstractProp_GetModelBounds () {
	
	if ( this->mGrid ) {
		if ( this->mGrid->GetRepeat ()) {
			return ZLBounds::GLOBAL;
		}
		return ZLBounds ( this->mGrid->GetFrame ());
	}
	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPropWithDeckAndGrid::MOAIAbstractProp_PickByPoint ( ZLVec3D loc ) {
	
	return this->MOAIHasDeckAndGrid::MOAIAbstractPickable_PickByPoint ( loc );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPropWithDeckAndGrid::MOAIAbstractProp_PickByRay ( ZLVec3D loc, ZLVec3D normal ) {
	
	return this->MOAIHasDeckAndGrid::MOAIAbstractPickable_PickByRay ( loc, normal );
}

//----------------------------------------------------------------//
bool MOAIAbstractPropWithDeckAndGrid::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( MOAIHasDeckAndGrid::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIAbstractProp::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}
