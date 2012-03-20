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
	
	self->mRect = state.GetRect < float >( 2 );

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
	
	self->mUVRect = state.GetRect < float >( 2 );

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
	
	MOAIQuadBrush quad;
	quad.SetVerts ( this->mRect );
	quad.SetUVs ( this->mUVRect );
	quad.Draw ( xOff, yOff, xScale, yScale );
}

//----------------------------------------------------------------//
USRect MOAIGfxQuad2D::GetRect ( ) {	
	return this->mRect;
}

//----------------------------------------------------------------//
USRect MOAIGfxQuad2D::GetRect ( u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );
	
	return this->mRect;
}

//----------------------------------------------------------------//
MOAIGfxQuad2D::MOAIGfxQuad2D () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck2D )
	RTTI_END
	
	this->SetContentMask ( MOAIProp::CAN_DRAW );
	
	// set up rects to draw a unit tile centered at the origin
	this->mRect.Init ( -0.5f, -0.5f, 0.5f, 0.5f );
	this->mUVRect.Init ( 0.0f, 1.0f, 1.0f, 0.0f );
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
		{ "setRect",			_setRect },
		{ "setUVRect",			_setUVRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
