// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSGRIDPROP_H
#define	MOAIGRAPHICSGRIDPROP_H

#include <moai-sim/MOAIAbstractPropWithDeckAndGrid.h>
#include <moai-sim/MOAIAbstractGraphicsProp.h>

//================================================================//
// MOAIGraphicsGridProp
//================================================================//
// TODO: doxygen
class MOAIGraphicsGridProp :
	public virtual MOAIAbstractPropWithDeckAndGrid,
	public virtual MOAIAbstractGraphicsProp {
private:

	//----------------------------------------------------------------//
	ZLAffine3D				AppendRot90SclTr						( const ZLAffine3D& mtx, const ZLAffine3D& append );
	ZLAffine3D				AppendSclTr								( const ZLAffine3D& mtx, const ZLAffine3D& append );
	void					DrawGrid								( const MOAICellCoord &c0, const MOAICellCoord &c1 );

	//----------------------------------------------------------------//
	bool					MOAIAbstractRenderNode_LoadGfxState		( u32 renderPhase );
	void					MOAIAbstractRenderNode_Render			( u32 renderPhase );
	bool					MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void					MOAINode_Update							();
	void					MOAIPartitionHull_AddToSortBuffer		( MOAIPartitionResultBuffer& buffer, u32 key = 0 );

public:

	DECL_LUA_FACTORY ( MOAIGraphicsGridProp )

	//----------------------------------------------------------------//
	void					GetGridFrameInView						( const ZLAffine3D& worldToLocalMtx, MOAICellCoord& c0, MOAICellCoord& c1 ); // TODO: this shoudln't be here
							MOAIGraphicsGridProp					();
	virtual					~MOAIGraphicsGridProp					();
};

#endif
