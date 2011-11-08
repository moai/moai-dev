// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRID_H
#define	MOAIGRID_H

#include <moaicore/MOAIGridSpace.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAITileFlags.h>

//================================================================//
// MOAIGrid
//================================================================//
/**	@name	MOAIGrid
	@text	Grid data object. Grid cells are indexed starting and (1,1).
			Grid indices will wrap if out of range.

	@flag	TILE_X_FLIP
	@flag	TILE_Y_FLIP
	@flag	TILE_XY_FLIP
	@flag	TILE_HIDE

	@const	TILE_BOTTOM_CENTER
	@const	TILE_CENTER
	@const	TILE_LEFT_BOTTOM
	@const	TILE_LEFT_CENTER
	@const	TILE_LEFT_TOP
	@const	TILE_RIGHT_BOTTOM
	@const	TILE_RIGHT_CENTER
	@const	TILE_RIGHT_TOP
	@const	TILE_TOP_CENTER
*/
class MOAIGrid :
	public MOAIObject,
	public MOAIGridSpace {
private:

	USLeanArray < u32 > mTiles;

	//----------------------------------------------------------------//
	static int	_clearTileFlags		( lua_State* L );
	static int	_getSize			( lua_State* L );
	static int	_getTile			( lua_State* L );
	static int	_getTileFlags		( lua_State* L );
	static int	_getTileLoc			( lua_State* L );
	static int	_locToCoord			( lua_State* L );
	static int	_setRow				( lua_State* L );
	static int	_setSize			( lua_State* L );
	static int	_setTile			( lua_State* L );
	static int	_setTileFlags		( lua_State* L );
	static int	_toggleTileFlags	( lua_State* L );
	static int	_wrapCoord			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIGrid )
	
	//----------------------------------------------------------------//
	u32				GetTile				( int xTile, int yTile );
					MOAIGrid			();
					~MOAIGrid			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			RowFromString		( u32 rowID, cc8* str );
	STLString		RowToString			( u32 rowID );
	void			SerializeIn			( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut		( MOAILuaState& state, MOAISerializer& serializer );
	void			SetTile				( u32 addr, u32 tile );
	void			SetTile				( int xTile, int yTile, u32 tile );
	u32				Size				();
};

#endif
