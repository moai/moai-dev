// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DJoint.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	destroy
	@text	Schedule joint for destruction.
	
	@in		MOAIBox2DBody self
	@out	nil
*/
int MOAIBox2DJoint::_destroy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	assert ( self->mWorld );
	self->mWorld->ScheduleDestruction ( *self );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAnchorA
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	anchorX
	@out	anchorY
*/
int MOAIBox2DJoint::_getAnchorA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2Vec2 anchor = self->mJoint->GetAnchorA ();
	lua_pushnumber ( state, anchor.x / unitsToMeters );
	lua_pushnumber ( state, anchor.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getAnchorB
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	anchorX
	@out	anchorY
*/
int MOAIBox2DJoint::_getAnchorB ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2Vec2 anchor = self->mJoint->GetAnchorB ();
	lua_pushnumber ( state, anchor.x / unitsToMeters );
	lua_pushnumber ( state, anchor.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getBodyA
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	MOAIBox2DBody body
*/
int MOAIBox2DJoint::_getBodyA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	MOAIBox2DBody* body = ( MOAIBox2DBody* )self->mJoint->GetBodyA ()->GetUserData ();;
	body->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getBodyB
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	MOAIBox2DBody body
*/
int MOAIBox2DJoint::_getBodyB ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	MOAIBox2DBody* body = ( MOAIBox2DBody* )self->mJoint->GetBodyB ()->GetUserData ();;
	body->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getReactionForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	number forceX
	@out	number forceY
*/
int MOAIBox2DJoint::_getReactionForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	// TODO
	//b2Vec2 force = self->mJoint->GetReactionForce ();
	//lua_pushnumber ( state, force.x );
	//lua_pushnumber ( state, force.y );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getReactionForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@out	number reactionTorque In degrees.
*/
int MOAIBox2DJoint::_getReactionTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	// TODO
	//float torque = self->mJoint->GetReactionTorque ();
	//lua_pushnumber ( state, torque );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setLimit
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@opt	number lower			Default value is 0.
	@opt	number upper			Default value is 0.
	@out	nil
*/
int MOAIBox2DJoint::_setLimit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	float lower	= state.GetValue < float >( 2, 0.0f );
	float upper	= state.GetValue < float >( 3, 0.0f );
	
	self->SetLimit ( lower, upper );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLimitEnabled
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@opt	boolean enabled			Default value is 'true'
	@out	nil
*/
int MOAIBox2DJoint::_setLimitEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	bool enabled = state.GetValue < bool >( 2, true );
	self->SetLimitEnabled ( enabled );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMotor
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@opt	number speed			Default value is 0.
	@opt	number max				Default value is 0.
	@out	nil
*/
int MOAIBox2DJoint::_setMotor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	float speed	= state.GetValue < float >( 2, 0.0f );
	float max	= state.GetValue < float >( 3, 0.0f );
	
	self->SetMotor ( speed, max );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMotorEnabled
	@text	See Box2D documentation.
	
	@in		MOAIBox2DJoint self
	@opt	boolean enabled			Default value is 'true'
	@out	nil
*/
int MOAIBox2DJoint::_setMotorEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DJoint, "U" )
	
	bool enabled = state.GetValue < bool >( 2, true );
	self->SetMotorEnabled ( enabled );
	
	return 0;
}

//================================================================//
// MOAIBox2DJoint
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DJoint::Destroy () {

	if ( this->mJoint ) {
		b2World* world = this->mWorld->mWorld;
		world->DestroyJoint ( this->mJoint );
		this->mJoint = 0;
		this->Release ();
	}
}

