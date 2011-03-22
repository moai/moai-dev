// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAISim.h>
#include <moaicore/MOAIFileCache.h>
#include <moaicore/MOAISlippyMap.h>
#include <moaicore/MOAITransform2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>modelToPolar ( xCoordinate, yCoordinate )</tt>\n
	\n
	Converts coordinates from model to polar coordinates.
	@param xCoordinate Value of the X coordinate to be converted.
	@param yCoordinate Value of the Y coordinate to be converted.
	@return Converted coordinates.
*/
int MOAISlippyMap::_modelToPolar ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "UNN" )

	double x = state.GetValue < double >( 2, 0.0 );
	double y = state.GetValue < double >( 3, 0.0 );

	USVec2D64 polar = self->ProjectMapToPolar ( x, y );

	lua_pushnumber ( state, polar.mX );
	lua_pushnumber ( state, polar.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>polarToModel ( xCoordinate, yCoordinate )</tt>\n
	\n
	Converts coordinates from polar to model coordinates.
	@param xCoordinate Value of the X coordinate to be converted.
	@param yCoordinate Value of the Y coordinate to be converted.
	@return Converted coordinates.
*/
int MOAISlippyMap::_polarToModel ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "UNN" )

	double lon = state.GetValue < double >( 2, 0.0 );
	double lat = state.GetValue < double >( 3, 0.0 );

	USVec2D model = self->ProjectPolarToMap ( lon, lat );

	lua_pushnumber ( state, model.mX );
	lua_pushnumber ( state, model.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>( x, y ) polarToWorld ( self, lon, lat )</tt>\n
\n
	Transforms (lon, lat) to (x, y) using slippy map's geometry and world transform.
	@param self (in)
	@param lon (in)
	@param lat (in)
	@param x (out)
	@param y (out)
*/
int MOAISlippyMap::_polarToWorld ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "UNN" )

	double lon = state.GetValue < double >( 2, 0.0 );
	double lat = state.GetValue < double >( 3, 0.0 );

	USVec2D loc = self->ProjectPolarToMap ( lon, lat );

	const USAffine2D modelToWorld = self->GetLocalToWorldMtx ();
	modelToWorld.Transform ( loc );

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setFileCache ( self, fileCache )</tt>\n
\n
	Attaches map to file cache object. File cache object is used to
	store map tiles on local media. Map will first attempt to load tiles
	from local source before trying web source. Downloaded tiles will
	be saved automatically based on file cache settings.
	@param self (in)
	@param fileCache (in)
*/
int MOAISlippyMap::_setFileCache ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "UUN" )
	
	MOAIFileCache* fileCache = state.GetLuaData < MOAIFileCache >( 2 );
	if ( !fileCache ) return 0;
	
	int priority = state.GetValue < int >( 3, 0 );
	
	self->SetFileCache ( fileCache, priority );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setFrame ( self, left, top, right, bottom )</tt>\n
\n
	Sets the geometry of the slippy map.
	@param self (in)
	@param left (in)
	@param top (in)
	@param right (in)
	@param bottom (in)
*/
int MOAISlippyMap::_setFrame ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "UNNNN" )

	USRect frame;
	
	frame.mXMin = state.GetValue < float >( 2, 0.0f );
	frame.mYMin = state.GetValue < float >( 3, 0.0f );
	frame.mXMax = state.GetValue < float >( 4, 0.0f );
	frame.mYMax = state.GetValue < float >( 5, 0.0f );
	
	self->SetFrame ( frame );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setLoadingTiles ( loadingTile, backgroundTile )</tt>\n
	\n
	Sets loading and background images for the map.
	@param loadingTile Image file for the loading tile for the map.
	@param backgroundTile Image file for the background tile for the map.
*/
int MOAISlippyMap::_setLoadingTiles ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "USS" )

	STLString loadingTile = state.GetValue ( 2, "" );
	STLString bgTile = state.GetValue ( 3, "" );
	
	self->SetLoadingTiles ( loadingTile, bgTile );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setTileKeyCallback ( self, func )</tt>\n
\n
	Sets a function called to generate a tile idenfier key. Only needed
	when using a file cache. This can be the tile's url or, better, a short
	string to uniquely identify the tile against any other in the file cache.
	If the file cache is only used by a single tile source, then simply returning
	a string containing the tile's coordinates and zoom level is sufficient. In
	the case of multiple tile sources, the source should be identified as well.
	The callback function is passed the x and y tile coordinates and the zoom level.
	@param self (in)
	@param func (in)
*/
int MOAISlippyMap::_setTileKeyCallback ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "UF" )
	
	self->mGetTileKey.SetRef ( state, 2, false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setTileSize ( self, tileSize )</tt>\n
\n
	Sets the expected map tile dimensions (usually 256 or 64).
	@param self (in)
	@param tileSize (in)
*/
int MOAISlippyMap::_setTileSize ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "UN" )

	self->mTileSize = state.GetValue < u32 >( 2, self->mTileSize );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setTileURLCallback ( self, func )</tt>\n
\n
	Sets the function to be called each time a tile is requested. The
	callback function is passed the x and y tile coordinate as well as the
	zoom level. Tiles are organized in a quad tree. Function should return a
	url string that identifies the tile image to download.
	@param self (in)
	@param func (in)
