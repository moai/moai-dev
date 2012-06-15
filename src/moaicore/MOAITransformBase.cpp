// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITransformBase.h>

//================================================================//
// MOAITransformBase
//================================================================//

//----------------------------------------------------------------//
/**	@name	getWorldDir
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

	USVec3D direction = self->mLocalToWorldMtx.GetHeading ();

	lua_pushnumber ( state, direction.mX );
	lua_pushnumber ( state, direction.mY );
	lua_pushnumber ( state, direction.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@name	getWorldLoc
	@text	Get the transform's location in world space.
	
	@in		MOAITransformBase self
	@out	number xLoc
	@out	number yLoc
	@out	number zLoc
*/
int MOAITransformBase::_getWorldLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	USVec3D loc = self->mLocalToWorldMtx.GetTranslation ();

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );
	lua_pushnumber ( state, loc.mZ );

	return 3;
}

//----------------------------------------------------------------//
/**	@name	getWorldRot
	@text	Get the transform's rotation in world space.
	
	@in		MOAITransformBase self
	@out	number degrees
*/
int MOAITransformBase::_getWorldRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	float rot = self->mLocalToWorldMtx.GetRot ();
	
	lua_pushnumber ( state, rot );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getWorldScl
	@text	Get the transform's scale in world space.
	
	@in		MOAITransformBase self
	@out	number xScale
	@out	number yScale
	@out	number zScale
*/
int MOAITransformBase::_getWorldScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	USVec3D scale = self->mLocalToWorldMtx.GetStretch ();
	
	lua_pushnumber ( state, scale.mX );
	lua_pushnumber ( state, scale.mY );
	lua_pushnumber ( state, scale.mZ );

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
				attrOp.Apply ( this->mLocalToWorldMtx.m [ USAffine3D::C2_R0 ], op, MOAIAttrOp::ATTR_READ );
				return true;
			
			case ATTR_WORLD_Y_LOC:
				attrOp.Apply ( this->mLocalToWorldMtx.m [ USAffine3D::C2_R1 ], op, MOAIAttrOp::ATTR_READ );
				return true;
			
			case ATTR_WORLD_Z_LOC:
				attrOp.Apply ( this->mLocalToWorldMtx.m [ USAffine3D::C2_R2 ], op, MOAIAttrOp::ATTR_READ );
				return true;
			
			case ATTR_WORLD_Z_ROT: {
				float rot = ( float )( atan2 ( this->mLocalToWorldMtx.m [ USAffine3D::C0_R0 ], this->mLocalToWorldMtx.m [ USAffine3D::C0_R1 ]) * R2D );
				attrOp.Apply ( rot, op, MOAIAttrOp::ATTR_READ );
				return true;
			}
			case ATTR_WORLD_X_SCL: {
				
				USVec3D axis;
			
				axis.mX =	this->mLocalToWorldMtx.m [ USAffine3D::C0_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ USAffine3D::C0_R1 ];
				axis.mZ =	this->mLocalToWorldMtx.m [ USAffine3D::C0_R2 ];
			
				attrOp.Apply ( axis.Length (), op, MOAIAttrOp::ATTR_READ );
				return true;
			}
			case ATTR_WORLD_Y_SCL: {
				
				USVec3D axis;
			
				axis.mX =	this->mLocalToWorldMtx.m [ USAffine3D::C1_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ USAffine3D::C1_R1 ];
				axis.mZ =	this->mLocalToWorldMtx.m [ USAffine3D::C1_R2 ];
				
				attrOp.Apply ( axis.Length (), op, MOAIAttrOp::ATTR_READ );
				return true;
			}
			case ATTR_WORLD_Z_SCL: {
				
				USVec3D axis;
			
				axis.mX =	this->mLocalToWorldMtx.m [ USAffine3D::C2_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ USAffine3D::C2_R1 ];
				axis.mZ =	this->mLocalToWorldMtx.m [ USAffine3D::C2_R2 ];
				
				attrOp.Apply ( axis.Length (), op, MOAIAttrOp::ATTR_READ );
				return true;
			}
			case TRANSFORM_TRAIT:
				attrOp.ApplyNoAdd < USAffine3D* >( &this->mLocalToWorldMtx, op, MOAIAttrOp::ATTR_READ );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
const USAffine3D& MOAITransformBase::GetLocalToWorldMtx () {

	return this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine3D* MOAITransformBase::GetLocTrait () {

	return &this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine3D* MOAITransformBase::GetTransformTrait () {

	return &this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine3D& MOAITransformBase::GetWorldToLocalMtx () {

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
}

//----------------------------------------------------------------//
void MOAITransformBase::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getWorldDir",		_getWorldDir },
		{ "getWorldLoc",		_getWorldLoc },
		{ "getWorldRot",		_getWorldRot },
		{ "getWorldScl",		_getWorldScl },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
