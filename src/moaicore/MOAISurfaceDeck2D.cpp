// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAISurfaceDeck2D.h>
#include <moaicore/MOAISurfaceSampler2D.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	reserveSurfaceLists
	@text	Reserve surface lists for deck.
	
	@in		MOAISurfaceDeck2D self
	@in		number nLists
	@out	nil
*/
int MOAISurfaceDeck2D::_reserveSurfaceLists ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISurfaceDeck2D, "UN" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mBrushes.Init ( total );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	reserveSurfaces
	@text	Reserve surfaces for a given list in deck.
	
	@in		MOAISurfaceDeck2D self
	@in		number idx
	@in		number nSurfaces
	@out	nil
*/
int MOAISurfaceDeck2D::_reserveSurfaces ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISurfaceDeck2D, "UNN" )

	u32 brushID		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 total		= state.GetValue < u32 >( 3, 0 );
	
	if ( brushID < self->mBrushes.Size ()) {
		self->mBrushes [ brushID ].mSurfaces.Init ( total );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSurface
	@text	Set a surface in a surface list.
	
	@in		MOAISurfaceDeck2D self
	@in		number idx
	@in		number surfaceIdx
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@out	nil
*/
int MOAISurfaceDeck2D::_setSurface ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISurfaceDeck2D, "UNNNNNN" )

	u32 brushID		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 surfaceID	= state.GetValue < u32 >( 3, 1 ) - 1;
	
	if ( brushID < self->mBrushes.Size ()) {
		MOAISurfaceBrush2D& brush = self->mBrushes [ brushID ];
		if ( surfaceID < brush.mSurfaces.Size ()) {
		
			MOAISurface2D& surface = brush.mSurfaces [ surfaceID ];
			
			surface.mV0.mX		= state.GetValue < float >( 4, 0.0f );
			surface.mV0.mY		= state.GetValue < float >( 5, 0.0f );
			
			surface.mV1.mX		= state.GetValue < float >( 6, 0.0f );
			surface.mV1.mY		= state.GetValue < float >( 7, 0.0f );
			
			if ( surfaceID == 0 ) {
				brush.mBounds.Init ( surface.mV0 );
				brush.mBounds.Grow ( surface.mV1 );
			}
			else {
				brush.mBounds.Grow ( surface.mV0 );
				brush.mBounds.Grow ( surface.mV1 );
			}
		}
		self->SetBoundsDirty ();
	}
	return 0;
}

//================================================================//
// MOAISurfaceDeck2D
//================================================================//

//----------------------------------------------------------------//
USBox MOAISurfaceDeck2D::ComputeMaxBounds () {
	
	u32 size = this->mBrushes.Size ();

	USRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );

	for ( u32 i = 0; i < size; ++i ) {
		rect.Grow ( this->mBrushes [ i ].mBounds );
	}

	USBox bounds;
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
void MOAISurfaceDeck2D::Draw ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( zScl );
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	gfxDevice.SetPenColor ( 0xffffffff );
	gfxDevice.SetPenWidth ( 1.0f );
	
	
	idx = idx - 1;
	
	MOAISurfaceBrush2D& brush = this->mBrushes [ idx ];
	
	u32 total = brush.mSurfaces.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		MOAISurface2D& surface = brush.mSurfaces [ i ];
		
		USVec2D v0 = surface.mV0;
		USVec2D v1 = surface.mV1;
		
		v0.Scale ( xScl, yScl );
		v1.Scale ( xScl, yScl );
		
		draw.DrawLine ( v0.mX + xOff, v0.mY + yOff, zOff, v1.mX + xOff, v1.mY + yOff, 1.0f );
	}
}

//----------------------------------------------------------------//
USBox MOAISurfaceDeck2D::GetItemBounds ( u32 idx ) {
	
	USBox bounds;
	
	u32 size = this->mBrushes.Size ();
	if ( size ) {
	
		idx = ( idx - 1 ) % size;
	
		USRect rect = this->mBrushes [ idx ].mBounds;
		bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
		return bounds;
	}
	
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
MOAISurfaceDeck2D::MOAISurfaceDeck2D () {

	RTTI_SINGLE ( MOAIDeck )
	this->SetContentMask ( MOAIProp::CAN_DRAW_DEBUG | MOAIProp::CAN_GATHER_SURFACES );
}

//----------------------------------------------------------------//
MOAISurfaceDeck2D::~MOAISurfaceDeck2D () {
}

//----------------------------------------------------------------//
void MOAISurfaceDeck2D::RegisterLuaClass ( MOAILuaState& state ) {

	this->MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAISurfaceDeck2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	this->MOAIDeck::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserveSurfaceLists",	_reserveSurfaceLists },
		{ "reserveSurfaces",		_reserveSurfaces },
		{ "setSurface",				_setSurface },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
