// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRID_H
#define	MOAIGRID_H

#include <moaicore/MOAIGridSpace.h>
#include <moaicore/MOAILua.h>

//================================================================//
// MOAIGrid
//================================================================//
/**	@name	MOAIGrid
	@text	Grid data object. Grid cells are indexed starting and (1,1).
			Grid indices will wrap if out of range.
*/
class MOAIGrid :
	public MOAIGridSpace {
private:

	USLeanArray < u32 > mTiles;
	USLeanArray < USColorVec > mColors;
	USLeanArray < float > mScales;

	//----------------------------------------------------------------//
	static int		_clearTileFlags		( lua_State* L );
	static int		_fill				( lua_State* L );
	static int		_getTile			( lua_State* L );
	static int		_getColor			( lua_State* L );
	static int		_getScale			( lua_State* L );
	static int		_getTileFlags		( lua_State* L );
	static int		_setRow				( lua_State* L );
	static int		_setTile			( lua_State* L );
	static int		_setColor			( lua_State* L );
	static int		_setScale			( lua_State* L );
	static int		_setTileFlags		( lua_State* L );
	static int		_streamTilesIn		( lua_State* L );
	static int		_streamTilesOut		( lua_State* L );
	static int		_toggleTileFlags	( lua_State* L );

	//----------------------------------------------------------------//
	void			OnResize			();

public:
	
	DECL_LUA_FACTORY ( MOAIGrid )
	
	//----------------------------------------------------------------//
	u32				GetTile				( int xTile, int yTile );
	USColorVec			GetColor			( int xTile, int yTile );
	float				GetScale			( int xTile, int yTile );
					MOAIGrid			();
					~MOAIGrid			();
	void			Fill				( u32 value );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			RowFromString		( u32 rowID, cc8* str );
	STLString		RowToString			( u32 rowID );
	void			SerializeIn			( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut		( MOAILuaState& state, MOAISerializer& serializer );
	void			SetTile				( u32 addr, u32 tile );
	void			SetColor				( u32 addr, const USColorVec& color );
	void			SetScale				( u32 addr, float scale );
	void			SetTile				( int xTile, int yTile, u32 tile );
	void			SetColor			( int xTile, int yTile, const USColorVec& color );
	void			SetScale			( int xTile, int yTile, float scale );
	size_t			StreamTilesIn		( USStream* stream );
	size_t			StreamTilesOut		( USStream* stream );
};

#endif
