// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIBoundsDeck.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getBounds
	@text	Return bounds for an item or the maximum bounds for the
			deck.

	@override

		@in		number idx
		@out	xMin
		@out	yMin
		@out	zMin
		@out	xMax
		@out	yMax
		@out	zMax
	
	@override

		@out	xMin
		@out	yMin
		@out	zMin
		@out	xMax
		@out	yMax
		@out	zMax
*/
int MOAIDeck::_getBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )
	
	ZLBox box;
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
	
		u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;
		box = self->GetBounds ( idx );
	}
	else {
	
		box = self->GetBounds ();
	}
	
	state.Push ( box );
	return 6;
}

//----------------------------------------------------------------//
/**	@lua	subdivideRect
	@text	Convenience method. Here for now as a class method, but maybe should
			move to MOAIGrid.
			
			Subdivides a rectangle given a tile width and height. A table tile
			rectangles will be returned. The tiles will be clipped to the original
			rect.

	@in		number tileWidth
	@in		number tileHeight
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIDeck::_subdivideRect ( lua_State* L ) {
	MOAI_LUA_SETUP_CLASS ( "NNNNNN" )

	float tileWidth		= state.GetValue < float >( 1, 1.0f );
	float tileHeight	= state.GetValue < float >( 2, 1.0f );

	tileWidth	= ABS ( tileWidth );
	tileHeight	= ABS ( tileHeight );

	ZLRect rect = state.GetRect < float >( 3 );
	
	u32 x0 = ( u32 )( rect.mXMin / tileWidth );
	u32 x1 = ( u32 )( rect.mXMax / tileWidth );
	
	u32 y0 = ( u32 )( rect.mYMin / tileHeight );
	u32 y1 = ( u32 )( rect.mYMax / tileHeight );
	
	u32 count	= 0;
	u32 xCount	= 0;
	u32 yCount	= 0;
	
	lua_newtable ( state );
	
	for ( u32 y = y0; y <= y1; ++y ) {
		for ( u32 x = x0; x <= x1; ++x ) {
		
			ZLRect tile;
		
			tile.mXMin	= x * tileWidth;
			tile.mYMin	= y * tileHeight;
			tile.mXMax	= tile.mXMin + tileWidth;
			tile.mYMax	= tile.mYMin + tileHeight;
		
			ZLRect sub;
			if ( tile.Intersect ( rect, sub )) {
			
				count++;
				state.Push ( count );
			
				lua_newtable ( state );
				state.SetFieldByIndex < float >( -1, 1, sub.mXMin );
				state.SetFieldByIndex < float >( -1, 2, sub.mYMin );
				state.SetFieldByIndex < float >( -1, 3, sub.mXMax );
				state.SetFieldByIndex < float >( -1, 4, sub.mYMax );
				
				lua_settable ( state, -3 );
				
				xCount = ( x - x0 ) + 1;
				yCount = ( y - y0 ) + 1;
			}
		}
	}

	state.Push ( xCount );
	state.Push ( yCount );

	return 3;
}

//================================================================//
// MOAIDeck
//================================================================//

//----------------------------------------------------------------//
bool MOAIDeck::Contains ( u32 idx, const ZLVec2D& vec ) {
	
	ZLRect bounds = this->GetBounds ( idx ).GetRect ( ZLBox::PLANE_XY );
	return bounds.Contains ( vec );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( u32 idx, MOAIMaterialBatch& materials ) {

	this->Draw ( idx, materials, ZLVec3D::ORIGIN, ZLVec3D::AXIS );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {
	
	if ( !idx || ( idx & MOAITileFlags::HIDDEN )) return;
	
	scale.mX = ( idx & MOAITileFlags::XFLIP ) ? -scale.mX : scale.mX;
	scale.mY = ( idx & MOAITileFlags::YFLIP ) ? -scale.mY : scale.mY;
	
	this->DrawIndex ( idx & MOAITileFlags::CODE_MASK, materials, offset, scale );
}

//----------------------------------------------------------------//
void MOAIDeck::DrawIndex ( u32 idx, MOAIMaterialBatch& materials, ZLVec3D offset, ZLVec3D scale ) {
	UNUSED ( idx );
	UNUSED ( materials );
	UNUSED ( offset );
	UNUSED ( scale );
}

//----------------------------------------------------------------//
void MOAIDeck::GetCollisionShape ( MOAICollisionShape& shape ) {

	shape.Set ();
}

//----------------------------------------------------------------//
bool MOAIDeck::Inside ( u32 idx, MOAIMaterialBatch& materials, u32 granularity, ZLVec3D vec, float pad ) {
	UNUSED ( idx );
	UNUSED ( materials );
	UNUSED ( granularity );
	UNUSED ( vec );
	UNUSED ( pad );
	
	// we've passed the trivial reject; if we don't have a more finely grained test, return 'true'
	return true;
}

//----------------------------------------------------------------//
MOAIDeck::MOAIDeck () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeck::~MOAIDeck () {
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "subdivideRect",			_subdivideRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getBounds",				_getBounds },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
