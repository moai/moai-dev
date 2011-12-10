// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGridSpace.h>

//================================================================//
// MOAICellCoord
//================================================================//

//----------------------------------------------------------------//
u32 MOAICellCoord::GetID () {

	return (( u32 )this->mX & 0xff ) | ((( u32 )this->mY & 0xff ) << 16 );
}

//----------------------------------------------------------------//
bool MOAICellCoord::IsEqual ( MOAICellCoord cellCoord ) {

	if ( this->mX != cellCoord.mX ) return false;
	if ( this->mY != cellCoord.mY ) return false;
	
	return true;
}

//----------------------------------------------------------------//
MOAICellCoord::MOAICellCoord () {
	mX = -1;
	mY = -1;
}

//----------------------------------------------------------------//
MOAICellCoord::~MOAICellCoord () {
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	cellAddrToCoord
	@text	Returns the coordinate of a cell given an address.

	@in		MOAIGridSpace self
	@in		number xTile
	@in		number yTile
	@out	number cellAddr
*/
int MOAIGridSpace::_cellAddrToCoord	( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UN" )

	u32 addr = state.GetValue < u32 >( 2, 1 ) - 1;
	
	MOAICellCoord coord = self->GetCellCoord ( addr );
	
	state.Push ( coord.mX + 1 );
	state.Push ( coord.mY + 1 );
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getCellAddr
	@text	Returns the address of a cell given a coordinate (in tiles).

	@in		MOAIGridSpace self
	@in		number xTile
	@in		number yTile
	@out	number cellAddr
*/
int MOAIGridSpace::_getCellAddr ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	int xTile		= state.GetValue < int >( 2, 1 ) - 1;
	int yTile		= state.GetValue < int >( 3, 1 ) - 1;
	
	lua_pushnumber ( state, self->GetCellAddr ( xTile, yTile ) + 1 );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getSize
	@text	Returns the dimensions of the grid (in tiles).

	@in		MOAIGridSpace self
	@out	number width
	@out	number height
*/
int MOAIGridSpace::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "U" )

	state.Push ( self->mWidth );
	state.Push ( self->mHeight );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getTileLoc
	@text	Returns the grid space coordinate of the tile. The optional 'position'
			flag determines the location of the coordinate within the tile.

	@in		MOAIGridSpace self
	@in		number xTile
	@in		number yTile
	@opt	number position		See MOAIGridSpace for list of positions. Default it MOAIGridSpace.TILE_CENTER.
	@out	number x
	@out	number y
*/
int MOAIGridSpace::_getTileLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )
	
	MOAICellCoord coord;
	
	coord.mX		= state.GetValue < int >( 2, 1 ) - 1;
	coord.mY		= state.GetValue < int >( 3, 1 ) - 1;
	u32 position	= state.GetValue < u32 >( 4, MOAIGridSpace::TILE_CENTER );
	
	USVec2D loc = self->GetTilePoint ( coord, position );
	state.Push ( loc.mX );
	state.Push ( loc.mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@name	locToCellAddr
	@text	Returns the address of a cell given a a coordinate in grid space.

	@in		MOAIGridSpace self
	@in		number x
	@in		number y
	@out	number cellAddr
*/
int MOAIGridSpace::_locToCellAddr ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0 );
	loc.mY = state.GetValue < float >( 3, 0 );
	
	MOAICellCoord coord;
	coord = self->GetCellCoord ( loc );

	state.Push ( self->GetCellAddr ( coord ) + 1 );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	locToCoord
	@text	Transforms a coordinate in grid space into a tile index.

	@in		MOAIGridSpace self
	@in		number x
	@in		number y
	@out	number xTile
	@out	number yTile
*/
int MOAIGridSpace::_locToCoord ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0 );
	loc.mY = state.GetValue < float >( 3, 0 );
	
	MOAICellCoord coord;
	coord = self->GetCellCoord ( loc );

	state.Push ( coord.mX + 1 );
	state.Push ( coord.mY + 1 );
	return 2;
}

