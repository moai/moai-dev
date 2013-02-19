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
	@opt	number x	in units, in world coordinates, converted to meters
	@opt	number y	in units, in world coordinates, converted to meters
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
	self->LuaRetain ( body );
	
	body->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addDistanceJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorA_X	in units, in world coordinates, converted to meters
	@in		number anchorA_Y	in units, in world coordinates, converted to meters
	@in		number anchorB_X	in units, in world coordinates, converted to meters
	@in		number anchorB_Y	in units, in world coordinates, converted to meters
	@opt	number frequencyHz			in Hz. Default value determined by Box2D
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
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
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
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addFrictionJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorX	in units, in world coordinates, converted to meters
	@in		number anchorY	in units, in world coordinates, converted to meters
	@opt	number maxForce			in kg * units / s^2, converted to N [kg * m / s^2]. 	Default value determined by Box2D
	@opt	number maxTorque		in kg * units / s^2 * units, converted to N-m [kg * m / s^2 * m].	Default value determined by Box2D
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addFrictionJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
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
	self->LuaRetain ( joint );
	
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
	
	MOAIBox2DJoint* jointA = state.GetLuaObject < MOAIBox2DJoint >( 2, true );
	MOAIBox2DJoint* jointB = state.GetLuaObject < MOAIBox2DJoint >( 3, true );
	
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
	self->LuaRetain ( joint );
	
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
	@in		number targetX	in units, in world coordinates, converted to meters
	@in		number targetY	in units, in world coordinates, converted to meters
	@in		number maxForce	in kg * units / s^2, converted to N [kg * m / s^2].
	@opt	number frequencyHz			in Hz. Default value determined by Box2D
	@opt	number dampingRatio			Default value determined by Box2D
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addMouseJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
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
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addPrismaticJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorA	in units, in world coordinates, converted to meters
	@in		number anchorB	in units, in world coordinates, converted to meters
	@in		number axisA	translation axis vector X component (no units)
	@in		number axisB	translation axis vector Y component (no units)
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addPrismaticJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
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
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addPulleyJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number groundAnchorA_X	in units, in world coordinates, converted to meters
	@in		number groundAnchorA_Y	in units, in world coordinates, converted to meters
	@in		number groundAnchorB_X	in units, in world coordinates, converted to meters
	@in		number groundAnchorB_Y	in units, in world coordinates, converted to meters
	@in		number anchorA_X	in units, in world coordinates, converted to meters
	@in		number anchorA_Y	in units, in world coordinates, converted to meters
	@in		number anchorB_X	in units, in world coordinates, converted to meters
	@in		number anchorB_Y	in units, in world coordinates, converted to meters
	@in		number ratio
	@in		number maxLengthA	in units, converted to meters
	@in		number maxLengthB	in units, converted to meters
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addPulleyJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNNNNNNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
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
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addRevoluteJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorX	in units, in world coordinates, converted to meters
	@in		number anchorY	in units, in world coordinates, converted to meters
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addRevoluteJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2RevoluteJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor );
	
	MOAIBox2DRevoluteJoint* joint = new MOAIBox2DRevoluteJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addRopeJoint
 @text	Create and add a rope joint to the world. See Box2D documentation.
 
 @in		MOAIBox2DWorld self
 @in		MOAIBox2DBody bodyA
 @in		MOAIBox2DBody bodyB 
 @in		number maxLength	in units, converted to meters
 @opt		number anchorAX		in units, in world coordinates, converted to meters
 @opt		number anchorAY		in units, in world coordinates, converted to meters
 @opt		number anchorBX		in units, in world coordinates, converted to meters
 @opt		number anchorBY		in units, in world coordinates, converted to meters
 @opt		number collideConnected		Default value is false		
 @out	MOAIBox2DJoint joint
 */
int	MOAIBox2DWorld::_addRopeJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUU" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	float maxLength = state.GetValue < float >( 4, 1 ) * self->mUnitsToMeters;
	
	b2RopeJointDef jointDef;
	jointDef.localAnchorA.x	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	jointDef.localAnchorA.y	= state.GetValue < float >( 6, 0 ) * self->mUnitsToMeters;

	jointDef.localAnchorB.x	= state.GetValue < float >( 7, 0 ) * self->mUnitsToMeters;
	jointDef.localAnchorB.y	= state.GetValue < float >( 8, 0 ) * self->mUnitsToMeters;

	jointDef.collideConnected = state.GetValue < bool >( 9, false );
	
	jointDef.bodyA = bodyA->mBody;
	jointDef.bodyB = bodyB->mBody;
	jointDef.maxLength = maxLength;
	
	MOAIBox2DRopeJoint* joint = new MOAIBox2DRopeJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}


