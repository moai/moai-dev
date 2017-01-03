// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPROP_H
#define	MOAIPROP_H

#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAIRenderable.h>
#include <moai-sim/MOAITransform.h>
#include <moai-core/MOAILuaSharedPtr.h>

class MOAICellCoord;
class MOAICollisionProp;
class MOAIDeck;
class MOAIDeckRemapper;
class MOAIGrid;
class MOAILayoutFrame;
class MOAIOverlapPrim2D;
class MOAIPartition;
class MOAIPartitionCell;
class MOAIPartitionLevel;
class MOAIPartitionResultBuffer;
class MOAISurfaceSampler2D;

class MOAIProp;

//================================================================//
// MOAIProp
//================================================================//
/**	@lua	MOAIProp
	@text	Base class for props.
	
	@attr	ATTR_INDEX
	@attr	ATTR_PARTITION
*/
class MOAIProp :
	public MOAIPartitionHull {
private:
	
	//----------------------------------------------------------------//
	static int			_getDeck					( lua_State* L );
	static int			_getGrid					( lua_State* L );
	static int			_getIndex					( lua_State* L );
	static int			_getWorldBounds				( lua_State* L );
	static int			_getWorldBoundsCenter		( lua_State* L );
	static int			_setDeck					( lua_State* L );
	static int			_setGrid					( lua_State* L );
	static int			_setGridScale				( lua_State* L );
	static int			_setIndex					( lua_State* L );
	static int			_setRemapper				( lua_State* L );

	//----------------------------------------------------------------//
	void				MOAIPartitionHull_AddToSortBuffer			( MOAIPartitionResultBuffer& buffer, u32 key = 0 );
	u32					MOAIPartitionHull_GetModelBounds			( ZLBox& bounds ); // get the prop bounds in model space

protected:

	MOAILuaSharedPtr < MOAIDeck >			mDeck;
	MOAILuaSharedPtr < MOAIDeckRemapper >	mRemapper; // TODO: replace with inline remapper
	u32										mIndex;
	
	MOAILuaSharedPtr < MOAIGrid >			mGrid;
	ZLVec2D									mGridScale;

public:

	DECL_ATTR_HELPER ( MOAIProp )

	static const s32 UNKNOWN_PRIORITY	= 0x80000000;
	static const int NO_SUBPRIM_ID		= 0xffffffff;

	enum {
		ATTR_INDEX,
		TOTAL_ATTR,
	};

	GET_SET ( u32, Index, mIndex )
	
	GET ( MOAIDeck*, Deck, mDeck )
	GET ( MOAIDeckRemapper*, Remapper, mRemapper )

	//----------------------------------------------------------------//
	bool				ApplyAttrOp				( u32 attrID, MOAIAttribute& attr, u32 op );
	void				GetGridBoundsInView		( MOAICellCoord& c0, MOAICellCoord& c1 ); // TODO: this shoudln't be here
						MOAIProp				();
	virtual				~MOAIProp				();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
