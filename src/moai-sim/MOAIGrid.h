// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRID_H
#define	MOAIGRID_H

#include <moai-sim/MOAIGridSpace.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>

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

	ZLLeanArray < u32 > mTiles;
	ZLLeanArray < ZLColorVec > mColors;
	ZLLeanArray < float > mScales;
	int mHasColors;
	ZLColorVec mDefaultColor;
	int mHasScales;

	//----------------------------------------------------------------//
	static int		_addColor		( lua_State* L );
	static int		_addScale		( lua_State* L );
	static int		_clearTileFlags		( lua_State* L );
	static int		_fill			( lua_State* L );
	static int		_fillColor		( lua_State* L );
	static int		_fillScale		( lua_State* L );
	static int		_getTile		( lua_State* L );
	static int		_getTileColor		( lua_State* L );
	static int		_getTileFlags		( lua_State* L );
	static int		_getTileScale		( lua_State* L );
	static int		_removeColor		( lua_State* L );
	static int		_removeScale		( lua_State* L );
	static int		_setRow			( lua_State* L );
	static int		_setTile		( lua_State* L );
	static int		_setTileColor		( lua_State* L );
	static int		_setTileFlags		( lua_State* L );
	static int		_setTileScale		( lua_State* L );
	static int		_streamTilesIn		( lua_State* L );
	static int		_streamTilesOut		( lua_State* L );
	static int		_toggleTileFlags	( lua_State* L );

	//----------------------------------------------------------------//
	void			OnResize			();
	void			pickDoDrawRegion		();
	void (*DoDrawRegion) ( MOAIGrid &grid, const MOAICellCoord &c0, const MOAICellCoord &c1, MOAIGfxDevice &gfxDevice, MOAILuaSharedPtr<MOAIDeck> &deck, MOAIDeckRemapper *remapper, float xOff, float yOff, float zOff, float xScl, float yScl );

public:
	
	DECL_LUA_FACTORY ( MOAIGrid )
	
	//----------------------------------------------------------------//
	void			AddColor		( const ZLColorVec &defaultColor );
	void			AddScale		( );
	u32			GetTile			( int xTile, int yTile );
	ZLColorVec		GetTileColor		( int xTile, int yTile );
	float			GetTileScale		( int xTile, int yTile );
				MOAIGrid		();
				~MOAIGrid		();
	void			Fill			( u32 value );
	void			FillColor		( const ZLColorVec &value );
	void			FillScale		( float value );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			RemoveColor		( );
	void			RemoveScale		( );
	void			RowFromString		( u32 rowID, cc8* str );
	STLString		RowToString			( u32 rowID );
	void			SerializeIn			( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut		( MOAILuaState& state, MOAISerializer& serializer );
	void			SetTile				( u32 addr, u32 tile );
	void			SetTile				( int xTile, int yTile, u32 tile );
	void                    SetTileColor                    ( int xTile, int yTile, const ZLColorVec &color );
	void                    SetTileScale                    ( int xTile, int yTile, float scale );
			 
	size_t			StreamTilesIn		( ZLStream* stream );
	size_t			StreamTilesOut		( ZLStream* stream );
	void                    DrawRegion              ( const MOAICellCoord &c0, const MOAICellCoord &c1, MOAIGfxDevice &gfxDevice, MOAILuaSharedPtr<MOAIDeck> &deck, MOAILuaSharedPtr<MOAIDeckRemapper> &remapper, float xOff, float yOff, float zOff, float xScl, float yScl ) {
		if (this->DoDrawRegion) {
			 this->DoDrawRegion ( *this, c0, c1, gfxDevice, deck, remapper, xOff, yOff, zOff, xScl, yScl );
		}
	}
};

#endif
