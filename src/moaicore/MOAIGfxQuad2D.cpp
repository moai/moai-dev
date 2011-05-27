// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIGfxQuad2D.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITexture.h>
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
/**	@name	setTexture
	@text	Set or load a texture for this deck.
	
	@in		MOAIGfxQuad2D self
	@in		variant texture			A MOAITexture, a MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAITexture.QUANTIZE, MOAITexture.TRUECOLOR, MOAITexture.PREMULTIPLY_ALPHA
	@out	MOAITexture texture
*/
int MOAIGfxQuad2D::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxQuad2D, "U" )

	self->mTexture = MOAITexture::AffirmTexture ( state, 2 );
	if ( self->mTexture ) {
		self->mTexture->PushLuaUserdata ( state );
		return 1;
	}
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
bool MOAIGfxQuad2D::Bind () {

	if ( this->mTexture ) {

		if ( !this->mTexture->Bind ()) return false;
		
		USDrawBuffer& drawBuffer = USDrawBuffer::Get ();
		USGLQuad::BindVertexFormat ( drawBuffer );
	}

	return true;
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::DrawPatch ( u32 idx, float xOff, float yOff, float xScale, float yScale ) {
	UNUSED ( idx );
	
	USGLQuad quad;
	quad.SetVerts ( this->mRect );
	quad.SetUVs ( this->mUVRect );
	quad.Draw ( xOff, yOff, xScale, yScale );
}

//----------------------------------------------------------------//
USRect MOAIGfxQuad2D::GetBounds ( u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );
	
	return this->mRect;
}

//----------------------------------------------------------------//
MOAIGfxQuad2D::MOAIGfxQuad2D () {

	RTTI_SINGLE ( MOAIDeck2D )
	this->SetContentMask ( MOAIProp::CAN_DRAW );
	
	// set up rects to draw a unit tile centered at the origin
	this->mRect.Init ( -0.5f, -0.5f, 0.5f, 0.5f );
	this->mUVRect.Init ( 0.0f, 1.0f, 1.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIGfxQuad2D::~MOAIGfxQuad2D () {
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::RegisterLuaClass ( USLuaState& state ) {

	this->MOAIDeck2D::RegisterLuaClass ( state );
	
	state.SetField ( -1, "FILTER_POINT", ( u32 )GL_NEAREST );
	state.SetField ( -1, "FILTER_BILERP", ( u32 )GL_LINEAR );
}

//----------------------------------------------------------------//
void MOAIGfxQuad2D::RegisterLuaFuncs ( USLuaState& state ) {

	this->MOAIDeck2D::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setRect",			_setRect },
		{ "setTexture",			_setTexture },
		{ "setUVRect",			_setUVRect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIGfxQuad2D::ToString () {

	STLString repr;

	return repr;
}
