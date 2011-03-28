// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAISurfaceSampler2D.h>
#include <moaicore/MOAITransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIDeck::_setDefaultShader ( lua_State* L ) {
	LUA_SETUP ( MOAIDeck, "UU" )
	
	self->mDefaultShader = state.GetLuaData < MOAIShader >( 2 );
	
	return 0;
}

//================================================================//
// MOAIDeck
//================================================================//

//----------------------------------------------------------------//
bool MOAIDeck::Bind () {

	return false;
}

//----------------------------------------------------------------//
bool MOAIDeck::Contains ( u32 idx, const USVec2D& vec ) {
	
	USRect bounds = this->GetBounds ( idx );
	return bounds.Contains ( vec );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( const USAffine2D& transform, u32 idx ) {
	UNUSED ( transform );
	UNUSED ( idx );
}

//----------------------------------------------------------------//
void MOAIDeck::Draw ( const USAffine2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
void MOAIDeck::DrawDebug ( const USAffine2D& transform, u32 idx ) {
	UNUSED ( transform );
	UNUSED ( idx );
}

//----------------------------------------------------------------//
void MOAIDeck::DrawDebug ( const USAffine2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
void MOAIDeck::GatherSurfaces ( u32 idx, MOAISurfaceSampler2D& sampler ) {
	UNUSED ( idx );
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
void MOAIDeck::GatherSurfaces ( MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1, MOAISurfaceSampler2D& sampler ) {
	UNUSED ( grid );
	UNUSED ( c0 );
	UNUSED ( c1 );
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
USRect MOAIDeck::GetBounds ( u32 idx ) {
	UNUSED ( idx );

	USRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	return rect;
}

//----------------------------------------------------------------//
u32 MOAIDeck::GetContentAddr ( u32 idx, u32 total ) {

	if ( total ) {
		return idx % total;
	}
	return ( u32 )NO_CONTENT;
}

//----------------------------------------------------------------//
MOAIDeck::MOAIDeck () {
	
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

	LuaReg regTable [] = {
		{ "setDefaultShader",		_setDefaultShader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
