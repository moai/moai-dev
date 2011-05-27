// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISURFACEDECK2D_H
#define	MOAISURFACEDECK2D_H

#include <moaicore/MOAIDeck2D.h>

//================================================================//
// MOAISurfaceBrush2D
//================================================================//
class MOAISurfaceBrush2D  {
private:

	friend class MOAISurfaceDeck2D;

	USLeanArray < USEdge2D >	mEdges;
	USRect						mBounds;

	//----------------------------------------------------------------//
};

//================================================================//
// MOAISurfaceDeck2D
//================================================================//
/**	@name	MOAISurfaceDeck2D
	@text	Deck of surface edge lists. Unused in this version of Moai.
*/
class MOAISurfaceDeck2D :
	virtual public MOAIDeck2D {
private:

	USLeanArray < MOAISurfaceBrush2D > mBrushes;

	//----------------------------------------------------------------//
	static int				_reserveSurfaceLists	( lua_State* L );
	static int				_reserveSurfaces		( lua_State* L );
	static int				_setSurface				( lua_State* L );

	//----------------------------------------------------------------//
	void					DrawDebug				( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip );
	void					GatherSurfaces			( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip, MOAISurfaceSampler2D& sampler );

public:
	
	DECL_LUA_FACTORY ( MOAISurfaceDeck2D )
	
	//----------------------------------------------------------------//
	void					DrawDebug				( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper );
	void					DrawDebug				( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1 );
	void					GatherSurfaces			( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler );
	void					GatherSurfaces			( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1, MOAISurfaceSampler2D& sampler );
	USRect					GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
							MOAISurfaceDeck2D		();
							~MOAISurfaceDeck2D		();
	void					RegisterLuaClass		( USLuaState& state );
	void					RegisterLuaFuncs		( USLuaState& state );
	STLString				ToString				();
};

#endif