//----------------------------------------------------------------//
/**	@name	addWeldJoint
	@text	Create and add a joint to the world. See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@in		MOAIBox2DBody bodyA
	@in		MOAIBox2DBody bodyB
	@in		number anchorX	in units, in world coordinates, converted to meters
	@in		number anchorY	in units, in world coordinates, converted to meters
	@out	MOAIBox2DJoint joint
*/
int	MOAIBox2DWorld::_addWeldJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
	if ( !( bodyA && bodyB )) return 0;
	
	b2Vec2 anchor;
	anchor.x	= state.GetValue < float >( 4, 0 ) * self->mUnitsToMeters;
	anchor.y	= state.GetValue < float >( 5, 0 ) * self->mUnitsToMeters;
	
	b2WeldJointDef jointDef;
	jointDef.Initialize ( bodyA->mBody, bodyB->mBody, anchor );
	
	MOAIBox2DWeldJoint* joint = new MOAIBox2DWeldJoint ();
	joint->SetJoint ( self->mWorld->CreateJoint ( &jointDef ));
	joint->SetWorld ( self );
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name addWheelJoint
	@text	Create and add a joint to the world. See Box2D documentation.
 
 @in		MOAIBox2DWorld self
 @in		MOAIBox2DBody bodyA
 @in		MOAIBox2DBody bodyB
 @in		number anchorX	in units, in world coordinates, converted to meters
 @in		number anchorY	in units, in world coordinates, converted to meters
 @in		number axisX	translation axis vector X component (no units)
 @in		number axisY	translation axis vector Y component (no units)
 @out	MOAIBox2DJoint joint
 */
int	MOAIBox2DWorld::_addWheelJoint ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "UUUNNNN" )
	
	if ( self->IsLocked ()) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DWorld_IsLocked );
		return 0;
	}
	
	MOAIBox2DBody* bodyA = state.GetLuaObject < MOAIBox2DBody >( 2, true );
	MOAIBox2DBody* bodyB = state.GetLuaObject < MOAIBox2DBody >( 3, true );
	
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
	self->LuaRetain ( joint );
	
	joint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAngularSleepTolerance
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@out	number angularSleepTolerance	in degrees/s, converted from radians/s
*/
int MOAIBox2DWorld::_getAngularSleepTolerance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	float tolerance = self->mWorld->GetAngularSleepTolerance();
	tolerance *= ( float )R2D;
	lua_pushnumber ( state, tolerance);
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
	@out	number gravityX		in units/s^2, converted from m/s^2
	@out	number gravityY		in units/s^2, converted from m/s^2
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
	@out	number linearSleepTolerance	in units/s, converted from m/s
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
	@opt	number angularSleepTolerance		in degrees/s, converted to radians/s. Default value is 0.0f.
	@out	nil
*/
int MOAIBox2DWorld::_setAngularSleepTolerance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	float tolerance = state.GetValue< float >( 2, 0.0f );
	tolerance *= ( float )R2D;
	self->mWorld->SetAngularSleepTolerance ( tolerance );
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
/**	@name	setDebugDrawEnabled
	@text	enable/disable debug drawing.
 
	@in		MOAIBox2dWorld self
	@in		number bEnable
	@out	nil
*/
int MOAIBox2DWorld::_setDebugDrawEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
 
	bool enabled = state.GetValue < bool >( 2, false );
 
	if(enabled)
	{
		self->mDebugDraw->SetFlags (
			b2Draw::e_shapeBit			|
			b2Draw::e_jointBit			|
			b2Draw::e_centerOfMassBit
		);
	}
	else
	{
		self->mDebugDraw->ClearFlags(0);
		self->mDebugDraw->SetFlags(0);
	}
 
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDebugDrawFlags
	@text	Sets mask for debug drawing.
	
	@in		MOAIBox2DWorld self
	@opt	number flags		One of MOAIBox2DWorld.DEBUG_DRAW_SHAPES, MOAIBox2DWorld.DEBUG_DRAW_JOINTS,
								MOAIBox2DWorld.DEBUG_DRAW_BOUNDS, MOAIBox2DWorld.DEBUG_DRAW_PAIRS,
								MOAIBox2DWorld.DEBUG_DRAW_CENTERS. Default value is MOAIBox2DWorld.DEBUG_DRAW_DEFAULT.
	@out	nil
