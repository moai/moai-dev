// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-core/ZLGridSpace.h>

double sqrt3 = 1.73205080756887729352;

//================================================================//
// ZLGridCoord
//================================================================//

//----------------------------------------------------------------//
u32 ZLGridCoord::GetID () {

	return (( u32 )this->mX & 0xff ) | ((( u32 )this->mY & 0xff ) << 16 );
}

//----------------------------------------------------------------//
bool ZLGridCoord::IsEqual ( ZLGridCoord cellCoord ) {

	if ( this->mX != cellCoord.mX ) return false;
	if ( this->mY != cellCoord.mY ) return false;
	
	return true;
}

//----------------------------------------------------------------//
ZLGridCoord::ZLGridCoord () {
	mX = -1;
	mY = -1;
}

//----------------------------------------------------------------//
ZLGridCoord::~ZLGridCoord () {
}

//================================================================//
// ZLGridSpace
//================================================================//

//----------------------------------------------------------------//
ZLVec2D ZLGridSpace::CellToWorld ( ZLGridCoord cellCoord, ZLVec2D loc ) const {

	ZLVec2D result;
	result.mX = ( loc.mX + ( float )cellCoord.mX ) * this->mCellWidth;
	result.mY = ( loc.mY + ( float )cellCoord.mY ) * this->mCellHeight;
	
	return result;
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::Clamp ( ZLGridCoord cellCoord ) const {

	int xMax = this->mWidth - 1;
	int yMax = this->mHeight - 1;

	ZLGridCoord result;
	
	result.mX = ( cellCoord.mX < 0 ) ? 0 : cellCoord.mX;
	result.mX = ( cellCoord.mX > xMax ) ? xMax : result.mX;

	result.mY = ( cellCoord.mY < 0 ) ? 0 : cellCoord.mY;
	result.mY = ( cellCoord.mY > yMax ) ? yMax : result.mY;

	return result;
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::ClampX ( ZLGridCoord cellCoord ) const {

	int xMax = this->mWidth - 1;

	ZLGridCoord result;
	
	result.mX = ( cellCoord.mX < 0 ) ? 0 : cellCoord.mX;
	result.mX = ( cellCoord.mX > xMax ) ? xMax : result.mX;
	
	result.mY = cellCoord.mY;

	return result;
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::ClampY ( ZLGridCoord cellCoord ) const {
	
	int yMax = this->mHeight - 1;

	ZLGridCoord result;
	
	result.mY = ( cellCoord.mY < 0 ) ? 0 : cellCoord.mY;
	result.mY = ( cellCoord.mY > yMax ) ? yMax : result.mY;

	result.mX = cellCoord.mX;

	return result;
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::GetAxialHexCellCoord ( float x, float y ) const {
	float t1, t2, q, r;

	/* normalize to units of hex dimensions */
	x = ( x / this->mCellWidth ) - 0.5f;
	y = ( y / this->mCellHeight );

	/*
	 * from a comment thread on Amit Patel's hexagon page:

	 	x = (x - halfHexWidth) / hexWidth;

	 	double t1 = z / hexRadius, t2 = Math.Floor(x + t1);
	 	double r = Math.Floor((Math.Floor(t1 - x) + t2) / 3);
	 	double q = Math.Floor((Math.Floor(2 * x + 1) + t2) / 3) - r;

	 	return new Coord((int) q, (int) r); 
	 * note that "hexRadius" is pretty close to "mHeight / 2" for
	 * our purposes. The example assumed that z increases as you go
	 * down the screen, but it also assumed that the hex grid was
	 * counting the same way.
	 */
	t1 = ( y * 2 );
	t2 = floorf ( x + t1 );
	r = floorf (( floorf ( t1 - x ) + t2 ) / 3.0f );
	q = floorf (( floorf ( 2.0f * x + 1.0f ) + t2) / 3.0f );

	return ZLGridCoord (( int )q, ( int )r );
}

//----------------------------------------------------------------//
ZLIndex ZLGridSpace::GetCellAddr ( ZLGridCoord cellCoord ) const {

	return this->GetCellAddr ( cellCoord.mX, cellCoord.mY );
}

//----------------------------------------------------------------//
ZLIndex ZLGridSpace::GetCellAddr ( int xCell, int yCell ) const {

	if ( !( this->mWidth && this->mHeight )) return 0;

	xCell = xCell % this->mWidth;
	if ( xCell < 0 ) xCell += this->mWidth;

	yCell = yCell % this->mHeight;
	if ( yCell < 0 ) yCell += this->mHeight;

	return ( ZLSize )(( yCell * this->mWidth ) + xCell );
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::GetCellCoord ( ZLIndex cellAddr ) const {
	
	ZLGridCoord cellCoord;
	
	cellCoord.mX = ( cellAddr % this->mWidth );
	cellCoord.mY = (( int )(( ZLSize )cellAddr / this->mWidth ) % this->mHeight );
	
	return cellCoord;
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::GetCellCoord ( ZLVec2D loc ) const {
	
	return this->GetCellCoord ( loc.mX, loc.mY );
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::GetCellCoord ( float x, float y ) const {
	
	ZLGridCoord cellCoord ( 0, 0 );
	
	switch ( this->mShape ) {
		
		case AXIAL_HEX_SHAPE:
			return this->GetAxialHexCellCoord ( x, y );

		case DIAMOND_SHAPE:
			return this->GetHexCellCoord ( x, y, 0.0f, 4.0f );
		
		case HEX_SHAPE:
			return this->GetHexCellCoord ( x, y, 2.0f, 10.0f );
			
		case OBLIQUE_SHAPE:
			return this->GetObliqueCellCoord ( x, y );
		
		case RECT_SHAPE:
			cellCoord.mX = ( int )floorf ( x / this->mCellWidth );
			cellCoord.mY = ( int )floorf ( y / this->mCellHeight );
			break;
	}
	
	return cellCoord;
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::GetCellCoord ( int xCell, int yCell ) const {

	ZLGridCoord cellCoord;
	cellCoord.mX = xCell;
	cellCoord.mY = yCell;
	return cellCoord;
}

//----------------------------------------------------------------//
ZLVec2D ZLGridSpace::GetCellPoint ( ZLGridCoord cellCoord, u32 position ) const {
			
	float xOff = 0.0f;
	
	if (( this->mShape & STAGGER_FLAG ) && ( cellCoord.mY & 0x01 )) {
		xOff = this->mCellWidth * 0.5f;
	}
	
	if ( this->mShape == AXIAL_HEX_SHAPE ) {
		xOff = this->mCellWidth * -0.5f * cellCoord.mY;
		return this->GetRectPoint (
			( cellCoord.mX * this->mCellWidth ) + xOff,
			( cellCoord.mY * this->mCellHeight * 3 / 4 ) - ( this->mCellHeight * 0.5f ),
			this->mCellWidth,
			this->mCellHeight * 2.0f,
			position
		);
	}
	
	return this->GetRectPoint (
		( cellCoord.mX * this->mCellWidth ) + xOff,
		( cellCoord.mY * this->mCellHeight ) - ( this->mCellHeight* 0.5f ),
		this->mCellWidth,
		this->mCellHeight * 2.0f,
		position
	);
}

//----------------------------------------------------------------//
ZLRect ZLGridSpace::GetCellRect ( ZLGridCoord cellCoord ) const {
			
	float xOff = 0.0f;
	float yOff = 0.0f;

	if (( this->mShape & STAGGER_FLAG ) && ( cellCoord.mY & 0x01 )) {
		xOff = this->mCellWidth * 0.5f;
	}
	if ( this->mShape == AXIAL_HEX_SHAPE ) {
		xOff = this->mCellWidth * -0.5f * cellCoord.mY;
		yOff = (cellCoord.mY - 1) * this->mCellHeight * 0.25f;
	}
	
	ZLRect rect;
	
	rect.mXMin = ( cellCoord.mX * this->mCellWidth ) + xOff;
	rect.mYMin = cellCoord.mY * this->mCellHeight + yOff;
	
	rect.mXMax = rect.mXMin + this->mCellWidth;
	rect.mYMax = rect.mYMin + this->mCellHeight;
	
	return rect;
}

//----------------------------------------------------------------//
ZLRect ZLGridSpace::GetFrame () const {

	ZLRect rect;

	rect.mXMin = 0.0f;
	rect.mYMin = 0.0f;
	
	rect.mXMax = this->mWidth * this->mCellWidth;
	rect.mYMax = this->mHeight * this->mCellHeight;
	
	if ( this->mShape & STAGGER_FLAG ) {
		rect.mXMax += this->mCellWidth * 0.5f;
		rect.mYMin -= this->mCellHeight * 0.5f;
		rect.mYMax += this->mCellHeight * 0.5f;
	}
	if ( this->mShape == AXIAL_HEX_SHAPE ) {
		rect.mXMin -= this->mCellWidth * 0.5f * (this->mHeight - 1);
		rect.mYMax -= this->mCellHeight * 0.25f * (this->mHeight - 1);
	}
	
	return rect;
}

//----------------------------------------------------------------//
ZLRect ZLGridSpace::GetFrame ( ZLGridCoord c0, ZLGridCoord c1 ) const {

	ZLRect rect0 = this->GetCellRect ( c0 );
	ZLRect rect1 = this->GetCellRect ( c1 );
	
	rect0.Grow ( rect1 );
	
	if ( this->mShape & STAGGER_FLAG ) {
		rect0.mXMax += this->mCellWidth * 0.5f;
		rect0.mYMin -= this->mCellHeight * 0.5f;
		rect0.mYMax += this->mCellHeight * 0.5f;
	}
	if ( this->mShape == AXIAL_HEX_SHAPE ) {
		float rectHeight = rect0.mYMax - rect0.mYMin;
		rect0.mXMin -= this->mCellWidth * 0.5f * rectHeight;
		rect0.mYMax -= this->mCellHeight * 0.25f * rectHeight;
	}
	
	return rect0;
}

//----------------------------------------------------------------//
void ZLGridSpace::GetFrameInRect ( ZLRect rect, ZLGridCoord& c0, ZLGridCoord& c1, ZLRect maxSize ) const {

	rect.Bless ();
	maxSize.Bless ();

	if ( this->mShape == AXIAL_HEX_SHAPE ) {

		c0 = this->GetAxialHexCellCoord ( rect.mXMin, rect.mYMin );
		c1 = this->GetAxialHexCellCoord ( rect.mXMax, rect.mYMax );

		int rectHeight = c1.mY - c0.mY;

		// you need an extra column per two rows
		c1.mX = c1.mX +  ( rectHeight >> 1 );
	} else {
		c0.mX = ( int )floorf ( ( rect.mXMin / this->mCellWidth )  - ( maxSize.mXMax / 0.5f - 1.0f ) );
		c0.mY = ( int )floorf ( ( rect.mYMin / this->mCellHeight ) - ( maxSize.mYMax / 0.5f - 1.0f ) );

		c1.mX = ( int )floorf ( ( rect.mXMax / this->mCellWidth )  + ( maxSize.mXMin / -0.5f - 1.0f ) );
		c1.mY = ( int )floorf ( ( rect.mYMax / this->mCellHeight ) + ( maxSize.mYMin / -0.5f - 1.0f ) );
	}
	
	if ( this->mShape & STAGGER_FLAG ) {
		c0.mX--;
		c0.mY--;
		c1.mY++;
	}
	
	if ( this->mShape == OBLIQUE_SHAPE ) {
		c0.mX--;
	}
	
	if ( !( this->mRepeat & REPEAT_X )) {
		c0 = this->ClampX ( c0 );
		c1 = this->ClampX ( c1 );
	}
	
	if ( !( this->mRepeat & REPEAT_Y )) {
		c0 = this->ClampY ( c0 );
		c1 = this->ClampY ( c1 );
	}
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::GetHexCellCoord ( float x, float y, float a, float b ) const {

	// get the coord in tiles
	float xUnit = ( x / this->mCellWidth );
	float yUnit = ( y / this->mCellHeight );
	
	// get the y tile index
	int yTile = ( int )floorf ( yUnit );
	
	int stepRight = 0;
	int stepLeft = -1;
	
	// need to offset x into the previous tile if y is odd
	if ( yTile & 0x01 ) {
		xUnit -= 0.5f;
		
		stepRight = 1;
		stepLeft = 0;
	}
	
	// get the x tile index
	int xTile = ( int )floorf ( xUnit );
	
	// now get the local coord
	float xLocal = ( xUnit - ( float )xTile ) * ( a + b );
	float yLocal = (( yUnit - ( float )yTile ) * 2.0f ) - 1.0f;

	// check the position of the coord depending on tile quadrant
	// offset the tile index if out of bounds on any corner
	if ( xLocal < ( a + 1.0f )) {
	
		if ( yLocal < 0.0f ) {
			if ( yLocal < ( a - xLocal )) {
				xTile = xTile + stepLeft;
				yTile = yTile - 1;
			}
		}
		else if ( yLocal > ( xLocal - a )) {
			xTile = xTile + stepLeft;
			yTile = yTile + 1;
		}
	}
	else if ( xLocal > ( b - 1.0f )) {
		
		if ( yLocal < 0.0f ) {
			if ( yLocal < ( xLocal - b )) {
				xTile = xTile + stepRight;
				yTile = yTile - 1;
			}
		}
		else if ( yLocal > ( b - xLocal )) {
			xTile = xTile + stepRight;
			yTile = yTile + 1;
		}
	}
	
	ZLGridCoord cellCoord ( xTile, yTile );
	return cellCoord;
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::GetObliqueCellCoord ( float x, float y ) const {

	// get the coord in tiles
	float xUnit = ( x / this->mCellWidth );
	float yUnit = ( y / this->mCellHeight );
	
	// get the tile index
	int xTile = ( int )floorf ( xUnit );
	int yTile = ( int )floorf ( yUnit );
	
	// now get the local coord
	float xLocal = ( xUnit - ( float )xTile );
	float yLocal = ( yUnit - ( float )yTile );
	
	if ( yLocal > xLocal ) {
		xTile = xTile - 1;
	}
	
	ZLGridCoord cellCoord ( xTile, yTile );
	return cellCoord;
}

//----------------------------------------------------------------//
ZLVec2D ZLGridSpace::GetRectPoint ( float x, float y, float width, float height, u32 position ) const {

	ZLVec2D origin;

	origin.mX = x;
	origin.mY = y;
	
	switch ( position ) {
		case TILE_LEFT_TOP: {
			break;
		}
		case TILE_RIGHT_TOP: {
			origin.mX = origin.mX + width;
			break;
		}
		case TILE_LEFT_BOTTOM: {
			origin.mY = origin.mY + height;
			break;
		}
		case TILE_RIGHT_BOTTOM: {
			origin.mX = origin.mX + width;
			origin.mY = origin.mY + height;
			break;
		}
		case TILE_LEFT_CENTER: {
			origin.mY = origin.mY + ( height * 0.5f );
			break;
		}
		case TILE_RIGHT_CENTER: {
			origin.mX = origin.mX + width;
			origin.mY = origin.mY + ( height * 0.5f );
			break;
		}
		case TILE_TOP_CENTER: {
			origin.mX = origin.mX + ( width * 0.5f );
			break;
		}
		case TILE_BOTTOM_CENTER: {
			origin.mX = origin.mX + ( width * 0.5f );
			origin.mY = origin.mY + height;
			break;
		}
		case TILE_CENTER:
		default: {
			origin.mX = origin.mX + ( width * 0.5f );
			origin.mY = origin.mY + ( height * 0.5f );
		}
	};
	
	return origin;
}

//----------------------------------------------------------------//
ZLVec2D ZLGridSpace::GetTilePoint ( ZLGridCoord cellCoord, u32 position ) const {
	
	float xOff = 0.0f;

	if (( this->mShape & STAGGER_FLAG ) && ( cellCoord.mY & 0x01 )) {
		xOff = this->mCellWidth * 0.5f;
	}
	
	if ( this->mShape == AXIAL_HEX_SHAPE ) {
		xOff = this->mCellWidth * -0.5f * cellCoord.mY;
		return this->GetRectPoint (
			( cellCoord.mX * this->mCellWidth ) + this->mXOff + xOff,
			( cellCoord.mY * this->mCellHeight * 3 / 4 ) + this->mYOff,
			this->mTileWidth,
			this->mTileHeight,
			position
		);
	}
	
	return this->GetRectPoint (
		( cellCoord.mX * this->mCellWidth ) + this->mXOff + xOff,
		( cellCoord.mY * this->mCellHeight ) + this->mYOff,
		this->mTileWidth,
		this->mTileHeight,
		position
	);
}

//----------------------------------------------------------------//
ZLVec3D ZLGridSpace::GetTilePoint ( ZLGridCoord cellCoord, u32 position, float z ) const {

	ZLVec2D point = this->GetTilePoint ( cellCoord, position );
	return ZLVec3D ( point.mX, point.mY, z );
}

//----------------------------------------------------------------//
ZLRect ZLGridSpace::GetTileRect ( ZLGridCoord cellCoord ) const {

	float xOff = 0.0f;

	if (( this->mShape & STAGGER_FLAG ) && ( cellCoord.mY & 0x01 )) {
		xOff = this->mCellWidth * 0.5f;
	}

	ZLRect rect;
	
	rect.mXMin = ( cellCoord.mX * this->mCellWidth ) + this->mXOff + xOff;
	rect.mYMin = ( cellCoord.mY * this->mCellHeight ) + this->mYOff;
	
	rect.mXMax = rect.mXMin + this->mTileWidth;
	rect.mYMax = rect.mYMin + this->mTileHeight;
	
	return rect;
}

//----------------------------------------------------------------//
int ZLGridSpace::GetTotalCells () const {

	return this->mWidth * this->mHeight;
}

//----------------------------------------------------------------//
void ZLGridSpace::Init ( int width, int height, float tileWidth, float tileHeight ) {

	this->mXOff = 0.0f;
	this->mYOff = 0.0f;
	
	this->mWidth = width;
	this->mHeight = height;
	
	this->mTileWidth = tileWidth;
	this->mTileHeight = tileHeight;
	
	this->mCellWidth = tileWidth;
	this->mCellHeight = tileHeight;
}

//----------------------------------------------------------------//
bool ZLGridSpace::IsValidCoord ( ZLGridCoord cellCoord ) const {

	if ( cellCoord.mX < 0 ) return false;
	if ( cellCoord.mY < 0 ) return false;

	if ( cellCoord.mX >= this->mWidth ) return false;
	if ( cellCoord.mY >= this->mHeight ) return false;
	
	return true;
}

//----------------------------------------------------------------//
ZLGridSpace::ZLGridSpace () :
	mXOff ( 0.0f ),
	mYOff ( 0.0f ),
	mCellWidth ( 1.0f ),
	mCellHeight ( 1.0f ),
	mTileWidth ( 0.0f ),
	mTileHeight ( 0.0f ),
	mWidth ( 0 ),
	mHeight ( 0 ),
	mShape ( RECT_SHAPE ),
	mRepeat ( 0 ) {
}

//----------------------------------------------------------------//
ZLGridSpace::~ZLGridSpace () {
}

//----------------------------------------------------------------//
void ZLGridSpace::OnResize () {
}

//----------------------------------------------------------------//
ZLGridCoord ZLGridSpace::WrapCellCoord ( int xCell, int yCell ) const {

	ZLGridCoord wrap;
	
	wrap.mX = xCell % this->mWidth;
	if ( wrap.mX < 0 ) wrap.mX += this->mWidth;
	
	wrap.mY = yCell % this->mHeight;
	if ( wrap.mY < 0 ) wrap.mY += this->mHeight;
	
	return wrap;
}

//----------------------------------------------------------------//
ZLVec2D ZLGridSpace::WorldToCell ( ZLGridCoord cellCoord, ZLVec2D loc ) const {
	
	ZLVec2D result = loc;
	result.mX = ( loc.mX / this->mCellWidth ) - ( float )cellCoord.mX;
	result.mY = ( loc.mY / this->mCellHeight ) - ( float )cellCoord.mY;
	
	return result;
}
