// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGRIDSPACE_H
#define ZLGRIDSPACE_H

#include <zl-core/ZLVec2D.h>
#include <zl-core/ZLTileFlags.h>

//================================================================//
// ZLGridCoord
//================================================================//
class ZLGridCoord :
	public ZLIntVec2D {
public:
	
	//----------------------------------------------------------------//
	u32			GetID				();
	bool		IsEqual				( ZLGridCoord cellCoord );
				ZLGridCoord		();
				~ZLGridCoord		();
	
	//----------------------------------------------------------------//
	inline ZLGridCoord ( int x, int y ) :
		ZLIntVec2D ( x, y ) {
	}
	
	//----------------------------------------------------------------//
	inline ZLGridCoord MakeOffset ( int x, int y ) {
		
		ZLGridCoord result;
		result.mX = this->mX + x;
		result.mY = this->mY + y;
		
		return result;
	}
};

//================================================================//
// ZLGridSpace
//================================================================//
/**	@lua	ZLGridSpace
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
class ZLGridSpace {
protected:

	float		mXOff;
	float		mYOff;

	float		mCellWidth;
	float		mCellHeight;
	
	float		mTileWidth;
	float		mTileHeight;

	int			mWidth;
	int			mHeight;

	u32			mShape;
	u32			mRepeat;

	//----------------------------------------------------------------//
	ZLGridCoord		GetAxialHexCellCoord	( float x, float y ) const;
	ZLGridCoord		GetHexCellCoord			( float x, float y, float a, float b ) const;
	ZLGridCoord		GetObliqueCellCoord		( float x, float y ) const;
	ZLVec2D			GetRectPoint			( float x, float y, float width, float height, u32 position ) const;
	virtual void	OnResize				();

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
	
	static const u32 REPEAT_X			= 0x00000001;
	static const u32 REPEAT_Y			= 0x00000002;
	
	static const u32 STAGGER_FLAG		= 0x80000000;
	static const u32 STAGGER_MASK		= 0x80000000;
	static const u32 SHAPE_MASK			= 0x7FFFFFFF;
	
	static const u32 RECT_SHAPE			= 0x00000000;
	static const u32 DIAMOND_SHAPE		= 0x00000001 | STAGGER_FLAG;
	static const u32 OBLIQUE_SHAPE		= 0x00000002;
	static const u32 HEX_SHAPE			= 0x00000003 | STAGGER_FLAG;
	static const u32 AXIAL_HEX_SHAPE	= 0x00000004;
	
	GET_SET_CONST ( float, XOff, mXOff )
	GET_SET_CONST ( float, YOff, mYOff )
	
	GET_SET_CONST ( float, CellWidth, mCellWidth )
	GET_SET_CONST ( float, CellHeight, mCellHeight )
	
	GET_SET_CONST ( float, TileWidth, mTileWidth )
	GET_SET_CONST ( float, TileHeight, mTileHeight )
	
	GET_SET_CONST ( int, Width, mWidth )
	GET_SET_CONST ( int, Height, mHeight )
	
	GET_SET_CONST ( u32, Shape, mShape )
	GET_SET_CONST ( u32, Repeat, mRepeat )
	
	//----------------------------------------------------------------//
	ZLVec2D				CellToWorld				( ZLGridCoord cellCoord, ZLVec2D loc ) const;
	
	ZLGridCoord			Clamp					( ZLGridCoord cellCoord ) const;
	ZLGridCoord			ClampX					( ZLGridCoord cellCoord ) const;
	ZLGridCoord			ClampY					( ZLGridCoord cellCoord ) const;
	
	ZLIndex				GetCellAddr				( ZLGridCoord cellCoord ) const;
	ZLIndex				GetCellAddr				( int xCell, int yCell ) const;
	ZLGridCoord			GetCellCoord			( ZLIndex cellAddr ) const;
	ZLGridCoord			GetCellCoord			( ZLVec2D loc ) const;
	ZLGridCoord			GetCellCoord			( float x, float y ) const;
	ZLGridCoord			GetCellCoord			( int xCell, int yCell ) const;
	
	ZLVec2D				GetCellPoint			( ZLGridCoord cellCoord, u32 position ) const;
	ZLRect				GetCellRect				( ZLGridCoord cellCoord ) const;
	
	ZLRect				GetFrame				() const;
	ZLRect				GetFrame				( ZLGridCoord c0, ZLGridCoord c1 ) const;
	void				GetFrameInRect			( ZLRect rect, ZLGridCoord& c0, ZLGridCoord& c1, ZLRect maxSize ) const;
	
	ZLVec2D				GetTilePoint			( ZLGridCoord cellCoord, u32 position ) const;
	ZLVec3D				GetTilePoint			( ZLGridCoord cellCoord, u32 position, float z ) const;
	ZLRect				GetTileRect				( ZLGridCoord cellCoord ) const;
	
	int					GetTotalCells			() const;
	
	ZLVec2D				GridToWorld				( ZLVec2D loc ) const;
	void				Init					( int width, int height, float tileWidth, float tileHeight );
	bool				IsValidCoord			( ZLGridCoord cellCoord ) const;
						ZLGridSpace				();
						~ZLGridSpace			();
	ZLGridCoord			WrapCellCoord			( int xCell, int yCell ) const;
	ZLVec2D				WorldToCell				( ZLGridCoord cellCoord, ZLVec2D loc ) const;
	ZLVec2D				WorldToGrid				( ZLVec2D loc ) const;
};

#endif
