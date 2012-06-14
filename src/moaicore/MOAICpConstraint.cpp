// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICpBody.h>
#include <moaicore/MOAICpConstraint.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_CHIPMUNK

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getBiasCoef
	@text	Returns the current bias coefficient.

	@in		MOAICpConstraint self
	@out	number bias			The bias coefficient.
*/
int MOAICpConstraint::_getBiasCoef ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpConstraint, "U" )
	
	if ( self->mConstraint ) {
		lua_pushnumber ( L, self->mConstraint->biasCoef );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getMaxBias
	@text	Returns the maximum bias coefficient.

	@in		MOAICpConstraint self
	@out	number bias			The maximum bias coefficient.
*/
int MOAICpConstraint::_getMaxBias ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpConstraint, "U" )
	
	if ( self->mConstraint ) {
		lua_pushnumber ( L, self->mConstraint->maxBias );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getMaxForce
	@text	Returns the maximum force allowed.

	@in		MOAICpConstraint self
	@out	number bias			The maximum force allowed.
*/
int MOAICpConstraint::_getMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpConstraint, "U" )
	
	if ( self->mConstraint ) {
		lua_pushnumber ( L, self->mConstraint->maxForce );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	newDampedRotarySpring
	@text	Creates a new damped rotary string between the two specified bodies.

	@in		MOAICpShape first
	@in		MOAICpShape second
	@in		number restAngle			The angle at which the spring is at rest.
	@in		number stiffness			The stiffness of the spring.
	@in		number damping				The damping applied to the spring.
	@out	MOAICpConstraint spring		The new spring.
*/
int MOAICpConstraint::_newDampedRotarySpring ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaObject < MOAICpBody >( 1, true );
	MOAICpBody* b = state.GetLuaObject < MOAICpBody >( 2, true );
	
	if ( !( a && b )) return 0;
	
	a->Retain ();
	b->Retain ();
		
	cpFloat restAngle	= state.GetValue < cpFloat >( 3, 0 );
	cpFloat stiffness	= state.GetValue < cpFloat >( 4, 0 );
	cpFloat damping		= state.GetValue < cpFloat >( 5, 0 );
		
	MOAICpConstraint* constraint = new MOAICpConstraint ();
	constraint->mConstraint = cpDampedRotarySpringNew ( a->mBody, b->mBody, restAngle, stiffness, damping );
	constraint->mConstraint->data = constraint;
		
	constraint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	newDampedSpring
	@text	Creates a new damped string between the two specified bodies.

	@in		MOAICpShape first
	@in		MOAICpShape second
	@in		number x1					The X position of the first anchor.
	@in		number y1					The Y position of the first anchor.
	@in		number x2					The X position of the second anchor.
	@in		number y2					The Y position of the second anchor.
	@in		number restAngle			The angle at which the spring is at rest.
	@in		number stiffness			The stiffness of the spring.
	@in		number damping				The damping applied to the spring.
	@out	MOAICpConstraint spring		The new spring.
*/
int MOAICpConstraint::_newDampedSpring ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNNNNNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaObject < MOAICpBody >( 1, true );
	MOAICpBody* b = state.GetLuaObject < MOAICpBody >( 2, true );
	
	if ( !( a && b )) return 0;
	
	a->Retain ();
	b->Retain ();
		
	cpVect anchr1;
	anchr1.x	= state.GetValue < cpFloat >( 3, 0 );
	anchr1.y	= state.GetValue < cpFloat >( 4, 0 );

	cpVect anchr2;
	anchr2.x	= state.GetValue < cpFloat >( 5, 0 );
	anchr2.y	= state.GetValue < cpFloat >( 6, 0 );
	
	cpFloat restLength	= state.GetValue < cpFloat >( 7, 0 );
	cpFloat stiffness	= state.GetValue < cpFloat >( 8, 0 );
	cpFloat damping		= state.GetValue < cpFloat >( 9, 0 );
		
	MOAICpConstraint* constraint = new MOAICpConstraint ();
	constraint->mConstraint = cpDampedSpringNew ( a->mBody, b->mBody, anchr1, anchr2, restLength, stiffness, damping );
	constraint->mConstraint->data = constraint;
		
	constraint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	newGearJoint
	@text	Creates a new gear joint between the two specified bodies.

	@in		MOAICpShape first
	@in		MOAICpShape second
	@in		number phase				The phase of the gear.
	@in		number ratio				The gear ratio.
	@out	MOAICpConstraint gear		The new gear joint.
*/
int MOAICpConstraint::_newGearJoint ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaObject < MOAICpBody >( 1, true );
	MOAICpBody* b = state.GetLuaObject < MOAICpBody >( 2, true );
	
	if ( !( a && b )) return 0;
	
	a->Retain ();
	b->Retain ();
		
	cpFloat phase	= state.GetValue < cpFloat >( 3, 0 );
	cpFloat ratio	= state.GetValue < cpFloat >( 4, 0 );

	MOAICpConstraint* constraint = new MOAICpConstraint ();
	constraint->mConstraint = cpGearJointNew ( a->mBody, b->mBody, phase, ratio );
	constraint->mConstraint->data = constraint;
		
	constraint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	newGrooveJoint
	@text	Creates a new groove joint between the two specified bodies.

	@in		MOAICpShape first
	@in		MOAICpShape second
	@in		number gx1
	@in		number gy1
	@in		number gx2
	@in		number gy2
	@in		number ax
	@in		number ay
	@out	MOAICpConstraint groove		The new groove joint.
*/
int MOAICpConstraint::_newGrooveJoint ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNNNNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaObject < MOAICpBody >( 1, true );
	MOAICpBody* b = state.GetLuaObject < MOAICpBody >( 2, true );
	
	if ( !( a && b )) return 0;
	
	a->Retain ();
	b->Retain ();
		
	cpVect groove_a;
	groove_a.x	= state.GetValue < cpFloat >( 3, 0 );
	groove_a.y	= state.GetValue < cpFloat >( 4, 0 );
	
	cpVect groove_b;
	groove_b.x	= state.GetValue < cpFloat >( 5, 0 );
	groove_b.y	= state.GetValue < cpFloat >( 6, 0 );
	
	cpVect anchr2;
	anchr2.x	= state.GetValue < cpFloat >( 7, 0 );
	anchr2.y	= state.GetValue < cpFloat >( 8, 0 );
	
	MOAICpConstraint* constraint = new MOAICpConstraint ();
	constraint->mConstraint = cpGrooveJointNew ( a->mBody, b->mBody, groove_a, groove_b, anchr2 );
	constraint->mConstraint->data = constraint;
		
	constraint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	newPinJoint
	@text	Creates a new pin joint between the two specified bodies.

	@in		MOAICpShape first
	@in		MOAICpShape second
	@in		number ax1
	@in		number ay1
	@in		number ax2
	@in		number ay2
	@out	MOAICpConstraint pin		The new pin joint.
*/
int MOAICpConstraint::_newPinJoint ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaObject < MOAICpBody >( 1, true );
	MOAICpBody* b = state.GetLuaObject < MOAICpBody >( 2, true );
	
	if ( !( a && b )) return 0;
	
	a->Retain ();
	b->Retain ();
		
	cpVect anchr1;
	anchr1.x	= state.GetValue < cpFloat >( 3, 0 );
	anchr1.y	= state.GetValue < cpFloat >( 4, 0 );

	cpVect anchr2;
	anchr2.x	= state.GetValue < cpFloat >( 5, 0 );
	anchr2.y	= state.GetValue < cpFloat >( 6, 0 );
		
	MOAICpConstraint* constraint = new MOAICpConstraint ();
	constraint->mConstraint = cpPinJointNew ( a->mBody, b->mBody, anchr1, anchr2 );
	constraint->mConstraint->data = constraint;
		
	constraint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	newPivotJoint
	@text	Creates a new pivot joint between the two specified bodies.

	@in		MOAICpShape first
	@in		MOAICpShape second
	@in		number x
	@in		number y
	@opt	number ax
	@opt	number ay
	@out	MOAICpConstraint pivot		The new pivot joint.
*/
int MOAICpConstraint::_newPivotJoint ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaObject < MOAICpBody >( 1, true );
	MOAICpBody* b = state.GetLuaObject < MOAICpBody >( 2, true );
	
	if ( !( a && b )) return 0;
	
	a->Retain ();
	b->Retain ();
	
	cpVect pivot;
	pivot.x		= state.GetValue < cpFloat >( 3, 0 );
	pivot.y		= state.GetValue < cpFloat >( 4, 0 );
	
	MOAICpConstraint* constraint = new MOAICpConstraint ();
	
	if ( state.CheckParams ( 5, "NN" )) {
		
		cpVect anchr2;
		anchr2.x	= state.GetValue < cpFloat >( 5, 0 );
		anchr2.y	= state.GetValue < cpFloat >( 6, 0 );
		
		constraint->mConstraint = cpPivotJointNew2 ( a->mBody, b->mBody, pivot, anchr2 );	
	}
	else {
		
		constraint->mConstraint = cpPivotJointNew ( a->mBody, b->mBody, pivot );
	}
	
	constraint->mConstraint->data = constraint;
	constraint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	newRatchetJoint
	@text	Creates a new ratchet joint between the two specified bodies.

	@in		MOAICpShape first
	@in		MOAICpShape second
	@in		number phase				The phase of the gear.
	@in		number ratchet				The ratchet value.
	@out	MOAICpConstraint ratchet	The new pivot joint.
*/
int MOAICpConstraint::_newRatchetJoint ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaObject < MOAICpBody >( 1, true );
	MOAICpBody* b = state.GetLuaObject < MOAICpBody >( 2, true );
	
	if ( !( a && b )) return 0;
	
	a->Retain ();
	b->Retain ();
	
	cpFloat phase	= state.GetValue < cpFloat >( 3, 0 );
	cpFloat ratchet	= state.GetValue < cpFloat >( 4, 0 );
	
	MOAICpConstraint* constraint = new MOAICpConstraint ();
	constraint->mConstraint = cpRatchetJointNew ( a->mBody, b->mBody, phase, ratchet );
	constraint->mConstraint->data = constraint;
		
	constraint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	newRotaryLimitJoint
	@text	Creates a new rotary limit joint between the two specified bodies.

	@in		MOAICpShape first
	@in		MOAICpShape second
	@in		number min					The minimum rotary value.
	@in		number max					The maximum rotary value.
	@out	MOAICpConstraint limit		The new rotary limit joint.
*/
int MOAICpConstraint::_newRotaryLimitJoint ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaObject < MOAICpBody >( 1, true );
	MOAICpBody* b = state.GetLuaObject < MOAICpBody >( 2, true );
	
	if ( !( a && b )) return 0;
	
	a->Retain ();
	b->Retain ();
	
	cpFloat min	= state.GetValue < cpFloat >( 3, 0 );
	cpFloat max	= state.GetValue < cpFloat >( 4, 0 );
	
	MOAICpConstraint* constraint = new MOAICpConstraint ();
	constraint->mConstraint = cpRotaryLimitJointNew ( a->mBody, b->mBody, min, max );
	constraint->mConstraint->data = constraint;
		
	constraint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	newSimpleMotor
	@text	Creates a new simple motor joint between the two specified bodies.

	@in		MOAICpShape first
	@in		MOAICpShape second
	@in		number rate					The rotation rate of the simple motor.
	@out	MOAICpConstraint motor		The new simple motor joint.
*/
int MOAICpConstraint::_newSimpleMotor ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUN" )) return 0;
	
	MOAICpBody* a = state.GetLuaObject < MOAICpBody >( 1, true );
	MOAICpBody* b = state.GetLuaObject < MOAICpBody >( 2, true );
	
	if ( !( a && b )) return 0;
	
	a->Retain ();
	b->Retain ();
		
	cpFloat rate = state.GetValue < cpFloat >( 3, 0 );
	
	MOAICpConstraint* constraint = new MOAICpConstraint ();
	constraint->mConstraint = cpSimpleMotorNew ( a->mBody, b->mBody, rate );
	constraint->mConstraint->data = constraint;
		
	constraint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	newSlideJoint
	@text	Creates a new slide joint between the two specified bodies.

	@in		MOAICpShape first
	@in		MOAICpShape second
	@in		number ax1
	@in		number ay1
	@in		number ax2
	@in		number ay2
	@in		number min
	@in		number max
	@out	MOAICpConstraint motor		The new slide joint.
*/
int MOAICpConstraint::_newSlideJoint ( lua_State* L ) {
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNNNNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaObject < MOAICpBody >( 1, true );
	MOAICpBody* b = state.GetLuaObject < MOAICpBody >( 2, true );
	
	if ( !( a && b )) return 0;
	
	a->Retain ();
	b->Retain ();
		
	cpVect anchr1;
	anchr1.x	= state.GetValue < cpFloat >( 3, 0 );
	anchr1.y	= state.GetValue < cpFloat >( 4, 0 );

	cpVect anchr2;
	anchr2.x	= state.GetValue < cpFloat >( 5, 0 );
	anchr2.y	= state.GetValue < cpFloat >( 6, 0 );
	
	cpFloat min	= state.GetValue < cpFloat >( 7, 0 );
	cpFloat max	= state.GetValue < cpFloat >( 8, 0 );
		
	MOAICpConstraint* constraint = new MOAICpConstraint ();
	constraint->mConstraint = cpSlideJointNew ( a->mBody, b->mBody, anchr1, anchr2, min, max );
	constraint->mConstraint->data = constraint;
		
	constraint->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setBiasCoef
	@text	Sets the current bias coefficient.

	@in		MOAICpConstraint self
	@in		number bias			The bias coefficient.
	@out	nil
*/
int MOAICpConstraint::_setBiasCoef ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpConstraint, "UN" )
	
	if ( self->mConstraint ) {
		self->mConstraint->biasCoef = state.GetValue < cpFloat >( 2, self->mConstraint->biasCoef );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMaxBias
	@text	Sets the maximum bias coefficient.

	@in		MOAICpConstraint self
	@in		number bias			The maximum bias coefficient.
	@out	nil
*/
int MOAICpConstraint::_setMaxBias ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpConstraint, "UN" )
	
	if ( self->mConstraint ) {
		self->mConstraint->maxBias = state.GetValue < cpFloat >( 2, self->mConstraint->maxBias );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMaxForce
	@text	Sets the maximum force allowed.

	@in		MOAICpConstraint self
	@in		number bias			The maximum force allowed.
	@out	nil
*/
int MOAICpConstraint::_setMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICpConstraint, "UN" )
	
	if ( self->mConstraint ) {
		self->mConstraint->maxForce = state.GetValue < cpFloat >( 2, self->mConstraint->maxForce );
	}
	return 0;
}

