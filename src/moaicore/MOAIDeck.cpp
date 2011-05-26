// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAISurfaceSampler2D.h>
#include <moaicore/MOAITransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setDefaultShader
	@text	Set the shader to use if neither the deck item nor the prop specifies a shader.
	
	@in		MOAIDeck self
	@in		MOAIShader shader
	@out	nil
*/
int MOAIDeck::_setDefaultShader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeck, "UU" )
	
	self->mDefaultShader = state.GetLuaObject < MOAIShader >( 2 );
	
	return 0;
}

//================================================================//
// MOAIDeck
//================================================================//

//----------------------------------------------------------------//
bool MOAIDeck::Bind () {

	return true;
}

//----------------------------------------------------------------//
bool MOAIDeck::Contains ( u32 idx, MOAIDeckRemapper* remapper, const USVec2D& vec ) {
	
	USRect bounds = this->GetBounds ( idx, remapper );
	return bounds.Contains ( vec );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( transform );
	UNUSED ( idx );
	UNUSED ( remapper );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
void MOAIDeck::DrawDebug ( const USAffine2D& transform, u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( transform );
	UNUSED ( idx );
	UNUSED ( remapper );
}

//----------------------------------------------------------------//
void MOAIDeck::DrawDebug ( const USAffine2D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1 ) {
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
void MOAIDeck::GatherSurfaces ( MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, USCellCoord& c0, USCellCoord& c1, MOAISurfaceSampler2D& sampler ) {
	UNUSED ( grid );
	UNUSED ( remapper );
	UNUSED ( gridScale );
	UNUSED ( c0 );
	UNUSED ( c1 );
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
USRect MOAIDeck::GetBounds ( u32 idx, MOAIDeckRemapper* remapper ) {
	UNUSED ( idx );
	UNUSED ( remapper );

	USRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	return rect;
}

//----------------------------------------------------------------//
MOAIDeck::MOAIDeck () :
	mContentMask ( 0xffffffff ) {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIDeck::~MOAIDeck () {
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDeck::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "setDefaultShader",		_setDefaultShader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
