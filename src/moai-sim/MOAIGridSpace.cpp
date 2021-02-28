// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGridSpace.h>

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
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	cellAddrToCoord
	@text	Returns the coordinate of a cell given an address.

	@in		MOAIGridSpace self
	@in		number cellAddr
	@out	number xTile
	@out	number yTile
*/
int MOAIGridSpace::_cellAddrToCoord	( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UN" )

	ZLIndex addr = state.GetValue < MOAILuaIndex >( 2, 0 );
	
	ZLGridCoord coord = self->GetCellCoord ( addr );
	
	state.Push ( coord.mX + 1 );
	state.Push ( coord.mY + 1 );
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getCellAddr
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
	
	state.Push ( MOAILuaIndex ( self->GetCellAddr ( xTile, yTile )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getCellSize
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
/**	@lua	getOffset
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
/**	@lua	getSize
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
/**	@lua	getTileLoc
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
	
	ZLGridCoord coord;
	
	coord.mX		= state.GetValue < int >( 2, 1 ) - 1;
	coord.mY		= state.GetValue < int >( 3, 1 ) - 1;
	u32 position	= state.GetValue < u32 >( 4, MOAIGridSpace::TILE_CENTER );
	
	ZLVec2D loc = self->GetTilePoint ( coord, position );
	state.Push ( loc.mX );
	state.Push ( loc.mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getTileSize
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
/**	@lua	initAxialHexGrid
	@text	Initialize a grid with hex tiles, using an axial coordinate system. The axial grid assumes that the flat sides of hexes are on the sides, and the points are up/down.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@opt	number tileWidth		Default value is 1.
	@opt	number tileHeight		Default value is 1.
	@opt	number xGutter			Default value is 0.
	@opt	number yGutter			Default value is 0.
	@out	nil
*/
int MOAIGridSpace::_initAxialHexGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	u32 width			= state.GetValue < u32 >( 2, 0 );
	u32 height			= state.GetValue < u32 >( 3, 0 );
	
	float tileWidth		= state.GetValue < float >( 4, 1.0f );
	float tileHeight	= state.GetValue < float >( 5, 1.0f );

	float xGutter		= state.GetValue < float >( 6, 0.0f );
	float yGutter		= state.GetValue < float >( 7, 0.0f );

	self->mShape = AXIAL_HEX_SHAPE;

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
/**	@lua	initDiamondGrid
	@text	Initialize a grid with diamond tiles.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@opt	number tileWidth		Default value is 1.
	@opt	number tileHeight		Default value is 1.
	@opt	number xGutter			Default value is 0.
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
/**	@lua	initHexGrid
	@text	Initialize a grid with hexagonal tiles.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@opt	number radius			Default value is 1.
	@opt	number xGutter			Default value is 0.
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
/**	@lua	initObliqueGrid
	@text	Initialize a grid with oblique tiles.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@opt	number tileWidth		Default value is 1.
	@opt	number tileHeight		Default value is 1.
	@opt	number xGutter			Default value is 0.
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
/**	@lua	initRectGrid
	@text	Initialize a grid with rectangular tiles.

	@in		MOAIGridSpace self
	@in		number width
	@in		number height
	@opt	number tileWidth		Default value is 1.
	@opt	number tileHeight		Default value is 1.
	@opt	number xGutter			Default value is 0.
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
/**	@lua	locToCellAddr
	@text	Returns the address of a cell given a a coordinate in grid space.

	@in		MOAIGridSpace self
	@in		number x
	@in		number y
	@out	number cellAddr
*/
int MOAIGridSpace::_locToCellAddr ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	ZLVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0 );
	loc.mY = state.GetValue < float >( 3, 0 );
	
	ZLGridCoord coord;
	coord = self->GetCellCoord ( loc );

	state.Push ( MOAILuaIndex ( self->GetCellAddr ( coord )));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	locToCoord
	@text	Transforms a coordinate in grid space into a tile index.

	@in		MOAIGridSpace self
	@in		number x
	@in		number y
	@out	number xTile
	@out	number yTile
*/
int MOAIGridSpace::_locToCoord ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )

	ZLVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0 );
	loc.mY = state.GetValue < float >( 3, 0 );
	
	ZLGridCoord coord;
	coord = self->GetCellCoord ( loc );

	state.Push ( coord.mX + 1 );
	state.Push ( coord.mY + 1 );
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	setRepeat
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
/**	@lua	setShape
	@text	Set the shape of the grid tiles.

	@in		MOAIGridSpace self
	@opt	number shape		One of MOAIGridSpace.RECT_SHAPE, MOAIGridSpace.DIAMOND_SHAPE,
								MOAIGridSpace.OBLIQUE_SHAPE, MOAIGridSpace.HEX_SHAPE, MOAIGridSpace.AXIAL_HEX_SHAPE.
								Default value is MOAIGridSpace.RECT_SHAPE.
	@out	nil
*/
int MOAIGridSpace::_setShape ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "U" )

	self->mShape = state.GetValue < u32 >( 2, RECT_SHAPE );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSize
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
/**	@lua	wrapCoord
	@text	Wraps a tile index to the range of the grid.

	@in		MOAIGridSpace self
	@in		number xTile
	@in		number yTile
	@out	number xTile
	@out	number yTile
*/
int MOAIGridSpace::_wrapCoord ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGridSpace, "UNN" )
	
	ZLGridCoord coord;
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
MOAIGridSpace::MOAIGridSpace ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ) {
		
	RTTI_BEGIN ( MOAIGridSpace )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGridSpace >)
		RTTI_VISITOR ( MOAIAbstractLuaSerializationVisitor, MOAILuaSerializationVisitor < MOAIGridSpace >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGridSpace::~MOAIGridSpace () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGridSpace::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	state.SetField ( -1, "TILE_X_FLIP", ( u32 )ZLTileFlags::XFLIP );
	state.SetField ( -1, "TILE_Y_FLIP", ( u32 )ZLTileFlags::YFLIP );
	state.SetField ( -1, "TILE_XY_FLIP", ( u32 )ZLTileFlags::FLIP_MASK );
	state.SetField ( -1, "TILE_HIDE", ( u32 )ZLTileFlags::HIDDEN );
	
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
	state.SetField ( -1, "AXIAL_HEX_SHAPE", AXIAL_HEX_SHAPE );
	state.SetField ( -1, "DIAMOND_SHAPE", DIAMOND_SHAPE );
	state.SetField ( -1, "OBLIQUE_SHAPE", OBLIQUE_SHAPE );
	state.SetField ( -1, "HEX_SHAPE", HEX_SHAPE );
}

