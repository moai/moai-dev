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
	
	ZLGridCoord	mMin;
	ZLGridCoord	mMax;
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
	void			_RegisterLuaClass				( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs				( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_SerializeIn					( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			_SerializeOut					( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	ZLBounds		MOAIDeck_GetBounds				();
	ZLBounds		MOAIDeck_GetBounds				( ZLIndex idx );
	void			MOAIDeck_Render					( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );

public:
	
	DECL_LUA_FACTORY ( MOAIMetaTileDeck2D )
		
	//----------------------------------------------------------------//
					MOAIMetaTileDeck2D		( ZLContext& context );
					~MOAIMetaTileDeck2D		();
};

#endif
