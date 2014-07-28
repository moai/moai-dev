// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRID_H
#define	MOAIGRID_H

#include <moai-sim/MOAIGridSpace.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>

//================================================================//
// MOAIGrid
//================================================================//
/**	@lua	MOAIGrid
	@text	Grid data object. Grid cells are indexed starting and (1,1).
			Grid indices will wrap if out of range.
*/
class MOAIGrid :
	public MOAIGridSpace {
private:

	ZLLeanArray < u32 > mTiles;

	//----------------------------------------------------------------//
	static int		_clearTileFlags		( lua_State* L );
	static int		_fill				( lua_State* L );
	static int		_getTile			( lua_State* L );
	static int		_getTileFlags		( lua_State* L );
	static int		_setRow				( lua_State* L );
	static int		_setTile			( lua_State* L );
	static int		_setTileFlags		( lua_State* L );
	static int		_streamTilesIn		( lua_State* L );
	static int		_streamTilesOut		( lua_State* L );
	static int		_toggleTileFlags	( lua_State* L );

	//----------------------------------------------------------------//
protected:
	void			OnResize			();

public:
	
	DECL_LUA_FACTORY ( MOAIGrid )
	
	//----------------------------------------------------------------//
	u32				GetTile				( int xTile, int yTile );
					MOAIGrid			();
					~MOAIGrid			();
	virtual	void	Draw				( MOAIDeck *deck, MOAIDeckRemapper *remapper, const MOAICellCoord &c0, const MOAICellCoord &c1 );
	void			Fill				( u32 value );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			RowFromString		( u32 rowID, cc8* str );
	STLString		RowToString			( u32 rowID );
	void			SerializeIn			( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut		( MOAILuaState& state, MOAISerializer& serializer );
	void			SetTile				( u32 addr, u32 tile );
	void			SetTile				( int xTile, int yTile, u32 tile );
	size_t			StreamTilesIn		( ZLStream* stream );
	size_t			StreamTilesOut		( ZLStream* stream );
};

#endif
