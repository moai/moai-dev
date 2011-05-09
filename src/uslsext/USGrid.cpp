// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USGrid.h>

//================================================================//
// USCellCoord
//================================================================//

//----------------------------------------------------------------//
u32 USCellCoord::GetID () {

	return (( u32 )this->mX & 0xff ) | ((( u32 )this->mY & 0xff ) << 16 );
}

//----------------------------------------------------------------//
bool USCellCoord::IsEqual ( USCellCoord cellCoord ) {

	if ( this->mX != cellCoord.mX ) return false;
	if ( this->mY != cellCoord.mY ) return false;
	
	return true;
}

//----------------------------------------------------------------//
USCellCoord::USCellCoord () {
	mX = -1;
	mY = -1;
}

//----------------------------------------------------------------//
USCellCoord::~USCellCoord () {
}

//================================================================//
// USGridSpace
//================================================================//

//----------------------------------------------------------------//
USVec2D USGridSpace::CellToWorld ( USCellCoord cellCoord, USVec2D loc ) const {

	USVec2D result;
	result.mX = ( loc.mX + ( float )cellCoord.mX ) * this->mCellWidth;
	result.mY = ( loc.mY + ( float )cellCoord.mY ) * this->mCellHeight;
	
	return result;
}

//----------------------------------------------------------------//
USCellCoord USGridSpace::Clamp ( USCellCoord cellCoord ) const {

	int xMax = this->mWidth - 1;
	int yMax = this->mHeight - 1;

	USCellCoord result;
	
	result.mX = ( cellCoord.mX < 0 ) ? 0 : cellCoord.mX;
	result.mX = ( cellCoord.mX > xMax ) ? xMax : result.mX;

	result.mY = ( cellCoord.mY < 0 ) ? 0 : cellCoord.mY;
	result.mY = ( cellCoord.mY > yMax ) ? yMax : result.mY;

	return result;
}

//----------------------------------------------------------------//
USCellCoord USGridSpace::ClampX ( USCellCoord cellCoord ) const {

	int xMax = this->mWidth - 1;

	USCellCoord result;
	
	result.mX = ( cellCoord.mX < 0 ) ? 0 : cellCoord.mX;
	result.mX = ( cellCoord.mX > xMax ) ? xMax : result.mX;
	
	result.mY = cellCoord.mY;

	return result;
}

//----------------------------------------------------------------//
USCellCoord USGridSpace::ClampY ( USCellCoord cellCoord ) const {
	
	int yMax = this->mHeight - 1;

	USCellCoord result;
	
	result.mY = ( cellCoord.mY < 0 ) ? 0 : cellCoord.mY;
	result.mY = ( cellCoord.mY > yMax ) ? yMax : result.mY;

	result.mX = cellCoord.mX;

	return result;
}

//----------------------------------------------------------------//
USRect USGridSpace::GetBounds () const {

	USRect rect;

	rect.mXMin = 0.0f;
	rect.mYMin = 0.0f;
	
	rect.mXMax = this->mWidth * this->mCellWidth;
	rect.mYMax = this->mHeight * this->mCellHeight;
	
	return rect;
}

//----------------------------------------------------------------//
USRect USGridSpace::GetBounds ( USCellCoord c0, USCellCoord c1 ) const {

	USRect rect0 = this->GetCellRect ( c0 );
	USRect rect1 = this->GetCellRect ( c1 );
	
	rect0.Grow ( rect1 );
	
	return rect0;
}

//----------------------------------------------------------------//
USMatrix2D USGridSpace::GetGridToWorldMtx () const {

	USMatrix2D mtx;
	mtx.Scale ( 1.0f, -1.0f );
	return mtx;
}

//----------------------------------------------------------------//
int USGridSpace::GetCellAddr ( USCellCoord cellCoord ) const {

	cellCoord.mX = cellCoord.mX % this->mWidth;
	if ( cellCoord.mX < 0 ) cellCoord.mX += this->mWidth;

	cellCoord.mY = cellCoord.mY % this->mHeight;
	if ( cellCoord.mY < 0 ) cellCoord.mY += this->mHeight;

	return ( cellCoord.mY * this->mWidth ) + cellCoord.mX;
}

//----------------------------------------------------------------//
USCellCoord USGridSpace::GetCellCoord ( int cellAddr ) const {
	
	USCellCoord cellCoord;
	
	cellCoord.mX = ( cellAddr % this->mWidth );
	cellCoord.mY = (( int )( cellAddr / this->mWidth ) % this->mHeight );
	
	return cellCoord;
}

//----------------------------------------------------------------//
USCellCoord USGridSpace::GetCellCoord ( USVec2D loc ) const {
	
	return this->GetCellCoord ( loc.mX, loc.mY );
}

//----------------------------------------------------------------//
USCellCoord USGridSpace::GetCellCoord ( float x, float y ) const {

	USCellCoord cellCoord;
	
	cellCoord.mX = ( int )( x / this->mCellWidth );
	cellCoord.mY = ( int )( y / this->mCellHeight );
	
	if ( x < 0.0f ) cellCoord.mX--;
	if ( y < 0.0f ) cellCoord.mY--;
	
	return cellCoord;
}

