// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DFixture.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	countContacts
	@text	Return the number of contacts in the current collision.
*/
int MOAIBox2DArbiter::_countContacts ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DArbiter, "U" )

	// TODO: Box2D
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DArbiter::_getContactDepth ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DArbiter, "UN" )

	// TODO: Box2D
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DArbiter::_getContactNormal ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DArbiter, "UN" )

	// TODO: Box2D
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DArbiter::_getContactPoint ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DArbiter, "UN" )

	// TODO: Box2D
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DArbiter::_getTotalImpulse ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DArbiter, "U" )

	// TODO: Box2D
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DArbiter::_getTotalImpulseWithFriction ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DArbiter, "U" )

	// TODO: Box2D
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DArbiter::_isFirstContact ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DArbiter, "U" )

	// TODO: Box2D
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DArbiter::_new ( lua_State* L ) {
	UNUSED ( L );
	
	printf ( "MOAIBox2DArbiter.new () is unsupported.\n" );
	return 0;
}

//================================================================//
// MOAIBox2DArbiter
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DArbiter::BeginContact ( b2Contact* contact ) {
	
	this->mContact = contact;
	this->mImpulse = 0;
	
	b2Fixture* fixtureA = contact->GetFixtureA ();
	b2Fixture* fixtureB = contact->GetFixtureB ();
	
	MOAIBox2DFixture* moaiFixtureA = ( MOAIBox2DFixture* )fixtureA->GetUserData ();
	MOAIBox2DFixture* moaiFixtureB = ( MOAIBox2DFixture* )fixtureB->GetUserData ();
	
	moaiFixtureA->BeginContact ( moaiFixtureB, this );
	moaiFixtureB->BeginContact ( moaiFixtureA, this );
}

//----------------------------------------------------------------//
void MOAIBox2DArbiter::EndContact ( b2Contact* contact ) {
	
	this->mContact = contact;
	this->mImpulse = 0;
	
	b2Fixture* fixtureA = contact->GetFixtureA ();
	b2Fixture* fixtureB = contact->GetFixtureB ();
	
	MOAIBox2DFixture* moaiFixtureA = ( MOAIBox2DFixture* )fixtureA->GetUserData ();
	MOAIBox2DFixture* moaiFixtureB = ( MOAIBox2DFixture* )fixtureB->GetUserData ();
	
	moaiFixtureA->EndContact ( moaiFixtureB, this );
	moaiFixtureB->EndContact ( moaiFixtureA, this );
}

//----------------------------------------------------------------//
MOAIBox2DArbiter::MOAIBox2DArbiter () :
	mContact ( 0 ),
	mImpulse ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaObject )
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
	
	moaiFixtureA->PostSolve ( moaiFixtureB, this );
	moaiFixtureB->PostSolve ( moaiFixtureA, this );
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
	
	moaiFixtureA->PreSolve ( moaiFixtureB, this );
	moaiFixtureB->PreSolve ( moaiFixtureA, this );
}

//----------------------------------------------------------------//
void MOAIBox2DArbiter::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "ALL", ( u32 )ALL );
	state.SetField ( -1, "BEGIN", ( u32 )BEGIN );
	state.SetField ( -1, "END", ( u32 )END );
	state.SetField ( -1, "PRE_SOLVE", ( u32 )PRE_SOLVE );
	state.SetField ( -1, "POST_SOLVE", ( u32 )POST_SOLVE );
	
	luaL_Reg regTable [] = {
		{ "new",							_new },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBox2DArbiter::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "countContacts",					_countContacts },
		{ "getContactDepth",				_getContactDepth },
		{ "getContactNormal",				_getContactNormal },
		{ "getContactPoint",				_getContactPoint },
		{ "getTotalImpulse",				_getTotalImpulse },
		{ "getTotalImpulseWithFriction",	_getTotalImpulseWithFriction },
		{ "isFirstContact",					_isFirstContact },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIBox2DArbiter::ToString () {

	STLString repr;
	return repr;
}

#endif