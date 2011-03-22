// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USGRID_H
#define USGRID_H

#include <uslsext/USMatrix2D.h>

//================================================================//
// USTileCoord
//================================================================//
class USTileCoord :
	public USIntVec2D {
public:
	
	//----------------------------------------------------------------//
	u32			GetID			();
	bool		IsEqual			( USTileCoord& tileCoord );
				USTileCoord		();
				~USTileCoord	();
};

//================================================================//
// USGridSpace
//================================================================//
class USGridSpace {
private:

	float			mTileWidth;
	float			mTileHeight;

	int				mWidth;
	int				mHeight;

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
	
	GET_SET ( float, TileWidth, mTileWidth )
	GET_SET ( float, TileHeight, mTileHeight )
	
	GET_SET ( int, Width, mWidth )
	GET_SET ( int, Height, mHeight )
	
	//----------------------------------------------------------------//
	void				Clamp					( USTileCoord& tileCoord );
	void				ClampX					( USTileCoord& tileCoord );
	void				ClampY					( USTileCoord& tileCoord );
	USRect				GetBounds				();
	USRect				GetBounds				( USTileCoord& c0, USTileCoord& c1 );
	
	USMatrix2D			GetGridToWorldMtx		();
	
	int					GetTileAddr				( USVec2D& loc );
	int					GetTileAddr				( int xTile, int yTile );
	int					GetTileAddr				( USTileCoord& tileCoord );
	int					GetTileAddr				( USTileCoord tileCoord, int xOff, int yOff );
	USTileCoord			GetTileCoord			( int tileAddr );
	USTileCoord			GetTileCoord			( USVec2D& loc );
	USTileCoord			GetTileCoord			( float x, float y );
	USTileCoord			GetTileCoord			( int xTile, int yTile );
	USVec2D				GetTilePoint			( int tileAddr, u32 position );
	USVec2D				GetTilePoint			( int xTile, int yTile, u32 position );
	USVec2D				GetTilePoint			( USTileCoord& tileCoord, u32 position );
	USRect				GetTileRect				( int tileAddr );
	USRect				GetTileRect				( int xTile, int yTile );
	USRect				GetTileRect				( USTileCoord& tileCoord );
	int					GetTotalTiles			();
	
	USMatrix2D			GetWorldToGridMtx		();
	
	USVec2D				GridToWorld				( USVec2D loc );
	void				Init					( int width, int height );
	bool				IsValidCoord			( int xTile, int yTile );
	bool				IsValidCoord			( USTileCoord& tileCoord );
	bool				IsValidCoord			( USVec2D& loc );
	void				SerializeIn				( USLuaState& state );
	void				SerializeOut			( USLuaState& state );
	void				Snap					( USVec2D& loc );
	USVec2D				TileToWorld				( USTileCoord& tileCoord, USVec2D& loc );
						USGridSpace				();
						~USGridSpace			();
	USTileCoord			WrapCellCoord			( const USTileCoord& coord );
	USVec2D				WorldToTile				( USTileCoord& tileCoord, USVec2D& loc );
	USVec2D				WorldToGrid				( USVec2D loc );
};

#endif
