// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractPropWithDeckAndIndex.h>

//================================================================//
// MOAIAbstractPropWithDeckAndIndex
//================================================================//
	
//----------------------------------------------------------------//
MOAIAbstractPropWithDeckAndIndex::MOAIAbstractPropWithDeckAndIndex () {
}

//----------------------------------------------------------------//
MOAIAbstractPropWithDeckAndIndex::~MOAIAbstractPropWithDeckAndIndex () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPropWithDeckAndIndex::MOAIAbstractPickable_PickByPoint ( ZLVec3D loc ) {

	return this->MOAIAbstractProp::MOAIAbstractPickable_PickByPoint ( loc );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPropWithDeckAndIndex::MOAIAbstractPickable_PickByRay ( ZLVec3D loc, ZLVec3D normal ) {

	return this->MOAIAbstractProp::MOAIAbstractPickable_PickByRay ( loc, normal );
}

//----------------------------------------------------------------//
ZLBounds MOAIAbstractPropWithDeckAndIndex::MOAIAbstractProp_GetModelBounds () {
	
	return this->mDeck ? this->mDeck->GetBounds ( this->mIndex ) : ZLBounds::EMPTY;
}


//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPropWithDeckAndIndex::MOAIAbstractProp_PickByPoint ( ZLVec3D loc ) {
	
	return this->MOAIHasDeckAndIndex::MOAIAbstractPickable_PickByPoint ( loc );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIAbstractPropWithDeckAndIndex::MOAIAbstractProp_PickByRay ( ZLVec3D loc, ZLVec3D normal ) {
	
	return this->MOAIHasDeckAndIndex::MOAIAbstractPickable_PickByRay ( loc, normal );
}

//----------------------------------------------------------------//
bool MOAIAbstractPropWithDeckAndIndex::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( MOAIHasDeckAndIndex::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	if ( MOAIAbstractProp::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}
