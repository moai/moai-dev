// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <chipmunk/chipmunk.h>
#include <moaicore/MOAICpBody.h>
#include <moaicore/MOAICpConstraint.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_CHIPMUNK

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_getBiasCoef ( lua_State* L ) {
	LUA_SETUP ( MOAICpConstraint, "U" )
	
	if ( self->mConstraint ) {
		lua_pushnumber ( L, self->mConstraint->biasCoef );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_getMaxBias ( lua_State* L ) {
	LUA_SETUP ( MOAICpConstraint, "U" )
	
	if ( self->mConstraint ) {
		lua_pushnumber ( L, self->mConstraint->maxBias );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_getMaxForce ( lua_State* L ) {
	LUA_SETUP ( MOAICpConstraint, "U" )
	
	if ( self->mConstraint ) {
		lua_pushnumber ( L, self->mConstraint->maxForce );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_newDampedRotarySpring ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaData < MOAICpBody >( 1 );
	MOAICpBody* b = state.GetLuaData < MOAICpBody >( 2 );
	
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_newDampedSpring ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNNNNNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaData < MOAICpBody >( 1 );
	MOAICpBody* b = state.GetLuaData < MOAICpBody >( 2 );
	
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_newGearJoint ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaData < MOAICpBody >( 1 );
	MOAICpBody* b = state.GetLuaData < MOAICpBody >( 2 );
	
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_newGrooveJoint ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNNNNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaData < MOAICpBody >( 1 );
	MOAICpBody* b = state.GetLuaData < MOAICpBody >( 2 );
	
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_newPinJoint ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaData < MOAICpBody >( 1 );
	MOAICpBody* b = state.GetLuaData < MOAICpBody >( 2 );
	
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_newPivotJoint ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaData < MOAICpBody >( 1 );
	MOAICpBody* b = state.GetLuaData < MOAICpBody >( 2 );
	
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_newRatchetJoint ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaData < MOAICpBody >( 1 );
	MOAICpBody* b = state.GetLuaData < MOAICpBody >( 2 );
	
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_newRotaryLimitJoint ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaData < MOAICpBody >( 1 );
	MOAICpBody* b = state.GetLuaData < MOAICpBody >( 2 );
	
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_newSimpleMotor ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UUN" )) return 0;
	
	MOAICpBody* a = state.GetLuaData < MOAICpBody >( 1 );
	MOAICpBody* b = state.GetLuaData < MOAICpBody >( 2 );
	
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_newSlideJoint ( lua_State* L ) {
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UUNNNNNN" )) return 0;
	
	MOAICpBody* a = state.GetLuaData < MOAICpBody >( 1 );
	MOAICpBody* b = state.GetLuaData < MOAICpBody >( 2 );
	
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_setBiasCoef ( lua_State* L ) {
	LUA_SETUP ( MOAICpConstraint, "UN" )
	
	if ( self->mConstraint ) {
		self->mConstraint->biasCoef = state.GetValue < cpFloat >( 2, self->mConstraint->biasCoef );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_setMaxBias ( lua_State* L ) {
	LUA_SETUP ( MOAICpConstraint, "UN" )
	
	if ( self->mConstraint ) {
		self->mConstraint->maxBias = state.GetValue < cpFloat >( 2, self->mConstraint->maxBias );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAICpConstraint::_setMaxForce ( lua_State* L ) {
	LUA_SETUP ( MOAICpConstraint, "UN" )
	
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
void MOAICpConstraint::RegisterLuaClass ( USLuaState& state ) {

	LuaReg regTable [] = {
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
void MOAICpConstraint::RegisterLuaFuncs ( USLuaState& state ) {
	
	LuaReg regTable [] = {
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

//----------------------------------------------------------------//
STLString MOAICpConstraint::ToString () {

	STLString repr;
	return repr;
}

#endif