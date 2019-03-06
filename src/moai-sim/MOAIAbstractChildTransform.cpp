// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractChildTransform.h>

//================================================================//
// MOAIAbstractChildTransform
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAIAbstractChildTransform self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAIAbstractChildTransform::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractChildTransform, "U" )

	MOAIAbstractBaseTransform* parent = state.GetLuaObject < MOAIAbstractBaseTransform >( 2, true );
	
	self->SetAttrLink ( AttrID::Pack ( INHERIT_TRANSFORM ), parent, MOAIAbstractBaseTransform::AttrID::Pack ( MOAIAbstractBaseTransform::TRANSFORM_TRAIT ));
	
	return 0;
}

//================================================================//
// MOAIAbstractChildTransform
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractChildTransform::MOAIAbstractChildTransform () {
	
	RTTI_SINGLE ( MOAIAbstractBaseTransform )
}

//----------------------------------------------------------------//
MOAIAbstractChildTransform::~MOAIAbstractChildTransform () {
}

//----------------------------------------------------------------//
void MOAIAbstractChildTransform::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIAbstractBaseTransform::RegisterLuaClass ( state );
	
	state.SetField ( -1, "INHERIT_LOC",			AttrID::Pack ( INHERIT_LOC ).ToRaw ());
	state.SetField ( -1, "INHERIT_TRANSFORM",	AttrID::Pack ( INHERIT_TRANSFORM ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAIAbstractChildTransform::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAbstractBaseTransform::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setParent",			_setParent },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIAbstractChildTransform::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	return MOAIAbstractBaseTransform::MOAINode_ApplyAttrOp ( attrID, attr, op );
}

//----------------------------------------------------------------//
void MOAIAbstractChildTransform::MOAINode_Update () {
	
	this->MOAIAbstractBaseTransform_BuildLocalToWorldMtx ( this->mLocalToWorldMtx );
	
	ZLAttribute attr;
	if ( this->PullLinkedAttr ( AttrID::Pack ( INHERIT_TRANSFORM ), attr )) {
		const ZLAffine3D inherit = attr.GetValue ( ZLAffine3D::IDENT );
		this->mLocalToWorldMtx.Append ( inherit );
	}
	else {
	
		if ( this->PullLinkedAttr ( AttrID::Pack ( INHERIT_LOC ), attr )) {
			
			const ZLAffine3D inherit = attr.GetValue ( ZLAffine3D::IDENT );
			
			ZLVec3D loc = this->mLocalToWorldMtx.GetTranslation ();
			
			inherit.Transform ( loc );
			
			this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R0 ] = loc.mX;
			this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R1 ] = loc.mY;
			this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R2 ] = loc.mZ;
		}
	}
	
	this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
}