//----------------------------------------------------------------//
USCellCoord USGridSpace::GetCellCoord ( int xCell, int yCell ) const {

	USCellCoord cellCoord;
	cellCoord.mX = xCell;
	cellCoord.mY = yCell;
	return cellCoord;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::GetCellPoint ( USCellCoord cellCoord, u32 position ) const {
	
	return this->GetRectPoint (
		cellCoord.mX * this->mCellWidth,
		cellCoord.mY * this->mCellHeight,
		this->mCellWidth,
		this->mCellHeight,
		position
	);
}

//----------------------------------------------------------------//
USRect USGridSpace::GetCellRect ( USCellCoord cellCoord ) const {

	USRect rect;

	rect.mXMin = cellCoord.mX * this->mCellWidth;
	rect.mYMin = cellCoord.mY * this->mCellHeight;
	
	rect.mXMax = rect.mXMin + this->mCellWidth;
	rect.mYMax = rect.mYMin + this->mCellHeight;
	
	return rect;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::GetRectPoint ( float x, float y, float width, float height, u32 position ) const {

	USVec2D origin;

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
USVec2D USGridSpace::GetTilePoint ( USCellCoord cellCoord, u32 position ) const {

	return this->GetRectPoint (
		( cellCoord.mX * this->mCellWidth ) + this->mXOff,
		( cellCoord.mY * this->mCellHeight ) + this->mYOff,
		this->mTileWidth,
		this->mTileHeight,
		position
	);
}

//----------------------------------------------------------------//
USRect USGridSpace::GetTileRect ( USCellCoord cellCoord ) const {

	USRect rect;
	
	rect.mXMin = ( cellCoord.mX * this->mCellWidth ) + this->mXOff;
	rect.mYMin = ( cellCoord.mY * this->mCellHeight ) + this->mXOff;
	
	rect.mXMax = rect.mXMin + this->mTileWidth;
	rect.mYMax = rect.mYMin + this->mTileHeight;
	
	return rect;
}

//----------------------------------------------------------------//
int USGridSpace::GetTotalCells () const {

	return this->mWidth * this->mHeight;
}

//----------------------------------------------------------------//
USMatrix2D USGridSpace::GetWorldToGridMtx () const {

	USMatrix2D mtx;
	mtx.Scale ( 1.0f, -1.0f );
	return mtx;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::GridToWorld ( USVec2D loc ) const {

	loc.mY = -loc.mY;
	return loc;
}

//----------------------------------------------------------------//
bool USGridSpace::IsValidCoord ( USCellCoord cellCoord ) const {

	if ( cellCoord.mX < 0 ) return false;
	if ( cellCoord.mY < 0 ) return false;

	if ( cellCoord.mX >= this->mWidth ) return false;
	if ( cellCoord.mY >= this->mHeight ) return false;
	
	return true;
}

//----------------------------------------------------------------//
void USGridSpace::SerializeIn ( USLuaState& state ) {
	
	this->mXOff			= state.GetField ( -1, "mXOff", this->mXOff );
	this->mYOff			= state.GetField ( -1, "mYOff", this->mYOff );
	
	this->mCellWidth	= state.GetField ( -1, "mCellWidth", this->mCellWidth );
	this->mCellHeight	= state.GetField ( -1, "mCellHeight", this->mCellHeight );
	
	this->mTileWidth	= state.GetField ( -1, "mTileWidth", this->mTileWidth );
	this->mTileHeight	= state.GetField ( -1, "mTileHeight", this->mTileHeight );
	
	this->mWidth		= state.GetField ( -1, "mWidth", this->mWidth );
	this->mHeight		= state.GetField ( -1, "mHeight", this->mHeight );
}

//----------------------------------------------------------------//
void USGridSpace::SerializeOut ( USLuaState& state ) {
	
	state.SetField ( -1, "mXOff", this->mXOff );
	state.SetField ( -1, "mYOff", this->mYOff );
	
	state.SetField ( -1, "mCellWidth", this->mCellWidth );
	state.SetField ( -1, "mCellHeight", this->mCellHeight );
	
	state.SetField ( -1, "mTileWidth", this->mTileWidth );
	state.SetField ( -1, "mTileHeight", this->mTileHeight );
	
	state.SetField ( -1, "mWidth", this->mWidth );
	state.SetField ( -1, "mHeight", this->mHeight );
}

//----------------------------------------------------------------//
USGridSpace::USGridSpace () :
	mXOff ( 0.0f ),
	mYOff ( 0.0f ),
	mCellWidth ( 1.0f ),
	mCellHeight ( 1.0f ),
	mTileWidth ( 0.0f ),
	mTileHeight ( 0.0f ),
	mWidth ( 0 ),
	mHeight ( 0 ) {
}

//----------------------------------------------------------------//
USGridSpace::~USGridSpace () {
}

//----------------------------------------------------------------//
USCellCoord USGridSpace::WrapCellCoord ( const USCellCoord coord ) const {

	USCellCoord wrap;
	
	wrap.mX = coord.mX % this->mWidth;
	if ( wrap.mX < 0 ) wrap.mX += this->mWidth;
	
	wrap.mY = coord.mY % this->mHeight;
	if ( wrap.mY < 0 ) wrap.mY += this->mHeight;
	
	return wrap;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::WorldToCell ( USCellCoord cellCoord, USVec2D loc ) const {
	
	USVec2D result = loc;
	result.mX = ( loc.mX / this->mCellWidth ) - ( float )cellCoord.mX;
	result.mY = ( loc.mY / this->mCellHeight ) - ( float )cellCoord.mY;
	
	return result;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::WorldToGrid ( USVec2D loc ) const {

	loc.mY = -loc.mY;
	return loc;
}
