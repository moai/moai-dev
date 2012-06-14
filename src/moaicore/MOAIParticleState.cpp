// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticleForce.h>
#include <moaicore/MOAIParticlePlugin.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleSystem.h>

class MOAIDataBuffer;

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clearForces
	@text	Removes all particle forces from the state.
	
	@in		MOAIParticleState self
	@out	nil
*/
int MOAIParticleState::_clearForces ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleState, "U" )
	
	self->ClearForces ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	pushForce
	@text	Adds a force to the state.
	
	@in		MOAIParticleState self
	@in		MOAIParticleForce force
	@out	nil
*/
int MOAIParticleState::_pushForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleState, "UU" )
	
	MOAIParticleForce* force = state.GetLuaObject < MOAIParticleForce >( 2, true );
	if ( force ) {
		self->PushForce ( *force );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDamping
	@text	Sets damping for particle physics model.
	
	@in		MOAIParticleState self
	@in		number damping
	@out	nil
*/
int MOAIParticleState::_setDamping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleState, "UN" )

	self->mDamping = state.GetValue < float >( 2, 0.0f );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setInitScript
	@text	Sets the particle script to use for initializing new particles.
	
	@in		MOAIParticleState self
	@opt	MOAIParticleScript script
	@out	nil
*/
int MOAIParticleState::_setInitScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleState, "U" )

	MOAIParticleScript* init = state.GetLuaObject < MOAIParticleScript >( 2, true );

	if ( init ) {
		init->Compile ();
	}
	self->mInit.Set ( *self, init );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMass
	@text	Sets range of masses (chosen randomly) for particles
			initialized by the state.
	
	@in		MOAIParticleState self
	@in		number minMass
	@opt	number maxMass			Default value is minMass.
	@out	nil
*/
int MOAIParticleState::_setMass ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleState, "UN" )

	float m0 = state.GetValue < float >( 2, 0.0f );
	float m1 = state.GetValue < float >( 3, m0 );

	self->mMassRange [ 0 ] = m0;
	self->mMassRange [ 1 ] = m1;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setNext
	@text	Sets the next state (if any).
	
	@in		MOAIParticleState self
	@opt	MOAIParticleState next Default value is nil.
	@out	nil
*/
int MOAIParticleState::_setNext ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleState, "U" )
	
	self->mNext = state.GetLuaObject < MOAIParticleState >( 2, true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPlugin
	@text	Sets the particle plugin to use for initializing and updating particles.
	
	@in		MOAIParticleState self
	@opt	MOAIParticlePlugin plugin
	@out	nil
*/
int MOAIParticleState::_setPlugin ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleState, "U" )

	self->mPlugin.Set ( *self, state.GetLuaObject < MOAIParticlePlugin >( 2, true ));
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRenderScript
	@text	Sets the particle script to use for rendering particles.
	
	@in		MOAIParticleState self
	@opt	MOAIParticleScript script
	@out	nil
*/
int MOAIParticleState::_setRenderScript ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleState, "U" )

	MOAIParticleScript* render = state.GetLuaObject < MOAIParticleScript >( 2, true );

	if ( render ) {
		render->Compile ();
	}
	self->mRender.Set ( *self, render );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTerm
	@text	Sets range of terms (chosen randomly) for particles
			initialized by the state.
	
	@in		MOAIParticleState self
	@in		number minTerm
	@opt	number maxTerm			Default value is minTerm.
	@out	nil
*/
int MOAIParticleState::_setTerm ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleState, "UN" )

	float t0 = state.GetValue < float >( 2, 0.0f );
	float t1 = state.GetValue < float >( 3, t0 );

	self->mTermRange [ 0 ] = t0;
	self->mTermRange [ 1 ] = t1;

	return 0;
}

//================================================================//
// MOAIParticleState
//================================================================//

//----------------------------------------------------------------//
void MOAIParticleState::ClearForces () {

	while ( this->mForces.Count ()) {
	
		ForceNode* forceNode = this->mForces.Head ();
		this->mForces.PopFront ();
		
		this->LuaRelease ( forceNode->Data ());
		delete forceNode;
	}
}

