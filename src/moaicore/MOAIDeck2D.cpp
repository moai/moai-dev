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

//----------------------------------------------------------------//
void MOAIDeck2D::Draw ( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper ) {
	
	idx = remapper ? remapper->Remap ( idx ) : idx;
	if ( !idx || ( idx & MOAITileFlags::HIDDEN )) return;
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, transform );
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
	
	float xScale = ( idx & MOAITileFlags::XFLIP ) ? -1.0f : 1.0f;
	float yScale = ( idx & MOAITileFlags::YFLIP ) ? -1.0f : 1.0f;
	
	this->DrawPatch ( idx & MOAITileFlags::CODE_MASK, 0.0f, 0.0f, xScale, yScale );
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
void MOAIDeck2D::Draw ( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 ) {
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM, transform );
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );

	float width = grid.GetTileWidth () * gridScale.mX;
	float height = grid.GetTileHeight () * gridScale.mY;

	for ( int y = c0.mY; y <= c1.mY; ++y ) {
		for ( int x = c0.mX; x <= c1.mX; ++x ) {
			
			u32 idx = grid.GetTile ( x, y );
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

//----------------------------------------------------------------//
void MOAIDeck2D::DrawDebug ( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( transform );
	UNUSED ( idx );
	UNUSED ( remapper );
}

//----------------------------------------------------------------//
void MOAIDeck2D::DrawDebug ( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
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

//----------------------------------------------------------------//
MOAIDeck2D::MOAIDeck2D () {
	
	RTTI_SINGLE ( MOAIDeck )
}

//----------------------------------------------------------------//
MOAIDeck2D::~MOAIDeck2D () {
}

//----------------------------------------------------------------//
void MOAIDeck2D::RegisterLuaClass ( USLuaState& state ) {

	this->MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeck2D::RegisterLuaFuncs ( USLuaState& state ) {

	this->MOAIDeck::RegisterLuaFuncs ( state );
}