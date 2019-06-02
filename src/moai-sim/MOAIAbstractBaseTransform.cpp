// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAbstractBaseTransform.h>

//================================================================//
// MOAIAbstractBaseTransform
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getWorldDir
	@text	Returns the normalized direction vector of the transform.
			This value is returned in world space so includes parent
			transforms (if any).
	
	@in		MOAIAbstractBaseTransform self
	@out	number xDirection
	@out	number yDirection
	@out	number zDirection
*/
int MOAIAbstractBaseTransform::_getWorldDir ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

	self->ForceUpdate ();

	state.Push ( self->mLocalToWorldMtx.GetHeading ());
	
	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getWorldLoc
	@text	Get the transform's location in world space.
	
	@in		MOAIAbstractBaseTransform self
	@out	number xLoc
	@out	number yLoc
	@out	number zLoc
*/
int MOAIAbstractBaseTransform::_getWorldLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

	self->ForceUpdate ();
	
	state.Push ( self->mLocalToWorldMtx.GetTranslation ());

	return 3;
}

//----------------------------------------------------------------//
/**	@lua	getWorldRot
	@text	Get the transform's rotation in world space.
	
	@in		MOAIAbstractBaseTransform self
	@out	number degrees
*/
int MOAIAbstractBaseTransform::_getWorldRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

	self->ForceUpdate ();

	state.Push ( self->mLocalToWorldMtx.GetRot ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getWorldScl
	@text	Get the transform's scale in world space.
	
	@in		MOAIAbstractBaseTransform self
	@out	number xScale
	@out	number yScale
	@out	number zScale
*/
int MOAIAbstractBaseTransform::_getWorldScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

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
int	MOAIAbstractBaseTransform::_getWorldXAxis ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

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
int	MOAIAbstractBaseTransform::_getWorldYAxis ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

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
int	MOAIAbstractBaseTransform::_getWorldZAxis ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

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
int	MOAIAbstractBaseTransform::_getWorldXNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

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
int	MOAIAbstractBaseTransform::_getWorldYNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

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
int	MOAIAbstractBaseTransform::_getWorldZNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

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
	@opt	number w			Default value is 1.
	@out	number x
	@out	number y
	@out	number z
	@out	number w
*/
int MOAIAbstractBaseTransform::_modelToWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

	self->ForceUpdate ();

	ZLVec4D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = state.GetValue < float >( 4, 0.0f );
	loc.mW = state.GetValue < float >( 5, 1.0f );

	ZLAffine3D modelToWorld = self->GetLocalToWorldMtx ();
	modelToWorld.Transform ( loc );

	state.Push ( loc );

	return 4;
}

//----------------------------------------------------------------//
/**	@lua	worldToModel
	@text	Transform a point in world space to model space.
	
	@in		MOAITransform self
	@opt	number x			Default value is 0.
	@opt	number y			Default value is 0.
	@opt	number z			Default value is 0.
	@opt	number w			Default value is 1.
	@out	number x
	@out	number y
	@out	number z
	@out	number w
*/
int MOAIAbstractBaseTransform::_worldToModel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractBaseTransform, "U" )

	self->ForceUpdate ();

	ZLVec4D loc;
	loc.mX = state.GetValue < float >( 2, 0.0f );
	loc.mY = state.GetValue < float >( 3, 0.0f );
	loc.mZ = state.GetValue < float >( 4, 0.0f );
	loc.mW = state.GetValue < float >( 5, 1.0f );

	ZLAffine3D worldToModel = self->GetWorldToLocalMtx ();
	worldToModel.Transform ( loc );

	state.Push ( loc );

	return 4;
}

//================================================================//
// MOAIAbstractBaseTransform
//================================================================//

