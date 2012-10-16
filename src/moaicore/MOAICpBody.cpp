// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICpBody.h>
#include <moaicore/MOAICpShape.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITransform.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_CHIPMUNK

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	activate
	@text	Activates a body after it has been put to sleep (physics will now be processed for this body again).

	@in		MOAICpBody self
	@out	nil
*/
int MOAICpBody::_activate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpBodyActivate ( self->mBody );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addCircle
	@text	Adds a circle to the body.

	@in		MOAICpBody self
	@in		number radius
	@in		number x
	@in		number y
	@out	MOAICpShape circle
*/
int MOAICpBody::_addCircle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UN" )
			
	cpFloat radius	= state.GetValue < cpFloat >( 2, 0 );
			
	cpVect offset;
	offset.x		= state.GetValue < cpFloat >( 3, 0 );
	offset.y		= state.GetValue < cpFloat >( 4, 0 );
	
	MOAICpShape* shape = new MOAICpShape ();
	shape->mShape	= cpCircleShapeNew ( 0, radius, offset );
	shape->mShape->data = shape;
	
	self->AttachShape ( *shape );
	shape->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addPolygon
	@text	Adds a polygon to the body.

	@in		MOAICpBody self
	@in		table polygon
	@out	MOAICpShape polygon
*/
int MOAICpBody::_addPolygon ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UT" )
	
	cpVect verts [ MOAICpShape::MAX_POLY_VERTS ];
	int numVerts = MOAICpShape::LoadVerts ( state, 2, verts, MOAICpShape::MAX_POLY_VERTS );
			
	if ( numVerts && cpPolyValidate ( verts, numVerts )) {
			
		cpVect offset;
		offset.x = 0;
		offset.y = 0;
		
		MOAICpShape* shape = new MOAICpShape ();
		shape->mShape	= cpPolyShapeNew ( 0, numVerts, verts, offset );
		shape->mShape->data = shape;
		
		self->AttachShape ( *shape );
		shape->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addRect
	@text	Adds a rectangle to the body.

	@in		MOAICpBody self
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@out	MOAICpShape rectangle
*/
int MOAICpBody::_addRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UNNNN" )
		
	USMetaRect < cpFloat > rect = state.GetRect < cpFloat >( 2 );
	rect.Bless ();
	
	cpVect verts [ 4 ];
	
	verts [ 0 ].x = rect.mXMin;
	verts [ 0 ].y = rect.mYMax;
	
	verts [ 1 ].x = rect.mXMax;
	verts [ 1 ].y = rect.mYMax;
	
	verts [ 2 ].x = rect.mXMax;
	verts [ 2 ].y = rect.mYMin;
	
	verts [ 3 ].x = rect.mXMin;
	verts [ 3 ].y = rect.mYMin;
	
	cpVect offset;
	offset.x = 0;
	offset.y = 0;
	
	MOAICpShape* shape = new MOAICpShape ();
	shape->mShape	= cpPolyShapeNew ( 0, 4, verts, offset );
	shape->mShape->data = shape;
	
	self->AttachShape ( *shape );
	shape->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	addSegment
	@text	Adds a segment to the body.

	@in		MOAICpBody self
	@in		number x1
	@in		number y1
	@in		number x2
	@in		number y2
	@opt	number radius
	@out	MOAICpShape segment
*/
int MOAICpBody::_addSegment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UNNNN" )
	
	cpVect a;
	a.x = state.GetValue < cpFloat >( 2, 0 );
	a.y = state.GetValue < cpFloat >( 3, 0 );
	
	cpVect b;
	b.x = state.GetValue < cpFloat >( 4, 0 );
	b.y = state.GetValue < cpFloat >( 5, 0 );
	
	cpFloat radius = state.GetValue < cpFloat >( 6, 0 );
	
	MOAICpShape* shape = new MOAICpShape ();
	shape->mShape	= cpSegmentShapeNew ( 0, a, b, radius );
	shape->mShape->data = shape;
	
	self->AttachShape ( *shape );
	shape->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	applyForce
	@text	Applies force to the body, taking into account any existing forces being applied.

	@in		MOAICpBody self
	@in		number fx
	@in		number fy
	@in		number rx
	@in		number ry
	@out	nil
*/
int MOAICpBody::_applyForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UNN" )
	
	cpVect f;
	f.x = state.GetValue < cpFloat >( 2, 0 );
	f.y = state.GetValue < cpFloat >( 3, 0 );
	
	cpVect r;
	r.x = state.GetValue < cpFloat >( 4, 0 );
	r.y = state.GetValue < cpFloat >( 5, 0 );
	
	cpBodyApplyForce( self->mBody, f, r );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	applyImpulse
	@text	Applies impulse to the body, taking into account any existing impulses being applied.

	@in		MOAICpBody self
	@in		number jx
	@in		number jy
	@in		number rx
	@in		number ry
	@out	nil
*/
int MOAICpBody::_applyImpulse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UNN" )
	
	cpVect j;
	j.x = state.GetValue < cpFloat >( 2, 0 );
	j.y = state.GetValue < cpFloat >( 3, 0 );
	
	cpVect r;
	r.x = state.GetValue < cpFloat >( 4, 0 );
	r.y = state.GetValue < cpFloat >( 5, 0 );
	
	cpBodyApplyImpulse( self->mBody, j, r );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getAngle
	@text	Returns the angle of the body.

	@in		MOAICpBody self
	@out	number angle		The current angle.
*/
int MOAICpBody::_getAngle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpFloat a = cpBodyGetAngle ( self->mBody );
	lua_pushnumber ( state, a );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getAngVel
	@text	Returns the angular velocity of the body.

	@in		MOAICpBody self
	@out	number angle		The current angular velocity.
*/
int MOAICpBody::_getAngVel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpFloat av = cpBodyGetAngVel ( self->mBody );
	lua_pushnumber ( state, av );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getForce
	@text	Returns the force of the body.

	@in		MOAICpBody self
	@out	number x			The X component of the current force being applied.
	@out	number y			The Y component of the current force being applied.
*/
int MOAICpBody::_getForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpVect f = cpBodyGetForce ( self->mBody );
	lua_pushnumber ( state, f.x );
	lua_pushnumber ( state, f.y );
		
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getMass
	@text	Returns the mass of the body.

	@in		MOAICpBody self
	@out	number mass			The current mass.
*/
int MOAICpBody::_getMass ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpFloat m = cpBodyGetMass ( self->mBody );
	lua_pushnumber ( state, m );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMoment
	@text	Returns the moment of the body.

	@in		MOAICpBody self
	@out	number moment		The current moment.
*/
int MOAICpBody::_getMoment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpFloat m = cpBodyGetMoment ( self->mBody );
	lua_pushnumber ( state, m );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getPos
	@text	Returns the position of the body.

	@in		MOAICpBody self
	@out	number x			The X position.
	@out	number y			The Y position.
*/
int MOAICpBody::_getPos ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpVect p = cpBodyGetPos ( self->mBody );
	lua_pushnumber ( state, p.x );
	lua_pushnumber ( state, p.y );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getRot
	@text	Returns the rotation of the body.

	@in		MOAICpBody self
	@out	number x			The X position.
	@out	number y			The Y position.
*/
int MOAICpBody::_getRot ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpVect r = cpBodyGetRot ( self->mBody );
	lua_pushnumber ( state, r.x );
	lua_pushnumber ( state, r.y );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getTorque
	@text	Returns the torque of the body.

	@in		MOAICpBody self
	@out	number torque		The current torque.
*/
int MOAICpBody::_getTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpFloat t = cpBodyGetTorque ( self->mBody );
	lua_pushnumber ( state, t );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getVel
	@text	Returns the velocity of the body.

	@in		MOAICpBody self
	@out	number x			The X component of the current velocity.
	@out	number y			The Y component of the current velocity.
*/
int MOAICpBody::_getVel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpVect v = cpBodyGetVel ( self->mBody );
	lua_pushnumber ( state, v.x );
	lua_pushnumber ( state, v.y );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	isSleeping
	@text	Returns whether the body is currently sleeping.

	@in		MOAICpBody self
	@out	boolean sleeping	Whether the body is sleeping.
*/
int MOAICpBody::_isSleeping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpBool is = cpBodyIsSleeping( self->mBody );
	lua_pushboolean ( state, ( is == cpTrue ));
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isStatic
	@text	Returns whether the body is static.

	@in		MOAICpBody self
	@out	boolean static		Whether the body static.
*/
int MOAICpBody::_isStatic ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpBool is = cpBodyIsStatic( self->mBody );
	lua_pushboolean ( state, ( is == cpTrue ));
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isRogue
	@text	Returns whether the body is not yet currently associated with a space.

	@in		MOAICpBody self
	@out	boolean static		Whether the body is not associated with a space.
*/
int MOAICpBody::_isRogue ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpBool is = cpBodyIsRogue( self->mBody );
	lua_pushboolean ( state, ( is == cpTrue ));
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	localToWorld
	@text	Converts the relative position to an absolute position based on position of the object being (0, 0) for the relative position.

	@in		MOAICpBody self
	@in		number rx			The relative X position.
	@in		number ry			The relative Y position.
	@out	number ax			The absolute X position.
	@out	number ay			The absolute Y position.
*/
int MOAICpBody::_localToWorld ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UNN" )
	
	cpVect v;
	v.x = state.GetValue < cpFloat >( 2, 0 );
	v.y = state.GetValue < cpFloat >( 3, 0 );

	v = cpBodyLocal2World ( self->mBody, v );
	
	lua_pushnumber ( state, v.x );
	lua_pushnumber ( state, v.y );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	new
	@text	Creates a new body with the specified mass and moment.

	@in		number m			The mass of the new body.
	@in		number i			The moment of the new body.
	@out	MOAICpBody body		The new body.
*/
int MOAICpBody::_new ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cpFloat m = state.GetValue < cpFloat >( 1, 1 );
	cpFloat i = state.GetValue < cpFloat >( 2, 1 );
	
	MOAICpBody* body = new MOAICpBody ();
	body->mBody = cpBodyNew ( m, i );
	body->mBody->data = body;

	body->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	newStatic
	@text	Creates a new static body.

	@out	MOAICpBody body		The new static body.
*/
int MOAICpBody::_newStatic ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAICpBody* body = new MOAICpBody ();
	body->mBody = cpBodyNewStatic ();
	body->mBody->data = body;

	body->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	resetForces
	@text	Resets all forces on the body.

	@in		MOAICpBody self
*/
int MOAICpBody::_resetForces ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpBodyResetForces( self->mBody );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAngle
	@text	Sets the angle of the body.

	@in		MOAICpBody self
	@in		number angle		The angle of the body.
	@out	nil
*/
int MOAICpBody::_setAngle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UN" )
	
	cpFloat a = state.GetValue < cpFloat >( 2, 0 );
	cpBodySetAngle( self->mBody, a );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAngVel
	@text	Sets the angular velocity of the body.

	@in		MOAICpBody self
	@in		number angvel		The angular velocity of the body.
	@out	nil
*/
int MOAICpBody::_setAngVel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UN" )
	
	cpFloat av = state.GetValue < cpFloat >( 2, 0 );
	cpBodySetAngVel( self->mBody, av );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setForce
	@text	Sets the force on the body.

	@in		MOAICpBody self
	@in		number forcex		The X force being applied to the body.
	@in		number forcey		The Y force being applied to the body.
	@out	nil
*/
int MOAICpBody::_setForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UNN" )
	
	cpVect f;
	f.x = state.GetValue < cpFloat >( 2, 0 );
	f.y = state.GetValue < cpFloat >( 3, 0 );
	cpBodySetForce( self->mBody, f );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMass
	@text	Sets the mass of the body.

	@in		MOAICpBody self
	@in		number mass			The mass of the body.
	@out	nil
*/
int MOAICpBody::_setMass ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UN" )
	
	cpFloat m = state.GetValue < cpFloat >( 2, 0 );
	cpBodySetMass ( self->mBody, m );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMoment
	@text	Sets the moment of the body.

	@in		MOAICpBody self
	@in		number moment		The moment of the body.
	@out	nil
*/
int MOAICpBody::_setMoment ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UN" )
	
	cpFloat i = state.GetValue < cpFloat >( 2, 0 );
	cpBodySetMoment ( self->mBody, i );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPos
	@text	Sets the position of the body.

	@in		MOAICpBody self
	@in		number x			The X position of the body.
	@in		number y			The Y position of the body.
	@out	nil
*/
int MOAICpBody::_setPos ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UNN" )
	
	cpVect p;
	p.x = state.GetValue < cpFloat >( 2, 0 );
	p.y = state.GetValue < cpFloat >( 3, 0 );
	cpBodySetPos( self->mBody, p );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRemoveFlag
	@text	Sets the removal flag on the body.

	@in		MOAICpBody self
	@in		number flag			The removal flag.
	@out	nil
*/
int MOAICpBody::_setRemoveFlag ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UN" )
	
	self->mRemoveFlag = state.GetValue < u32 >( 2, NONE );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTorque
	@text	Sets the torque of the body.

	@in		MOAICpBody self
	@in		number torque		The torque of the body.
	@out	nil
*/
int MOAICpBody::_setTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UN" )
	
	cpFloat t = state.GetValue < cpFloat >( state, 2 );
	cpBodySetTorque ( self->mBody, t );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVel
	@text	Sets the velocity of the body.

	@in		MOAICpBody self
	@in		number x			The horizontal velocity.
	@in		number y			The vertical velocity.
	@out	nil
*/
int MOAICpBody::_setVel ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UNN" )
	
	cpVect v;
	v.x = state.GetValue < cpFloat >( 2, 0 );
	v.y = state.GetValue < cpFloat >( 3, 0 );
	cpBodySetVel( self->mBody, v );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	sleep
	@text	Puts the body to sleep (physics will no longer be processed for it until it is activated).

	@in		MOAICpBody self
	@out	nil
*/
int MOAICpBody::_sleep ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "U" )
	
	cpBodySleep ( self->mBody );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	sleepWithGroup
	@text	Forces an object to sleep. Pass in another sleeping body
			to add the object to the sleeping body's existing group.

	@in		MOAICpBody self
	@in		MOAICpBody group
	@out	nil
