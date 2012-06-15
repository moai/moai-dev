// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRIDDECK2D_H
#define	MOAIGRIDDECK2D_H

#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIGridDeckBrush
//================================================================//
class MOAIGridDeckBrush {
private:

	friend class MOAIGridDeck2D;
	
	MOAICellCoord	mMin;
	MOAICellCoord	mMax;
	USVec2D			mOffset;

public:

	//----------------------------------------------------------------//
			MOAIGridDeckBrush		();
};

//================================================================//
// MOAIGridDeck2D
//================================================================//
/**	@name	MOAIGridDeck2D
	@text	This deck renders 'brushes' which are sampled from a tile map.
			The tile map is specified by the attached grid, deck and remapper.
			Each 'brush' defines a rectangle of tiles to draw and an offset.
*/
class MOAIGridDeck2D :
	public MOAIDeck {
private:
	
	USLeanArray < MOAIGridDeckBrush > mBrushes;
	
	MOAILuaSharedPtr < MOAIGrid > mGrid;
	MOAILuaSharedPtr < MOAIDeck > mDeck;
	MOAILuaSharedPtr < MOAIDeckRemapper > mRemapper;
	
	//----------------------------------------------------------------//
	static int		_reserveBrushes			( lua_State* L );
	static int		_setBrush				( lua_State* L );
	static int		_setDeck				( lua_State* L );
	static int		_setGrid				( lua_State* L );
	static int		_setRemapper			( lua_State* L );
	
	//----------------------------------------------------------------//
	USBox			ComputeMaxBounds		();
	USBox			GetItemBounds			( u32 idx );

public:
	
	DECL_LUA_FACTORY ( MOAIGridDeck2D )
	
	//----------------------------------------------------------------//
	void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	void			GetGfxState				( MOAIDeckGfxState& gfxState );
					MOAIGridDeck2D			();
					~MOAIGridDeck2D			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
