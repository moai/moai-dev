// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISURFACEDECK2D_H
#define	MOAISURFACEDECK2D_H

#if 0

//================================================================//
// MOAISurfaceBrush2D
//================================================================//
class MOAISurfaceBrush2D  {
private:

	friend class MOAISurfaceDeck2D;

	ZLLeanArray < ZLEdge2D >	mEdges;
	ZLRect						mBounds;

	//----------------------------------------------------------------//
};

//================================================================//
// MOAISurfaceDeck2D
//================================================================//
/**	@lua	MOAISurfaceDeck2D
	@text	Deck of surface edge lists. Unused in this version of Moai.
*/
class MOAISurfaceDeck2D :
	virtual public MOAIStandardDeck {
private:

	ZLLeanArray < MOAISurfaceBrush2D > mBrushes;

	//----------------------------------------------------------------//
	static int		_reserveSurfaceLists	( lua_State* L );
	static int		_reserveSurfaces		( lua_State* L );
	static int		_setSurface				( lua_State* L );

	//----------------------------------------------------------------//
	ZLBox			ComputeMaxBounds		();
	//void			DrawDebug				( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip );
	void			GatherSurfaces			( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip, MOAISurfaceSampler2D& sampler );
	ZLBox			GetItemBounds			( u32 idx );

	//----------------------------------------------------------------//
	ZLBounds				MOAIDeck_ComputeMaxBounds		();
	void					MOAIDeck_Draw					( ZLIndex idx );
	ZLBounds				MOAIDeck_GetBounds				( ZLIndex idx );
	MOAICollisionShape*		MOAIDeck_GetCollisionShape		( ZLIndex idx );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec2D& vec, u32 granularity, ZLBounds* result );
	bool					MOAIDeck_Overlap				( ZLIndex idx, const ZLVec3D& vec, u32 granularity, ZLBounds* result );

public:
	
	DECL_LUA_FACTORY ( MOAISurfaceDeck2D )
	
	//----------------------------------------------------------------//
	//void			DrawDebug				( const ZLAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper );
	//void			DrawDebug				( const ZLAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, ZLVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 );
	//void			GatherSurfaces			( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler );
	//void			GatherSurfaces			( MOAIGrid& grid, MOAIDeckRemapper* remapper, ZLVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1, MOAISurfaceSampler2D& sampler );
					MOAISurfaceDeck2D		();
					~MOAISurfaceDeck2D		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
#endif