*/
int MOAISlippyMap::_setTileURLCallback ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "UF" )
	
	self->mGetTileURL.SetRef ( state, 2, false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setZoomRange ( self, minZoom, maxZoom )</tt>\n
\n
	Sets the minimum and maximum acceptable zoom range for the
	map. Should reflect range of current tile provider.
	@param self (in)
	@param minZoom (in)
	@param maxZoom (in)
*/
int MOAISlippyMap::_setZoomRange ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "UNN" )

	self->mMinZoom = state.GetValue < u32 >( 2, self->mMinZoom );
	self->mMaxZoom = state.GetValue < u32 >( 3, self->mMaxZoom );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( lon, lat ) worldToPolar ( self, x, y )</tt>\n
\n
	Transforms (x, y) to (lon, lat) using slippy map's geometry and inverse world transform.
	@param self (in)
	@param x (in)
	@param y (in)
	@param lon (out)
	@param lat (out)
*/
int MOAISlippyMap::_worldToPolar ( lua_State* L ) {
	LUA_SETUP ( MOAISlippyMap, "UNN" )

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );

	const USAffine2D worldToModel = self->GetWorldToLocalMtx ();
	worldToModel.Transform ( loc );

	USVec2D64 polar = self->ProjectMapToPolar ( loc.mX, loc.mY );

	lua_pushnumber ( state, polar.mX );
	lua_pushnumber ( state, polar.mY );

	return 2;
}

//================================================================//
// MOAISlippyMap
//================================================================//

//----------------------------------------------------------------//
void MOAISlippyMap::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	MOAITransform2D::ApplyAttrOp ( attrID, attrOp );
}

//----------------------------------------------------------------//
u32 MOAISlippyMap::CountAttributes () {

	return MOAISlippyMap::TOTAL_ATTR;
}

//----------------------------------------------------------------//
STLString MOAISlippyMap::DoTileCallback ( USLuaRef& callback, USQuadCoord& quadID ) {

	STLString key;

	if ( callback ) {
		USLuaStateHandle state = callback.GetSelf ();
		lua_pushnumber ( state, quadID.mLevel );
		lua_pushnumber ( state, quadID.mXTile );
		lua_pushnumber ( state, quadID.mYTile );
		state.DebugCall ( 3, 1 );
		
		key = state.GetValue < cc8* >( -1, "" );
	}

	return key;
}

//----------------------------------------------------------------//
void MOAISlippyMap::Draw () {

	USDrawBuffer& drawBuffer = USDrawBuffer::Get ();

	drawBuffer.SetVtxTransform ( this->GetLocalToWorldMtx ());
	this->LoadShader ();
	
	u32 zoom = this->ComputeZoom ();
	this->SetZoom ( zoom );
	
	this->USSlipMapBrush::Draw ( 0, false );
	this->Update ( 1.0f / 600.0f );
}

//----------------------------------------------------------------//
u32 MOAISlippyMap::GetLocalFrame ( USRect& frame ) {

	frame = this->GetFrame ();
	return FRAME_OK;
}

//----------------------------------------------------------------//
u32 MOAISlippyMap::GetMaxZoom () {

	return this->mMaxZoom;
}

//----------------------------------------------------------------//
u32 MOAISlippyMap::GetMinZoom () {

	return this->mMinZoom;
}

//----------------------------------------------------------------//
u32 MOAISlippyMap::GetTileSize () {

	return this->mTileSize;
}

//----------------------------------------------------------------//
STLString MOAISlippyMap::GetTileStoreKey ( USQuadCoord& quadID ) {

	return this->DoTileCallback ( this->mGetTileKey, quadID );
}

//----------------------------------------------------------------//
STLString MOAISlippyMap::GetTileURL ( USQuadCoord& quadID ) {

	return this->DoTileCallback ( this->mGetTileURL, quadID );
}

//----------------------------------------------------------------//
MOAISlippyMap::MOAISlippyMap () :
	mMinZoom ( 1 ),
	mMaxZoom ( 18 ),
	mTileSize ( 256 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxPrim2D )
	RTTI_END
	
	this->SetQueryMask ( MOAIContentLibrary2D::CAN_DRAW );
}

//----------------------------------------------------------------//
MOAISlippyMap::~MOAISlippyMap () {
}

//----------------------------------------------------------------//
void MOAISlippyMap::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAISlippyMap::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAITransform2D::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "modelToPolar",			_modelToPolar },
		{ "polarToModel",			_polarToModel },
		{ "polarToWorld",			_polarToWorld },
		{ "setFileCache",			_setFileCache },
		{ "setFrame",				_setFrame },
		{ "setLoadingTiles",		_setLoadingTiles },
		{ "setTileKeyCallback",		_setTileKeyCallback },
		{ "setTileSize",			_setTileSize },
		{ "setTileURLCallback",		_setTileURLCallback },
		{ "setZoomRange",			_setZoomRange },
		{ "worldToPolar",			_worldToPolar },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAISlippyMap::ToString () {

	STLString repr ( MOAIGfxPrim2D::ToString ());

	return repr;
}

