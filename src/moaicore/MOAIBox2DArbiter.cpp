// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DFixture.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getContactNormal
 @text	Returns the normal for the contact.
 
 @in	MOAIBox2DArbiter self
 @out	number normal.x
 @out	number normal.y
 */
int MOAIBox2DArbiter::_getContactNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DArbiter, "U" )
	
	state.Push ( self->mContactNormal.x );
	state.Push ( self->mContactNormal.y );
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getNormalImpulse
	@text	Returns total normal impulse for contact.
	
	@in		MOAIBox2DArbiter self
	@out	number impulse in kg * units / s converted from kg * m / s
*/
int MOAIBox2DArbiter::_getNormalImpulse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DArbiter, "U" )

	float impulse = self->mNormalImpulse;
	const float metersToUnits = 1 / self->GetUnitsToMeters();
	impulse = impulse * metersToUnits;

	state.Push ( impulse );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getTangentImpulse
	@text	Returns total tangent impulse for contact.
	
	@in		MOAIBox2DArbiter self
	@out	number impulse in kg * units / s converted from kg * m / s
*/
int MOAIBox2DArbiter::_getTangentImpulse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DArbiter, "U" )

	float impulse = self->mTangentImpulse;
	const float metersToUnits = 1 / self->GetUnitsToMeters();
	impulse = impulse * metersToUnits;

	state.Push ( impulse );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setContactEnabled
	@text	Enabled or disable the contact.
	
	@in		MOAIBox2DArbiter self
	@out	number impulse
*/
int MOAIBox2DArbiter::_setContactEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DArbiter, "U" )
	
	bool enabled = state.GetValue < bool >( 2, false );
	self->mContact->SetEnabled ( enabled );
	
	return 0;
}

//================================================================//
// MOAIBox2DArbiter
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DArbiter::BeginContact ( b2Contact* contact ) {
	
	this->mContact = contact;
	this->mImpulse = 0;
	
	this->mContactNormal = b2Vec2();
	this->mNormalImpulse = 0.0f;
	this->mTangentImpulse = 0.0f;
	
	b2Fixture* fixtureA = contact->GetFixtureA ();
	b2Fixture* fixtureB = contact->GetFixtureB ();
	
	MOAIBox2DFixture* moaiFixtureA = ( MOAIBox2DFixture* )fixtureA->GetUserData ();
	MOAIBox2DFixture* moaiFixtureB = ( MOAIBox2DFixture* )fixtureB->GetUserData ();
	
	moaiFixtureA->HandleCollision ( BEGIN, moaiFixtureB, this );
	moaiFixtureB->HandleCollision ( BEGIN, moaiFixtureA, this );
}

//----------------------------------------------------------------//
void MOAIBox2DArbiter::EndContact ( b2Contact* contact ) {
	
	this->mContact = contact;
	this->mImpulse = 0;
	
	b2Fixture* fixtureA = contact->GetFixtureA ();
	b2Fixture* fixtureB = contact->GetFixtureB ();
	
	MOAIBox2DFixture* moaiFixtureA = ( MOAIBox2DFixture* )fixtureA->GetUserData ();
	MOAIBox2DFixture* moaiFixtureB = ( MOAIBox2DFixture* )fixtureB->GetUserData ();
	
	moaiFixtureA->HandleCollision ( END, moaiFixtureB, this );
	moaiFixtureB->HandleCollision ( END, moaiFixtureA, this );
}

//----------------------------------------------------------------//
float MOAIBox2DArbiter::GetUnitsToMeters ( ) const {
	if (this->mWorld) {
		return this->mWorld->GetUnitsToMeters();
	} else {
		return 1.0f;
	}
}

//----------------------------------------------------------------//
MOAIBox2DArbiter::MOAIBox2DArbiter ( ) :
	mContact ( 0 ),
	mImpulse ( 0 ),
	mWorld ( NULL ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DArbiter::MOAIBox2DArbiter ( const MOAIBox2DWorld& world ) :
	mContact ( 0 ),
	mImpulse ( 0 ),
	mWorld ( &world ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DArbiter::~MOAIBox2DArbiter () {
}

//----------------------------------------------------------------//
void MOAIBox2DArbiter::PostSolve ( b2Contact* contact, const b2ContactImpulse* impulse ) {

	this->mContact = contact;
	this->mImpulse = impulse;

	b2Fixture* fixtureA = contact->GetFixtureA ();
	b2Fixture* fixtureB = contact->GetFixtureB ();
	
	MOAIBox2DFixture* moaiFixtureA = ( MOAIBox2DFixture* )fixtureA->GetUserData ();
	MOAIBox2DFixture* moaiFixtureB = ( MOAIBox2DFixture* )fixtureB->GetUserData ();
		
	b2WorldManifold* worldManifold = new b2WorldManifold ();
	contact->GetWorldManifold ( worldManifold );
	this->mContactNormal = worldManifold->normal;
	delete worldManifold;
	
	b2Manifold* manifold = contact->GetManifold ();
	u32 totalPoints = manifold->pointCount;
	
	this->mNormalImpulse = 0.0f;
	this->mTangentImpulse = 0.0f;
	
	for ( u32 i = 0; i < totalPoints; ++i ) {
		this->mNormalImpulse += impulse->normalImpulses [ i ];
		this->mTangentImpulse += impulse->tangentImpulses [ i ];
	}
	
	moaiFixtureA->HandleCollision ( POST_SOLVE, moaiFixtureB, this );
	moaiFixtureB->HandleCollision ( POST_SOLVE, moaiFixtureA, this );
}

//----------------------------------------------------------------//
void MOAIBox2DArbiter::PreSolve ( b2Contact* contact, const b2Manifold* oldManifold ) {
	UNUSED ( oldManifold );
	
	this->mContact = contact;
	this->mImpulse = 0;
	
	b2Fixture* fixtureA = contact->GetFixtureA ();
	b2Fixture* fixtureB = contact->GetFixtureB ();
	
	MOAIBox2DFixture* moaiFixtureA = ( MOAIBox2DFixture* )fixtureA->GetUserData ();
	MOAIBox2DFixture* moaiFixtureB = ( MOAIBox2DFixture* )fixtureB->GetUserData ();
	
	moaiFixtureA->HandleCollision ( PRE_SOLVE, moaiFixtureB, this );
	moaiFixtureB->HandleCollision ( PRE_SOLVE, moaiFixtureA, this );
}

//----------------------------------------------------------------//
void MOAIBox2DArbiter::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "ALL", ( u32 )ALL );
	state.SetField ( -1, "BEGIN", ( u32 )BEGIN );
	state.SetField ( -1, "END", ( u32 )END );
	state.SetField ( -1, "PRE_SOLVE", ( u32 )PRE_SOLVE );
	state.SetField ( -1, "POST_SOLVE", ( u32 )POST_SOLVE );
	
	luaL_Reg regTable [] = {
		{ "new",							MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBox2DArbiter::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getContactNormal",			_getContactNormal },
		{ "getNormalImpulse",			_getNormalImpulse },
		{ "getTangentImpulse",			_getTangentImpulse },
		{ "setContactEnabled",			_setContactEnabled },
		{ "new",						MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif