// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAITransformBase.h>

//================================================================//
// MOAITransformBase
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getWorldDir
	@text	Returns the normalized direction vector of the transform.
			This value is returned in world space so includes parent
			transforms (if any).
	
	@in		MOAITransformBase self
	@out	number xDirection
	@out	number yDirection
	@out	number zDirection
*/
int MOAITransformBase::_getWorldDir ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	state.Push ( self->mLocalToWorldMtx.GetHeading ());
	
	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getWorldLoc
	@text	Get the transform's location in world space.
	
	@in		MOAITransformBase self
	@out	number xLoc
	@out	number yLoc
	@out	number zLoc
*/
int MOAITransformBase::_getWorldLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();
	
	state.Push ( self->mLocalToWorldMtx.GetTranslation ());

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getWorldRot
	@text	Get the transform's rotation in world space.
	
	@in		MOAITransformBase self
	@out	number degrees
*/
int MOAITransformBase::_getWorldRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	state.Push ( self->mLocalToWorldMtx.GetRot ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getWorldScl
	@text	Get the transform's scale in world space.
	
	@in		MOAITransformBase self
	@out	number xScale
	@out	number yScale
	@out	number zScale
*/
int MOAITransformBase::_getWorldScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	state.Push ( self->mLocalToWorldMtx.GetStretch ());

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getWorldXAxis
	@text	Return the X axis of the coordinate system (includes scale);
	
	@in		MOAITransform self
	@out	number x
	@out	number y
	@out	number z
*/
int	MOAITransformBase::_getWorldXAxis ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	state.Push ( self->mLocalToWorldMtx.GetXAxis ());

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getWorldXAxis
	@text	Return the Y axis of the coordinate system (includes scale);
	
	@in		MOAITransform self
	@out	number x
	@out	number y
	@out	number z
*/
int	MOAITransformBase::_getWorldYAxis ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	state.Push ( self->mLocalToWorldMtx.GetYAxis ());

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getWorldXAxis
	@text	Return the Z axis of the coordinate system (includes scale);
	
	@in		MOAITransform self
	@out	number x
	@out	number y
	@out	number z
*/
int	MOAITransformBase::_getWorldZAxis ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	state.Push ( self->mLocalToWorldMtx.GetZAxis ());

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getWorldXNormal
	@text	Return the X axis of the coordinate system (normalized);
	
	@in		MOAITransform self
	@out	number x
	@out	number y
	@out	number z
	@opt	number length
*/
int	MOAITransformBase::_getWorldXNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	ZLVec3D axis = self->mLocalToWorldMtx.GetXAxis ();
	float length = axis.NormSafe ();

	state.Push ( axis );
	lua_pushnumber ( state, length );

	return 4;
}

//----------------------------------------------------------------//
/**	@lua	getWorldYNormal
	@text	Return the Y axis of the coordinate system (normalized);
	
	@in		MOAITransform self
	@out	number x
	@out	number y
	@out	number z
	@opt	number length
*/
int	MOAITransformBase::_getWorldYNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	ZLVec3D axis = self->mLocalToWorldMtx.GetYAxis ();
	float length = axis.NormSafe ();

	state.Push ( axis );
	lua_pushnumber ( state, length );

	return 4;
}

//----------------------------------------------------------------//
/**	@lua	getWorldZNormal
	@text	Return the Z axis of the coordinate system (normalized);
	
	@in		MOAITransform self
	@out	number x
	@out	number y
	@out	number z
	@opt	number length
*/
int	MOAITransformBase::_getWorldZNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	ZLVec3D axis = self->mLocalToWorldMtx.GetZAxis ();
	float length = axis.NormSafe ();

	state.Push ( axis );
	lua_pushnumber ( state, length );

	return 4;
}

//----------------------------------------------------------------//
/**	@lua	modelToWorld
	@text	Transform a point in model space to world space.
	
	@in		MOAITransform self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@opt	number z			Default value is 0.
	@out	number x
	@out	number y
	@out	number z
*/
int MOAITransformBase::_modelToWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	ZLVec3D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = state.GetValue < float >( 4, 0.0f );

	ZLAffine3D modelToWorld = self->GetLocalToWorldMtx ();
	modelToWorld.Transform ( loc );

	state.Push ( loc );

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	setParent
	@text	This method has been deprecated. Use MOAINode setAttrLink instead.
	
	@in		MOAITransformBase self
	@opt	MOAINode parent		Default value is nil.
	@out	nil
