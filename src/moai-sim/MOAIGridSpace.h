// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGRIDSPACE_H
#define MOAIGRIDSPACE_H

//================================================================//
// MOAIGridSpace
//================================================================//
/**	@lua	MOAIGridSpace
	@text	Represents spatial configuration of a grid. The grid is made
			up of cells. Inside of each cell is a tile. The tile can be
			larger or smaller than the cell and also offset from the cell.
			By default, tiles are the same size of their cells and are
			no offset.

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
	
	@const	SQUARE_SHAPE
	@const	DIAMOND_SHAPE
	@const	OBLIQUE_SHAPE
	@const	HEX_SHAPE
	@const	AXIAL_HEX_SHAPE
*/
class MOAIGridSpace :
	public virtual MOAILuaObject,
	public virtual ZLGridSpace {
protected:

	//----------------------------------------------------------------//
	static int		_cellAddrToCoord		( lua_State* L );
	static int		_getCellAddr			( lua_State* L );
	static int		_getCellSize			( lua_State* L );
	static int		_getOffset				( lua_State* L );
	static int		_getSize				( lua_State* L );
	static int		_getTileLoc				( lua_State* L );
	static int		_getTileSize			( lua_State* L );
	static int		_initAxialHexGrid		( lua_State* L );
	static int		_initDiamondGrid		( lua_State* L );
	static int		_initHexGrid			( lua_State* L );
	static int		_initObliqueGrid		( lua_State* L );
	static int		_initRectGrid			( lua_State* L );
	static int		_locToCellAddr			( lua_State* L );
	static int		_locToCoord				( lua_State* L );
	static int		_setRepeat				( lua_State* L );
	static int		_setShape				( lua_State* L );
	static int		_setSize				( lua_State* L );
	static int		_wrapCoord				( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_SerializeIn			( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	void			_SerializeOut			( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	DECL_LUA_FACTORY ( MOAIGridSpace )
	
	//----------------------------------------------------------------//
					MOAIGridSpace			( ZLContext& context );
					~MOAIGridSpace			();
};

#endif
