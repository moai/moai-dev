// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DFixture.h>
#include <moaicore/MOAIBox2DWorld.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

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
int MOAIBox2DBody::_addCircle ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "UNNN" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2CircleShape circleShape;

	circleShape.m_p.x		= state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	circleShape.m_p.y		= state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	circleShape.m_radius	= state.GetValue < float >( 4, 1.0f ) * unitsToMeters;
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	
	MOAIBox2DFixture* fixture = new MOAIBox2DFixture ();
	fixture->SetFixture ( self->mBody->CreateFixture ( &fixtureDef ));
	fixture->SetWorld ( self->mWorld );
	fixture->Retain ();

	fixture->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_addPolygon ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_addRect ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "UNNNN" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	USRect rect = state.GetRect < float >( 2 );
	rect.Bless ();
	
	float hx = rect.Width () * 0.5f * unitsToMeters;
	float hy = rect.Height () * 0.5f * unitsToMeters;
	
	b2Vec2 center;
	center.x = ( rect.mXMin * unitsToMeters ) + hx;
	center.y = ( rect.mYMin * unitsToMeters ) + hy;
	
	b2PolygonShape polygonShape;
	polygonShape.SetAsBox ( hx, hy, center, 0.0f );
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &polygonShape;
	
	MOAIBox2DFixture* fixture = new MOAIBox2DFixture ();
	fixture->SetFixture ( self->mBody->CreateFixture ( &fixtureDef ));
	fixture->SetWorld ( self->mWorld );
	fixture->Retain ();

	fixture->PushLuaUserdata ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_applyAngularImpulse ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "UN" )
	
	float impulse = state.GetValue < float >( 2, 0.0f ) * ( float )D2R;
	self->mBody->ApplyAngularImpulse ( impulse );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_applyForce ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "UNN" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2Vec2 force;
	force.x = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	force.y = state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	
	b2Vec2 point;
	point.x = state.GetValue < float >( 4, 0.0f ) * unitsToMeters;
	point.y = state.GetValue < float >( 5, 0.0f ) * unitsToMeters;
	
	self->mBody->ApplyForce ( force, point );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_applyLinearImpulse ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "UNN" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2Vec2 impulse;
	impulse.x = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	impulse.y = state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	
	b2Vec2 point;
	point.x = state.GetValue < float >( 4, 0.0f ) * unitsToMeters;
	point.y = state.GetValue < float >( 5, 0.0f ) * unitsToMeters;
	
	self->mBody->ApplyLinearImpulse ( impulse, point );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_applyTorque ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	float torque = state.GetValue < float >( 2, 0.0f ) * ( float )D2R;
	self->mBody->ApplyTorque ( torque );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_destroyFixture ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	MOAIBox2DFixture* fixture = state.GetLuaData < MOAIBox2DFixture >( 2 );
	
	if ( self->mBody ) {
		if ( fixture->mFixture ) {
			self->mBody->DestroyFixture ( fixture->mFixture );
			fixture->Release ();
		}
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
int MOAIBox2DBody::_getAngle ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	float angle = self->mBody->GetAngle () * ( float )R2D;
	lua_pushnumber ( state, angle );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_getAngularVelocity ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	float omega = self->mBody->GetAngularVelocity () * ( float )R2D;
	lua_pushnumber ( state, omega );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_getLinearVelocity ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2Vec2 velocity = self->mBody->GetLinearVelocity ();
	lua_pushnumber ( state, velocity.x / unitsToMeters );
	lua_pushnumber ( state, velocity.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_getLocalCenter ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2Vec2 center = self->mBody->GetLocalCenter ();
	lua_pushnumber ( state, center.x / unitsToMeters );
	lua_pushnumber ( state, center.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_getPosition ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2Vec2 position = self->mBody->GetPosition ();
	lua_pushnumber ( state, position.x / unitsToMeters );
	lua_pushnumber ( state, position.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_getWorldCenter ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2Vec2 center = self->mBody->GetWorldCenter ();
	lua_pushnumber ( state, center.x / unitsToMeters );
	lua_pushnumber ( state, center.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_isActive ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	bool isActive = self->mBody->IsActive ();
	lua_pushboolean ( state, isActive );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_isAwake ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	bool isAwake = self->mBody->IsAwake ();
	lua_pushboolean ( state, isAwake );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_isBullet ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	bool isBullet = self->mBody->IsBullet ();
	lua_pushboolean ( state, isBullet );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_isFixedRotation ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	bool isFixedRotation = self->mBody->IsFixedRotation ();
	lua_pushboolean ( state, isFixedRotation );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_resetMassData ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	self->mBody->ResetMassData ();
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_setActive ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )

	bool active = state.GetValue < bool >( 2, false );
	self->mBody->SetActive ( active );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_setAngularDamping ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "UN" )
	
	float damping = state.GetValue < float >( 2, 0.0f );
	self->mBody->SetAngularDamping ( damping );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_setAngularVelocity ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	float omega = state.GetValue < float >( 2, 0.0f ) * ( float )D2R;
	
	self->mBody->SetAngularVelocity ( omega );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_setAwake ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	bool awake = state.GetValue < bool >( 2, true );
	self->mBody->SetAwake ( awake );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_setBullet ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	bool bullet = state.GetValue < bool >( 2, true );
	self->mBody->SetBullet ( bullet );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_setFixedRotation ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	
	bool fixedRotation = state.GetValue < bool >( 2, true );
	self->mBody->SetFixedRotation ( fixedRotation );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_setLinearDamping ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "UN" )
	
	float damping = state.GetValue < float >( 2, 0.0f );
	self->mBody->SetLinearDamping ( damping );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_setLinearVelocity ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2Vec2 v;
	v.x		= state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	v.y		= state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	
	self->mBody->SetLinearVelocity ( v );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_setMassData ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "UN" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2MassData massData;
	self->mBody->GetMassData ( &massData );
	
	massData.mass		= state.GetValue < float >( 2, massData.mass );
	massData.I			= state.GetValue < float >( 3, massData.I );
	massData.center.x	= state.GetValue < float >( 4, massData.center.x ) * unitsToMeters;
	massData.center.y	= state.GetValue < float >( 5, massData.center.y ) * unitsToMeters;
	
	self->mBody->SetMassData ( &massData );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIBox2DBody::_setTransform ( lua_State* L ) {
	LUA_SETUP ( MOAIBox2DBody, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	b2Vec2 position;
	position.x		= state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	position.y		= state.GetValue < float >( 3, 0.0f ) * unitsToMeters;
	float angle		= state.GetValue < float >( 4, 0.0f ) * ( float )D2R;
	
	self->mBody->SetTransform ( position, angle );
	
	return 0;
}

//================================================================//
// MOAIBox2DBody
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DBody::MOAIBox2DBody () :
	mBody ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransformBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DBody::~MOAIBox2DBody () {
}

//----------------------------------------------------------------//
void MOAIBox2DBody::OnDepNodeUpdate () {

	if ( this->mBody ) {
		
		b2Transform transform = this->mBody->GetTransform ();
		
		float* m = this->mLocalToWorldMtx.m;
		
		m [ USAffine2D::C0_R0 ] = ( float )transform.R.col1.x;
		m [ USAffine2D::C0_R1 ] = ( float )transform.R.col1.y;

		m [ USAffine2D::C1_R0 ] = ( float )transform.R.col2.x;
		m [ USAffine2D::C1_R1 ] = ( float )transform.R.col2.y;

		m [ USAffine2D::C2_R0 ] = ( float )transform.position.x;
		m [ USAffine2D::C2_R1 ] = ( float )transform.position.y;
		
		this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
	}
}

//----------------------------------------------------------------//
void MOAIBox2DBody::RegisterLuaClass ( USLuaState& state ) {

	MOAITransformBase::RegisterLuaClass ( state );
	
	state.SetField ( -1, "DYNAMIC",		( u32 )b2_dynamicBody );
	state.SetField ( -1, "KINEMATIC",	( u32 )b2_kinematicBody );
	state.SetField ( -1, "STATIC",		( u32 )b2_staticBody );
}

//----------------------------------------------------------------//
void MOAIBox2DBody::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAITransformBase::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "addCircle",				_addCircle },
		{ "addPolygon",				_addPolygon },
		{ "addRect",				_addRect },
		{ "applyAngularImpulse",	_applyAngularImpulse },
		{ "applyForce",				_applyForce },
		{ "applyLinearImpulse",		_applyLinearImpulse },
		{ "applyTorque",			_applyTorque },
		{ "destroyFixture",			_destroyFixture },
		{ "getAngle",				_getAngle },
		{ "getAngularVelocity",		_getAngularVelocity },
		{ "getLinearVelocity",		_getLinearVelocity },
		{ "getLocalCenter",			_getLocalCenter },
		{ "getPosition",			_getPosition },
		{ "getWorldCenter",			_getWorldCenter },
		{ "isActive",				_isActive },
		{ "isAwake",				_isAwake },
		{ "isBullet",				_isBullet },
		{ "isFixedRotation",		_isFixedRotation },
		{ "resetMassData",			_resetMassData },
		{ "setActive",				_setActive },
		{ "setAngularDamping",		_setAngularDamping },
		{ "setAngularVelocity",		_setAngularVelocity },
		{ "setAwake",				_setAwake },
		{ "setBullet",				_setBullet },
		{ "setFixedRotation",		_setFixedRotation },
		{ "setLinearDamping",		_setLinearDamping },
		{ "setLinearVelocity",		_setLinearVelocity },
		{ "setMassData",			_setMassData },
		{ "setTransform",			_setTransform },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBox2DBody::SetBody ( b2Body* body ) {

	this->mBody = body;
	body->SetUserData ( this );
}

//----------------------------------------------------------------//
STLString MOAIBox2DBody::ToString () {

	STLString repr;
	return repr;
}

#endif