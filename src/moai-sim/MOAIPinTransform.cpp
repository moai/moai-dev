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
	MOAI_LUA_SETUP ( MOAIPinTransform, "UUU" );
	
	MOAILayer* sourceLayer = state.GetLuaObject < MOAILayer >( 2, true );
	if ( !sourceLayer ) return 0;
	
	MOAILayer* destLayer = state.GetLuaObject < MOAILayer >( 3, true );
	if ( !destLayer ) return 0;
	
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

	this->mSourceLayer.Set ( *this, 0 );
	this->mDestLayer.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIPinTransform::OnDepNodeUpdate () {
	
	MOAITransform::OnDepNodeUpdate ();
	
	if ( !( this->mSourceLayer && this->mDestLayer )) return;
	
	ZLVec3D loc = this->mLocalToWorldMtx.GetTranslation ();
	
	ZLMatrix4x4 mtx;
	
	this->mSourceLayer->GetWorldToWndMtx ().Project ( loc );
	this->mDestLayer->GetWndToWorldMtx ().Transform ( loc );
	
//	this->mSourceLayer->GetWorldToWndMtx ( mtx );
//	mtx.Project ( loc );
//	
//	this->mDestLayer->GetWndToWorldMtx ( mtx );
//	mtx.Transform ( loc );
	
	this->mLocalToWorldMtx.Translate ( loc.mX, loc.mY, 0.0f );
	this->mWorldToLocalMtx.Translate ( -loc.mX, -loc.mY, 0.0f );
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

