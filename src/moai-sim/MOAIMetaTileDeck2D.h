// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMETATILE2D_H
#define	MOAIMETATILE2D_H

//================================================================//
// MOAIMetaTile
//================================================================//
class MOAIMetaTile {
private:

	friend class MOAIMetaTileDeck2D;
	
	MOAICellCoord	mMin;
	MOAICellCoord	mMax;
	ZLVec2D			mOffset;

public:

	//----------------------------------------------------------------//
			MOAIMetaTile		();
};

//================================================================//
// MOAIMetaTileDeck2D
//================================================================//
/**	@lua	MOAIMetaTileDeck2D
	@text	This deck renders 'metatiles' which are sampled from a tile map.
			The tile map is specified by the attached grid and deck.
			Each 'brush' defines a rectangle of tiles to draw and an offset.
*/
class MOAIMetaTileDeck2D :
	public MOAIDeck {
private:
	
	ZLLeanArray < MOAIMetaTile > mBrushes;
	
	MOAILuaSharedPtr < MOAIGrid > mGrid;
	MOAILuaSharedPtr < MOAIDeck > mDeck;
	
	//----------------------------------------------------------------//
	static int		_reserveMetaTiles		( lua_State* L );
	static int		_setDeck				( lua_State* L );
	static int		_setGrid				( lua_State* L );
	static int		_setMetaTile			( lua_State* L );

	//----------------------------------------------------------------//
	ZLBounds				MOAIDeck_ComputeMaxAABB			();
	void					MOAIDeck_Draw					( ZLIndex idx );
	ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( ZLIndex idx );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );

public:
	
	DECL_LUA_FACTORY ( MOAIMetaTileDeck2D )
	
	//using MOAIDeck::DrawIndex;
	
	//----------------------------------------------------------------//
							MOAIMetaTileDeck2D		();
							~MOAIMetaTileDeck2D		();
	void					MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void					MOAILuaObject_SerializeIn				( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void					MOAILuaObject_SerializeOut			( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );
};

#endif