//================================================================//
// MOAICpConstraint
//================================================================//

//----------------------------------------------------------------//
void MOAICpConstraint::CpAddToSpace ( cpSpace* space ) {

	assert ( space );
	assert ( this->mConstraint );
	cpSpaceAddConstraint ( space, this->mConstraint );
}

//----------------------------------------------------------------//
void MOAICpConstraint::CpRemoveFromSpace ( cpSpace* space ) {

	assert ( space );
	assert ( this->mConstraint );
	cpSpaceRemoveConstraint ( space, this->mConstraint );
}

//----------------------------------------------------------------//
MOAICpConstraint::MOAICpConstraint () :
	mConstraint ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAICpPrim )
	RTTI_END
}

//----------------------------------------------------------------//
MOAICpConstraint::~MOAICpConstraint () {

	if ( this->mConstraint ) {
		(( MOAICpBody* )this->mConstraint->a->data )->Release ();
		(( MOAICpBody* )this->mConstraint->b->data )->Release ();
		cpConstraintFree ( this->mConstraint );
	}
}

//----------------------------------------------------------------//
void MOAICpConstraint::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "newDampedRotarySpring",		_newDampedRotarySpring },
		{ "newDampedSpring",			_newDampedSpring },
		{ "newGearJoint",				_newGearJoint },
		{ "newGrooveJoint",				_newGrooveJoint },
		{ "newPinJoint",				_newPinJoint },
		{ "newPivotJoint",				_newPivotJoint },
		{ "newRatchetJoint",			_newRatchetJoint },
		{ "newRotaryLimitJoint",		_newRotaryLimitJoint },
		{ "newSimpleMotor",				_newSimpleMotor },
		{ "newSlideJoint",				_newSlideJoint },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICpConstraint::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getBiasCoef",				_getBiasCoef },
		{ "getMaxBias",					_getMaxBias },
		{ "getMaxForce",				_getMaxForce },
		{ "setBiasCoef",				_setBiasCoef },
		{ "setMaxBias",					_setMaxBias },
		{ "setMaxForce",				_setMaxForce },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