*/
int MOAIBox2DWorld::_setDebugDrawFlags ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWorld, "U" )
	
	u32 flags = state.GetValue < u32 >( 2, DEBUG_DRAW_DEFAULT );
	if ( self->mDebugDraw ) {
		self->mDebugDraw->SetFlags ( flags );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setGravity
	@text	See Box2D documentation.
	
	@in		MOAIBox2DWorld self
	@opt	number gravityX			in units/s^2, converted to m/s^2. Default value is 0.
	@opt	number gravityY			in units/s^2, converted to m/s^2. Default value is 0.
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
	@opt	number linearSleepTolerance		in units/s, converted to m/s. Default value is 0.0f.
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
		this->LuaRelease ( prim );
	}
	
	while ( this->mDestroyJoints ) {
		MOAIBox2DPrim* prim = this->mDestroyJoints;
		this->mDestroyJoints = this->mDestroyJoints->mDestroyNext;
		prim->Destroy ();
		
		prim->SetWorld ( 0 );
		this->LuaRelease ( prim );
	}
	
	while ( this->mDestroyBodies ) {
		MOAIBox2DPrim* prim = this->mDestroyBodies;
		this->mDestroyBodies = this->mDestroyBodies->mDestroyNext;
		prim->Destroy ();
		
		prim->SetWorld ( 0 );
		this->LuaRelease ( prim );
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
	
	this->mArbiter.Set ( *this, new MOAIBox2DArbiter ( *this ));
	
	b2Vec2 gravity ( 0.0f, 0.0f );
	this->mWorld = new b2World ( gravity);
	this->mWorld->SetContactListener ( this->mArbiter );
	this->mWorld->SetDestructionListener ( this );
	this->mWorld->SetAllowSleeping(true);
	this->mDebugDraw = new MOAIBox2DDebugDraw ();
	this->mWorld->SetDebugDraw ( this->mDebugDraw );
	
	this->mDebugDraw->SetFlags ( DEBUG_DRAW_DEFAULT );
}

//----------------------------------------------------------------//
MOAIBox2DWorld::~MOAIBox2DWorld () {

	this->mWorld->SetContactListener ( 0 );

	while ( b2Body* body = this->mWorld->GetBodyList ()) {
		MOAIBox2DBody* moaiBody = ( MOAIBox2DBody* )body->GetUserData ();
		
		this->mWorld->DestroyBody ( body );
		moaiBody->mBody = 0;
		moaiBody->SetWorld ( 0 );
		this->LuaRelease ( moaiBody );
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
	
	state.SetField ( -1, "DEBUG_DRAW_SHAPES", ( u32 )DEBUG_DRAW_SHAPES );
	state.SetField ( -1, "DEBUG_DRAW_JOINTS", ( u32 )DEBUG_DRAW_JOINTS );
	state.SetField ( -1, "DEBUG_DRAW_BOUNDS", ( u32 )DEBUG_DRAW_BOUNDS );
	state.SetField ( -1, "DEBUG_DRAW_PAIRS", ( u32 )DEBUG_DRAW_PAIRS );
	state.SetField ( -1, "DEBUG_DRAW_CENTERS", ( u32 )DEBUG_DRAW_CENTERS );
	
	state.SetField ( -1, "DEBUG_DRAW_DEFAULT", ( u32 )DEBUG_DRAW_DEFAULT );
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
		{ "setDebugDrawEnabled",		_setDebugDrawEnabled },
		{ "setDebugDrawFlags",			_setDebugDrawFlags },
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
		this->LuaRelease ( moaiFixture );
	}
}

//----------------------------------------------------------------//
void MOAIBox2DWorld::SayGoodbye ( b2Joint* joint ) {

	MOAIBox2DJoint* moaiJoint = ( MOAIBox2DJoint* )joint->GetUserData ();
	if ( moaiJoint->mJoint ) {
		moaiJoint->mJoint = 0;
		moaiJoint->SetWorld ( 0 );
		this->LuaRelease ( moaiJoint );
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