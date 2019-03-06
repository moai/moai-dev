// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractViewLayer.h>
#include <moai-sim/MOAIPinTransform.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getWorldBounds
	@text	Return the prop's world bounds or 'nil' if prop bounds is
			global or missing.
 
	@in		MOAIPartitionHull self
	@out	number xMin
	@out	number yMin
	@out	number zMin
	@out	number xMax
	@out	number yMax
	@out	number zMax
*/
int MOAIPinTransform::_getWorldBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPinTransform, "U" )
	
	self->ForceUpdate ();
	if ( !self->mWorldBounds.IsOK ()) return 0;
	state.Push ( self->mWorldBounds.mAABB );
	return 6;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPinTransform::_getWorldBoundsCenter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPinTransform, "U" )
	
	self->ForceUpdate ();
	if ( !self->mWorldBounds.IsOK ()) return 0;
	
	ZLVec3D center;
	self->mWorldBounds.mAABB.GetCenter ( center );
	state.Push ( center );

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize the bridge transform (map coordinates in one layer onto
			another; useful for rendering screen space objects tied to world
			space coordinates - map pins, for example).
	
	@in		MOAIPinTransform self
	@in		MOAIAbstractViewLayer sourceLayer
	@in		MOAIAbstractViewLayer destLayer
	@out	nil
*/
int MOAIPinTransform::_init ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIPinTransform, "UUU" );
	
	MOAIAbstractViewLayer* sourceLayer = state.GetLuaObject < MOAIAbstractViewLayer >( 2, true );
	if ( !sourceLayer ) return 0;
	
	MOAIAbstractViewLayer* destLayer = state.GetLuaObject < MOAIAbstractViewLayer >( 3, true );
	if ( !destLayer ) return 0;
	
	self->SetDependentMember ( self->mSourceLayer, sourceLayer );
	self->SetDependentMember ( self->mDestLayer, destLayer );
	
	return 0;
}

//================================================================//
// MOAIPinTransform
//================================================================//

//----------------------------------------------------------------//
MOAIPinTransform::MOAIPinTransform () :
	mFront ( 1.0 ),
	mWorldBounds ( ZLBounds::EMPTY ) {
	
	RTTI_SINGLE ( MOAITransform )
}

//----------------------------------------------------------------//
MOAIPinTransform::~MOAIPinTransform () {

	this->mSourceLayer.Set ( *this, 0 );
	this->mDestLayer.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIPinTransform::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITransform::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_FRONT", 					AttrID::Pack ( ATTR_FRONT ).ToRaw ());
	state.SetField ( -1, "ATTR_INHERIT_WORLD_BOUNDS", 	AttrID::Pack ( ATTR_INHERIT_WORLD_BOUNDS ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAIPinTransform::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransform::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getWorldBounds",				_getWorldBounds },
		{ "getWorldBoundsCenter",		_getWorldBoundsCenter },
		{ "init",						_init },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIPinTransform::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	if ( AttrID::Check ( attrID )) {
		switch ( attrID.Unpack ()) {
			case ATTR_FRONT:
				attr.Apply ( this->mFront, op, ZLAttribute::ATTR_READ );
				return true;
		}
	}
	
	return MOAITransform::MOAINode_ApplyAttrOp ( attrID, attr, op );
}

//----------------------------------------------------------------//
void MOAIPinTransform::MOAINode_Update () {
	
	MOAITransform::MOAINode_Update ();
	
	if ( !( this->mSourceLayer && this->mDestLayer )) return;
	
	ZLVec3D loc = this->mLocalToWorldMtx.GetTranslation ();
	
	const ZLMatrix4x4& worldToWndMtx = this->mSourceLayer->GetWorldToWndMtx ();
	const ZLMatrix4x4& wndToWorldMtx = this->mDestLayer->GetWndToWorldMtx ();
	
	worldToWndMtx.Project ( loc );
	wndToWorldMtx.Transform ( loc );
	
	ZLAttribute attr;
	if ( this->PullLinkedAttr ( AttrID::Pack ( ATTR_INHERIT_WORLD_BOUNDS ), attr )) {
	
		this->mWorldBounds = ZLBounds::EMPTY;
	
		const ZLBounds* bounds = attr.GetVariant < const ZLBounds* >( &ZLBounds::EMPTY );
		if ( bounds->IsOK ()) {
			ZLBoxCorners corners ( bounds->mOBB );
			corners.Project ( worldToWndMtx );
			corners.Transform ( wndToWorldMtx );
			this->mWorldBounds.Init ( ZLBox ( corners ));
		}
	}
	
	// update the transforms
	this->mLocalToWorldMtx.Translate ( loc.mX, loc.mY, loc.mZ );
	this->mWorldToLocalMtx.Translate ( -loc.mX, -loc.mY, -loc.mZ );
	
	// Z component is at the back of the NDC's near plane
	this->mFront = loc.mZ < -1.0f ? 0.0f : 1.0f;
}
