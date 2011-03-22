// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIContentLibrary2D.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAISurfaceSampler2D.h>
#include <moaicore/MOAITransform2D.h>

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
int MOAIContentLibrary2D::_setDefaultShader ( lua_State* L ) {
	LUA_SETUP ( MOAIContentLibrary2D, "UU" )
	
	self->mDefaultShader = state.GetLuaData < MOAIShader >( 2 );
	
	return 0;
}

//================================================================//
// MOAIContentLibrary2D
//================================================================//

//----------------------------------------------------------------//
bool MOAIContentLibrary2D::Bind () {

	return false;
}

//----------------------------------------------------------------//
bool MOAIContentLibrary2D::Contains ( u32 idx, const USVec2D& vec ) {
	
	USRect bounds = this->GetBounds ( idx );
	return bounds.Contains ( vec );
}

//----------------------------------------------------------------//
void MOAIContentLibrary2D::Draw ( MOAIDrawingMtx2D& transform, u32 idx ) {
	UNUSED ( transform );
	UNUSED ( idx );
}

//----------------------------------------------------------------//
void MOAIContentLibrary2D::Draw ( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
void MOAIContentLibrary2D::DrawDebug ( MOAIDrawingMtx2D& transform, u32 idx ) {
	UNUSED ( transform );
	UNUSED ( idx );
}

//----------------------------------------------------------------//
void MOAIContentLibrary2D::DrawDebug ( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 ) {
	UNUSED ( transform );
	UNUSED ( grid );
	UNUSED ( c0 );
	UNUSED ( c1 );
}

//----------------------------------------------------------------//
void MOAIContentLibrary2D::GatherSurfaces ( u32 idx, MOAISurfaceSampler2D& sampler ) {
	UNUSED ( idx );
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
void MOAIContentLibrary2D::GatherSurfaces ( MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1, MOAISurfaceSampler2D& sampler ) {
	UNUSED ( grid );
	UNUSED ( c0 );
	UNUSED ( c1 );
	UNUSED ( sampler );
}

//----------------------------------------------------------------//
USRect MOAIContentLibrary2D::GetBounds ( u32 idx ) {
	UNUSED ( idx );

	USRect rect;
	rect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	return rect;
}

//----------------------------------------------------------------//
u32 MOAIContentLibrary2D::GetContentAddr ( u32 idx, u32 total ) {

	if ( total ) {
		return idx % total;
	}
	return ( u32 )NO_CONTENT;
}

//----------------------------------------------------------------//
MOAIContentLibrary2D::MOAIContentLibrary2D () {
	
	RTTI_SINGLE ( USLuaData )
}

//----------------------------------------------------------------//
MOAIContentLibrary2D::~MOAIContentLibrary2D () {
}

//----------------------------------------------------------------//
void MOAIContentLibrary2D::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIContentLibrary2D::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "setDefaultShader",		_setDefaultShader },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