*/
int MOAICpBody::_sleepWithGroup ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UU" )
	
	MOAICpBody* group = state.GetLuaObject < MOAICpBody >( 2, true );
	if ( group && group->mBody ) {
		cpBodySleepWithGroup ( self->mBody, group->mBody );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	worldToLocal
	@text	Converts the absolute position to a relative position based on
			position of the object being (0, 0) for the relative position.

	@in		MOAICpBody self
	@in		number ax			The absolute X position.
	@in		number ay			The absolute Y position.
	@out	number rx			The relative X position.
	@out	number ry			The relative Y position.
*/
int MOAICpBody::_worldToLocal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpBody, "UNN" )
	
	cpVect v;
	v.x = state.GetValue < cpFloat >( 2, 0 );
	v.y = state.GetValue < cpFloat >( 3, 0 );

	v = cpBodyWorld2Local ( self->mBody, v );
	
	lua_pushnumber ( state, v.x );
	lua_pushnumber ( state, v.y );
	
	return 2;
}

//================================================================//
// MOAICpBody
//================================================================//

//----------------------------------------------------------------//
void MOAICpBody::AttachShape ( MOAICpShape& shape ) {

	shape.Retain ();
	shape.mShape->body = this->mBody;
	this->mShapes.PushBack ( shape.mLinkInBody );
}

