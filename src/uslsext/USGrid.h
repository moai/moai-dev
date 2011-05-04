// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USGRID_H
#define USGRID_H

#include <uslsext/USMatrix2D.h>

//================================================================//
// USCellCoord
//================================================================//
class USCellCoord :
	public USIntVec2D {
public:
	
	//----------------------------------------------------------------//
	u32			GetID			();
	bool		IsEqual			( USCellCoord cellCoord );
				USCellCoord		();
				~USCellCoord	();
	
	//----------------------------------------------------------------//
	inline USCellCoord ( int x, int y ) :
		USIntVec2D ( x, y ) {
	}
	
	//----------------------------------------------------------------//
	inline USCellCoord MakeOffset ( int x, int y ) {
		
		USCellCoord result;
		result.mX = this->mX + x;
		result.mY = this->mY + y;
		
		return result;
	}
};

//================================================================//
// USGridSpace
//================================================================//
class USGridSpace {
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
	USVec2D			GetRectPoint		( float x, float y, float width, float height, u32 position ) const;

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
	USVec2D				CellToWorld				( USCellCoord cellCoord, USVec2D loc ) const;
	
	USCellCoord			Clamp					( USCellCoord cellCoord ) const;
	USCellCoord			ClampX					( USCellCoord cellCoord ) const;
	USCellCoord			ClampY					( USCellCoord cellCoord ) const;
	USRect				GetBounds				() const;
	USRect				GetBounds				( USCellCoord c0, USCellCoord c1 ) const;
	
	USMatrix2D			GetGridToWorldMtx		() const;
	
	int					GetCellAddr				( USCellCoord cellCoord ) const;
	USCellCoord			GetCellCoord			( int cellAddr ) const;
	USCellCoord			GetCellCoord			( USVec2D loc ) const;
	USCellCoord			GetCellCoord			( float x, float y ) const;
	USCellCoord			GetCellCoord			( int xCell, int yCell ) const;
	
	USVec2D				GetCellPoint			( USCellCoord cellCoord, u32 position ) const;
	USRect				GetCellRect				( USCellCoord cellCoord ) const;
	
	USVec2D				GetTilePoint			( USCellCoord cellCoord, u32 position ) const;
	USRect				GetTileRect				( USCellCoord cellCoord ) const;
	
	int					GetTotalCells			() const;
	
	USMatrix2D			GetWorldToGridMtx		() const;
	
	USVec2D				GridToWorld				( USVec2D loc ) const;
	bool				IsValidCoord			( USCellCoord cellCoord ) const;
	void				SerializeIn				( USLuaState& state );
	void				SerializeOut			( USLuaState& state );
	
						USGridSpace				();
						~USGridSpace			();
	USCellCoord			WrapCellCoord			( USCellCoord coord ) const;
	USVec2D				WorldToCell				( USCellCoord cellCoord, USVec2D loc ) const;
	USVec2D				WorldToGrid				( USVec2D loc ) const;
};

#endif
