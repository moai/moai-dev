// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorDrawingDeck.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVectorDrawingDeck::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawingDeck, "U" )

	u32 total = state.GetValue < u32 >( 2, 0 );
	self->mDrawings.Init ( total );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIVectorDrawingDeck::_setDrawing ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIVectorDrawingDeck, "U" )

	u32 idx = state.GetValue < u32 >( 2, 1 ) - 1;

	self->mDrawings [ idx ].Set ( *self, state.GetLuaObject < MOAIVectorDrawing >( 3, true ));

	return 0;
}

//================================================================//
// MOAIVectorDrawingDeck
//================================================================//

//----------------------------------------------------------------//
void MOAIVectorDrawingDeck::Clear () {

	for ( u32 i = 0; i < this->mDrawings.Size (); ++i ) {
		this->mDrawings [ i ].Set ( *this, 0 );
	}
	this->mDrawings.Clear ();
}

//----------------------------------------------------------------//
ZLBox MOAIVectorDrawingDeck::ComputeMaxBounds () {
	ZLBox bounds;
	bounds.Init ( -50, 50, 50, -50, 0, 0 );
	return bounds;
}

//----------------------------------------------------------------//
ZLBox MOAIVectorDrawingDeck::GetItemBounds ( u32 idx ) {
	UNUSED ( idx );
	ZLBox bounds;
	bounds.Init ( -50, 50, 50, -50, 0, 0 );
	return bounds;
}

//----------------------------------------------------------------//
void MOAIVectorDrawingDeck::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( idx );
	UNUSED ( xOff );
	UNUSED ( yOff );
	UNUSED ( zOff );
	UNUSED ( xScl );
	UNUSED ( yScl );
	UNUSED ( zScl );
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_MODEL );
	gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
	gfxDevice.SetTexture ();
	
	u32 size = this->mDrawings.Size ();
	if ( size ) {
		idx = ( idx - 1 ) % size;
		this->mDrawings [ idx ]->Draw ();
	}
}

//----------------------------------------------------------------//
MOAIVectorDrawingDeck::MOAIVectorDrawingDeck () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
	
	this->mDefaultShaderID = MOAIShaderMgr::LINE_SHADER_3D;
}

//----------------------------------------------------------------//
MOAIVectorDrawingDeck::~MOAIVectorDrawingDeck () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIVectorDrawingDeck::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIVectorDrawingDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "reserve",			_reserve },
		{ "setDrawing",			_setDrawing },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