//----------------------------------------------------------------//
void MOAICpBody::ClearShapes () {

	// release all the shapes associated with this body
	ShapeIt shapeIt = this->mShapes.Head ();
	while ( shapeIt ) {
		MOAICpShape* shape = shapeIt->Data ();
		shapeIt = shapeIt->Next ();
		
		shape->Remove ();
		shape->Release ();
	}
	this->mShapes.Clear ();
}

//----------------------------------------------------------------//
void MOAICpBody::CpAddToSpace ( cpSpace* space ) {

	this->mRemoveFlag = NONE;

	assert ( space );
	assert ( this->mBody );
	cpSpaceAddBody ( space, this->mBody );
}

//----------------------------------------------------------------//
void MOAICpBody::CpRemoveFromSpace ( cpSpace* space ) {

	assert ( space );
	assert ( this->mBody );
	cpSpaceRemoveBody ( space, this->mBody );
}

//----------------------------------------------------------------//
void MOAICpBody::DoRemove () {

	if ( this->mRemoveFlag == REMOVE_BODY ) {
		this->Remove ();
	}
	
	if ( this->mRemoveFlag == REMOVE_BODY_AND_SHAPES ) {
		
		ShapeIt shapeIt = this->mShapes.Head ();
		for ( ; shapeIt ; shapeIt = shapeIt->Next () ) {
			MOAICpShape* shape = shapeIt->Data ();
			shape->Remove ();
		}
		this->Remove ();
	}
	
	this->mRemoveFlag = NONE;
}

