// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIQuadBrush.h>
#include <moai-sim/MOAIStretchDeck.h>

//================================================================//
// lua
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

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();

	if ( this->mStretchFactor > 0.0f ) {
	
		// TODO: propery implement stretch factor
	
		ZLMatrix4x4 worldTransform = gfxMgr.GetMtx ( MOAIGfxMgr::MODEL_TO_WORLD_MTX );
		stretch = worldTransform.GetStretch ();
		
		ZLMatrix4x4 noStretchMtx;
		noStretchMtx.Scale ( 1.0f / stretch.mX, 1.0f / stretch.mY, 1.0f / stretch.mZ );
		noStretchMtx.Append ( worldTransform );
		noStretchMtx.Append ( gfxMgr.GetMtx ( MOAIGfxMgr::WORLD_TO_CLIP_MTX ));
		
		gfxMgr.SetVertexTransform ( noStretchMtx );
	}
	else {
	
		gfxMgr.SetVertexTransform ( MOAIGfxMgr::MODEL_TO_CLIP_MTX );
	}
	
	return stretch;
}

//----------------------------------------------------------------//
MOAIStretchDeck::MOAIStretchDeck () :
	mStretchFactor ( 1.0f ) {

	RTTI_BEGIN ( MOAIStretchDeck )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIStretchDeck >)
		RTTI_EXTEND ( MOAIDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStretchDeck::~MOAIStretchDeck () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIStretchDeck::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIStretchDeck::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setStretchFactor",		_setStretchFactor },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
