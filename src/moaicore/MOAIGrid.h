// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRID_H
#define	MOAIGRID_H

//================================================================//
// MOAIGrid
//================================================================//
/**	@brief Grid data object.
*/
class MOAIGrid :
	public USLuaData,
	public USGridSpace {
private:

	USLeanArray < u32 > mTiles;

	//----------------------------------------------------------------//
	static int	_clearTileFlags		( lua_State* L );
	static int	_getTile			( lua_State* L );
	static int	_getTileFlags		( lua_State* L );
	static int	_setRow				( lua_State* L );
	static int	_setSize			( lua_State* L );
	static int	_setTile			( lua_State* L );
	static int	_setTileFlags		( lua_State* L );
	static int	_toggleTileFlags	( lua_State* L );
	
public:
	
	DECL_LUA_DATA ( MOAIGrid )
	
	//----------------------------------------------------------------//
	u32				GetTile				( int xTile, int yTile );
	void			Init				( u32 width, u32 height, float tileWidth, float tileHeight, u32 fill );
					MOAIGrid			();
					~MOAIGrid			();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	void			RowFromString		( u32 rowID, cc8* str );
	STLString		RowToString			( u32 rowID );
	void			SerializeIn			( USLuaState& state, USLuaSerializer& serializer );
	void			SerializeOut		( USLuaState& state, USLuaSerializer& serializer );
	void			SetTile				( u32 addr, u32 tile );
	void			SetTile				( int xTile, int yTile, u32 tile );
	u32				Size				();
	STLString		ToString			();
};

#endif
