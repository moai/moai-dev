// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGfxQuad2D.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIMultiTexture.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setQuad
	@text	Set model space quad. Vertex order is clockwise from
			upper left (xMin, yMax)
	
	@in		MOAIGfxQuad2D self
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number x3
	@in		number y3
	@out	nil
*/
int MOAIGfxQuad2D::_setQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "UNNNNNNNN" )

	USQuad quad;
	
	quad.mV [ 0 ].mX = state.GetValue < float >( 2, 0.0f );
	quad.mV [ 0 ].mY = state.GetValue < float >( 3, 0.0f );
	quad.mV [ 1 ].mX = state.GetValue < float >( 4, 0.0f );
	quad.mV [ 1 ].mY = state.GetValue < float >( 5, 0.0f );
	quad.mV [ 2 ].mX = state.GetValue < float >( 6, 0.0f );
	quad.mV [ 2 ].mY = state.GetValue < float >( 7, 0.0f );
	quad.mV [ 3 ].mX = state.GetValue < float >( 8, 0.0f );
	quad.mV [ 3 ].mY = state.GetValue < float >( 9, 0.0f );

	self->mQuad.SetVerts ( quad.mV [ 0 ], quad.mV [ 1 ], quad.mV [ 2 ], quad.mV [ 3 ]);

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRect
	@text	Set the model space dimensions of the quad.
	
	@in		MOAIGfxQuad2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIGfxQuad2D::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "UNNNN" )
	
	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float x1	= state.GetValue < float >( 4, 0.0f );
	float y1	= state.GetValue < float >( 5, 0.0f );
	
	self->mQuad.SetVerts ( x0, y0, x1, y1 );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVQuad
	@text	Set the UV space dimensions of the quad. Vertex order is
			clockwise from upper left (xMin, yMax)
	
	@in		MOAIGfxQuad2D self
	@in		number x0
	@in		number y0
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@in		number x3
	@in		number y3
	@out	nil
*/
int MOAIGfxQuad2D::_setUVQuad ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "UNNNNNNNN" )

	USQuad quad;
	
	quad.mV [ 0 ].mX = state.GetValue < float >( 2, 0.0f );
	quad.mV [ 0 ].mY = state.GetValue < float >( 3, 0.0f );
	quad.mV [ 1 ].mX = state.GetValue < float >( 4, 0.0f );
	quad.mV [ 1 ].mY = state.GetValue < float >( 5, 0.0f );
	quad.mV [ 2 ].mX = state.GetValue < float >( 6, 0.0f );
	quad.mV [ 2 ].mY = state.GetValue < float >( 7, 0.0f );
	quad.mV [ 3 ].mX = state.GetValue < float >( 8, 0.0f );
	quad.mV [ 3 ].mY = state.GetValue < float >( 9, 0.0f );

	self->mQuad.SetUVs ( quad.mV [ 0 ], quad.mV [ 1 ], quad.mV [ 2 ], quad.mV [ 3 ]);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUVRect
	@text	Set the UV space dimensions of the quad.
	
	@in		MOAIGfxQuad2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIGfxQuad2D::_setUVRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "UNNNN" )
	
	float u0	= state.GetValue < float >( 2, 0.0f );
	float v0	= state.GetValue < float >( 3, 0.0f );
	float u1	= state.GetValue < float >( 4, 0.0f );
	float v1	= state.GetValue < float >( 5, 0.0f );

	self->mQuad.SetUVs ( u0, v0, u1, v1 );

	return 0;
}

//================================================================//
// MOAIGfxQuad2D
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxQuad2D::DrawPatch ( u32 idx, float xOff, float yOff, float xScale, float yScale ) {
	UNUSED ( idx );
	
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	MOAIQuadBrush::BindVertexFormat ( gfxDevice );
	
	this->mQuad.Draw ( xOff, yOff, xScale, yScale );
}

//----------------------------------------------------------------//
USRect MOAIGfxQuad2D::GetRect ( ) {	
	return this->mQuad.GetVtxBounds ();
}

//----------------------------------------------------------------//
USRect MOAIGfxQuad2D::GetRect ( u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );
	
	return this->mQuad.GetVtxBounds ();
}

//----------------------------------------------------------------//
MOAIGfxQuad2D::MOAIGfxQuad2D () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck2D )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
	
	// set up rects to draw a unit tile centered at the origin
	this->mQuad.SetVerts ( -0.5f, -0.5f, 0.5f, 0.5f );
	this->mQuad.SetUVs ( 0.0f, 1.0f, 1.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIGfxQuad2D::~MOAIGfxQuad2D () {

	this->mTexture.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck2D::RegisterLuaClass ( state );
	
	state.SetField ( -1, "FILTER_POINT", ( u32 )GL_NEAREST );
	state.SetField ( -1, "FILTER_BILERP", ( u32 )GL_LINEAR );
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck2D::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setQuad",			_setQuad },
		{ "setRect",			_setRect },
		{ "setUVQuad",			_setUVQuad },
		{ "setUVRect",			_setUVRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