//----------------------------------------------------------------//
MOAIBox2DJoint::MOAIBox2DJoint () :
	mJoint ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DJoint::~MOAIBox2DJoint () {

	MOAIBox2DBody* bodyA = ( MOAIBox2DBody* )this->mJoint->GetBodyA ();
	MOAIBox2DBody* bodyB = ( MOAIBox2DBody* )this->mJoint->GetBodyB ();
	
	bodyA->Release ();
	bodyB->Release ();
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::RegisterLuaFuncs ( USLuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "destroy",				_destroy },
		{ "getAnchorA",				_getAnchorA },
		{ "getAnchorB",				_getAnchorB },
		{ "getBodyA",				_getBodyA },
		{ "getBodyB",				_getBodyB },
		{ "getReactionForce",		_getReactionForce },
		{ "getReactionTorque",		_getReactionTorque },
		{ "setLimit",				_setLimit },
		{ "setLimitEnabled",		_setLimitEnabled },
		{ "setMotor",				_setMotor },
		{ "setMotorEnabled",		_setMotorEnabled },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::SetJoint ( b2Joint* joint ) {

	this->mJoint = joint;
	joint->SetUserData ( this );
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::SetLimit ( float lower, float upper ) {
	float unitsToMeters = this->GetUnitsToMeters ();
	
	b2JointType type = this->mJoint->GetType ();
	switch ( type ) {
		
		case e_lineJoint: {
			
			b2LineJoint* joint = ( b2LineJoint* )this->mJoint;
			joint->SetLimits ( lower * unitsToMeters, upper * unitsToMeters );
			joint->EnableLimit ( true );
			break;
		}
		
		case e_prismaticJoint: {
			
			b2PrismaticJoint* joint = ( b2PrismaticJoint* )this->mJoint;
			joint->SetLimits ( lower * unitsToMeters, upper * unitsToMeters );
			joint->EnableLimit ( true );
			break;
		}
		
		case e_revoluteJoint: {
		
			b2RevoluteJoint* joint = ( b2RevoluteJoint* )this->mJoint;
			joint->SetLimits ( lower * ( float )D2R, upper * ( float )D2R );
			joint->EnableLimit ( true );
			break;
		}
		
		default:
			break;
	}
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::SetLimitEnabled ( bool enabled ) {
	
	b2JointType type = this->mJoint->GetType ();
	switch ( type ) {
		
		case e_lineJoint: {
			
			b2LineJoint* joint = ( b2LineJoint* )this->mJoint;
			joint->EnableLimit ( enabled );
			break;
		}
		
		case e_prismaticJoint: {
			
			b2PrismaticJoint* joint = ( b2PrismaticJoint* )this->mJoint;
			joint->EnableLimit ( enabled );
			break;
		}
		
		case e_revoluteJoint: {
		
			b2RevoluteJoint* joint = ( b2RevoluteJoint* )this->mJoint;
			joint->EnableLimit ( enabled );
			break;
		}
		
		default:
			break;
	}
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::SetMotor ( float speed, float max ) {
	float unitsToMeters = this->GetUnitsToMeters ();
	
	b2JointType type = this->mJoint->GetType ();
	switch ( type ) {
		
		case e_lineJoint: {
		
			b2LineJoint* joint = ( b2LineJoint* )this->mJoint;
			joint->SetMotorSpeed ( speed * unitsToMeters );
			joint->SetMaxMotorForce ( max * unitsToMeters );
			joint->EnableMotor ( speed != 0.0f );
			break;
		}
		
		case e_prismaticJoint: {
		
			b2PrismaticJoint* joint = ( b2PrismaticJoint* )this->mJoint;
			joint->SetMotorSpeed ( speed * unitsToMeters );
			joint->SetMaxMotorForce ( max * unitsToMeters );
			joint->EnableMotor ( speed != 0.0f );
			break;
		}
		
		case e_revoluteJoint: {
		
			b2RevoluteJoint* joint = ( b2RevoluteJoint* )this->mJoint;
			joint->SetMotorSpeed ( speed * ( float )D2R );
			joint->SetMaxMotorTorque ( max * ( float )D2R );
			joint->EnableMotor ( speed != 0.0f );
			break;
		}
		
		default:
			break;
	}
}

//----------------------------------------------------------------//
void MOAIBox2DJoint::SetMotorEnabled ( bool enabled ) {
	
	b2JointType type = this->mJoint->GetType ();
	switch ( type ) {
		
		case e_lineJoint: {
			
			b2LineJoint* joint = ( b2LineJoint* )this->mJoint;
			joint->EnableMotor ( enabled );
			break;
		}
		
		case e_prismaticJoint: {
			
			b2PrismaticJoint* joint = ( b2PrismaticJoint* )this->mJoint;
			joint->EnableMotor ( enabled );
			break;
		}
		
		case e_revoluteJoint: {
		
			b2RevoluteJoint* joint = ( b2RevoluteJoint* )this->mJoint;
			joint->EnableMotor ( enabled );
			break;
		}
		
		default:
			break;
	}
}

//----------------------------------------------------------------//
STLString MOAIBox2DJoint::ToString () {

	STLString repr;
	return repr;
}

#endif