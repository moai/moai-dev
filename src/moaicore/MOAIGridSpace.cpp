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
/**	@name	getCellSize
	@text	Returns the dimensions of a single grid cell.

	@in		MOAIGridSpace self
	@out	number width
	@out	number height
*/
int MOAIGridSpace::_getCellSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "U" )

	state.Push ( self->mCellWidth );
	state.Push ( self->mCellHeight );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getOffset
	@text	Returns the offset of tiles from cells.

	@in		MOAIGridSpace self
	@out	number xOff
	@out	number yOff
*/
int MOAIGridSpace::_getOffset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "U" )

	state.Push ( self->mXOff );
	state.Push ( self->mYOff );
	
	return 2;
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
/**	@name	getTileSize
	@text	Returns the dimensions of a single grid tile.

	@in		MOAIGridSpace self
	@out	number width
	@out	number height
*/
int MOAIGridSpace::_getTileSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "U" )

	state.Push ( self->mTileWidth );
	state.Push ( self->mTileHeight );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	initDiamondGrid
	@text	Initialize a grid with hexagonal tiles.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@opt	number tileWidth		Default valus is 1.
	@opt	number tileHeight		Default valus is 1.
	@opt	number xGutter			Default valus is 0.
	@opt	number yGutter			Default value is 0.
	@out	nil
*/
int MOAIGridSpace::_initDiamondGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	u32 width			= state.GetValue < u32 >( 2, 0 );
	u32 height			= state.GetValue < u32 >( 3, 0 );
	
	float tileWidth		= state.GetValue < float >( 4, 1.0f );
	float tileHeight	= state.GetValue < float >( 5, 1.0f );

	float xGutter		= state.GetValue < float >( 6, 0.0f );
	float yGutter		= state.GetValue < float >( 7, 0.0f );

	self->mShape = DIAMOND_SHAPE;

	self->mWidth = width;
	self->mHeight = height;
	
	self->mCellWidth = tileWidth;
	self->mCellHeight = tileHeight * 0.5f;
	
	self->mXOff = xGutter * 0.5f;
	self->mYOff = ( yGutter * 0.5f ) - ( tileHeight * 0.25f );
	
	self->mTileWidth = tileWidth - xGutter;
	self->mTileHeight = tileHeight - yGutter;
	
	self->OnResize ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	initHexGrid
	@text	Initialize a grid with hexagonal tiles.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@opt	number radius			Default valus is 1.
	@opt	number xGutter			Default valus is 0.
	@opt	number yGutter			Default value is 0.
	@out	nil
*/
int MOAIGridSpace::_initHexGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	u32 width			= state.GetValue < u32 >( 2, 0 );
	u32 height			= state.GetValue < u32 >( 3, 0 );
	
	float hRad			= state.GetValue < float >( 4, 1.0f ) * 0.5f;

	float xGutter		= state.GetValue < float >( 5, 0.0f );
	float yGutter		= state.GetValue < float >( 6, 0.0f );

	float tileWidth = hRad * 6.0f;
	float tileHeight = hRad * ( float )( M_SQRT3 * 2.0 );

	self->mShape = HEX_SHAPE;

	self->mWidth = width;
	self->mHeight = height;
	
	self->mCellWidth = tileWidth;
	self->mCellHeight = tileHeight * 0.5f;
	
	self->mXOff = hRad + ( xGutter * 0.5f );
	self->mYOff = ( yGutter * 0.5f ) - ( tileHeight * 0.25f );
	
	self->mTileWidth = ( hRad * 4.0f ) - xGutter;
	self->mTileHeight = tileHeight - yGutter;
	
	self->OnResize ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	initObliqueGrid
	@text	Initialize a grid with oblique tiles.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@opt	number tileWidth		Default valus is 1.
	@opt	number tileHeight		Default valus is 1.
	@opt	number xGutter			Default valus is 0.
	@opt	number yGutter			Default value is 0.
	@out	nil
