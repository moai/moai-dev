// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAITransformNode.h>

//================================================================//
// MOAITransformNode
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAITransformNode self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAITransformNode::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformNode, "U" )

	MOAITransformNodeBase* parent = state.GetLuaObject < MOAITransformNodeBase >( 2, true );
	
	self->SetAttrLink ( AttrID::Pack ( INHERIT_TRANSFORM ), parent, MOAITransformNodeBase::AttrID::Pack ( MOAITransformNodeBase::TRANSFORM_TRAIT ));
	
	return 0;
}

//================================================================//
// MOAITransformNode
//================================================================//

//----------------------------------------------------------------//
MOAITransformNode::MOAITransformNode () {
	
	RTTI_SINGLE ( MOAITransformNodeBase )
}

//----------------------------------------------------------------//
MOAITransformNode::~MOAITransformNode () {
}

//----------------------------------------------------------------//
void MOAITransformNode::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAITransformNodeBase::RegisterLuaClass ( state );
	
	state.SetField ( -1, "INHERIT_LOC",			AttrID::Pack ( INHERIT_LOC ).ToRaw ());
	state.SetField ( -1, "INHERIT_TRANSFORM",	AttrID::Pack ( INHERIT_TRANSFORM ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAITransformNode::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransformNodeBase::RegisterLuaFuncs ( state );
	
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
bool MOAITransformNode::MOAINode_ApplyAttrOp ( MOAIAttrID attrID, MOAIAttribute& attr, u32 op ) {

	return MOAITransformNodeBase::MOAINode_ApplyAttrOp ( attrID, attr, op );
}

//----------------------------------------------------------------//
void MOAITransformNode::MOAINode_Update () {
	
	this->MOAITransformNodeBase_BuildLocalToWorldMtx ( this->mLocalToWorldMtx );
	
	MOAIAttribute attr;
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
