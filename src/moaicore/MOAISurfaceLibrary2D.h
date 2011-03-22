// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISURFACELIBRARY2D_H
#define	MOAISURFACELIBRARY2D_H

#include <moaicore/MOAIContentLibrary2D.h>

//================================================================//
// MOAISurfaceBrush2D
//================================================================//
class MOAISurfaceBrush2D  {
private:

	friend class MOAISurfaceLibrary2D;

	USLeanArray < USEdge2D >	mEdges;
	USRect						mBounds;

	//----------------------------------------------------------------//
};

//================================================================//
// MOAISurfaceLibrary2D
//================================================================//
/**	@brief Surface library class.
*/
class MOAISurfaceLibrary2D :
	virtual public MOAIContentLibrary2D {
private:

	USLeanArray < MOAISurfaceBrush2D > mBrushes;

	//----------------------------------------------------------------//
	static int				_reserveBrushes		( lua_State* L );
	static int				_reserveSurfaces	( lua_State* L );
	static int				_setSurface			( lua_State* L );

	//----------------------------------------------------------------//
	void					DrawDebug				( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip );
	void					GatherSurfaces			( u32 idx, float xOff, float yOff, bool xFlip, bool yFlip, MOAISurfaceSampler2D& sampler );

public:
	
	DECL_LUA_DATA ( MOAISurfaceLibrary2D )
	
	//----------------------------------------------------------------//
	void					DrawDebug				( MOAIDrawingMtx2D& transform, u32 idx );
	void					DrawDebug				( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 );
	void					GatherSurfaces			( u32 idx, MOAISurfaceSampler2D& sampler );
	void					GatherSurfaces			( MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1, MOAISurfaceSampler2D& sampler );
	USRect					GetBounds				( u32 idx );
							MOAISurfaceLibrary2D	();
							~MOAISurfaceLibrary2D	();
	void					RegisterLuaClass		( USLuaState& state );
	void					RegisterLuaFuncs		( USLuaState& state );
	STLString				ToString				();
};

#endif