*/
int MOAIGridSpace::_initObliqueGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	u32 width			= state.GetValue < u32 >( 2, 0 );
	u32 height			= state.GetValue < u32 >( 3, 0 );
	
	float tileWidth		= state.GetValue < float >( 4, 1.0f );
	float tileHeight	= state.GetValue < float >( 5, 1.0f );

	float xGutter		= state.GetValue < float >( 6, 0.0f );
	float yGutter		= state.GetValue < float >( 7, 0.0f );
	
	self->mShape = OBLIQUE_SHAPE;

	self->mWidth = width;
	self->mHeight = height;
	
	self->mCellWidth = tileWidth;
	self->mCellHeight = tileHeight;
	
	self->mXOff = xGutter * 0.5f;
	self->mYOff = yGutter * 0.5f;
	
	self->mTileWidth = ( tileWidth * 2.0f ) - xGutter;
	self->mTileHeight = tileHeight - yGutter;
	
	self->OnResize ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	initRectGrid
	@text	Initialize a grid with rectangular tiles.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@opt	number tileWidth		Default valus is 1.
	@opt	number tileHeight		Default valus is 1.
	@opt	number xGutter			Default valus is 0.
	@opt	number yGutter			Default value is 0.
	@out	nil
*/
int MOAIGridSpace::_initRectGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	u32 width			= state.GetValue < u32 >( 2, 0 );
	u32 height			= state.GetValue < u32 >( 3, 0 );
	
	float tileWidth		= state.GetValue < float >( 4, 1.0f );
	float tileHeight	= state.GetValue < float >( 5, 1.0f );

	float xGutter		= state.GetValue < float >( 6, 0.0f );
	float yGutter		= state.GetValue < float >( 7, 0.0f );
	
	self->mShape = RECT_SHAPE;

	self->mWidth = width;
	self->mHeight = height;
	
	self->mCellWidth = tileWidth;
	self->mCellHeight = tileHeight;
	
	self->mXOff = xGutter * 0.5f;
	self->mYOff = yGutter * 0.5f;
	
	self->mTileWidth = tileWidth - xGutter;
	self->mTileHeight = tileHeight - yGutter;
	
	self->OnResize ();
	
	return 0;
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
/**	@name	setRepeat
	@text	Repeats a grid indexer along X or Y. Only used when a grid
			is attached.
	
	@in		MOAIGridSpace self
	@opt	boolean repeatX		Default value is true.
	@opt	boolean repeatY		Default value is repeatX.
	@out	nil
*/
int MOAIGridSpace::_setRepeat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "U" )

	bool repeatX = state.GetValue < bool >( 2, true );
	bool repeatY = state.GetValue < bool >( 3, repeatX );

	self->mRepeat = 0;
	self->mRepeat |= repeatX ? REPEAT_X : 0;
	self->mRepeat |= repeatY ? REPEAT_Y : 0;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setShape
	@text	Set the shape of the grid tiles.

	@in		MOAIGridSpace self
	@opt	number shape		One of MOAIGridSpace.RECT_SHAPE, MOAIGridSpace.DIAMOND_SHAPE,
								MOAIGridSpace.OBLIQUE_SHAPE, MOAIGridSpace.HEX_SHAPE.
								Default value is MOAIGridSpace.RECT_SHAPE.
	@out	nil
*/
int MOAIGridSpace::_setShape ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "U" )

	self->mShape = state.GetValue < u32 >( 2, RECT_SHAPE );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSize
	@text	Initializes dimensions of grid and reserves storage for tiles.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@opt	number cellWidth	Default value is 1.
	@opt	number cellHeight	Default value is 1.
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

	coord = self->WrapCellCoord ( coord.mX, coord.mY );
	
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
	
	if ( this->mShape & STAGGER_FLAG ) {
		rect.mXMax += this->mCellWidth * 0.5f;
		rect.mYMin -= this->mCellHeight * 0.5f;
		rect.mYMax += this->mCellHeight * 0.5f;
	}
	
	return rect;
}

