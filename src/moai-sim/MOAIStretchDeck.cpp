// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIMaterialMgr.h>
#include <moai-sim/MOAIQuadBrush.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIStretchDeck.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAIStretchDeck::_setStretchFactor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStretchDeck, "U" )

	self->mStretchFactor = state.GetValue < float >( 2, 0.0f );

	return 0;
}

//================================================================//
// MOAIStretchDeck
//================================================================//

//----------------------------------------------------------------//
ZLVec3D MOAIStretchDeck::BindStretchVertexTransform () const {

	ZLVec3D stretch ( 1.0f, 1.0f, 1.0f );

	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;

	if ( this->mStretchFactor > 0.0f ) {
	
		// TODO: propery implement stretch factor
	
		ZLMatrix4x4 worldTransform = gfxState.GetMtx ( MOAIGfxState::MODEL_TO_WORLD_MTX );
		stretch = worldTransform.GetStretch ();
		
		ZLMatrix4x4 noStretchMtx;
		noStretchMtx.Scale ( 1.0f / stretch.mX, 1.0f / stretch.mY, 1.0f / stretch.mZ );
		noStretchMtx.Append ( worldTransform );
		noStretchMtx.Append ( gfxState.GetMtx ( MOAIGfxState::WORLD_TO_CLIP_MTX ));
		
		gfxState.SetVertexTransform ( noStretchMtx );
	}
	else {
	
		gfxState.SetVertexTransform ( MOAIGfxState::MODEL_TO_CLIP_MTX );
	}
	
	return stretch;
}

//----------------------------------------------------------------//
MOAIStretchDeck::MOAIStretchDeck () :
	mStretchFactor ( 1.0f ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStretchDeck::~MOAIStretchDeck () {
}

//----------------------------------------------------------------//
void MOAIStretchDeck::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIStretchDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setStretchFactor",		_setStretchFactor },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
