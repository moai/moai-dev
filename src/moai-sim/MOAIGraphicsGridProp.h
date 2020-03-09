// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSGRIDPROP_H
#define	MOAIGRAPHICSGRIDPROP_H

#include <moai-sim/MOAIHasDeckAndGrid.h>
#include <moai-sim/MOAIGraphicsPropBase.h>

//================================================================//
// MOAIGraphicsGridProp
//================================================================//
// TODO: doxygen
class MOAIGraphicsGridProp :
	public virtual MOAIHasDeckAndGrid,
	public virtual MOAIGraphicsPropBase {
private:

	//----------------------------------------------------------------//
	ZLAffine3D		AppendRot90SclTr						( const ZLAffine3D& mtx, const ZLAffine3D& append );
	ZLAffine3D		AppendSclTr								( const ZLAffine3D& mtx, const ZLAffine3D& append );
	void			DrawGrid								( const MOAICellCoord &c0, const MOAICellCoord &c1 );

	//----------------------------------------------------------------//
	ZLBounds		MOAIAbstractProp_GetModelBounds			(); // get the prop bounds in model space
	void			MOAIDrawable_Draw						( int subPrimID );
	void			MOAILuaObject_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_SerializeIn				( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			MOAILuaObject_SerializeOut				( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	bool			MOAINode_ApplyAttrOp					( ZLAttrID attrID, ZLAttribute& attr, u32 op );
	void			MOAINode_Update							();
	void			MOAIPartitionHull_AddToSortBuffer		( MOAIPartitionResultBuffer& buffer, u32 key = 0 );

public:

	DECL_LUA_FACTORY ( MOAIGraphicsGridProp )

	//----------------------------------------------------------------//
	void			GetGridFrameInView						( const ZLAffine3D& worldToLocalMtx, MOAICellCoord& c0, MOAICellCoord& c1 ); // TODO: this shoudln't be here
					MOAIGraphicsGridProp					();
	virtual			~MOAIGraphicsGridProp					();
};

#endif
