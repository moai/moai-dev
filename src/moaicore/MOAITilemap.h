// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITILEMAP_H
#define	MOAITILEMAP_H

#include <moaicore/MOAIGfxPrim2D.h>

class MOAIContentLibrary2D;

//================================================================//
// MOAITilemap
//================================================================//
/**	@brief Graphics primitive for rendering grid based tile maps.
*/
class MOAITilemap :
	public MOAIGfxPrim2D {
private:
	
	USRef < MOAIGrid >	mGrid;
	bool				mRepeatX;
	bool				mRepeatY;
	
	//----------------------------------------------------------------//
	static int			_getGrid				( lua_State* L );
	static int			_getTile				( lua_State* L );
	static int			_getTileLoc				( lua_State* L );
	static int			_setGrid				( lua_State* L );
	static int			_setRepeat				( lua_State* L );
	static int			_setTile				( lua_State* L );
	static int			_wrapCoord				( lua_State* L );
	static int			_worldToCoord			( lua_State* L );
	
public:

	enum {
		TOTAL_ATTR = MOAIGfxPrim2D::TOTAL_ATTR,
	};
	
	DECL_LUA_DATA ( MOAITilemap )
	
	GET_SET ( MOAIGrid*, Grid, mGrid )
	
	//----------------------------------------------------------------//
	void			ApplyAttrOp				( u32 attrID, USAttrOp& attrOp );
	u32				CountAttributes			();
	void			Draw					();
	void			DrawDebug				();
	void			GatherSurfaces			( MOAISurfaceSampler2D& sampler );
	void			GetBoundsInView			( USTileCoord& c0, USTileCoord& c1 );
	u32				GetLocalFrame			( USRect& frame );
	USVec2D			GetTileLoc				( int xTile, int yTile, u32 position = USGridSpace::TILE_CENTER );
					MOAITilemap				();
					~MOAITilemap			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void			SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	STLString		ToString				();
};

#endif