*/
int MOAITransformBase::_setParent ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	MOAINode* parent = state.GetLuaObject < MOAINode >( 2, true );
	
	self->SetAttrLink ( PACK_ATTR ( MOAITransformBase, INHERIT_TRANSFORM ), parent, PACK_ATTR ( MOAITransformBase, TRANSFORM_TRAIT ));
	
	//MOAILogF ( state, MOAILogMessages::MOAI_FunctionDeprecated_S, "setParent" );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	worldToModel
	@text	Transform a point in world space to model space.
	
	@in		MOAITransform self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@opt	number z			Default value is 0.
	@out	number x
	@out	number y
	@out	number z
*/
int MOAITransformBase::_worldToModel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	self->ForceUpdate ();

	ZLVec3D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = state.GetValue < float >( 4, 0.0f );

	ZLAffine3D worldToModel = self->GetWorldToLocalMtx ();
	worldToModel.Transform ( loc );

	state.Push ( loc );

	return 3;
}

//================================================================//
// MOAITransformBase
//================================================================//

//----------------------------------------------------------------//
bool MOAITransformBase::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	// TODO: these values may need to be cached for performance reasons
	if ( MOAITransformBaseAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			
			case ATTR_WORLD_X_LOC:
				attrOp.Apply ( this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R0 ], op, MOAIAttrOp::ATTR_READ, MOAIAttrOp::ATTR_TYPE_FLOAT );
				return true;
			
			case ATTR_WORLD_Y_LOC:
				attrOp.Apply ( this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R1 ], op, MOAIAttrOp::ATTR_READ, MOAIAttrOp::ATTR_TYPE_FLOAT );
				return true;
			
			case ATTR_WORLD_Z_LOC:
				attrOp.Apply ( this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R2 ], op, MOAIAttrOp::ATTR_READ, MOAIAttrOp::ATTR_TYPE_FLOAT );
				return true;
			
			case ATTR_WORLD_Z_ROT: {
				float rot = ( float )( atan2 ( this->mLocalToWorldMtx.m [ ZLAffine3D::C0_R0 ], this->mLocalToWorldMtx.m [ ZLAffine3D::C0_R1 ]) * R2D );
				attrOp.Apply ( rot, op, MOAIAttrOp::ATTR_READ, MOAIAttrOp::ATTR_TYPE_FLOAT );
				return true;
			}
			case ATTR_WORLD_X_SCL: {
				
				ZLVec3D axis;
			
				axis.mX =	this->mLocalToWorldMtx.m [ ZLAffine3D::C0_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ ZLAffine3D::C0_R1 ];
				axis.mZ =	this->mLocalToWorldMtx.m [ ZLAffine3D::C0_R2 ];
			
				attrOp.Apply ( axis.Length (), op, MOAIAttrOp::ATTR_READ, MOAIAttrOp::ATTR_TYPE_FLOAT );
				return true;
			}
			case ATTR_WORLD_Y_SCL: {
				
				ZLVec3D axis;
			
				axis.mX =	this->mLocalToWorldMtx.m [ ZLAffine3D::C1_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ ZLAffine3D::C1_R1 ];
				axis.mZ =	this->mLocalToWorldMtx.m [ ZLAffine3D::C1_R2 ];
				
				attrOp.Apply ( axis.Length (), op, MOAIAttrOp::ATTR_READ, MOAIAttrOp::ATTR_TYPE_FLOAT );
				return true;
			}
			case ATTR_WORLD_Z_SCL: {
				
				ZLVec3D axis;
			
				axis.mX =	this->mLocalToWorldMtx.m [ ZLAffine3D::C2_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ ZLAffine3D::C2_R1 ];
				axis.mZ =	this->mLocalToWorldMtx.m [ ZLAffine3D::C2_R2 ];
				
				attrOp.Apply ( axis.Length (), op, MOAIAttrOp::ATTR_READ, MOAIAttrOp::ATTR_TYPE_FLOAT );
				return true;
			}
			case TRANSFORM_TRAIT:
				attrOp.ApplyNoAdd < ZLAffine3D* >( &this->mLocalToWorldMtx, op, MOAIAttrOp::ATTR_READ, MOAIAttrOp::ATTR_TYPE_TRANSFORM );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
