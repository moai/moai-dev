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
	public virtual MOAIStandardDeck {
private:

	ZLLeanArray < MOAISurfaceBrush2D > mBrushes;

	//----------------------------------------------------------------//
	static int		_reserveSurfaceLists	( lua_State* L );
	static int		_reserveSurfaces		( lua_State* L );
	static int		_setSurface				( lua_State* L );

	//----------------------------------------------------------------//
	//void			DrawDebug				( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip );
	void			GatherSurfaces			( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip, MOAISurfaceSampler2D& sampler );
	ZLBox			GetItemAABB				( u32 idx );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	ZLBounds		MOAIDeck_GetBounds		();
	ZLBounds		MOAIDeck_GetBounds		( ZLIndex idx );
	void			MOAIDeck_Render			( ZLIndex idx, MOAIRenderPhaseEnum::_ renderPhase );

public:
	
	DECL_LUA_FACTORY ( MOAISurfaceDeck2D )
	
	//----------------------------------------------------------------//
	//void			DrawDebug				( const ZLAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper );
	//void			DrawDebug				( const ZLAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, ZLVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 );
	//void			GatherSurfaces			( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler );
	//void			GatherSurfaces			( MOAIGrid& grid, MOAIDeckRemapper* remapper, ZLVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1, MOAISurfaceSampler2D& sampler );
					MOAISurfaceDeck2D		();
					~MOAISurfaceDeck2D		();
	
};

#endif
#endif
