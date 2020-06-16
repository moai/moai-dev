// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTPROPWITHDECKANDINDEX_H
#define	MOAIABSTRACTPROPWITHDECKANDINDEX_H

#include <moai-sim/MOAIAbstractProp.h>
#include <moai-sim/MOAIHasDeckAndIndex.h>

//================================================================//
// MOAIAbstractPropWithDeckAndIndex
//================================================================//
// TODO: doxygen
class MOAIAbstractPropWithDeckAndIndex :
	public virtual MOAIAbstractProp,
	public virtual MOAIHasDeckAndIndex {
protected:

	//----------------------------------------------------------------//
	MOAIPickResult		MOAIAbstractPickable_PickByPoint		( ZLVec3D loc );
	MOAIPickResult		MOAIAbstractPickable_PickByRay			( ZLVec3D loc, ZLVec3D normal );
	ZLBounds			MOAIAbstractProp_GetModelBounds			();
	MOAIPickResult		MOAIAbstractProp_PickByPoint			( ZLVec3D loc );
	MOAIPickResult		MOAIAbstractProp_PickByRay				( ZLVec3D loc, ZLVec3D normal );
	bool				MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );

public:

	//----------------------------------------------------------------//
						MOAIAbstractPropWithDeckAndIndex		();
						~MOAIAbstractPropWithDeckAndIndex		();
};

#endif