//----------------------------------------------------------------//
const ZLAffine3D& MOAIAbstractBaseTransform::GetLocalToWorldMtx () const {

	return this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const ZLAffine3D* MOAIAbstractBaseTransform::GetLocTrait () const {

	return &this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const ZLAffine3D* MOAIAbstractBaseTransform::GetTransformTrait () const {

	return &this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const ZLAffine3D& MOAIAbstractBaseTransform::GetWorldToLocalMtx () const {

	return this->mWorldToLocalMtx;
}

//----------------------------------------------------------------//
MOAIAbstractBaseTransform::MOAIAbstractBaseTransform () {
	
	RTTI_SINGLE ( MOAINode )
	
	this->mLocalToWorldMtx.Ident ();
	this->mWorldToLocalMtx.Ident ();
}

//----------------------------------------------------------------//
MOAIAbstractBaseTransform::~MOAIAbstractBaseTransform () {
}

//----------------------------------------------------------------//
void MOAIAbstractBaseTransform::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAINode, MOAILuaObject_RegisterLuaClass ( composer, state ));
	
	state.SetField ( -1, "ATTR_WORLD_X_LOC",	AttrID::Pack ( ATTR_WORLD_X_LOC ).ToRaw ());
	state.SetField ( -1, "ATTR_WORLD_Y_LOC",	AttrID::Pack ( ATTR_WORLD_Y_LOC ).ToRaw ());
	state.SetField ( -1, "ATTR_WORLD_Z_LOC",	AttrID::Pack ( ATTR_WORLD_Z_LOC ).ToRaw ());
	state.SetField ( -1, "ATTR_WORLD_Z_ROT",	AttrID::Pack ( ATTR_WORLD_Z_ROT ).ToRaw ());
	state.SetField ( -1, "ATTR_WORLD_X_SCL",	AttrID::Pack ( ATTR_WORLD_X_SCL ).ToRaw ());
	state.SetField ( -1, "ATTR_WORLD_Y_SCL",	AttrID::Pack ( ATTR_WORLD_Y_SCL ).ToRaw ());
	state.SetField ( -1, "ATTR_WORLD_Z_SCL",	AttrID::Pack ( ATTR_WORLD_Z_SCL ).ToRaw ());
	state.SetField ( -1, "TRANSFORM_TRAIT",		AttrID::Pack ( TRANSFORM_TRAIT ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAIAbstractBaseTransform::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAINode, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	
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
		{ "worldToModel",		_worldToModel },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIAbstractBaseTransform::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	// TODO: these values may need to be cached for performance reasons
	if ( AttrID::Check ( attrID )) {

		switch ( attrID.Unpack ()) {
			
			case ATTR_WORLD_X_LOC:
				attr.Apply ( this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R0 ], op, ZLAttribute::ATTR_READ );
				return true;
			
			case ATTR_WORLD_Y_LOC:
				attr.Apply ( this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R1 ], op, ZLAttribute::ATTR_READ );
				return true;
			
			case ATTR_WORLD_Z_LOC:
				attr.Apply ( this->mLocalToWorldMtx.m [ ZLAffine3D::C3_R2 ], op, ZLAttribute::ATTR_READ );
				return true;
			
			case ATTR_WORLD_Z_ROT: {
				float rot = ( float )( atan2 ( this->mLocalToWorldMtx.m [ ZLAffine3D::C0_R0 ], this->mLocalToWorldMtx.m [ ZLAffine3D::C0_R1 ]) * R2D );
				attr.Apply ( rot, op, ZLAttribute::ATTR_READ );
				return true;
			}
			case ATTR_WORLD_X_SCL: {
				
				ZLVec3D axis;
				
				axis.mX =	this->mLocalToWorldMtx.m [ ZLAffine3D::C0_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ ZLAffine3D::C0_R1 ];
				axis.mZ =	this->mLocalToWorldMtx.m [ ZLAffine3D::C0_R2 ];
			
				attr.Apply ( axis.Length (), op, ZLAttribute::ATTR_READ );
				return true;
			}
			case ATTR_WORLD_Y_SCL: {
				
				ZLVec3D axis;
			
				axis.mX =	this->mLocalToWorldMtx.m [ ZLAffine3D::C1_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ ZLAffine3D::C1_R1 ];
				axis.mZ =	this->mLocalToWorldMtx.m [ ZLAffine3D::C1_R2 ];
				
				attr.Apply ( axis.Length (), op, ZLAttribute::ATTR_READ );
				return true;
			}
			case ATTR_WORLD_Z_SCL: {
				
				ZLVec3D axis;
			
				axis.mX =	this->mLocalToWorldMtx.m [ ZLAffine3D::C2_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ ZLAffine3D::C2_R1 ];
				axis.mZ =	this->mLocalToWorldMtx.m [ ZLAffine3D::C2_R2 ];
				
				attr.Apply ( axis.Length (), op, ZLAttribute::ATTR_READ );
				return true;
			}
			case TRANSFORM_TRAIT:
			
				attr.ApplyNoAdd ( this->mLocalToWorldMtx, op, ZLAttribute::ATTR_READ );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIAbstractBaseTransform::MOAINode_Update () {
	
	this->MOAIAbstractBaseTransform_BuildLocalToWorldMtx ( this->mLocalToWorldMtx );
	this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
}