//----------------------------------------------------------------//
void MOAIParticleState::GatherForces ( USVec3D& loc, USVec3D& velocity, float mass, float step ) {

	USVec3D result;

	USVec3D acceleration ( 0.0f, 0.0f, 0.0f );
	USVec3D offset ( 0.0f, 0.0f, 0.0f );
		
	ForceNode* forceNode = this->mForces.Head ();
	for ( ; forceNode; forceNode = forceNode->Next ()) {
		MOAIParticleForce* particleForce = forceNode->Data ();
		particleForce->Eval ( loc, mass, acceleration, offset );
	}
	
	velocity.mX += acceleration.mX * step;
	velocity.mY += acceleration.mY * step;
	
	velocity.Scale ( USFloat::Clamp ( 1.0f - ( this->mDamping * step ), 0.0f, 1.0f ));
	
	loc.mX += ( velocity.mX + offset.mX ) * step;
	loc.mY += ( velocity.mY + offset.mY ) * step;
}

//----------------------------------------------------------------//
void MOAIParticleState::InitParticle ( MOAIParticleSystem& system, MOAIParticle& particle ) {

	if ( this->mInit ) {
		this->mInit->Run ( system, particle, 0.0f, 0.0f );
	}
	
	MOAIParticlePlugin* plugin = this->mPlugin;
	if ( plugin ) {
		plugin->OnInit ( particle.mData, &particle.mData [ MOAIParticle::TOTAL_PARTICLE_REG ]);
	}
	
	particle.mAge = 0.0f;
	particle.mTerm = USFloat::Rand ( this->mTermRange [ 0 ], this->mTermRange [ 1 ]);
	particle.mMass = USFloat::Rand ( this->mMassRange [ 0 ], this->mMassRange [ 1 ]);
	particle.mState = this;
}

//----------------------------------------------------------------//
MOAIParticleState::MOAIParticleState () :
	mDamping ( 0.0f ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
	
	this->mMassRange [ 0 ] = 1.0f;
	this->mMassRange [ 1 ] = 1.0f;
	
	this->mTermRange [ 0 ] = 1.0f;
	this->mTermRange [ 1 ] = 1.0f;
}

//----------------------------------------------------------------//
MOAIParticleState::~MOAIParticleState () {

	this->ClearForces ();
	
	this->mInit.Set ( *this, 0 );
	this->mRender.Set ( *this, 0 );
	this->mPlugin.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIParticleState::PushForce ( MOAIParticleForce& force ) {

	this->LuaRetain ( &force );

	ForceNode* forceNode = new ForceNode ();
	forceNode->Data ( &force );
	
	this->mForces.PushBack ( *forceNode );
}

//----------------------------------------------------------------//
void MOAIParticleState::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIParticleState::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "clearForces",			_clearForces },
		{ "pushForce",				_pushForce },
		{ "setDamping",				_setDamping },
		{ "setInitScript",			_setInitScript },
		{ "setMass",				_setMass },
		{ "setPlugin",				_setPlugin },
		{ "setNext",				_setNext },
		{ "setRenderScript",		_setRenderScript },
		{ "setTerm",				_setTerm },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleState::ProcessParticle ( MOAIParticleSystem& system, MOAIParticle& particle, float step ) {

	float t0 = particle.mAge / particle.mTerm;
	particle.mAge += step;
	
	if ( particle.mAge > particle.mTerm ) {
		particle.mAge = particle.mTerm;
	}
	
	float t1 = particle.mAge / particle.mTerm;
	
	float* r = particle.mData;
	
	USVec3D loc;
	USVec3D vel;
	
	loc.mX = r [ MOAIParticle::PARTICLE_X ];
	loc.mY = r [ MOAIParticle::PARTICLE_Y ];
	loc.mZ = 0.0f;
	
	vel.mX = r [ MOAIParticle::PARTICLE_DX ];
	vel.mY = r [ MOAIParticle::PARTICLE_DY ];
	vel.mZ = 0.0f;
	
	this->GatherForces ( loc, vel, particle.mMass, step );
	
	r [ MOAIParticle::PARTICLE_X ]	= loc.mX;
	r [ MOAIParticle::PARTICLE_Y ]	= loc.mY;
	r [ MOAIParticle::PARTICLE_DX ]	= vel.mX;
	r [ MOAIParticle::PARTICLE_DY ]	= vel.mY;

	if ( this->mRender ) {
		this->mRender->Run ( system, particle, t0, t1 );
	}
	
	MOAIParticlePlugin* plugin = this->mPlugin;
	if ( plugin ) {
		
		AKUParticleSprite sprite;
		plugin->OnRender ( particle.mData, &particle.mData [ MOAIParticle::TOTAL_PARTICLE_REG ], &sprite, t0, t1, particle.mTerm );
		system.PushSprite ( sprite );
	}

	if ( particle.mAge >= particle.mTerm ) {
		
		if ( this->mNext ) {
			this->mNext->InitParticle ( system, particle );
		}
		else {
			particle.mState = 0;
		}
	}
}
