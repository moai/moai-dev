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
	@text	Returns the normalized direction vector ot the transform.
			This value is returned in world space so includes parent
			transforms (if any).
	
	@in		MOAITransform self
	@out	number xDirection
	@out	number yDirection
*/
int MOAITransformBase::_getWorldDir ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	USVec2D direction = self->mLocalToWorldMtx.GetHeading ();

	lua_pushnumber ( state, direction.mX );
	lua_pushnumber ( state, direction.mY );

	return 2;
}

//----------------------------------------------------------------//
/**	@name	getWorldLoc
	@text	Get the transform's location in world space.
	
	@in		MOAITransformBase self
	@out	number xLoc
	@out	number yLoc
*/
int MOAITransformBase::_getWorldLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	USVec2D loc = self->mLocalToWorldMtx.GetTranslation ();

	lua_pushnumber ( state, loc.mX );
	lua_pushnumber ( state, loc.mY );

	return 2;
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
*/
int MOAITransformBase::_getWorldScl ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITransformBase, "U" )

	USVec2D scale = self->mLocalToWorldMtx.GetStretch ();
	
	lua_pushnumber ( state, scale.mX );
	lua_pushnumber ( state, scale.mY );

	return 2;
}

//================================================================//
// MOAITransformBase
//================================================================//

//----------------------------------------------------------------//
bool MOAITransformBase::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	// TODO: these values may need to be cached for performance reasons
	if ( MOAITransformBaseAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			
			case ATTR_WORLD_X_LOC:
				attrOp.Op ( this->mLocalToWorldMtx.m [ USAffine2D::C2_R0 ]);
				return true;
			
			case ATTR_WORLD_Y_LOC:
				attrOp.Op ( this->mLocalToWorldMtx.m [ USAffine2D::C2_R1 ]);
				return true;
				
			case ATTR_WORLD_Z_ROT:
				attrOp.Op (( float )( atan2 ( this->mLocalToWorldMtx.m [ USAffine2D::C0_R0 ], this->mLocalToWorldMtx.m [ USAffine2D::C0_R1 ]) * R2D ));
				return true;
			
			case ATTR_WORLD_X_SCL: {
				
				USVec2D axis;
			
				axis.mX =	this->mLocalToWorldMtx.m [ USAffine2D::C0_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ USAffine2D::C0_R1 ];
			
				attrOp.Op ( axis.Length ());
				return true;
			}
			
			case ATTR_WORLD_Y_SCL: {
				
				USVec2D axis;
			
				axis.mX =	this->mLocalToWorldMtx.m [ USAffine2D::C1_R0 ];
				axis.mY =	this->mLocalToWorldMtx.m [ USAffine2D::C1_R1 ];
				
				attrOp.Op ( axis.Length ());
				return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------//
const USAffine2D& MOAITransformBase::GetLocalToWorldMtx () {

	return this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine2D* MOAITransformBase::GetTransformTrait () {

	return &this->mLocalToWorldMtx;
}

//----------------------------------------------------------------//
const USAffine2D& MOAITransformBase::GetWorldToLocalMtx () {

	return this->mWorldToLocalMtx;
}

//----------------------------------------------------------------//
MOAITransformBase::MOAITransformBase () {
	
	RTTI_SINGLE ( MOAITraits )
	
	this->mLocalToWorldMtx.Ident ();
	this->mWorldToLocalMtx.Ident ();
}

//----------------------------------------------------------------//
MOAITransformBase::~MOAITransformBase () {
}

//----------------------------------------------------------------//
void MOAITransformBase::RegisterLuaClass ( USLuaState& state ) {
	
	MOAITraits::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_WORLD_X_LOC",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_X_LOC ));
	state.SetField ( -1, "ATTR_WORLD_Y_LOC",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_Y_LOC ));
	state.SetField ( -1, "ATTR_WORLD_Z_ROT",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_Z_ROT ));
	state.SetField ( -1, "ATTR_WORLD_X_SCL",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_X_SCL ));
	state.SetField ( -1, "ATTR_WORLD_Y_SCL",	MOAITransformBaseAttr::Pack ( ATTR_WORLD_Y_SCL ));
}

//----------------------------------------------------------------//
void MOAITransformBase::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAITraits::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getWorldDir",		_getWorldDir },
		{ "getWorldLoc",		_getWorldLoc },
		{ "getWorldRot",		_getWorldRot },
		{ "getWorldScl",		_getWorldScl },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAITransformBase::ToString () {

	STLString repr;
	return repr;
}
