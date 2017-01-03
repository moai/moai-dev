// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSGRIDPROP_H
#define	MOAIGRAPHICSGRIDPROP_H

#include <moai-sim/MOAIGridPropBase.h>
#include <moai-sim/MOAIGraphicsPropBase.h>

class MOAICellCoord;
class MOAICollisionShape;
class MOAIDeck;
class MOAIDeckRemapper;
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
	bool					ApplyAttrOp					( u32 attrID, MOAIAttribute& attr, u32 op );

	//----------------------------------------------------------------//
	void					MOAIAbstractDrawable_Draw					( int subPrimID, float lod );
	void					MOAIPartitionHull_AddToSortBuffer			( MOAIPartitionResultBuffer& buffer, u32 key = 0 );
	u32						MOAIPartitionHull_GetModelBounds			( ZLBox& bounds ); // get the prop bounds in model space

public:

	DECL_LUA_FACTORY ( MOAIGraphicsGridProp )

	//----------------------------------------------------------------//
							MOAIGraphicsGridProp		();
	virtual					~MOAIGraphicsGridProp		();
	void					OnDepNodeUpdate				();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
