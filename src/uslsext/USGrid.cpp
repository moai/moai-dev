// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USGrid.h>

//================================================================//
// USTileCoord
//================================================================//

//----------------------------------------------------------------//
u32 USTileCoord::GetID () {

	return (( u32 )this->mX & 0xff ) | ((( u32 )this->mY & 0xff ) << 16 );
}

//----------------------------------------------------------------//
bool USTileCoord::IsEqual ( USTileCoord& tileCoord ) {

	if ( this->mX != tileCoord.mX ) return false;
	if ( this->mY != tileCoord.mY ) return false;
	
	return true;
}

//----------------------------------------------------------------//
USTileCoord::USTileCoord () {
	mX = -1;
	mY = -1;
}

//----------------------------------------------------------------//
USTileCoord::~USTileCoord () {
}

//================================================================//
// USGridSpace
//================================================================//

//----------------------------------------------------------------//
void USGridSpace::Clamp ( USTileCoord& tileCoord ) {

	this->ClampX ( tileCoord );
	this->ClampY ( tileCoord );
}

//----------------------------------------------------------------//
void USGridSpace::ClampX ( USTileCoord& tileCoord ) {

	int xMax = this->mWidth - 1;
	
	if ( tileCoord.mX < 0 ) tileCoord.mX = 0;
	if ( tileCoord.mX > xMax ) tileCoord.mX = xMax;
}

//----------------------------------------------------------------//
void USGridSpace::ClampY ( USTileCoord& tileCoord ) {

	int yMax = this->mHeight - 1;
		
	if ( tileCoord.mY < 0 ) tileCoord.mY = 0;
	if ( tileCoord.mY > yMax ) tileCoord.mY = yMax;
}

//----------------------------------------------------------------//
USRect USGridSpace::GetBounds () {

	USRect rect;

	rect.mXMin = 0.0f;
	rect.mYMin = 0.0f;
	
	rect.mXMax = this->mWidth * this->mTileWidth;
	rect.mYMax = this->mHeight * this->mTileHeight;
	
	return rect;
}

//----------------------------------------------------------------//
USRect USGridSpace::GetBounds ( USTileCoord& c0, USTileCoord& c1 ) {

	USRect rect0 = this->GetTileRect ( c0 );
	USRect rect1 = this->GetTileRect ( c1 );
	
	rect0.Grow ( rect1 );
	
	return rect0;
}

//----------------------------------------------------------------//
USMatrix2D USGridSpace::GetGridToWorldMtx () {

	USMatrix2D mtx;
	mtx.Scale ( 1.0f, -1.0f );
	return mtx;
}

//----------------------------------------------------------------//
int USGridSpace::GetTileAddr ( USVec2D& loc ) {

	USTileCoord coord = this->GetTileCoord ( loc );
	return this->GetTileAddr ( coord );
}

//----------------------------------------------------------------//
int USGridSpace::GetTileAddr ( int xTile, int yTile ) {

	xTile = xTile % this->mWidth;
	if ( xTile < 0 ) xTile += this->mWidth;

	yTile = yTile % this->mHeight;
	if ( yTile < 0 ) yTile += this->mHeight;

	return ( yTile * this->mWidth ) + xTile;
}

//----------------------------------------------------------------//
int USGridSpace::GetTileAddr ( USTileCoord& tileCoord ) {

	return this->GetTileAddr ( tileCoord.mX, tileCoord.mY );
}

//----------------------------------------------------------------//
int USGridSpace::GetTileAddr ( USTileCoord tileCoord, int xOff, int yOff ) {

	return this->GetTileAddr ( tileCoord.mX + xOff, tileCoord.mY + yOff );
}

//----------------------------------------------------------------//
USTileCoord USGridSpace::GetTileCoord ( int tileAddr ) {
	
	USTileCoord tileCoord;
	
	tileCoord.mX = ( tileAddr % this->mWidth );
	tileCoord.mY = (( int )( tileAddr / this->mWidth ) % this->mHeight );
	
	return tileCoord;
}