//----------------------------------------------------------------//
MOAICpBody::MOAICpBody () :
	mBody ( 0 ),
	mRemoveFlag ( NONE ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransformBase )
		RTTI_EXTEND ( MOAICpPrim )
	RTTI_END
}

//----------------------------------------------------------------//
MOAICpBody::~MOAICpBody () {

	this->ClearShapes ();
	
	if ( this->mBody ) {
		// and free the body itself
		cpBodyFree ( this->mBody );
	}
}

//----------------------------------------------------------------//
void MOAICpBody::OnDepNodeUpdate () {

	if ( this->mBody ) {
		
		cpVect pos = cpBodyGetPos ( this->mBody );
		cpVect rot = cpBodyGetRot ( this->mBody );
		
		float* m = this->mLocalToWorldMtx.m;
		
		m [ USAffine3D::C0_R0 ] = ( float )rot.x;
		m [ USAffine3D::C0_R1 ] = ( float )rot.y;
		m [ USAffine3D::C0_R2 ] = 0.0f;

		m [ USAffine3D::C1_R0 ] = -( float )rot.y;
		m [ USAffine3D::C1_R1 ] = ( float )rot.x;
		m [ USAffine3D::C1_R2 ] = 0.0f;

		m [ USAffine3D::C2_R0 ] = 0.0f;
		m [ USAffine3D::C2_R1 ] = 0.0f;
		m [ USAffine3D::C2_R2 ] = 1.0f;

		m [ USAffine3D::C3_R0 ] = ( float )pos.x;
		m [ USAffine3D::C3_R1 ] = ( float )pos.y;
		m [ USAffine3D::C3_R2 ] = 0.0f;
		
		this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
	}
}

