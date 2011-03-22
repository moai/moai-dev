// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIParticleForce.h>

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
int MOAIParticleForce::_initAttractor ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleForce, "UNN" )

	self->mType		= ATTRACTOR;
	self->mRadius	= state.GetValue < float >( 2, 0.0f );
	self->mPull		= state.GetValue < float >( 3, 0.0f );
	
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIParticleForce::_initLinear ( lua_State* L ) {
	LUA_SETUP ( MOAIParticleForce, "UNN" )

	self->mType = LINEAR;
	self->mVec.mX = state.GetValue < float >( 2, 0.0f );
	self->mVec.mY = state.GetValue < float >( 3, 0.0f );

	self->ScheduleUpdate ();

	return 0;
}

//================================================================//
// MOAIParticleForce
//================================================================//

//----------------------------------------------------------------//
USVec2D MOAIParticleForce::GetAcceleration ( const USVec2D& loc ) {

	USVec2D acceleration ( 0.0f, 0.0f );

	USVec2D origin = this->mLocalToWorldMtx.GetTranslation ();

	switch ( this->mType ) {
		
		case ATTRACTOR: {
		
			USVec2D vec = origin;
			vec.Sub ( loc );
			
			float dist = vec.NormSafe () / this->mRadius;
			if ( dist <= 1.0f ) {
				
				dist = 1.0f - dist;
				dist = dist * dist * this->mPull;
				
				acceleration.mX = vec.mX * dist;
				acceleration.mY = vec.mY * dist;
			}
			break;
		}
		case LINEAR: {
			acceleration = this->mWorldVec;
			break;
		}
	}
	
	return acceleration;
}

//----------------------------------------------------------------//
MOAIParticleForce::MOAIParticleForce () :
	mType ( LINEAR ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransform2D )
	RTTI_END
	
	this->mVec.Init ( 0.0f, 0.0f );
	
	this->mWorldLoc.Init ( 0.0f, 0.0f );
	this->mWorldVec.Init ( 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIParticleForce::~MOAIParticleForce () {
}

//----------------------------------------------------------------//
void MOAIParticleForce::OnDepNodeUpdate () {

	this->MOAITransform2D::OnDepNodeUpdate ();
	
	const USAffine2D& localToWorldMtx = this->GetLocalToWorldMtx ();
	
	this->mWorldLoc = localToWorldMtx.GetTranslation ();
	
	this->mWorldVec = this->mVec;
	localToWorldMtx.TransformVec ( this->mWorldVec );
}

//----------------------------------------------------------------//
void MOAIParticleForce::RegisterLuaClass ( USLuaState& state ) {

	this->MOAITransform2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIParticleForce::RegisterLuaFuncs ( USLuaState& state ) {
	
	this->MOAITransform2D::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "initAttractor",		_initAttractor },
		{ "initLinear",			_initLinear },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIParticleForce::ToString () {

	STLString repr;
	return repr;
}
