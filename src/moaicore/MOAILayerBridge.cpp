// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILayer.h>
#include <moaicore/MOAILayerBridge.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize the bridge transform (map coordinates in one layer onto
			another; useful for rendering screen space objects tied to world
			space coordinates - map pins, for example).
	
	@in		MOAILayerBridge self
	@in		MOAITransformBase sourceTransform
	@in		MOAILayer sourceLayer
	@in		MOAILayer destLayer
	@out	nil
*/
int MOAILayerBridge::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayerBridge, "UUUU" );
	
	MOAITransformBase* sourceTransform = state.GetLuaObject < MOAITransformBase >( 2, true );
	if ( !sourceTransform ) return 0;
	
	MOAILayer* sourceLayer = state.GetLuaObject < MOAILayer >( 3, true );
	if ( !sourceLayer ) return 0;
	
	MOAILayer* destLayer = state.GetLuaObject < MOAILayer >( 4, true );
	if ( !destLayer ) return 0;
	
	self->SetDependentMember ( self->mSourceTransform, sourceTransform );
	self->SetDependentMember ( self->mSourceLayer, sourceLayer );
	self->SetDependentMember ( self->mDestLayer, destLayer );
	
	return 0;
}

//================================================================//
// MOAILayerBridge
//================================================================//

//----------------------------------------------------------------//
MOAILayerBridge::MOAILayerBridge () {
	
	RTTI_SINGLE ( MOAITransform )
}

//----------------------------------------------------------------//
MOAILayerBridge::~MOAILayerBridge () {
}

//----------------------------------------------------------------//
void MOAILayerBridge::OnDepNodeUpdate () {
	
	if ( !( this->mSourceTransform && this->mSourceLayer && this->mDestLayer )) return; 
	
	//this->mLocalToWorldMtx = this->mSourceTransform->GetLocalToWorldMtx ();
	//
	//USAffine3D mtx;
	//
	//this->mSourceLayer->GetWorldToWndMtx ( mtx );
	//this->mLocalToWorldMtx.Append ( mtx );
	//
	//this->mDestLayer->GetWndToWorldMtx ( mtx );
	//this->mLocalToWorldMtx.Append ( mtx );
	//
	//this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
}

//----------------------------------------------------------------//
void MOAILayerBridge::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITransform::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAILayerBridge::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

