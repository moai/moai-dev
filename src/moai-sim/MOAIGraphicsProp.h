// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSPROP_H
#define	MOAIGRAPHICSPROP_H

#include <moai-sim/MOAIAbstractPropWithDeckAndIndex.h>
#include <moai-sim/MOAIAbstractGraphicsProp.h>

//================================================================//
// MOAIGraphicsProp
//================================================================//
// TODO: doxygen
class MOAIGraphicsProp :
	public virtual MOAIAbstractPropWithDeckAndIndex,
	public virtual MOAIAbstractGraphicsProp {
protected:

	//----------------------------------------------------------------//
	bool					MOAIAbstractRenderNode_LoadGfxState		( u32 renderPhase );
	void					MOAIAbstractRenderNode_Render			( u32 renderPhase );
	bool					MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void					MOAINode_Update							();

public:

	DECL_LUA_FACTORY ( MOAIGraphicsProp )

	//----------------------------------------------------------------//
							MOAIGraphicsProp			();
	virtual					~MOAIGraphicsProp			();
};

#endif
