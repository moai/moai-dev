// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSGRIDPROP_H
#define	MOAIGRAPHICSGRIDPROP_H

#include <moai-sim/MOAIGridPropBase.h>
#include <moai-sim/MOAIGraphicsPropBase.h>

class MOAICellCoord;
class MOAICollisionShape;
class MOAIDeck;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIMaterialBatch;
class MOAIOverlapPrim2D;
class MOAIPartition;
class MOAIPartitionCell;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAIScissorRect;
class MOAIShader;
class MOAISurfaceSampler2D;
class MOAISingleTexture;

//================================================================//
// MOAIGraphicsGridProp
//================================================================//
// TODO: doxygen
class MOAIGraphicsGridProp :
	public MOAIGridPropBase,
	public MOAIGraphicsPropBase {
private:

	//----------------------------------------------------------------//
	ZLAffine3D		AppendRot90SclTr						( const ZLAffine3D& mtx, const ZLAffine3D& append );
	ZLAffine3D		AppendSclTr								( const ZLAffine3D& mtx, const ZLAffine3D& append );
	void			DrawGrid								( const MOAICellCoord &c0, const MOAICellCoord &c1 );

	//----------------------------------------------------------------//
	void			MOAIDrawable_Draw						( int subPrimID );
	bool			MOAINode_ApplyAttrOp					( u32 attrID, MOAIAttribute& attr, u32 op );
	void			MOAINode_Update							();
	void			MOAIPartitionHull_AddToSortBuffer		( MOAIPartitionResultBuffer& buffer, u32 key = 0 );
	u32				MOAIPartitionHull_GetModelBounds		( ZLBox& bounds ); // get the prop bounds in model space

public:

	DECL_LUA_FACTORY ( MOAIGraphicsGridProp )

	//----------------------------------------------------------------//
					MOAIGraphicsGridProp		();
	virtual			~MOAIGraphicsGridProp		();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
