// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	destroy
	@text	Schedule joint for destruction.
	
	@in		MOAIBox2DJoint self
	@out	nil
*/
int MOAIBox2DJoint::_destroy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	if ( self->mWorld ) {
		self->mWorld->ScheduleDestruction ( *self );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getAnchorA
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	number anchorX		in units, in world coordinates, converted to meters
	@out	number anchorY		in units, in world coordinates, converted to meters
*/
int MOAIBox2DJoint::_getAnchorA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2Vec2 anchor = self->mJoint->GetAnchorA ();
	lua_pushnumber ( state, anchor.x / unitsToMeters );
	lua_pushnumber ( state, anchor.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getAnchorB
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	number anchorX		in units, in world coordinates, converted from meters
	@out	number anchorY		in units, in world coordinates, converted from meters
*/
int MOAIBox2DJoint::_getAnchorB ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2Vec2 anchor = self->mJoint->GetAnchorB ();
	lua_pushnumber ( state, anchor.x / unitsToMeters );
	lua_pushnumber ( state, anchor.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getBodyA
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	MOAIBox2DBody body
*/
int MOAIBox2DJoint::_getBodyA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	MOAIBox2DBody* body = ( MOAIBox2DBody* )self->mJoint->GetBodyA ()->GetUserData ();
	body->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getBodyB
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	MOAIBox2DBody body
*/
int MOAIBox2DJoint::_getBodyB ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	MOAIBox2DBody* body = ( MOAIBox2DBody* )self->mJoint->GetBodyB ()->GetUserData ();
	body->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getReactionForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	number forceX	in kg * units / s^2 converted from N [kg * m / s^2]
	@out	number forceY	in kg * units / s^2 converted from N [kg * m / s^2]
*/
int MOAIBox2DJoint::_getReactionForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	float step = ( float )( 1.0 / MOAISim::Get ().GetStep ());
	
	b2Vec2 force = self->mJoint->GetReactionForce ( step );
	lua_pushnumber ( state, force.x / unitsToMeters );
	lua_pushnumber ( state, force.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getReactionTorque
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	number reactionTorque	in (kg * units / s^2) * units, converted from N-m.
*/
int MOAIBox2DJoint::_getReactionTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	float step = ( float )( 1.0 / MOAISim::Get ().GetStep ());
	
	/* Convert from N-m (kg m / s^2) * m to (kg unit / s^2) * unit */
	float unitsToMeters = self->GetUnitsToMeters();
	float torque = self->mJoint->GetReactionTorque ( step );
	lua_pushnumber ( state, torque / ( unitsToMeters * unitsToMeters ) );
	
	return 1;
}

//================================================================//
// MOAIBox2DJoint
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DJoint::Clear () {
	
	if ( this->mJoint ) {
	
		b2Body* b2BodyA = this->mJoint->GetBodyA ();
		b2Body* b2BodyB = this->mJoint->GetBodyB ();
	
		if ( b2BodyA ) {
			MOAIBox2DBody* bodyA = ( MOAIBox2DBody* )b2BodyA->GetUserData ();
			if ( bodyA ) {
				this->LuaRelease ( bodyA );
			}
		}
		
		if ( b2BodyB ) {
			MOAIBox2DBody* bodyB = ( MOAIBox2DBody* )b2BodyB->GetUserData ();
			if ( bodyB ) {
				this->LuaRelease ( bodyB );
			}
		}
		
		this->mJoint = 0;
	}
	this->mWorld = 0;
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::Destroy () {
	
	if ( this->mJoint ) {
		b2World* world = this->mWorld->mWorld;
		world->DestroyJoint ( this->mJoint );
		this->Clear ();
	}
}

//----------------------------------------------------------------//
MOAIBox2DJoint::MOAIBox2DJoint () :
	mJoint ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DJoint::~MOAIBox2DJoint () {
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DPrim::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DPrim::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "destroy",				_destroy },
		{ "getAnchorA",				_getAnchorA },
		{ "getAnchorB",				_getAnchorB },
		{ "getBodyA",				_getBodyA },
		{ "getBodyB",				_getBodyB },
		{ "getReactionForce",		_getReactionForce },
		{ "getReactionTorque",		_getReactionTorque },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::SetJoint ( b2Joint* joint ) {

	this->mJoint = joint;
	joint->SetUserData ( this );
}