//----------------------------------------------------------------//
void MOAICpBody::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITransformBase::RegisterLuaClass ( state );

	state.SetField ( -1, "NONE", ( u32 )NONE );
	state.SetField ( -1, "REMOVE_BODY", ( u32 )REMOVE_BODY );
	state.SetField ( -1, "REMOVE_BODY_AND_SHAPES", ( u32 )REMOVE_BODY_AND_SHAPES );

	luaL_Reg regTable [] = {
		{ "new",				_new },
		{ "newStatic",			_newStatic },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICpBody::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITransformBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "activate",			_activate },
		{ "addCircle",			_addCircle },
		{ "addPolygon",			_addPolygon },
		{ "addRect",			_addRect },
		{ "addSegment",			_addSegment },
		{ "applyForce",			_applyForce },
		{ "applyImpulse",		_applyImpulse },
		{ "getAngle",			_getAngle },
		{ "getAngVel",			_getAngVel },
		{ "getForce",			_getForce },
		{ "getMass",			_getMass },
		{ "getMoment",			_getMoment },
		{ "getPos",				_getPos },
		{ "getRot",				_getRot },
		{ "getTorque",			_getTorque },
		{ "getVel",				_getVel },
		{ "isSleeping",			_isSleeping },
		{ "isStatic",			_isStatic },
		{ "isRogue",			_isRogue },
		{ "localToWorld",		_localToWorld },
		{ "resetForces",		_resetForces },
		{ "setAngle",			_setAngle },
		{ "setAngVel",			_setAngVel },
		{ "setForce",			_setForce },
		{ "setMass",			_setMass },
		{ "setMoment",			_setMoment },
		{ "setPos",				_setPos },
		{ "setRemoveFlag",		_setRemoveFlag },
		{ "setTorque",			_setTorque },
		{ "setVel",				_setVel },
		{ "sleep",				_sleep },
		{ "sleepWithGroup",		_sleepWithGroup },
		{ "worldToLocal",		_worldToLocal },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