//----------------------------------------------------------------//
USTileCoord USGridSpace::GetTileCoord ( USVec2D& loc ) {
	
	return this->GetTileCoord ( loc.mX, loc.mY );
}

//----------------------------------------------------------------//
USTileCoord USGridSpace::GetTileCoord ( float x, float y ) {

	USTileCoord tileCoord;
	
	tileCoord.mX = ( int )( x / this->mTileWidth );
	tileCoord.mY = ( int )( y / this->mTileHeight );
	
	if ( x < 0.0f ) tileCoord.mX--;
	if ( y < 0.0f ) tileCoord.mY--;
	
	return tileCoord;
}

//----------------------------------------------------------------//
USTileCoord USGridSpace::GetTileCoord ( int xTile, int yTile ) {

	USTileCoord tileCoord;
	tileCoord.mX = xTile;
	tileCoord.mY = yTile;
	return tileCoord;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::GetTilePoint ( int tileAddr, u32 position ) {

	USTileCoord tileCoord = this->GetTileCoord ( tileAddr );
	return this->GetTilePoint ( tileCoord.mX, tileCoord.mY, position );
}

//----------------------------------------------------------------//
USVec2D USGridSpace::GetTilePoint ( int xTile, int yTile, u32 position ) {

	USVec2D origin;

	origin.mX = xTile * this->mTileWidth;
	origin.mY = yTile * this->mTileHeight;
	
	switch ( position ) {
		case TILE_LEFT_TOP: {
			break;
		}
		case TILE_RIGHT_TOP: {
			origin.mX = origin.mX + this->mTileWidth;
			break;
		}
		case TILE_LEFT_BOTTOM: {
			origin.mY = origin.mY + this->mTileHeight;
			break;
		}
		case TILE_RIGHT_BOTTOM: {
			origin.mX = origin.mX + this->mTileWidth;
			origin.mY = origin.mY + this->mTileHeight;
			break;
		}
		case TILE_LEFT_CENTER: {
			origin.mY = origin.mY + ( this->mTileHeight * 0.5f );
			break;
		}
		case TILE_RIGHT_CENTER: {
			origin.mX = origin.mX + this->mTileWidth;
			origin.mY = origin.mY + ( this->mTileHeight * 0.5f );
			break;
		}
		case TILE_TOP_CENTER: {
			origin.mX = origin.mX + ( this->mTileWidth * 0.5f );
			break;
		}
		case TILE_BOTTOM_CENTER: {
			origin.mX = origin.mX + ( this->mTileWidth * 0.5f );
			origin.mY = origin.mY + this->mTileHeight;
			break;
		}
		case TILE_CENTER:
		default: {
			origin.mX = origin.mX + ( this->mTileWidth * 0.5f );
			origin.mY = origin.mY + ( this->mTileHeight * 0.5f );
		}
	};
	
	return origin;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::GetTilePoint ( USTileCoord& tileCoord, u32 position ) {
	
	return this->GetTilePoint ( tileCoord.mX, tileCoord.mY, position );
}

//----------------------------------------------------------------//
USRect USGridSpace::GetTileRect ( int tileAddr ) {

	USTileCoord tileCoord = this->GetTileCoord ( tileAddr );
	return this->GetTileRect ( tileCoord.mX, tileCoord.mY );
}

//----------------------------------------------------------------//
USRect USGridSpace::GetTileRect ( int xTile, int yTile ) {

	USRect rect;

	rect.mXMin = xTile * this->mTileWidth;
	rect.mYMin = yTile * this->mTileHeight;
	
	rect.mXMax = rect.mXMin + this->mTileWidth;
	rect.mYMax = rect.mYMin + this->mTileHeight;
	
	return rect;
}

//----------------------------------------------------------------//
USRect USGridSpace::GetTileRect ( USTileCoord& tileCoord ) {
	
	return this->GetTileRect ( tileCoord.mX, tileCoord.mY );
}

//----------------------------------------------------------------//
int USGridSpace::GetTotalTiles () {

	return this->mWidth * this->mHeight;
}

//----------------------------------------------------------------//
USMatrix2D USGridSpace::GetWorldToGridMtx () {

	USMatrix2D mtx;
	mtx.Scale ( 1.0f, -1.0f );
	return mtx;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::GridToWorld ( USVec2D loc ) {

	loc.mY = -loc.mY;
	return loc;
}

//----------------------------------------------------------------//
bool USGridSpace::IsValidCoord ( int xTile, int yTile ) {

	if ( xTile < 0 ) return false;
	if ( yTile < 0 ) return false;

	if ( xTile >= this->mWidth ) return false;
	if ( yTile >= this->mHeight ) return false;
	
	return true;
}

//----------------------------------------------------------------//
bool USGridSpace::IsValidCoord ( USTileCoord& tileCoord ) {
	
	return this->IsValidCoord ( tileCoord.mX, tileCoord.mY );
}

//----------------------------------------------------------------//
bool USGridSpace::IsValidCoord ( USVec2D& loc ) {

	if ( loc.mX < 0.0f ) return false;
	if ( loc.mY < 0.0f ) return false;

	if ( loc.mX >= ( this->mWidth * this->mTileWidth )) return false;
	if ( loc.mY >= ( this->mHeight * this->mTileHeight )) return false;
	
	return true;
}

//----------------------------------------------------------------//
void USGridSpace::SerializeIn ( USLuaState& state ) {
	
	this->mTileWidth	= state.GetField ( -1, "mTileWidth", this->mTileWidth );
	this->mTileHeight	= state.GetField ( -1, "mTileHeight", this->mTileHeight );
	
	this->mWidth		= state.GetField ( -1, "mWidth", this->mWidth );
	this->mHeight		= state.GetField ( -1, "mHeight", this->mHeight );
}

//----------------------------------------------------------------//
void USGridSpace::SerializeOut ( USLuaState& state ) {
	
	state.SetField ( -1, "mTileWidth", this->mTileWidth );
	state.SetField ( -1, "mTileHeight", this->mTileHeight );
	
	state.SetField ( -1, "mWidth", this->mWidth );
	state.SetField ( -1, "mHeight", this->mHeight );
}

//----------------------------------------------------------------//
void USGridSpace::Snap ( USVec2D& loc ) {
	
	USTileCoord coord = this->GetTileCoord ( loc );
	
	loc.mX = coord.mX * this->mTileWidth;
	loc.mY = coord.mY * this->mTileHeight;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::TileToWorld ( USTileCoord& tileCoord, USVec2D& loc ) {

	USVec2D result;
	result.mX = ( loc.mX + ( float )tileCoord.mX ) * this->mTileWidth;
	result.mY = ( loc.mY + ( float )tileCoord.mY ) * this->mTileHeight;
	
	return result;
}

//----------------------------------------------------------------//
USGridSpace::USGridSpace () :
	mTileWidth ( 1.0f ),
	mTileHeight ( 1.0f ),
	mWidth ( 0 ),
	mHeight ( 0 ) {
}

//----------------------------------------------------------------//
USGridSpace::~USGridSpace () {
}

//----------------------------------------------------------------//
USTileCoord USGridSpace::WrapCellCoord ( const USTileCoord& coord ) {

	USTileCoord wrap;
	
	wrap.mX = coord.mX % this->mWidth;
	if ( wrap.mX < 0 ) wrap.mX += this->mWidth;
	
	wrap.mY = coord.mY % this->mHeight;
	if ( wrap.mY < 0 ) wrap.mY += this->mHeight;
	
	return wrap;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::WorldToTile ( USTileCoord& tileCoord, USVec2D& loc ) {
	
	USVec2D result = loc;
	result.mX = ( loc.mX / this->mTileWidth ) - ( float )tileCoord.mX;
	result.mY = ( loc.mY / this->mTileHeight ) - ( float )tileCoord.mY;
	
	return result;
}

//----------------------------------------------------------------//
USVec2D USGridSpace::WorldToGrid ( USVec2D loc ) {

	loc.mY = -loc.mY;
	return loc;
}
