// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGRIDSPACE_H
#define MOAIGRIDSPACE_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAITileFlags.h>

//================================================================//
// MOAICellCoord
//================================================================//
class MOAICellCoord :
	public USIntVec2D {
public:
	
	//----------------------------------------------------------------//
	u32			GetID			();
	bool		IsEqual			( MOAICellCoord cellCoord );
				MOAICellCoord		();
				~MOAICellCoord	();
	
	//----------------------------------------------------------------//
	inline MOAICellCoord ( int x, int y ) :
		USIntVec2D ( x, y ) {
	}
	
	//----------------------------------------------------------------//
	inline MOAICellCoord MakeOffset ( int x, int y ) {
		
		MOAICellCoord result;
		result.mX = this->mX + x;
		result.mY = this->mY + y;
		
		return result;
	}
};

//================================================================//
// MOAIGridSpace
//================================================================//
/**	@name	MOAIGridSpace
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
*/
class MOAIGridSpace :
	public virtual MOAILuaObject {
protected:

	float		mXOff;
	float		mYOff;

	float		mCellWidth;
	float		mCellHeight;
	
	float		mTileWidth;
	float		mTileHeight;

	int			mWidth;
	int			mHeight;

	//----------------------------------------------------------------//
	static int		_cellAddrToCoord	( lua_State* L );
	static int		_getCellAddr		( lua_State* L );
	static int		_getSize			( lua_State* L );
	static int		_getTileLoc			( lua_State* L );
	static int		_locToCellAddr		( lua_State* L );
	static int		_locToCoord			( lua_State* L );
	static int		_setSize			( lua_State* L );
	static int		_wrapCoord			( lua_State* L );

	//----------------------------------------------------------------//
	USVec2D			GetRectPoint		( float x, float y, float width, float height, u32 position ) const;
	virtual void	OnResize			();

public:
	
	enum {
		TILE_LEFT_TOP,
		TILE_RIGHT_TOP,
		TILE_LEFT_BOTTOM,
		TILE_RIGHT_BOTTOM,
		
		TILE_LEFT_CENTER,
		TILE_RIGHT_CENTER,
		TILE_TOP_CENTER,
		TILE_BOTTOM_CENTER,
		
		TILE_CENTER,
	};
	
	enum {
		GRID_RECT,
		GRID_ISO,
		GRID_OBLIQUE,
		GRID_HEX,
	};
	
	GET_SET ( float, XOff, mXOff )
	GET_SET ( float, YOff, mYOff )
	
	GET_SET ( float, CellWidth, mCellWidth )
	GET_SET ( float, CellHeight, mCellHeight )
	
	GET_SET ( float, TileWidth, mTileWidth )
	GET_SET ( float, TileHeight, mTileHeight )
	
	GET_SET ( int, Width, mWidth )
	GET_SET ( int, Height, mHeight )
	
	//----------------------------------------------------------------//
	USVec2D				CellToWorld				( MOAICellCoord cellCoord, USVec2D loc ) const;
	
	MOAICellCoord		Clamp					( MOAICellCoord cellCoord ) const;
	MOAICellCoord		ClampX					( MOAICellCoord cellCoord ) const;
	MOAICellCoord		ClampY					( MOAICellCoord cellCoord ) const;
	USRect				GetBounds				() const;
	USRect				GetBounds				( MOAICellCoord c0, MOAICellCoord c1 ) const;
	
	USMatrix3x3			GetGridToWorldMtx		() const;
	
	int					GetCellAddr				( MOAICellCoord cellCoord ) const;
	int					GetCellAddr				( int xCell, int yCell ) const;
	MOAICellCoord		GetCellCoord			( int cellAddr ) const;
	MOAICellCoord		GetCellCoord			( USVec2D loc ) const;
	MOAICellCoord		GetCellCoord			( float x, float y ) const;
	MOAICellCoord		GetCellCoord			( int xCell, int yCell ) const;
	
	USVec2D				GetCellPoint			( MOAICellCoord cellCoord, u32 position ) const;
	USRect				GetCellRect				( MOAICellCoord cellCoord ) const;
	
	USVec2D				GetTilePoint			( MOAICellCoord cellCoord, u32 position ) const;
	USRect				GetTileRect				( MOAICellCoord cellCoord ) const;
	
	int					GetTotalCells			() const;
	
	USMatrix3x3			GetWorldToGridMtx		() const;
	
	USVec2D				GridToWorld				( USVec2D loc ) const;
	void				Init					( int width, int height, float tileWidth, float tileHeight );
	bool				IsValidCoord			( MOAICellCoord cellCoord ) const;
						MOAIGridSpace			();
						~MOAIGridSpace			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	MOAICellCoord		WrapCellCoord			( MOAICellCoord coord ) const;
	USVec2D				WorldToCell				( MOAICellCoord cellCoord, USVec2D loc ) const;
	USVec2D				WorldToGrid				( USVec2D loc ) const;
};

#endif
