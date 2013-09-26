// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAILayer.h>
#include <moai-sim/MOAIPinTransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize the bridge transform (map coordinates in one layer onto
			another; useful for rendering screen space objects tied to world
			space coordinates - map pins, for example).
	
	@in		MOAIPinTransform self
	@in		MOAITransformBase sourceTransform
	@in		MOAILayer sourceLayer
	@in		MOAILayer destLayer
	@out	nil
*/
int MOAIPinTransform::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPinTransform, "UUUU" );
	
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
// MOAIPinTransform
//================================================================//

//----------------------------------------------------------------//
MOAIPinTransform::MOAIPinTransform () {
	
	RTTI_SINGLE ( MOAITransform )
}

//----------------------------------------------------------------//
MOAIPinTransform::~MOAIPinTransform () {

	this->mSourceTransform.Set ( *this, 0 );
	this->mSourceLayer.Set ( *this, 0 );
	this->mDestLayer.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIPinTransform::OnDepNodeUpdate () {
	
	if ( !( this->mSourceTransform && this->mSourceLayer && this->mDestLayer )) return; 
	
	this->mLocalToWorldMtx = this->mSourceTransform->GetLocalToWorldMtx ();
	
	ZLMatrix4x4 mtx;
	
	ZLMatrix4x4 localToWorld;
	localToWorld.Init ( this->mLocalToWorldMtx );
	
	this->mSourceLayer->GetWorldToWndMtx ( mtx );
	localToWorld.Append ( mtx );
	
	this->mDestLayer->GetWndToWorldMtx ( mtx );
	localToWorld.Append ( mtx );
	
	this->mLocalToWorldMtx.Init ( localToWorld );
	this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
}

//----------------------------------------------------------------//
void MOAIPinTransform::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITransform::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIPinTransform::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

