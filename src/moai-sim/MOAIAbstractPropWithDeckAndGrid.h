// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTPROPWITHDECKANDGRID_H
#define	MOAIABSTRACTPROPWITHDECKANDGRID_H

#include <moai-sim/MOAIAbstractProp.h>
#include <moai-sim/MOAIHasDeckAndGrid.h>

//================================================================//
// MOAIAbstractPropWithDeckAndGrid
//================================================================//
// TODO: doxygen
class MOAIAbstractPropWithDeckAndGrid :
	public virtual MOAIAbstractProp,
	public virtual MOAIHasDeckAndGrid {
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
						MOAIAbstractPropWithDeckAndGrid			();
						~MOAIAbstractPropWithDeckAndGrid		();
};

#endif
