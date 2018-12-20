// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSGRIDPROP_H
#define	MOAIGRAPHICSGRIDPROP_H

#include <moai-sim/MOAIDeckHolderWithGrid.h>
#include <moai-sim/MOAIGraphicsPropBase.h>

//================================================================//
// MOAIGraphicsGridProp
//================================================================//
// TODO: doxygen
class MOAIGraphicsGridProp :
	public virtual MOAIDeckHolderWithGrid,
	public virtual MOAIGraphicsPropBase {
private:

	//----------------------------------------------------------------//
	ZLAffine3D		AppendRot90SclTr						( const ZLAffine3D& mtx, const ZLAffine3D& append );
	ZLAffine3D		AppendSclTr								( const ZLAffine3D& mtx, const ZLAffine3D& append );
	void			DrawGrid								( const MOAICellCoord &c0, const MOAICellCoord &c1 );

	//----------------------------------------------------------------//
	void			MOAIAbstractDrawable_Draw				( int subPrimID );
	ZLBounds		MOAIAbstractProp_GetModelBounds			(); // get the prop bounds in model space
	bool			MOAINode_ApplyAttrOp					( MOAIAttrID attrID, MOAIAttribute& attr, u32 op );
	void			MOAINode_Update							();
	void			MOAIPartitionHull_AddToSortBuffer		( MOAIPartitionResultBuffer& buffer, u32 key = 0 );

public:

	DECL_LUA_FACTORY ( MOAIGraphicsGridProp )

	//----------------------------------------------------------------//
	void			GetGridBoundsInView			( const ZLAffine3D& worldToLocalMtx, MOAICellCoord& c0, MOAICellCoord& c1 ); // TODO: this shoudln't be here
					MOAIGraphicsGridProp		();
	virtual			~MOAIGraphicsGridProp		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
