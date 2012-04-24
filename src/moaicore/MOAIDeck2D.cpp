// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeck2D.h>
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAISurfaceSampler2D.h>
#include <moaicore/MOAITransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIDeck2D
//================================================================//

/*
//----------------------------------------------------------------//
void MOAIDeck2D::Draw ( u32 idx, MOAIDeckRemapper* remapper ) {
	
	idx = remapper ? remapper->Remap ( idx ) : idx;
	if ( !idx || ( idx & MOAITileFlags::HIDDEN )) return;
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
	
	float xScale = ( idx & MOAITileFlags::XFLIP ) ? -1.0f : 1.0f;
	float yScale = ( idx & MOAITileFlags::YFLIP ) ? -1.0f : 1.0f;
	
	this->DrawPatch ( idx & MOAITileFlags::CODE_MASK, 0.0f, 0.0f, xScale, yScale );
}

//----------------------------------------------------------------//
void MOAIDeck2D::Draw ( bool reload, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, int cellAddr ) {

	if ( reload ) {
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
	}

	float width = grid.GetTileWidth () * gridScale.mX;
	float height = grid.GetTileHeight () * gridScale.mY;
			
	MOAICellCoord coord;
	coord = grid.GetCellCoord ( cellAddr );
	
	u32 idx = grid.GetTile ( coord.mX, coord.mY );
	idx = remapper ? remapper->Remap ( idx ) : idx;
	
	if ( !idx || ( idx & MOAITileFlags::HIDDEN )) return;
	
	USVec2D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );
	
	float xScale = ( idx & MOAITileFlags::XFLIP ) ? -width : width;
	float yScale = ( idx & MOAITileFlags::YFLIP ) ? -height : height;
	
	this->DrawPatch ( idx & MOAITileFlags::CODE_MASK, loc.mX, loc.mY, xScale, yScale );
}

//----------------------------------------------------------------//
void MOAIDeck2D::Draw ( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
	
	// kill the offset from the model matrix; grid will provide this
	USMatrix4x4 transform = gfxDevice.GetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
	
	float width = grid.GetTileWidth () * gridScale.mX;
	float height = grid.GetTileHeight () * gridScale.mY;

	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
			
			u32 idx = grid.GetTile ( wrap.mX, wrap.mY );
			idx = remapper ? remapper->Remap ( idx ) : idx;
			
			if ( !idx || ( idx & MOAITileFlags::HIDDEN )) continue;
			
			MOAICellCoord coord ( x, y );
			USVec2D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );
			
			float xScale = ( idx & MOAITileFlags::XFLIP ) ? -width : width;
			float yScale = ( idx & MOAITileFlags::YFLIP ) ? -height : height;
			
			this->DrawPatch ( idx & MOAITileFlags::CODE_MASK, loc.mX, loc.mY, xScale, yScale );
		}
	}
}

// test of grid drawing for iso
void MOAIDeck2D::Draw ( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 ) {
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	// store the original view and proj matrices
	USMatrix4x4 viewMtx = gfxDevice.GetVertexTransform ( MOAIGfxDevice::VTX_VIEW_TRANSFORM );
	USMatrix4x4 projMtx = gfxDevice.GetVertexTransform ( MOAIGfxDevice::VTX_PROJ_TRANSFORM );
	
	// clear them out
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM ); // model to identity
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_VIEW_TRANSFORM ); // view to identity
	
	// set up screen space drawing
	USMatrix4x4 wndToNorm = gfxDevice.GetWndToNormMtx ();
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_PROJ_TRANSFORM, wndToNorm ); // draw in screen space
	
	// build the model to wnd matrix
	USMatrix4x4 modelToWnd;
	modelToWnd.Init ( transform );
	modelToWnd.Append ( viewMtx );
	modelToWnd.Append ( projMtx );
	modelToWnd.Append ( gfxDevice.GetNormToWndMtx ());
	
	// vertex modes are business as usual
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );

	float width = grid.GetTileWidth () * gridScale.mX;
	float height = grid.GetTileHeight () * gridScale.mY;

	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			MOAICellCoord wrap = grid.WrapCellCoord ( x, y );
			
			u32 idx = grid.GetTile ( wrap.mX, wrap.mY );
			idx = remapper ? remapper->Remap ( idx ) : idx;
			
			if ( !idx || ( idx & MOAITileFlags::HIDDEN )) continue;
			
			MOAICellCoord coord ( x, y );
			USVec2D loc = grid.GetTilePoint ( coord, MOAIGridSpace::TILE_CENTER );
			
			// transform the loc into screen space
			// TODO: handle 3D projection
			modelToWnd.Transform ( loc );
			
			float xScale = ( idx & MOAITileFlags::XFLIP ) ? -width : width;
			float yScale = ( idx & MOAITileFlags::YFLIP ) ? -height : height;
			
			this->DrawPatch ( idx & MOAITileFlags::CODE_MASK, loc.mX, loc.mY, xScale, yScale );
		}
	}
	
	// restore the view and proj matrices
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_VIEW_TRANSFORM, viewMtx );
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_PROJ_TRANSFORM, projMtx );	
}

//----------------------------------------------------------------//
void MOAIDeck2D::DrawDebug ( u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );
}

//----------------------------------------------------------------//
void MOAIDeck2D::DrawDebug ( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 ) {
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
void MOAIDeck2D::DrawPatch ( u32 idx, float xOff, float yOff, float xScale, float yScale ) {
	UNUSED ( idx );
	UNUSED ( xOff );
	UNUSED ( yOff );
	UNUSED ( xScale );
	UNUSED ( yScale );
}

//----------------------------------------------------------------//
void MOAIDeck2D::GatherSurfaces ( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler ) {
	UNUSED ( idx );
	UNUSED ( remapper );
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
void MOAIDeck2D::GatherSurfaces ( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1, MOAISurfaceSampler2D& sampler ) {
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
	UNUSED ( sampler );
}
*/

//----------------------------------------------------------------//
USBox MOAIDeck2D::GetBounds () {

	USRect rect = this->GetRect ();
	USBox bounds;
	
	bounds.mMin.Init ( rect.mXMin, rect.mYMin, 0.0f );
	bounds.mMax.Init ( rect.mXMax, rect.mYMax, 0.0f );
	
	return bounds;
}

//----------------------------------------------------------------//
USBox MOAIDeck2D::GetBounds ( u32 idx, MOAIDeckRemapper* remapper ) {

	USRect rect = this->GetRect ( idx, remapper );
	USBox bounds;
	
	bounds.mMin.Init ( rect.mXMin, rect.mYMin, 0.0f );
	bounds.mMax.Init ( rect.mXMax, rect.mYMax, 0.0f );
	
	return bounds;
}

//----------------------------------------------------------------//
MOAIDeck2D::MOAIDeck2D () {
	
	RTTI_SINGLE ( MOAIDeck )
}

//----------------------------------------------------------------//
MOAIDeck2D::~MOAIDeck2D () {
}

//----------------------------------------------------------------//
void MOAIDeck2D::RegisterLuaClass ( MOAILuaState& state ) {

	this->MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeck2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	this->MOAIDeck::RegisterLuaFuncs ( state );
}