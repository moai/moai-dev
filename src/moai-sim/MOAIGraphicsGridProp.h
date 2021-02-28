// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSGRIDPROP_H
#define	MOAIGRAPHICSGRIDPROP_H

#include <moai-sim/MOAIPropWithDeckAndGrid.h>
#include <moai-sim/MOAIAbstractGraphicsProp.h>

//================================================================//
// MOAIGraphicsGridProp
//================================================================//
// TODO: doxygen
class MOAIGraphicsGridProp :
	public virtual MOAIPropWithDeckAndGrid,
	public virtual MOAIAbstractGraphicsProp {
private:

	//----------------------------------------------------------------//
	bool					MOAIAbstractRenderNode_LoadGfxState		( MOAIRenderPhaseEnum::_ renderPhase );
	bool					MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void					MOAINode_Update							();

public:

	DECL_LUA_FACTORY ( MOAIGraphicsGridProp )

	//----------------------------------------------------------------//
							MOAIGraphicsGridProp					( ZLContext& context );
	virtual					~MOAIGraphicsGridProp					();
};

#endif
