// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIContentLibrary2D.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAISurfaceSampler2D.h>
#include <moaicore/MOAITilemap.h>
#include <moaicore/MOAITransform2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( grid ) getGrid ( self )</tt>\n
\n
	Returns the grid object associated with this tilemap.
	@param self (in)
	@param grid (out)
*/
int MOAITilemap::_getGrid ( lua_State* L ) {
	LUA_SETUP ( MOAITilemap, "UU" )
	
	if ( self->mGrid ) {
		self->mGrid->PushLuaInstance ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( tilecode ) getTile ( self, xTile, yTile )</tt>\n
\n
	Returns the tile code stored at (xTile, yTile).
	@param self (in)
	@param xTile (in)
	@param yTile (in)
	@param tilecode (out)
*/
int MOAITilemap::_getTile ( lua_State* L ) {
	LUA_SETUP ( MOAITilemap, "UNN" )
	
	if ( !self->mGrid ) return 0;
	
	int xTile = state.GetValue < int >( 2, 0 );
	int yTile = state.GetValue < int >( 3, 0 );
	
	u32 tile = self->mGrid->GetTile ( xTile, yTile );
	state.Push ( tile );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( x, y ) getTileLoc ( self, xTile, yTile, position )</tt>\n
\n
	Returns the world space coordinate of the tile with respect to the tilemap's
	geometry and world transform.
	The optional 'position' flag determines the location of the coordinate within the tile:
	
	MOAITilemap.TILE_LEFT_TOP
	MOAITilemap.TILE_RIGHT_TOP
	MOAITilemap.TILE_LEFT_BOTTOM
	MOAITilemap.TILE_RIGHT_BOTTOM
	MOAITilemap.TILE_LEFT_CENTER
	MOAITilemap.TILE_RIGHT_CENTER
	MOAITilemap.TILE_TOP_CENTER
	MOAITilemap.TILE_BOTTOM_CENTER
	MOAITilemap.TILE_CENTER
	
	@param self (in)
	@param xTile (in)
	@param yTile (in)
	@param position (in)
	@param x (out)
	@param y (out)
*/
int MOAITilemap::_getTileLoc ( lua_State* L ) {
	LUA_SETUP ( MOAITilemap, "UNN" )
	
	if ( !self->mGrid ) return 0;
	
	int xTile		= state.GetValue < int >( 2, 0 );
	int yTile		= state.GetValue < int >( 3, 0 );
	u32 position	= state.GetValue < u32 >( 4, USGridSpace::TILE_CENTER );
	
	USVec2D loc = self->GetTileLoc ( xTile, yTile, position );
	state.Push ( loc.mX );
	state.Push ( loc.mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setGrid ( self, grid )</tt>\n
\n
	Sets the grid for the tilemap. The grid provides the array of tilecodes
	used by the tilemap for looking up content from the content source.
	@param self (in)
	@param self (grid)
*/
int MOAITilemap::_setGrid ( lua_State* L ) {
	LUA_SETUP ( MOAITilemap, "UU" )
	
	MOAIGrid* grid = state.GetLuaData < MOAIGrid >( 2 );
	if ( !grid ) return 0;
	
	self->mGrid = grid;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setRepeat ( self, repeat )</tt>\n
\n
	Sets the tilemap to wrap (repeat continuously when rendered or queried against)
	or clamp (render once, like a sprite).
	@param self (in)
	@param repeatX (in)
	@param repeatY (in)
*/
int MOAITilemap::_setRepeat ( lua_State* L ) {
	LUA_SETUP ( MOAITilemap, "UB" )

	self->mRepeatX = state.GetValue < bool >( 2, false );
	self->mRepeatY = state.GetValue < bool >( 3, self->mRepeatX );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>() setTile ( self, xTile, yTile, tilecode )</tt>\n
\n
	Sets the tilecode for a given index.
	@param self (in)
	@param xTile (in)
	@param yTile (in)
	@param tilecode (in)
*/
int MOAITilemap::_setTile ( lua_State* L ) {
	LUA_SETUP ( MOAITilemap, "UNNN" )

	if ( !self->mGrid ) return 0;

	int xTile = state.GetValue < int >( 2, 0 );
	int yTile = state.GetValue < int >( 3, 0 );
	u32 tile = state.GetValue < u32 >( 4, 0 );

	self->mGrid->SetTile ( xTile, yTile, tile );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( xTile, yTile ) wrapCoord ( self, xTile, yTile )</tt>\n
\n
	Wraps a given tile index ot the extents of the tilemap's grid.
	@param self (in)
	@param xTile (in)
	@param yTile (in)
	@param xTile (out)
	@param yTile (out)
*/
int MOAITilemap::_wrapCoord ( lua_State* L ) {
	LUA_SETUP ( MOAITilemap, "UNN" )
	
	USTileCoord coord;
	coord.mX = state.GetValue < int >( 2, 0 );
	coord.mY = state.GetValue < int >( 3, 0 );

	if ( self->mGrid ) {
		self->mGrid->WrapCellCoord ( coord );
	}
	
	state.Push ( coord.mX );
	state.Push ( coord.mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>( xTile, yTile ) worldToCoord ( self, x, y )</tt>\n
\n
	Transforms a world coordinate to a grid index.
	@param self (in)
	@param x (in)
	@param y (in)
	@param xTile (out)
	@param yTile (out)
*/
int MOAITilemap::_worldToCoord ( lua_State* L ) {
	LUA_SETUP ( MOAITilemap, "UNN" )

	if ( !self->mGrid ) return 0;

	USVec2D loc;
	loc.mX = state.GetValue < float >( 2, 0 );
	loc.mY = state.GetValue < float >( 3, 0 );

	const USAffine2D& worldToModel = self->GetWorldToLocalMtx ();
	worldToModel.Transform ( loc );
	
	USTileCoord coord;
	coord = self->mGrid->GetTileCoord ( loc );

	state.Push ( coord.mX );
	state.Push ( coord.mY );
	return 2;
}

//================================================================//
// MOAITilemap
//================================================================//

//----------------------------------------------------------------//
void MOAITilemap::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	MOAITransform2D::ApplyAttrOp ( attrID, attrOp );
}

//----------------------------------------------------------------//
u32 MOAITilemap::CountAttributes () {

	return MOAIGfxPrim2D::TOTAL_ATTR;
}

//----------------------------------------------------------------//
void MOAITilemap::Draw () {

	if ( this->mGrid && this->BindGfxSource ()) {
		
		this->LoadShader ();
		
		USTileCoord c0;
		USTileCoord c1;
		
		this->GetBoundsInView ( c0, c1 );
		this->mGfxSource->Draw ( *this, *this->mGrid, c0, c1 );
	}
}

//----------------------------------------------------------------//
void MOAITilemap::DrawDebug () {
	
	if ( this->mGfxSource && this->mGrid ) {
		
		USTileCoord c0;
		USTileCoord c1;
		
		this->GetBoundsInView ( c0, c1 );
		
		MOAIDebugLines& debugLines = MOAIDebugLines::Get ();
		
		debugLines.SetPenColor ( 0x40ffffff );
		debugLines.SetPenWidth ( 2 );
		
		this->mGfxSource->DrawDebug ( *this, *this->mGrid, c0, c1 );
	}
}

//----------------------------------------------------------------//
void MOAITilemap::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {

	if ( this->mGfxSource && this->mGrid ) {
		
		sampler.SetSourcePrim ( this );
		
		USRect localRect = sampler.GetLocalRect ();
		
		USTileCoord c0 = this->mGrid->GetTileCoord ( localRect.mXMin, localRect.mYMin );
		USTileCoord c1 = this->mGrid->GetTileCoord ( localRect.mXMax, localRect.mYMax );
		
		if ( this->mRepeatX == false ) {
			this->mGrid->ClampX ( c0 );
			this->mGrid->ClampX ( c1 );
		}
		
		if ( this->mRepeatY == false ) {
			this->mGrid->ClampY ( c0 );
			this->mGrid->ClampY ( c1 );
		}
		
		this->mGfxSource->GatherSurfaces ( *this->mGrid, c0, c1, sampler );
	}	
}

//----------------------------------------------------------------//
void MOAITilemap::GetBoundsInView ( USTileCoord& c0, USTileCoord& c1 ) {

	const USAffine2D& invWorldMtx = this->GetWorldToLocalMtx ();

	// view quad in world space
	USViewQuad viewQuad;
	viewQuad.Init ();
	viewQuad.Transform ( invWorldMtx );
	
	USRect viewRect = viewQuad.mBounds;
	viewRect.Bless ();
	
	c0 = this->mGrid->GetTileCoord ( viewRect.mXMin, viewRect.mYMin );
	c1 = this->mGrid->GetTileCoord ( viewRect.mXMax, viewRect.mYMax );
	
	if ( this->mRepeatX == false ) {
		this->mGrid->ClampX ( c0 );
		this->mGrid->ClampX ( c1 );
	}
	
	if ( this->mRepeatY == false ) {
		this->mGrid->ClampY ( c0 );
		this->mGrid->ClampY ( c1 );
	}
}

//----------------------------------------------------------------//
u32 MOAITilemap::GetLocalFrame ( USRect& frame ) {

	if ( this->mGrid ) {
	
		frame = this->mGrid->GetBounds ();
	
		if ( this->mRepeatX || this->mRepeatY ) {
			return FRAME_GLOBAL;
		}
		else {
			return FRAME_OK;
		}
	}
	return FRAME_EMPTY;
}

//----------------------------------------------------------------//
USVec2D MOAITilemap::GetTileLoc ( int xTile, int yTile, u32 position ) {

	USVec2D loc ( 0.0f, 0.0f );
	
	if ( this->mGrid ) {	
		loc = this->mGrid->GetTilePoint ( xTile, yTile, position );
	}
	
	const USAffine2D modelToWorld = this->GetLocalToWorldMtx ();
	modelToWorld.Transform ( loc );
	return loc;
}

//----------------------------------------------------------------//
MOAITilemap::MOAITilemap () :
	mRepeatX ( false ),
	mRepeatY ( false ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIGfxPrim2D )
	RTTI_END
	
	this->mScale.Init ( 1.0f, -1.0f );
}

//----------------------------------------------------------------//
MOAITilemap::~MOAITilemap () {
}

//----------------------------------------------------------------//
void MOAITilemap::RegisterLuaClass ( USLuaState& state ) {
	
	MOAIPrim::RegisterLuaClass ( state );
	MOAIGfxPrim2D::RegisterLuaClass ( state );
	
	state.SetField ( -1, "TILE_LEFT_TOP", ( u32 )USGridSpace::TILE_LEFT_TOP );
	state.SetField ( -1, "TILE_RIGHT_TOP", ( u32 )USGridSpace::TILE_RIGHT_TOP );
	state.SetField ( -1, "TILE_LEFT_BOTTOM", ( u32 )USGridSpace::TILE_LEFT_BOTTOM );
	state.SetField ( -1, "TILE_RIGHT_BOTTOM", ( u32 )USGridSpace::TILE_RIGHT_BOTTOM );
	
	state.SetField ( -1, "TILE_LEFT_CENTER", ( u32 )USGridSpace::TILE_LEFT_CENTER );
	state.SetField ( -1, "TILE_RIGHT_CENTER", ( u32 )USGridSpace::TILE_RIGHT_CENTER );
	state.SetField ( -1, "TILE_TOP_CENTER", ( u32 )USGridSpace::TILE_TOP_CENTER );
	state.SetField ( -1, "TILE_BOTTOM_CENTER", ( u32 )USGridSpace::TILE_BOTTOM_CENTER );
	
	state.SetField ( -1, "TILE_CENTER", ( u32 )USGridSpace::TILE_CENTER );
}

//----------------------------------------------------------------//
void MOAITilemap::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIPrim::RegisterLuaFuncs ( state );
	MOAIGfxPrim2D::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "getGrid",			_getGrid },
		{ "getTile",			_getTile },
		{ "getTileLoc",			_getTileLoc },
		{ "setGrid",			_setGrid },
		{ "setRepeat",			_setRepeat },
		{ "setTile",			_setTile },
		{ "wrapCoord",			_wrapCoord },
		{ "worldToCoord",		_worldToCoord },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITilemap::SerializeIn ( USLuaState& state, USLuaSerializer& serializer ) {
	
	this->mGfxSource = serializer.GetRefField < MOAIContentLibrary2D >( state, -1, "mGfxSource" );
	this->mGrid = serializer.GetRefField < MOAIGrid >( state, -1, "mGrid" );
}

//----------------------------------------------------------------//
void MOAITilemap::SerializeOut ( USLuaState& state, USLuaSerializer& serializer ) {
	
	serializer.SetRefField ( state, -1, "mGfxSource", this->mGfxSource );
	serializer.SetRefField ( state, -1, "mGrid", this->mGrid );
}

//----------------------------------------------------------------//
STLString MOAITilemap::ToString () {

	STLString repr( MOAITransform2D::ToString () );

	PRETTY_PRINT ( repr, mGfxSource )

	return repr;
}