const ZLAffine3D& MOAITransformBase::GetLocalToWorldMtx () const {

	return this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const ZLAffine3D* MOAITransformBase::GetLocTrait () const {

	return &this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const ZLAffine3D* MOAITransformBase::GetTransformTrait () const {

	return &this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const ZLAffine3D& MOAITransformBase::GetWorldToLocalMtx () const {

	return this->mWorldToLocalMtx;
}

//----------------------------------------------------------------//
MOAITransformBase::MOAITransformBase () {
	
	RTTI_SINGLE ( MOAINode )
	
	this->mLocalToWorldMtx.Ident ();
	this->mWorldToLocalMtx.Ident ();
}

//----------------------------------------------------------------//
MOAITransformBase::~MOAITransformBase () {
}

//----------------------------------------------------------------//
void MOAITransformBase::OnDepNodeUpdate () {
	
	this->BuildLocalToWorldMtx ( this->mLocalToWorldMtx );
	
	const ZLAffine3D* inherit = this->GetLinkedValue < ZLAffine3D* >( MOAITransformBaseAttr::Pack ( INHERIT_TRANSFORM ), 0 );
	if ( inherit ) {
		this->mLocalToWorldMtx.Append ( *inherit );
	}
	else {
	
		inherit = this->GetLinkedValue < ZLAffine3D* >( MOAITransformBaseAttr::Pack ( INHERIT_LOC ), 0 );
		if ( inherit ) {
			
			ZLVec3D loc = this->mLocalToWorldMtx.GetTranslation ();
			
			inherit->Transform ( loc );
			
			this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R0 ] = loc.mX;
			this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R1 ] = loc.mY;
			this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R2 ] = loc.mZ;
		}
	}
	
	this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
}

//----------------------------------------------------------------//
void MOAITransformBase::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_WORLD_X_LOC",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_X_LOC ));
	state.SetField ( -1, "ATTR_WORLD_Y_LOC",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_Y_LOC ));
	state.SetField ( -1, "ATTR_WORLD_Z_LOC",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_Z_LOC ));
	state.SetField ( -1, "ATTR_WORLD_Z_ROT",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_Z_ROT ));
	state.SetField ( -1, "ATTR_WORLD_X_SCL",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_X_SCL ));
	state.SetField ( -1, "ATTR_WORLD_Y_SCL",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_Y_SCL ));
	state.SetField ( -1, "ATTR_WORLD_Z_SCL",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_Z_SCL ));
	state.SetField ( -1, "TRANSFORM_TRAIT",		MOAITransformBaseAttr::Pack ( TRANSFORM_TRAIT ));
	
	state.SetField ( -1, "INHERIT_LOC",			MOAITransformBaseAttr::Pack ( INHERIT_LOC ));
	state.SetField ( -1, "INHERIT_TRANSFORM",	MOAITransformBaseAttr::Pack ( INHERIT_TRANSFORM ));
}

//----------------------------------------------------------------//
void MOAITransformBase::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getWorldDir",		_getWorldDir },
		{ "getWorldLoc",		_getWorldLoc },
		{ "getWorldRot",		_getWorldRot },
		{ "getWorldScl",		_getWorldScl },
		
		{ "getWorldXAxis",		_getWorldXAxis },
		{ "getWorldYAxis",		_getWorldYAxis },
		{ "getWorldZAxis",		_getWorldZAxis },
		
		{ "getWorldXNormal",	_getWorldXNormal },
		{ "getWorldYNormal",	_getWorldYNormal },
		{ "getWorldZNormal",	_getWorldZNormal },
		
		{ "modelToWorld",		_modelToWorld },
		{ "setParent",			_setParent },
		{ "worldToModel",		_worldToModel },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
