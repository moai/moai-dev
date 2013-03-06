// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticle.h>
#include <moaicore/MOAIParticleForce.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	initAttractor
	@text	Greater force is exerted on particles as they approach
			attractor.
	
	@in		MOAIParticleForce self
	@in		number radius Size of the attractor.
	@opt	number magnitude Strength of the attractor.
	@out	nil
*/
int MOAIParticleForce::_initAttractor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleForce, "UNN" )

	self->mShape	= ATTRACTOR;
	self->mRadius	= state.GetValue < float >( 2, 0.0f );
	self->mPull		= state.GetValue < float >( 3, 0.0f );
	
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	initBasin
	@text	Greater force is exerted on particles as they leave
			attractor.
	
	@in		MOAIParticleForce self
	@in		number radius Size of the attractor.
	@opt	number magnitude Strength of the attractor.
	@out	nil
*/
int MOAIParticleForce::_initBasin ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleForce, "UNN" )

	self->mShape	= BASIN;
	self->mRadius	= state.GetValue < float >( 2, 0.0f );
	self->mPull		= state.GetValue < float >( 3, 0.0f );

	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	initLinear
	@text	A constant linear force will be applied to the particles.
	
	@in		MOAIParticleForce self
	@in		number x
	@opt	number y
	@out	nil
*/
int MOAIParticleForce::_initLinear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleForce, "UNN" )

	self->mShape = LINEAR;
	self->mVec.mX = state.GetValue < float >( 2, 0.0f );
	self->mVec.mY = state.GetValue < float >( 3, 0.0f );

	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	initRadial
	@text	A constant radial force will be applied to the particles.
	
	@in		MOAIParticleForce self
	@in		number magnitude
	@out	nil
*/
int MOAIParticleForce::_initRadial ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleForce, "UN" )

	self->mShape = RADIAL;
	self->mPull = state.GetValue < float >( 2, 0.0f );

	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setType
	@text	Set the type of force. FORCE will factor in the particle's mass. GRAVITY
			will ignore the particle's mass. OFFSET will ignore both mass and damping.
	
	@in		MOAIParticleForce self
	@in		number type				One of MOAIParticleForce.FORCE, MOAIParticleForce.GRAVITY, MOAIParticleForce.OFFSET
	@out	nil
*/
int MOAIParticleForce::_setType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleForce, "UN" )

	self->mType = state.GetValue < u32 >( 2, self->mType );

	return 0;
}

//================================================================//
// MOAIParticleForce
//================================================================//

//----------------------------------------------------------------//
void MOAIParticleForce::Eval ( const USVec3D& loc, float mass, USVec3D& acceleration, USVec3D& offset ) {

	USVec3D force;
	USVec3D origin = this->mLocalToWorldMtx.GetTranslation ();

	switch ( this->mShape ) {
		
		case ATTRACTOR: {
		
			USVec3D vec = origin;
			vec.Sub ( loc );
			
			float dist = vec.NormSafe () / this->mRadius;
			if ( dist <= 1.0f ) {
				
				dist = 1.0f - dist;
				dist = dist * dist * this->mPull;
				
				force.mX = vec.mX * dist;
				force.mY = vec.mY * dist;
			}
			break;
		}
		case BASIN: {
			
			USVec3D vec = origin;
			vec.Sub ( loc );
			
			float dist = vec.NormSafe () / this->mRadius;
			
			dist = dist * dist * this->mPull;
			
			force.mX = vec.mX * dist;
			force.mY = vec.mY * dist;
			
			break;
		}
		case LINEAR: {
			force = this->mWorldVec;
			break;
		}
		case RADIAL: {
			
			force = origin;
			force.Sub ( loc );
			force.NormSafe ();
			force.Scale ( this->mPull );
			
			break;
		}
	}
	
	switch ( this->mType ) {
	
		case FORCE:
			force.Scale ( 1.0f / mass );
			acceleration.Add ( force );
			break;
	
		case GRAVITY:
			acceleration.Add ( force );
			break;
		
		case OFFSET:
			offset.Add ( force );
			break;
	}
}

//----------------------------------------------------------------//
MOAIParticleForce::MOAIParticleForce () :
	mShape ( LINEAR ),
	mType ( GRAVITY ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform )
	RTTI_END
	
	this->mVec.Init ( 0.0f, 0.0f, 0.0f );
	
	this->mWorldLoc.Init ( 0.0f, 0.0f, 0.0f );
	this->mWorldVec.Init ( 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIParticleForce::~MOAIParticleForce () {
}

//----------------------------------------------------------------//
void MOAIParticleForce::OnDepNodeUpdate () {

	this->MOAITransform::OnDepNodeUpdate ();
	
	const USAffine3D& localToWorldMtx = this->GetLocalToWorldMtx ();
	
	this->mWorldLoc = localToWorldMtx.GetTranslation ();
	
	this->mWorldVec = this->mVec;
	localToWorldMtx.TransformVec ( this->mWorldVec );
}

//----------------------------------------------------------------//
void MOAIParticleForce::RegisterLuaClass ( MOAILuaState& state ) {

	this->MOAITransform::RegisterLuaClass ( state );
	
	state.SetField ( -1, "FORCE", ( u32 )FORCE );
	state.SetField ( -1, "GRAVITY", ( u32 )GRAVITY );
	state.SetField ( -1, "OFFSET", ( u32 )OFFSET );
}

//----------------------------------------------------------------//
void MOAIParticleForce::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	this->MOAITransform::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "initAttractor",		_initAttractor },
		{ "initBasin",			_initBasin },
		{ "initLinear",			_initLinear },
		{ "initRadial",			_initRadial },
		{ "setType",			_setType },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