//----------------------------------------------------------------//
/**	@name	setSize
	@text	Initializes dimensions of grid and reserves storage for tiles.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@in		number cellWidth	Default value is 1.
	@in		number cellHeight	Default value is 1.
	@opt	number xOff			X offset of the tile from the cell.
	@opt	number yOff			Y offset of the tile from the cell.
	@opt	number tileWidth	Default value is cellWidth.
	@opt	number tileHeight	Default value is cellHeight.
	@out	nil
*/
int MOAIGridSpace::_setSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	u32 width			= state.GetValue < u32 >( 2, 0 );
	u32 height			= state.GetValue < u32 >( 3, 0 );
	
	float cellWidth		= state.GetValue < float >( 4, 1.0f );
	float cellHeight	= state.GetValue < float >( 5, 1.0f );

	float xOff			= state.GetValue < float >( 6, 0.0f );
	float yOff			= state.GetValue < float >( 7, 0.0f );
	
	float tileWidth		= state.GetValue < float >( 8, cellWidth );
	float tileHeight	= state.GetValue < float >( 9, cellHeight );
	
	self->SetWidth ( width );
	self->SetHeight ( height );
	
	self->SetCellWidth ( cellWidth );
	self->SetCellHeight ( cellHeight );
	
	self->SetXOff ( xOff );
	self->SetYOff ( yOff );
	
	self->SetTileWidth ( tileWidth );
	self->SetTileHeight ( tileHeight );
	
	self->OnResize ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	wrapCoord
	@text	Wraps a tile index to the range of the grid.

	@in		MOAIGridSpace self
	@in		number xTile
	@in		number yTile
	@out	number xTile
	@out	number yTile
*/
int MOAIGridSpace::_wrapCoord ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )
	
	MOAICellCoord coord;
	coord.mX = state.GetValue < int >( 2, 1 ) - 1;
	coord.mY = state.GetValue < int >( 3, 1 ) - 1;

	self->WrapCellCoord ( coord );
	
	state.Push ( coord.mX + 1 );
	state.Push ( coord.mY + 1 );
	return 2;
}

//================================================================//
// MOAIGridSpace
//================================================================//

