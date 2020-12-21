// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSPROP_H
#define	MOAIGRAPHICSPROP_H

#include <moai-sim/MOAIPropWithDeckAndIndex.h>
#include <moai-sim/MOAIAbstractGraphicsProp.h>

//================================================================//
// MOAIGraphicsProp
//================================================================//
// TODO: doxygen
class MOAIGraphicsProp :
	public virtual MOAIPropWithDeckAndIndex,
	public virtual MOAIAbstractGraphicsProp {
protected:

	//----------------------------------------------------------------//
	bool					MOAIAbstractRenderNode_LoadGfxState		( MOAIRenderPhaseEnum::_ renderPhase );
	bool					MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void					MOAINode_Update							();

public:

	DECL_LUA_FACTORY ( MOAIGraphicsProp )

	//----------------------------------------------------------------//
							MOAIGraphicsProp			();
	virtual					~MOAIGraphicsProp			();
};

#endif
