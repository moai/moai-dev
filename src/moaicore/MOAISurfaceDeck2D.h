// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISURFACEDECK2D_H
#define	MOAISURFACEDECK2D_H

#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAISurface2D.h>

//================================================================//
// MOAISurfaceBrush2D
//================================================================//
class MOAISurfaceBrush2D  {
private:

	friend class MOAISurfaceDeck2D;

	USLeanArray < MOAISurfaceEdge2D >	mSurfaces;
	USRect								mBounds;

	//----------------------------------------------------------------//
};

//================================================================//
// MOAISurfaceDeck2D
//================================================================//
/**	@name	MOAISurfaceDeck2D
	@text	Deck of surface edge lists. Unused in this version of Moai.
*/
class MOAISurfaceDeck2D :
	virtual public MOAIDeck {
private:

	USLeanArray < MOAISurfaceBrush2D > mBrushes;

	//----------------------------------------------------------------//
	static int		_reserveSurfaceLists	( lua_State* L );
	static int		_reserveSurfaces		( lua_State* L );
	static int		_setSurface				( lua_State* L );

	//----------------------------------------------------------------//
	USBox			ComputeMaxBounds		();
	//void			DrawDebug				( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip );
	//void			GatherSurfaces			( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip, MOAISurfaceSampler2D& sampler );
	USBox			GetItemBounds			( u32 idx );

public:
	
	DECL_LUA_FACTORY ( MOAISurfaceDeck2D )
	
	//----------------------------------------------------------------//
	void			Draw					( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
					MOAISurfaceDeck2D		();
					~MOAISurfaceDeck2D		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SampleSurfaces			( u32 idx, MOAISurfaceSampler2D& sampler, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
};

#endif
