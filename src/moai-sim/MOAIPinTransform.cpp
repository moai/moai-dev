// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAILayer.h>
#include <moai-sim/MOAIPinTransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	init
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
bool MOAIPinTransform::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIPinTransformAttr::Check ( attrID )) {
		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_FRONT:
				attrOp.Apply ( this->mFront, op, MOAIAttrOp::ATTR_READ, MOAIAttrOp::ATTR_TYPE_FLOAT );
				return true;
		}
	}
	
	return MOAITransform::ApplyAttrOp ( attrID, attrOp, op );
}

//----------------------------------------------------------------//
MOAIPinTransform::MOAIPinTransform () :
	mFront ( 1.0f )
{
	
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
	
	this->mLocalToWorldMtx.Translate ( loc.mX, loc.mY, 0.0f );
	this->mWorldToLocalMtx.Translate ( -loc.mX, -loc.mY, 0.0f );
	
	// Z component is at the back of the NDC's near plane
	this->mFront = loc.mZ < -1.0f ? 0.0f : 1.0f;
}

//----------------------------------------------------------------//
void MOAIPinTransform::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITransform::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_FRONT", MOAIPinTransformAttr::Pack ( ATTR_FRONT ) );
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