//----------------------------------------------------------------//
USRect MOAIGridSpace::GetBounds ( MOAICellCoord c0, MOAICellCoord c1 ) const {

	USRect rect0 = this->GetCellRect ( c0 );
	USRect rect1 = this->GetCellRect ( c1 );
	
	rect0.Grow ( rect1 );
	
	if ( this->mShape & STAGGER_FLAG ) {
		rect0.mXMax += this->mCellWidth * 0.5f;
		rect0.mYMin -= this->mCellHeight * 0.5f;
		rect0.mYMax += this->mCellHeight * 0.5f;
	}
	
	return rect0;
}

//----------------------------------------------------------------//
void MOAIGridSpace::GetBoundsInRect ( USRect rect, MOAICellCoord& c0, MOAICellCoord& c1, USRect maxSize ) const {

	rect.Bless ();
	maxSize.Bless ();

	c0.mX = ( int )floorf ( ( rect.mXMin / this->mCellWidth )  - ( maxSize.mXMax / 0.5f - 1.0f ) );
	c0.mY = ( int )floorf ( ( rect.mYMin / this->mCellHeight ) - ( maxSize.mYMax / 0.5f - 1.0f ) );

	c1.mX = ( int )floorf ( ( rect.mXMax / this->mCellWidth )  + ( maxSize.mXMin / -0.5f - 1.0f ) );
	c1.mY = ( int )floorf ( ( rect.mYMax / this->mCellHeight ) + ( maxSize.mYMin / -0.5f - 1.0f ) );
	
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
	
	MOAICellCoord cellCoord ( 0, 0 );
	
	switch ( this->mShape ) {
		
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
MOAICellCoord MOAIGridSpace::GetCellCoord ( int xCell, int yCell ) const {

	MOAICellCoord cellCoord;
	cellCoord.mX = xCell;
	cellCoord.mY = yCell;
	return cellCoord;
}

//----------------------------------------------------------------//
USVec2D MOAIGridSpace::GetCellPoint ( MOAICellCoord cellCoord, u32 position ) const {
			
	float xOff = 0.0f;
	
	if (( this->mShape & STAGGER_FLAG ) && ( cellCoord.mY & 0x01 )) {
		xOff = this->mCellWidth * 0.5f;
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
USRect MOAIGridSpace::GetCellRect ( MOAICellCoord cellCoord ) const {
			
	float xOff = 0.0f;

	if (( this->mShape & STAGGER_FLAG ) && ( cellCoord.mY & 0x01 )) {
		xOff = this->mCellWidth * 0.5f;
	}
	
	USRect rect;
	
	rect.mXMin = ( cellCoord.mX * this->mCellWidth ) + xOff;
	rect.mYMin = cellCoord.mY * this->mCellHeight;
	
	rect.mXMax = ( rect.mXMin + this->mCellWidth ) + xOff;
	rect.mYMax = rect.mYMin + this->mCellHeight;
	
	return rect;
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::GetHexCellCoord ( float x, float y, float a, float b ) const {

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
	
	MOAICellCoord cellCoord ( xTile, yTile );
	return cellCoord;
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::GetObliqueCellCoord ( float x, float y ) const {

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
	
	MOAICellCoord cellCoord ( xTile, yTile );
	return cellCoord;
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
	
	float xOff = 0.0f;

	if (( this->mShape & STAGGER_FLAG ) && ( cellCoord.mY & 0x01 )) {
		xOff = this->mCellWidth * 0.5f;
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
USRect MOAIGridSpace::GetTileRect ( MOAICellCoord cellCoord ) const {

	float xOff = 0.0f;

	if (( this->mShape & STAGGER_FLAG ) && ( cellCoord.mY & 0x01 )) {
		xOff = this->mCellWidth * 0.5f;
	}

	USRect rect;
	
	rect.mXMin = ( cellCoord.mX * this->mCellWidth ) + this->mXOff + xOff;
	rect.mYMin = ( cellCoord.mY * this->mCellHeight ) + this->mYOff;
	
	rect.mXMax = rect.mXMin + this->mTileWidth;
	rect.mYMax = rect.mYMin + this->mTileHeight;
	
	return rect;
}

//----------------------------------------------------------------//
int MOAIGridSpace::GetTotalCells () const {

	return this->mWidth * this->mHeight;
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
	mHeight ( 0 ),
	mShape ( RECT_SHAPE ),
	mRepeat ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGridSpace::~MOAIGridSpace () {
}

//----------------------------------------------------------------//
void MOAIGridSpace::OnResize () {
}

//----------------------------------------------------------------//
void MOAIGridSpace::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );
	
	this->mXOff			= state.GetField ( -1, "mXOff", this->mXOff );
	this->mYOff			= state.GetField ( -1, "mYOff", this->mYOff );
	
	this->mCellWidth	= state.GetField ( -1, "mCellWidth", this->mCellWidth );
	this->mCellHeight	= state.GetField ( -1, "mCellHeight", this->mCellHeight );
	
	this->mTileWidth	= state.GetField ( -1, "mTileWidth", this->mTileWidth );
	this->mTileHeight	= state.GetField ( -1, "mTileHeight", this->mTileHeight );
	
	this->mWidth		= state.GetField ( -1, "mWidth", this->mWidth );
	this->mHeight		= state.GetField ( -1, "mHeight", this->mHeight );
	
	this->mShape		= state.GetField ( -1, "mShape", this->mShape );
	this->mRepeat		= state.GetField ( -1, "mRepeat", this->mRepeat );
}

//----------------------------------------------------------------//
void MOAIGridSpace::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );
	
	state.SetField ( -1, "mXOff", this->mXOff );
	state.SetField ( -1, "mYOff", this->mYOff );
	
	state.SetField ( -1, "mCellWidth", this->mCellWidth );
	state.SetField ( -1, "mCellHeight", this->mCellHeight );
	
	state.SetField ( -1, "mTileWidth", this->mTileWidth );
	state.SetField ( -1, "mTileHeight", this->mTileHeight );
	
	state.SetField ( -1, "mWidth", this->mWidth );
	state.SetField ( -1, "mHeight", this->mHeight );
	
	state.SetField ( -1, "mShape", this->mShape );
	state.SetField ( -1, "mRepeat", this->mRepeat );
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
	
	state.SetField ( -1, "RECT_SHAPE", RECT_SHAPE );
	state.SetField ( -1, "DIAMOND_SHAPE", DIAMOND_SHAPE );
	state.SetField ( -1, "OBLIQUE_SHAPE", OBLIQUE_SHAPE );
	state.SetField ( -1, "HEX_SHAPE", HEX_SHAPE );
}

//----------------------------------------------------------------//
void MOAIGridSpace::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "cellAddrToCoord",	_cellAddrToCoord },
		{ "getCellAddr",		_getCellAddr },
		{ "getCellSize",		_getCellSize },
		{ "getOffset",			_getOffset },
		{ "getSize",			_getSize },
		{ "getTileLoc",			_getTileLoc },
		{ "getTileSize",		_getTileSize },
		{ "initDiamondGrid",	_initDiamondGrid },
		{ "initHexGrid",		_initHexGrid },
		{ "initObliqueGrid",	_initObliqueGrid },
		{ "initRectGrid",		_initRectGrid },
		{ "locToCellAddr",		_locToCellAddr },
		{ "locToCoord",			_locToCoord },
		{ "setRepeat",			_setRepeat },
		{ "setShape",			_setShape },
		{ "setSize",			_setSize },
		{ "wrapCoord",			_wrapCoord },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAICellCoord MOAIGridSpace::WrapCellCoord ( int xCell, int yCell ) const {

	MOAICellCoord wrap;
	
	wrap.mX = xCell % this->mWidth;
	if ( wrap.mX < 0 ) wrap.mX += this->mWidth;
	
	wrap.mY = yCell % this->mHeight;
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

