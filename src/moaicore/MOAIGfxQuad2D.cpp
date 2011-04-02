// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIGfxQuad2D.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>setRect ( self, xMin, yMin, xMax, yMax )</tt>\n
\n
	Convenience method. Sets the default dimentions for primitives displaying this texture.
	@param self (in)
	@param xMin (in)
	@param yMin (in)
	@param xMax (in)
	@param yMax (in)
*/
int MOAIGfxQuad2D::_setRect ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuad2D, "UNNNN" )
	
	self->mRect = state.GetRect < float >( 2 );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxQuad2D::_setTexture ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuad2D, "U" )

	self->mTexture = MOAITexture::AffirmTexture ( state, 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIGfxQuad2D::_setUVRect ( lua_State* L ) {
	LUA_SETUP ( MOAIGfxQuad2D, "UNNNN" )
	
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
void MOAIGfxQuad2D::Draw ( u32 idx, float xOff, float yOff, float xScale, float yScale ) {
	UNUSED ( idx );
	
	USGLQuad quad;
	quad.SetVerts ( this->mRect );
	quad.SetUVs ( this->mUVRect );
	quad.Draw ( xOff, yOff, xScale, yScale );
}

//----------------------------------------------------------------//
USRect MOAIGfxQuad2D::GetBounds ( u32 idx ) {
	UNUSED ( idx );
	
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

	LuaReg regTable [] = {
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