//----------------------------------------------------------------//
void MOAIGridSpace::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "cellAddrToCoord",	_cellAddrToCoord },
		{ "getCellAddr",		_getCellAddr },
		{ "getCellSize",		_getCellSize },
		{ "getOffset",			_getOffset },
		{ "getSize",			_getSize },
		{ "getTileLoc",			_getTileLoc },
		{ "getTileSize",		_getTileSize },
		{ "initDiamondGrid",	_initDiamondGrid },
		{ "initAxialHexGrid",		_initAxialHexGrid },
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
void MOAIGridSpace::_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( serializer );
	if ( history.Visit ( *this )) return;
	
	this->mXOff			= state.GetFieldValue ( -1, "mXOff", this->mXOff );
	this->mYOff			= state.GetFieldValue ( -1, "mYOff", this->mYOff );
	
	this->mCellWidth	= state.GetFieldValue ( -1, "mCellWidth", this->mCellWidth );
	this->mCellHeight	= state.GetFieldValue ( -1, "mCellHeight", this->mCellHeight );
	
	this->mTileWidth	= state.GetFieldValue ( -1, "mTileWidth", this->mTileWidth );
	this->mTileHeight	= state.GetFieldValue ( -1, "mTileHeight", this->mTileHeight );
	
	this->mWidth		= state.GetFieldValue ( -1, "mWidth", this->mWidth );
	this->mHeight		= state.GetFieldValue ( -1, "mHeight", this->mHeight );
	
	this->mShape		= state.GetFieldValue ( -1, "mShape", this->mShape );
	this->mRepeat		= state.GetFieldValue ( -1, "mRepeat", this->mRepeat );
}

//----------------------------------------------------------------//
void MOAIGridSpace::_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( serializer );
	if ( history.Visit ( *this )) return;
	
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
