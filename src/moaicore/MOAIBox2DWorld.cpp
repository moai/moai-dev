// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DDebugDraw.h>
#include <moaicore/MOAIBox2DDistanceJoint.h>
#include <moaicore/MOAIBox2DFixture.h>
#include <moaicore/MOAIBox2DFrictionJoint.h>
#include <moaicore/MOAIBox2DGearJoint.h>
#include <moaicore/MOAIBox2DJoint.h>
#include <moaicore/MOAIBox2DMouseJoint.h>
#include <moaicore/MOAIBox2DPrismaticJoint.h>
#include <moaicore/MOAIBox2DPulleyJoint.h>
#include <moaicore/MOAIBox2DRevoluteJoint.h>
#include <moaicore/MOAIBox2DRopeJoint.h>
#include <moaicore/MOAIBox2DWeldJoint.h>
#include <moaicore/MOAIBox2DWheelJoint.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// MOAIBox2DPrim
//================================================================//

//----------------------------------------------------------------//
float MOAIBox2DPrim::GetUnitsToMeters () {

	if ( this->mWorld ) {
		return this->mWorld->GetUnitsToMeters ();
	}
	return 1.0f;
}

//----------------------------------------------------------------//
MOAIBox2DPrim::MOAIBox2DPrim () :
	mWorld ( 0 ),
	mDestroy ( false ),
	mDestroyNext ( 0 ) {
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	addBody
	@text	Create and add a body to the world.
	
	@in		MOAIBox2DWorld self
	@in		number type		One of MOAIBox2DBody.DYNAMIC, MOAIBox2DBody.KINEMATIC, MOAIBox2DBody.STATIC
	@opt	number x
	@opt	number y
	@out	MOAIBox2DBody joint
*/
int MOAIBox2DWorld::_addBody ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	u32 type	= state.GetValue < u32 >( 2, 0 );
	float x		= state.GetValue < float >( 3, 0.0f ) * self->mUnitsToMeters;
	float y		= state.GetValue < float >( 4, 0.0f ) * self->mUnitsToMeters;
	
	b2BodyDef groundBodyDef;
	groundBodyDef.type = ( b2BodyType )type;
	groundBodyDef.position.Set ( x, y );
	
	MOAIBox2DBody* body = new MOAIBox2DBody ();
	body->SetBody ( self->mWorld->CreateBody ( &groundBodyDef ));
	body->SetWorld ( self );
	self->LuaRetain ( *body );
	
	body->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addDistanceJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorA_X
	@in		number anchorA_Y
	@in		number anchorB_X
	@in		number anchorB_Y
	@opt	number frequencyHz			Default value determined by Box2D
	@opt	number dampingRatio			Default value determined by Box2D
	@opt	number collideConnected		Default value is false
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addDistanceJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2 );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3 );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchorA;
	anchorA.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchorA.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 anchorB;
	anchorB.x	= state.GetValue < float >( 6, 0 ) * self->mUnitsToMeters;
	anchorB.y	= state.GetValue < float >( 7, 0 ) * self->mUnitsToMeters;
	
	b2DistanceJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchorA, anchorB );
	
	jointDef.frequencyHz	= state.GetValue < float >( 8, jointDef.frequencyHz );
	jointDef.dampingRatio	= state.GetValue < float >( 9, jointDef.dampingRatio );
	jointDef.collideConnected = state.GetValue < bool >( 10, false );
	
	MOAIBox2DDistanceJoint* joint = new MOAIBox2DDistanceJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( *joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addFrictionJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorX
	@in		number anchorY
	@opt	number maxForce			Converted to N. 	Default value determined by Box2D
	@opt	number maxTorque		Converted to N-m.	Default value determined by Box2D
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addFrictionJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2 );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3 );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2FrictionJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor );

	float unitsToMeters = self->GetUnitsToMeters();

	jointDef.maxForce	= state.GetValue < float >( 6, jointDef.maxForce / unitsToMeters ) * unitsToMeters;
	/* Convert to/from N-m (kg m / s^2) * m from/to (kg unit / s^2) * unit */
	jointDef.maxTorque	= state.GetValue < float >( 7, jointDef.maxTorque / ( unitsToMeters * unitsToMeters ) ) * unitsToMeters * unitsToMeters;
	
	MOAIBox2DFrictionJoint* joint = new MOAIBox2DFrictionJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( *joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addGearJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DJoint jointA
	@in		MOAIBox2DJoint jointB
	@in		float ratio
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addGearJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DJoint* jointA = state.GetLuaObject < MOAIBox2DJoint >( 2 );
	MOAIBox2DJoint* jointB = state.GetLuaObject < MOAIBox2DJoint >( 3 );
	
	if ( !( jointA && jointB )) return 0;
	
	b2GearJointDef jointDef;
	
	jointDef.bodyA	= jointA->mJoint->GetBodyA ();
	jointDef.bodyB	= jointB->mJoint->GetBodyB ();
	jointDef.joint1	= jointA->mJoint;
	jointDef.joint2	= jointB->mJoint;
	jointDef.ratio	= state.GetValue < float >( 4, 0.0f );
	
	MOAIBox2DGearJoint* joint = new MOAIBox2DGearJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( *joint );
	
	joint->mJointA.Set ( *self, jointA );
	joint->mJointB.Set ( *self, jointB );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addMouseJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number targetX
	@in		number targetY
	@in		number maxForce
	@opt	number frequencyHz			Default value determined by Box2D
	@opt	number dampingRatio			Default value determined by Box2D
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addMouseJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2 );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3 );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 target;
	target.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	target.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2MouseJointDef jointDef;
	jointDef.bodyA			= bodyA->mBody;
	jointDef.bodyB			= bodyB->mBody;
	jointDef.target			= target;
	jointDef.maxForce		= state.GetValue < float >( 6, 0.0f ) * self->mUnitsToMeters;
	jointDef.frequencyHz	= state.GetValue < float >( 7, jointDef.frequencyHz );
	jointDef.dampingRatio	= state.GetValue < float >( 8, jointDef.dampingRatio );
	
	MOAIBox2DMouseJoint* joint = new MOAIBox2DMouseJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( *joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addPrismaticJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorA
	@in		number anchorB
	@in		number axisA
	@in		number axisB
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addPrismaticJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2 );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3 );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 axis;
	axis.x		= state.GetValue < float >( 6, 0 );
	axis.y		= state.GetValue < float >( 7, 0 );
	
	b2PrismaticJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor, axis );
	
	MOAIBox2DPrismaticJoint* joint = new MOAIBox2DPrismaticJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( *joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addPulleyJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number groundAnchorA_X
	@in		number groundAnchorA_Y
	@in		number groundAnchorB_X
	@in		number groundAnchorB_Y
	@in		number anchorA_X
	@in		number anchorA_Y
	@in		number anchorB_X
	@in		number anchorB_Y
	@in		number ratio
	@in		number maxLengthA
	@in		number maxLengthB
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addPulleyJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNNNNNNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2 );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3 );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 groundAnchorA;
	groundAnchorA.x		= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	groundAnchorA.y		= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 groundAnchorB;
	groundAnchorB.x		= state.GetValue < float >( 6, 0 ) * self->mUnitsToMeters;
	groundAnchorB.y		= state.GetValue < float >( 7, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 anchorA;
	anchorA.x			= state.GetValue < float >( 8, 0 ) * self->mUnitsToMeters;
	anchorA.y			= state.GetValue < float >( 9, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 anchorB;
	anchorB.x			= state.GetValue < float >( 10, 0 ) * self->mUnitsToMeters;
	anchorB.y			= state.GetValue < float >( 11, 0 ) * self->mUnitsToMeters;
	
	float ratio			= state.GetValue < float >( 12, 0 );
	
	b2PulleyJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, groundAnchorA, groundAnchorB, anchorA, anchorB, ratio );
	
	jointDef.lengthA	= state.GetValue < float >( 13, 0 ) * self->mUnitsToMeters;
	jointDef.lengthB	= state.GetValue < float >( 14, 0 ) * self->mUnitsToMeters;
	
	MOAIBox2DPulleyJoint* joint = new MOAIBox2DPulleyJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( *joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addRevoluteJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorX
	@in		number anchorY
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addRevoluteJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2 );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3 );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2RevoluteJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor );
	
	MOAIBox2DRevoluteJoint* joint = new MOAIBox2DRevoluteJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( *joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addRopeJoint
 @text	Create and add a rope joint to the world. See Box2D documentation.
 
 @in		MOAIBox2DWorld self
 @in		MOAIBox2DBody bodyA
 @in		MOAIBox2DBody bodyB 
 @in		number maxLength
 @opt		number anchorAX
 @opt		number anchorAY
 @opt		number anchorBX
 @opt		number anchorBY
 @out	MOAIBox2DJoint joint
 */
int	MOAIBox2DWorld::_addRopeJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUU" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2 );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3 );
	
	if ( !( bodyA && bodyB )) return 0;
	
	float maxLength = state.GetValue < float >( 4, 1 ) * self->mUnitsToMeters;
	
	b2Vec2 anchorA;
	anchorA.x	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	anchorA.y	= state.GetValue < float >( 6, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 anchorB;
	anchorB.x	= state.GetValue < float >( 7, 0 ) * self->mUnitsToMeters;
	anchorB.y	= state.GetValue < float >( 8, 0 ) * self->mUnitsToMeters;
	
	b2RopeJointDef jointDef;
	jointDef.bodyA = bodyA->mBody;
	jointDef.bodyB = bodyB->mBody;
	jointDef.maxLength = maxLength;
	
	MOAIBox2DRopeJoint* joint = new MOAIBox2DRopeJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( *joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}


//----------------------------------------------------------------//
/**	@name	addWeldJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorX
	@in		number anchorY
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addWeldJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2 );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3 );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2WeldJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor );
	
	MOAIBox2DWeldJoint* joint = new MOAIBox2DWeldJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( *joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name addWheelJoint
	@text	Create and add a joint to the world. See Box2D documentation.
 
 @in		MOAIBox2DWorld self
 @in		MOAIBox2DBody bodyA
 @in		MOAIBox2DBody bodyB
 @in		number anchorX
 @in		number anchorY
 @in		number axisX
 @in		number axisY
 @out	MOAIBox2DJoint joint
 */
int	MOAIBox2DWorld::_addWheelJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2 );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3 );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2Vec2 axis;
	axis.x		= state.GetValue < float >( 6, 0 );
	axis.y      = state.GetValue < float >( 7, 0 );
	
	b2WheelJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor, axis );
	
	MOAIBox2DWheelJoint* joint = new MOAIBox2DWheelJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( *joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAngularSleepTolerance
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	number angularSleepTolerance
*/
int MOAIBox2DWorld::_getAngularSleepTolerance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	lua_pushnumber ( state, self->mWorld->GetAngularSleepTolerance ());
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAutoClearForces
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	boolean autoClearForces
*/
int MOAIBox2DWorld::_getAutoClearForces ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	bool autoClearForces = self->mWorld->GetAutoClearForces ();
	lua_pushboolean ( L, autoClearForces );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getGravity
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	number gravityX
	@out	number gravityY
*/
int MOAIBox2DWorld::_getGravity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	b2Vec2 gravity = self->mWorld->GetGravity ();
	
	lua_pushnumber ( L, gravity.x / self->mUnitsToMeters );
	lua_pushnumber ( L, gravity.y / self->mUnitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getLinearSleepTolerance
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	number linearSleepTolerance
*/
int MOAIBox2DWorld::_getLinearSleepTolerance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	lua_pushnumber ( state, self->mWorld->GetLinearSleepTolerance () / self->mUnitsToMeters );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getTimeToSleep
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	number timeToSleep
*/
int MOAIBox2DWorld::_getTimeToSleep ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	lua_pushnumber ( state, self->mWorld->GetTimeToSleep ());
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAngularSleepTolerance
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number angularSleepTolerance		Default value is 0.0f.
	@out	nil
*/
int MOAIBox2DWorld::_setAngularSleepTolerance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	self->mWorld->SetAngularSleepTolerance ( state.GetValue < float >( 2, 0.0f ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAutoClearForces
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	boolean autoClearForces		Default value is 'true'
	@out	nil
*/
int MOAIBox2DWorld::_setAutoClearForces ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	bool autoClearForces = state.GetValue < bool >( 2, true );
	
	self->mWorld->SetAutoClearForces ( autoClearForces );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setGravity
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number gravityX			Default value is 0.
	@opt	number gravityY			Default value is 0.
	@out	nil
*/
int MOAIBox2DWorld::_setGravity ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	b2Vec2 gravity;
	
	gravity.x	= state.GetValue < float >( 2, 0.0f ) * self->mUnitsToMeters;
	gravity.y	= state.GetValue < float >( 3, 0.0f ) * self->mUnitsToMeters;
	
	self->mWorld->SetGravity ( gravity );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setIterations
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number velocityIteratons			Default value is current value of velocity iterations.
	@opt	number positionIterations			Default value is current value of positions iterations.
	@out	nil
*/
int MOAIBox2DWorld::_setIterations ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	self->mVelocityIterations = state.GetValue < u32 >( 2, self->mVelocityIterations );
	self->mPositionIterations = state.GetValue < u32 >( 3, self->mPositionIterations );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLinearSleepTolerance
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number linearSleepTolerance		Default value is 0.0f.
	@out	nil
*/
int MOAIBox2DWorld::_setLinearSleepTolerance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	self->mWorld->SetLinearSleepTolerance ( state.GetValue < float >( 2, 0.0f ) * self->mUnitsToMeters );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTimeToSleep
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number timeToSleep				Default value is 0.0f.
	@out	nil
*/
int MOAIBox2DWorld::_setTimeToSleep ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	self->mWorld->SetTimeToSleep ( state.GetValue < float >( 2, 0.0f ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUnitsToMeters
	@text	Sets a scale factor for converting game world units to Box2D meters.
	
	@in		MOAIBox2DWorld self
	@opt	number unitsToMeters			Default value is 1.
	@out	nil
*/
int MOAIBox2DWorld::_setUnitsToMeters ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	self->mUnitsToMeters = state.GetValue ( 2, 1.0f );
	
	return 0;
}

//================================================================//
// MOAIBox2DWorld
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DWorld::Destroy () {

	if ( this->mLock ) return;
	this->mLock = true;

	while ( this->mDestroyFixtures ) {
		MOAIBox2DPrim* prim = this->mDestroyFixtures;
		this->mDestroyFixtures = this->mDestroyFixtures->mDestroyNext;
		prim->Destroy ();
		
		prim->SetWorld ( 0 );
		this->LuaRelease ( *prim );
	}
	
	while ( this->mDestroyJoints ) {
		MOAIBox2DPrim* prim = this->mDestroyJoints;
		this->mDestroyJoints = this->mDestroyJoints->mDestroyNext;
		prim->Destroy ();
		
		prim->SetWorld ( 0 );
		this->LuaRelease ( *prim );
	}
	
	while ( this->mDestroyBodies ) {
		MOAIBox2DPrim* prim = this->mDestroyBodies;
		this->mDestroyBodies = this->mDestroyBodies->mDestroyNext;
		prim->Destroy ();
		
		prim->SetWorld ( 0 );
		this->LuaRelease ( *prim );
	}
	
	this->mLock = false;
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::DrawDebug () {

	if ( this->mDebugDraw ) {
		
		MOAIDraw::Bind ();
		
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		
		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_WORLD, MOAIGfxDevice::VTX_STAGE_PROJ );
		gfxDevice.SetVertexTransform ( MOAIGfxDevice::VTX_WORLD_TRANSFORM );
		
		this->mDebugDraw->mScale = 1.0f / this->mUnitsToMeters;
		this->mWorld->DrawDebugData ();
	}
}

//----------------------------------------------------------------//
bool MOAIBox2DWorld::IsDone () {

	return false;
}

//----------------------------------------------------------------//
bool MOAIBox2DWorld::IsLocked () {

	if ( this->mWorld ) {
		return this->mWorld->IsLocked ();
	}
	return false;
}

//----------------------------------------------------------------//
MOAIBox2DWorld::MOAIBox2DWorld () :
	mLock ( false ),
	mVelocityIterations ( 10 ),
	mPositionIterations ( 10 ),
	mUnitsToMeters ( 1.0f ),
	mDestroyBodies ( 0 ),
	mDestroyFixtures ( 0 ),
	mDestroyJoints ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mArbiter.Set ( *this, new MOAIBox2DArbiter ());
	
	b2Vec2 gravity ( 0.0f, 0.0f );
	this->mWorld = new b2World ( gravity);
	this->mWorld->SetContactListener ( this->mArbiter );
	this->mWorld->SetDestructionListener ( this );
	this->mWorld->SetAllowSleeping(true);
	this->mDebugDraw = new MOAIBox2DDebugDraw ();
	this->mWorld->SetDebugDraw ( this->mDebugDraw );
	
	this->mDebugDraw->SetFlags (
		b2Draw::e_shapeBit			|
		b2Draw::e_jointBit			|
		//b2DebugDraw::e_aabbBit			|
		//b2DebugDraw::e_pairBit			|
		b2Draw::e_centerOfMassBit
	);
}

//----------------------------------------------------------------//
MOAIBox2DWorld::~MOAIBox2DWorld () {

	this->mWorld->SetContactListener ( 0 );

	while ( b2Body* body = this->mWorld->GetBodyList ()) {
		MOAIBox2DBody* moaiBody = ( MOAIBox2DBody* )body->GetUserData ();
		
		this->mWorld->DestroyBody ( body );
		moaiBody->mBody = 0;
		moaiBody->SetWorld ( 0 );
		this->LuaRelease ( *moaiBody );
	}
	
	this->mArbiter.Set ( *this, 0 );
	
	delete ( this->mDebugDraw );
	delete ( this->mWorld );
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::OnUpdate ( float step ) {
	
	this->mLock = true;
	this->mWorld->Step ( step, this->mVelocityIterations, this->mPositionIterations );
	this->mLock = false;
	
	this->Destroy ();
	
	b2Body* body = this->mWorld->GetBodyList ();
	for ( ; body; body = body->GetNext ()) {
		if ( body->IsActive () && body->IsAwake ()) {
			MOAIBox2DBody* moaiBody = ( MOAIBox2DBody* )body->GetUserData ();
			moaiBody->ScheduleUpdate ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "addBody",					_addBody },
		{ "addDistanceJoint",			_addDistanceJoint },
		{ "addFrictionJoint",			_addFrictionJoint },
		{ "addGearJoint",				_addGearJoint },
		{ "addMouseJoint",				_addMouseJoint },
		{ "addPrismaticJoint",			_addPrismaticJoint },
		{ "addPulleyJoint",				_addPulleyJoint },
		{ "addRevoluteJoint",			_addRevoluteJoint },
		{ "addRopeJoint",				_addRopeJoint },
		{ "addWeldJoint",				_addWeldJoint },
		{ "addWheelJoint",				_addWheelJoint },
		{ "getAngularSleepTolerance",	_getAngularSleepTolerance },
		{ "getAutoClearForces",			_getAutoClearForces },
		{ "getGravity",					_getGravity },
		{ "getLinearSleepTolerance",	_getLinearSleepTolerance },
		{ "getTimeToSleep",				_getTimeToSleep },
		{ "setAngularSleepTolerance",	_setAngularSleepTolerance },
		{ "setAutoClearForces",			_setAutoClearForces },
		{ "setGravity",					_setGravity },
		{ "setIterations",				_setIterations },
		{ "setLinearSleepTolerance",	_setLinearSleepTolerance },
		{ "setTimeToSleep",				_setTimeToSleep },
		{ "setUnitsToMeters",			_setUnitsToMeters },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::SayGoodbye ( b2Fixture* fixture ) {

	MOAIBox2DFixture* moaiFixture = ( MOAIBox2DFixture* )fixture->GetUserData ();
	if ( moaiFixture->mFixture ) {
		moaiFixture->mFixture = 0;
		moaiFixture->SetWorld ( 0 );
		this->LuaRelease ( *moaiFixture );
	}
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::SayGoodbye ( b2Joint* joint ) {

	MOAIBox2DJoint* moaiJoint = ( MOAIBox2DJoint* )joint->GetUserData ();
	if ( moaiJoint->mJoint ) {
		moaiJoint->mJoint = 0;
		moaiJoint->SetWorld ( 0 );
		this->LuaRelease ( *moaiJoint );
	}
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::ScheduleDestruction ( MOAIBox2DBody& body ) {

	if ( !body.mDestroy ) {
		body.mDestroyNext = this->mDestroyBodies;
		this->mDestroyBodies = &body;
		body.mDestroy = true;
	}
	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::ScheduleDestruction ( MOAIBox2DFixture& fixture ) {

	if ( !fixture.mDestroy ) {
		fixture.mDestroyNext = this->mDestroyFixtures;
		this->mDestroyFixtures = &fixture;
		fixture.mDestroy = true;
	}
	this->Destroy ();
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::ScheduleDestruction ( MOAIBox2DJoint& joint ) {

	if ( !joint.mDestroy ) {
		joint.mDestroyNext = this->mDestroyJoints;
		this->mDestroyJoints = &joint;
		joint.mDestroy = true;
	}
	this->Destroy ();
}

#endif