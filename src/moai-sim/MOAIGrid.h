// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRID_H
#define	MOAIGRID_H

#include <moai-sim/MOAIGridSpace.h>
#include <moai-sim/MOAIDeck.h>

class MOAIGfxScriptBatch;

//================================================================//
// MOAIGrid
//================================================================//
/**	@lua	MOAIGrid
	@text	Grid data object. Grid cells are indexed starting and (1,1).
			Grid indices will wrap if out of range.
*/
class MOAIGrid :
	public virtual MOAIGridSpace {
private:

	ZLLeanArray < u32 >			mTiles; // TODO: fix size

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

protected:

	//----------------------------------------------------------------//
	void			OnResize			();

	//----------------------------------------------------------------//
	void			_RegisterLuaClass	( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs	( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_SerializeIn		( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			_SerializeOut		( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	DECL_LUA_FACTORY ( MOAIGrid )
	
	//----------------------------------------------------------------//
	void			Fill				( u32 value );
	u32				GetTile				( ZLIndex addr ) const;
	u32				GetTile				( int xTile, int yTile ) const;
					MOAIGrid			( ZLContext& context );
					~MOAIGrid			();
	void			RowFromString		( u32 rowID, cc8* str );
	STLString		RowToString			( u32 rowID );
	void			SetTile				( ZLIndex addr, u32 tile );
	void			SetTile				( int xTile, int yTile, u32 tile );
	size_t			StreamTilesIn		( ZLStream* stream );
	size_t			StreamTilesOut		( ZLStream* stream );
};

#endif
