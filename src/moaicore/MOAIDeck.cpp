// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGfxResource.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIMultiTexture.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAISurfaceSampler2D.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAITransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setShader
	@text	Set the shader to use if neither the deck item nor the prop specifies a shader.
	
	@in		MOAIDeck self
	@in		MOAIShader shader
	@out	nil
*/
int MOAIDeck::_setShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "UU" )
	
	self->mShader.Set ( *self, state.GetLuaObject < MOAIShader >( 2 ));
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTexture
	@text	Set or load a texture for this deck.
	
	@in		MOAIDeck self
	@in		variant texture		A MOAITexture, MOAIMultiTexture, MOAIDataBuffer or a path to a texture file
	@opt	number transform	Any bitwise combination of MOAITextureBase.QUANTIZE, MOAITextureBase.TRUECOLOR, MOAITextureBase.PREMULTIPLY_ALPHA
	@out	MOAIGfxState texture
*/
int MOAIDeck::_setTexture ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "U" )

	MOAIGfxState* texture = MOAITexture::AffirmTexture ( state, 2 );
	self->mTexture.Set ( *self, texture );

	if ( texture ) {
		self->mTexture->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIDeck
//================================================================//

//----------------------------------------------------------------//
bool MOAIDeck::Contains ( u32 idx, MOAIDeckRemapper* remapper, const USVec2D& vec ) {
	
	USRect bounds = this->GetBounds ( idx, remapper ).GetRect ( USBox::PLANE_XY );
	return bounds.Contains ( vec );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( const USAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( transform );
	UNUSED ( idx );
	UNUSED ( remapper );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( const USAffine3D& transform, bool reload, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, int cellAddr ) {
	UNUSED ( transform );
	UNUSED ( reload );
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( cellAddr );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( const USAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
void MOAIDeck::DrawDebug ( const USAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( transform );
	UNUSED ( idx );
	UNUSED ( remapper );
}

//----------------------------------------------------------------//
void MOAIDeck::DrawDebug ( const USAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
void MOAIDeck::GatherSurfaces ( u32 idx, MOAIDeckRemapper* remapper, MOAISurfaceSampler2D& sampler ) {
	UNUSED ( idx );
	UNUSED ( remapper );
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
void MOAIDeck::GatherSurfaces ( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1, MOAISurfaceSampler2D& sampler ) {
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
USBox MOAIDeck::GetBounds () {
	return GetBounds ( 0, NULL );
}

//----------------------------------------------------------------//
USBox MOAIDeck::GetBounds ( u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );

	USBox bounds;
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
	return bounds;
}

//----------------------------------------------------------------//
MOAIGfxState* MOAIDeck::GetShader () {

	return this->mShader ? this->mShader : this->GetShaderDefault ();
}

//----------------------------------------------------------------//
MOAIGfxState* MOAIDeck::GetShaderDefault () {

	return &MOAIShaderMgr::Get ().GetShader ( MOAIShaderMgr::DECK2D_SHADER );
}

//----------------------------------------------------------------//
MOAIGfxState* MOAIDeck::GetTexture () {

	return this->mTexture;
}

//----------------------------------------------------------------//
MOAIDeck::MOAIDeck () :
	mContentMask ( 0xffffffff ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIDeck::~MOAIDeck () {

	this->mShader.Set ( *this, 0 );
	this->mTexture.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "setShader",				_setShader },
		{ "setTexture",				_setTexture },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
