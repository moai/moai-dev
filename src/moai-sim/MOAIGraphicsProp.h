// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRAPHICSPROP_H
#define	MOAIGRAPHICSPROP_H

#include <moai-sim/MOAIIndexedPropBase.h>
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
// MOAIGraphicsProp
//================================================================//
// TODO: doxygen
class MOAIGraphicsProp :
	public MOAIIndexedPropBase,
	public MOAIGraphicsPropBase {
private:

	//----------------------------------------------------------------//
	void					MOAIAbstractDrawable_Draw				( int subPrimID, float lod );
	bool					MOAINode_ApplyAttrOp					( u32 attrID, MOAIAttribute& attr, u32 op );
	void					MOAINode_Update							();
	u32						MOAIPartitionHull_GetModelBounds		( ZLBox& bounds ); // get the prop bounds in model space
	bool					MOAIPartitionHull_Inside				( ZLVec3D vec, float pad );

public:

	DECL_LUA_FACTORY ( MOAIGraphicsProp )

	//----------------------------------------------------------------//
							MOAIGraphicsProp			();
	virtual					~MOAIGraphicsProp			();
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
