// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DFixture.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getContactNormal
 @text	Returns the normal for the contact.
 
 @in	MOAIBox2DArbiter self
 @out	number normal.x
 @out	number normal.y
 */
int MOAIBox2DArbiter::_getContactNormal ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DArbiter, "U" )
	
	self->AffirmContactData ();
	state.Push ( self->mContactNormal.x );
	state.Push ( self->mContactNormal.y );
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getContactPoints
	@text	Returns the contact points in world space. There can be 0, 1 or 2 points (see box2d manual section 4.11)
	
	@in		MOAIBox2DArbiter self
	@out	number p1.x			in units, world coordinates, converted from meters
	@out	number p1.y			in units, world coordinates, converted from meters
	@out	number p2.x
	@out	number p2.y
*/
int MOAIBox2DArbiter::_getContactPoints ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DArbiter, "U" )
	
	self->AffirmContactData ();

	float u2m = self->GetUnitsToMeters();
	u32 count = 0;
	for ( u32 i = 0; i < self->mTotalPoints; ++i ) {
		state.Push ( self->mContactPoints [ i ].x / u2m );
		state.Push ( self->mContactPoints [ i ].y / u2m );
		count += 2;
	}
	
	return count;
}

//----------------------------------------------------------------//
/**	@lua	getNormalImpulse
	@text	Returns total normal impulse for contact.
	
	@in		MOAIBox2DArbiter self
	@out	number impulse			Impulse in kg * units / s converted from kg * m / s
*/
int MOAIBox2DArbiter::_getNormalImpulse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DArbiter, "U" )

	self->AffirmContactData ();
	float impulse = self->mNormalImpulse;
	const float metersToUnits = 1 / self->GetUnitsToMeters();
	impulse = impulse * metersToUnits;

	state.Push ( impulse );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getTangentImpulse
	@text	Returns total tangent impulse for contact.
	
	@in		MOAIBox2DArbiter self
	@out	number impulse			Impulse in kg * units / s converted from kg * m / s
*/
int MOAIBox2DArbiter::_getTangentImpulse ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DArbiter, "U" )

	self->AffirmContactData ();
	float impulse = self->mTangentImpulse;
	const float metersToUnits = 1 / self->GetUnitsToMeters();
	impulse = impulse * metersToUnits;

	state.Push ( impulse );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setContactEnabled
	@text	Enabled or disable the contact.
	
	@in		MOAIBox2DArbiter self
	@in		boolean enabled
	@out	nil
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
void MOAIBox2DArbiter::AffirmContactData () {
	
	if ( !this->mContactDirty ) {
		return;
	}
	
	this->mNormalImpulse = 0.0f;
	this->mTangentImpulse = 0.0f;
	this->mTotalPoints = 0;
	
	if ( this->mContact ) {
		b2Manifold* manifold = this->mContact->GetManifold ();
		u32 totalPoints = manifold->pointCount;
		
		b2WorldManifold worldManifold;
		this->mContact->GetWorldManifold ( &worldManifold );
		
		this->mContactNormal = worldManifold.normal;
		this->mTotalPoints = totalPoints;
		for ( u32 i = 0; i < totalPoints; ++i ) {
			this->mContactPoints [ i ] = worldManifold.points [ i ];
		}
	}
	else {
		this->mContactNormal = b2Vec2 ();
	}
	
	if ( this->mImpulse ) {
		for ( u32 i = 0; i < this->mTotalPoints; ++i ) {
			this->mNormalImpulse += this->mImpulse->normalImpulses [ i ];
			this->mTangentImpulse += this->mImpulse->tangentImpulses [ i ];
		}
	}
	
	this->mContactDirty = false;
}

//----------------------------------------------------------------//
void MOAIBox2DArbiter::BeginContact ( b2Contact* contact ) {
	
	this->mContact = contact;
	this->mImpulse = 0;
	this->mContactDirty = true;
	
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
	this->mContactDirty = true;
	
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
	this->mContactDirty = true;

	b2Fixture* fixtureA = contact->GetFixtureA ();
	b2Fixture* fixtureB = contact->GetFixtureB ();
	
	MOAIBox2DFixture* moaiFixtureA = ( MOAIBox2DFixture* )fixtureA->GetUserData ();
	MOAIBox2DFixture* moaiFixtureB = ( MOAIBox2DFixture* )fixtureB->GetUserData ();
	
	moaiFixtureA->HandleCollision ( POST_SOLVE, moaiFixtureB, this );
	moaiFixtureB->HandleCollision ( POST_SOLVE, moaiFixtureA, this );
}

//----------------------------------------------------------------//
void MOAIBox2DArbiter::PreSolve ( b2Contact* contact, const b2Manifold* oldManifold ) {
	UNUSED ( oldManifold );
	
	this->mContact = contact;
	this->mImpulse = 0;
	this->mContactDirty = true;
	
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
		{ "getContactPoints",			_getContactPoints },
		{ "getNormalImpulse",			_getNormalImpulse },
		{ "getTangentImpulse",			_getTangentImpulse },
		{ "setContactEnabled",			_setContactEnabled },
		{ "new",						MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
