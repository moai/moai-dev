// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILayer2D.h>
#include <moaicore/MOAILayerBridge2D.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>() init ( self, sourceLayer, targetLayer )</tt>\n
\n
	Initialize the bridge transform (map coordinates in one layer onto
	another; useful for rendering screen space objects tied to world
	space coordinates - map pins, for example).
	@param self (in)
	@param sourceLayer (in)
	@param targetLayer (in)
*/
int MOAILayerBridge2D::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAILayerBridge2D, "UUUU" );
	
	MOAITransformBase* sourceTransform = state.GetLuaObject < MOAITransformBase >( 2 );
	if ( !sourceTransform ) return 0;
	
	MOAILayer2D* sourceLayer = state.GetLuaObject < MOAILayer2D >( 3 );
	if ( !sourceLayer ) return 0;
	
	MOAILayer2D* destLayer = state.GetLuaObject < MOAILayer2D >( 4 );
	if ( !destLayer ) return 0;
	
	self->SetDependentMember ( self->mSourceTransform, sourceTransform );
	self->SetDependentMember ( self->mSourceLayer, sourceLayer );
	self->SetDependentMember ( self->mDestLayer, destLayer );
	
	return 0;
}

//================================================================//
// MOAILayerBridge2D
//================================================================//

//----------------------------------------------------------------//
MOAILayerBridge2D::MOAILayerBridge2D () {
	
	RTTI_SINGLE ( MOAITransform )
}

//----------------------------------------------------------------//
MOAILayerBridge2D::~MOAILayerBridge2D () {
}

//----------------------------------------------------------------//
void MOAILayerBridge2D::OnDepNodeUpdate () {
	
	if ( !( this->mSourceTransform && this->mSourceLayer && this->mDestLayer )) return; 
	
	this->mLocalToWorldMtx = this->mSourceTransform->GetLocalToWorldMtx ();
	
	USAffine3D mtx;
	
	this->mSourceLayer->GetWorldToWndMtx ( mtx );
	this->mLocalToWorldMtx.Append ( mtx );
	
	this->mDestLayer->GetWndToWorldMtx ( mtx );
	this->mLocalToWorldMtx.Append ( mtx );
	
	this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
}

//----------------------------------------------------------------//
void MOAILayerBridge2D::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITransform::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAILayerBridge2D::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