//----------------------------------------------------------------//
USVec2D MOAIGridSpace::CellToWorld ( MOAICellCoord cellCoord, USVec2D loc ) const {

	USVec2D result;
	result.mX = ( loc.mX + ( float )cellCoord.mX ) * this->mCellWidth;
	result.mY = ( loc.mY + ( float )cellCoord.mY ) * this->mCellHeight;
	
	return result;
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::Clamp ( MOAICellCoord cellCoord ) const {

	int xMax = this->mWidth - 1;
	int yMax = this->mHeight - 1;

	MOAICellCoord result;
	
	result.mX = ( cellCoord.mX < 0 ) ? 0 : cellCoord.mX;
	result.mX = ( cellCoord.mX > xMax ) ? xMax : result.mX;

	result.mY = ( cellCoord.mY < 0 ) ? 0 : cellCoord.mY;
	result.mY = ( cellCoord.mY > yMax ) ? yMax : result.mY;

	return result;
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::ClampX ( MOAICellCoord cellCoord ) const {

	int xMax = this->mWidth - 1;

	MOAICellCoord result;
	
	result.mX = ( cellCoord.mX < 0 ) ? 0 : cellCoord.mX;
	result.mX = ( cellCoord.mX > xMax ) ? xMax : result.mX;
	
	result.mY = cellCoord.mY;

	return result;
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::ClampY ( MOAICellCoord cellCoord ) const {
	
	int yMax = this->mHeight - 1;

	MOAICellCoord result;
	
	result.mY = ( cellCoord.mY < 0 ) ? 0 : cellCoord.mY;
	result.mY = ( cellCoord.mY > yMax ) ? yMax : result.mY;

	result.mX = cellCoord.mX;

	return result;
}

//----------------------------------------------------------------//
USRect MOAIGridSpace::GetBounds () const {

	USRect rect;

	rect.mXMin = 0.0f;
	rect.mYMin = 0.0f;
	
	rect.mXMax = this->mWidth * this->mCellWidth;
	rect.mYMax = this->mHeight * this->mCellHeight;
	
	return rect;
}

//----------------------------------------------------------------//
USRect MOAIGridSpace::GetBounds ( MOAICellCoord c0, MOAICellCoord c1 ) const {

	USRect rect0 = this->GetCellRect ( c0 );
	USRect rect1 = this->GetCellRect ( c1 );
	
	rect0.Grow ( rect1 );
	
	return rect0;
}

//----------------------------------------------------------------//
USMatrix3x3 MOAIGridSpace::GetGridToWorldMtx () const {

	USMatrix3x3 mtx;
	mtx.Scale ( 1.0f, -1.0f );
	return mtx;
}

//----------------------------------------------------------------//
int MOAIGridSpace::GetCellAddr ( MOAICellCoord cellCoord ) const {

	return this->GetCellAddr ( cellCoord.mX, cellCoord.mY );
}

//----------------------------------------------------------------//
int MOAIGridSpace::GetCellAddr ( int xCell, int yCell ) const {

	if ( !( this->mWidth && this->mHeight )) return 0;

	xCell = xCell % this->mWidth;
	if ( xCell < 0 ) xCell += this->mWidth;

	yCell = yCell % this->mHeight;
	if ( yCell < 0 ) yCell += this->mHeight;

	return ( yCell* this->mWidth ) + xCell;
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::GetCellCoord ( int cellAddr ) const {
	
	MOAICellCoord cellCoord;
	
	cellCoord.mX = ( cellAddr % this->mWidth );
	cellCoord.mY = (( int )( cellAddr / this->mWidth ) % this->mHeight );
	
	return cellCoord;
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::GetCellCoord ( USVec2D loc ) const {
	
	return this->GetCellCoord ( loc.mX, loc.mY );
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::GetCellCoord ( float x, float y ) const {

	MOAICellCoord cellCoord;
	
	cellCoord.mX = ( int )( x / this->mCellWidth );
	cellCoord.mY = ( int )( y / this->mCellHeight );
	
	if ( x < 0.0f ) cellCoord.mX--;
	if ( y < 0.0f ) cellCoord.mY--;
	
	return cellCoord;
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::GetCellCoord ( int xCell, int yCell ) const {

	MOAICellCoord cellCoord;
	cellCoord.mX = xCell;
	cellCoord.mY = yCell;
	return cellCoord;
}

//----------------------------------------------------------------//
USVec2D MOAIGridSpace::GetCellPoint ( MOAICellCoord cellCoord, u32 position ) const {
	
	return this->GetRectPoint (
		cellCoord.mX * this->mCellWidth,
		cellCoord.mY * this->mCellHeight,
		this->mCellWidth,
		this->mCellHeight,
		position
	);
}

//----------------------------------------------------------------//
USRect MOAIGridSpace::GetCellRect ( MOAICellCoord cellCoord ) const {

	USRect rect;

	rect.mXMin = cellCoord.mX * this->mCellWidth;
	rect.mYMin = cellCoord.mY * this->mCellHeight;
	
	rect.mXMax = rect.mXMin + this->mCellWidth;
	rect.mYMax = rect.mYMin + this->mCellHeight;
	
	return rect;
}

//----------------------------------------------------------------//
USVec2D MOAIGridSpace::GetRectPoint ( float x, float y, float width, float height, u32 position ) const {

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
USVec2D MOAIGridSpace::GetTilePoint ( MOAICellCoord cellCoord, u32 position ) const {

	return this->GetRectPoint (
		( cellCoord.mX * this->mCellWidth ) + this->mXOff,
		( cellCoord.mY * this->mCellHeight ) + this->mYOff,
		this->mTileWidth,
		this->mTileHeight,
		position
	);
}

//----------------------------------------------------------------//
USRect MOAIGridSpace::GetTileRect ( MOAICellCoord cellCoord ) const {

	USRect rect;
	
	rect.mXMin = ( cellCoord.mX * this->mCellWidth ) + this->mXOff;
	rect.mYMin = ( cellCoord.mY * this->mCellHeight ) + this->mXOff;
	
	rect.mXMax = rect.mXMin + this->mTileWidth;
	rect.mYMax = rect.mYMin + this->mTileHeight;
	
	return rect;
}

//----------------------------------------------------------------//
int MOAIGridSpace::GetTotalCells () const {

	return this->mWidth * this->mHeight;
}

//----------------------------------------------------------------//
USMatrix3x3 MOAIGridSpace::GetWorldToGridMtx () const {

	USMatrix3x3 mtx;
	mtx.Scale ( 1.0f, -1.0f );
	return mtx;
}

//----------------------------------------------------------------//
USVec2D MOAIGridSpace::GridToWorld ( USVec2D loc ) const {

	loc.mY = -loc.mY;
	return loc;
}

//----------------------------------------------------------------//
void MOAIGridSpace::Init ( int width, int height, float tileWidth, float tileHeight ) {

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
bool MOAIGridSpace::IsValidCoord ( MOAICellCoord cellCoord ) const {

	if ( cellCoord.mX < 0 ) return false;
	if ( cellCoord.mY < 0 ) return false;

	if ( cellCoord.mX >= this->mWidth ) return false;
	if ( cellCoord.mY >= this->mHeight ) return false;
	
	return true;
}

//----------------------------------------------------------------//
MOAIGridSpace::MOAIGridSpace () :
	mXOff ( 0.0f ),
	mYOff ( 0.0f ),
	mCellWidth ( 1.0f ),
	mCellHeight ( 1.0f ),
	mTileWidth ( 0.0f ),
	mTileHeight ( 0.0f ),
	mWidth ( 0 ),
	mHeight ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGridSpace::~MOAIGridSpace () {
}

//----------------------------------------------------------------//
void MOAIGridSpace::OnResize () {
}

//----------------------------------------------------------------//
void MOAIGridSpace::SerializeIn ( MOAILuaState& state ) {
	
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
void MOAIGridSpace::SerializeOut ( MOAILuaState& state ) {
	
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
void MOAIGridSpace::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "TILE_X_FLIP", ( u32 )MOAITileFlags::XFLIP );
	state.SetField ( -1, "TILE_Y_FLIP", ( u32 )MOAITileFlags::YFLIP );
	state.SetField ( -1, "TILE_XY_FLIP", ( u32 )MOAITileFlags::FLIP_MASK );
	state.SetField ( -1, "TILE_HIDE", ( u32 )MOAITileFlags::HIDDEN );
	
	state.SetField ( -1, "TILE_LEFT_TOP", ( u32 )MOAIGridSpace::TILE_LEFT_TOP );
	state.SetField ( -1, "TILE_RIGHT_TOP", ( u32 )MOAIGridSpace::TILE_RIGHT_TOP );
	state.SetField ( -1, "TILE_LEFT_BOTTOM", ( u32 )MOAIGridSpace::TILE_LEFT_BOTTOM );
	state.SetField ( -1, "TILE_RIGHT_BOTTOM", ( u32 )MOAIGridSpace::TILE_RIGHT_BOTTOM );
	
	state.SetField ( -1, "TILE_LEFT_CENTER", ( u32 )MOAIGridSpace::TILE_LEFT_CENTER );
	state.SetField ( -1, "TILE_RIGHT_CENTER", ( u32 )MOAIGridSpace::TILE_RIGHT_CENTER );
	state.SetField ( -1, "TILE_TOP_CENTER", ( u32 )MOAIGridSpace::TILE_TOP_CENTER );
	state.SetField ( -1, "TILE_BOTTOM_CENTER", ( u32 )MOAIGridSpace::TILE_BOTTOM_CENTER );
	
	state.SetField ( -1, "TILE_CENTER", ( u32 )MOAIGridSpace::TILE_CENTER );
}

//----------------------------------------------------------------//
void MOAIGridSpace::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "cellAddrToCoord",	_cellAddrToCoord },
		{ "getCellAddr",		_getCellAddr },
		{ "getSize",			_getSize },
		{ "getTileLoc",			_getTileLoc },
		{ "locToCellAddr",		_locToCellAddr },
		{ "locToCoord",			_locToCoord },
		{ "setSize",			_setSize },
		{ "wrapCoord",			_wrapCoord },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::WrapCellCoord ( const MOAICellCoord coord ) const {

	MOAICellCoord wrap;
	
	wrap.mX = coord.mX % this->mWidth;
	if ( wrap.mX < 0 ) wrap.mX += this->mWidth;
	
	wrap.mY = coord.mY % this->mHeight;
	if ( wrap.mY < 0 ) wrap.mY += this->mHeight;
	
	return wrap;
}

//----------------------------------------------------------------//
USVec2D MOAIGridSpace::WorldToCell ( MOAICellCoord cellCoord, USVec2D loc ) const {
	
	USVec2D result = loc;
	result.mX = ( loc.mX / this->mCellWidth ) - ( float )cellCoord.mX;
	result.mY = ( loc.mY / this->mCellHeight ) - ( float )cellCoord.mY;
	
	return result;
}

//----------------------------------------------------------------//
USVec2D MOAIGridSpace::WorldToGrid ( USVec2D loc ) const {

	loc.mY = -loc.mY;
	return loc;
}
