// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIParticleDistanceEmitter.h>
#include <moai-sim/MOAIParticleSystem.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	reset
	@text	Resets the distance traveled. Use this to avoid large emissions
			when 'warping' the emitter to a new location.
	
	@in		MOAIParticleDistanceEmitter self
	@out	nil
*/
int MOAIParticleDistanceEmitter::_reset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleDistanceEmitter, "U" )

	self->mReset = true;
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDistance
	@text	Set the travel distance required for new particle emission.
	
	@in		MOAIParticleDistanceEmitter self
	@in		number min	Minimum distance.
	@opt	number max	Maximum distance. Default value is min.
	@out	nil
*/
int MOAIParticleDistanceEmitter::_setDistance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleDistanceEmitter, "UN" )

	float min = state.GetValue < float >( 2, 0.0f );
	float max = state.GetValue < float >( 3, min );

	self->SetDistanceRange ( min, max );
	return 0;
}

//================================================================//
// MOAIParticleDistanceEmitter
//================================================================//

//----------------------------------------------------------------//
float MOAIParticleDistanceEmitter::GetRandomDistance () {

	return ZLFloat::Rand ( this->mMinDistance, this->mMaxDistance );
}

//----------------------------------------------------------------//
MOAIParticleDistanceEmitter::MOAIParticleDistanceEmitter () :
	mReset ( true ),
	mMinDistance ( 0.0f ),
	mMaxDistance ( 0.0f ),
	mEmitDistance ( 0.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIParticleEmitter )
	RTTI_END
	
	this->mEmitLoc.Init ( 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIParticleDistanceEmitter::~MOAIParticleDistanceEmitter () {
}

//----------------------------------------------------------------//
void MOAIParticleDistanceEmitter::OnDepNodeUpdate () {

	MOAITransform::OnDepNodeUpdate ();

	if ( !this->mSystem ) {
		this->mReset = true;
		return;
	}

	ZLVec3D loc = this->mLocalToWorldMtx.GetTranslation ();
	float dist = ZLDist::VecToVec ( loc, this->mEmitLoc );

	if ( this->mReset ) {
		
		this->mEmitDistance = this->GetRandomDistance ();
		this->mEmitLoc = loc;
		this->mReset = false;
		
		return;
	}
	
	if (( this->mEmitDistance > 0.0f ) && ( dist >= this->mEmitDistance )) {
		
		ZLVec3D moveVec = loc;
		moveVec.Sub ( this->mEmitLoc );
		moveVec.NormSafe ();
		
		while ( dist >= this->mEmitDistance ) {
		
			ZLAffine3D offset;
			
			offset.m [ ZLAffine3D::C0_R0 ] = moveVec.mY;
			offset.m [ ZLAffine3D::C0_R1 ] = -moveVec.mX;
			offset.m [ ZLAffine3D::C0_R2 ] = 0.0f;
			
			offset.m [ ZLAffine3D::C1_R0 ] = moveVec.mX;
			offset.m [ ZLAffine3D::C1_R1 ] = moveVec.mY;
			offset.m [ ZLAffine3D::C1_R2 ] = 0.0f;
			
			offset.m [ ZLAffine3D::C2_R0 ] = 0.0f;
			offset.m [ ZLAffine3D::C2_R1 ] = 0.0f;
			offset.m [ ZLAffine3D::C2_R2 ] = 1.0f;
			
			offset.m [ ZLAffine3D::C3_R0 ] = 0.0f;
			offset.m [ ZLAffine3D::C3_R1 ] = 0.0f;
			offset.m [ ZLAffine3D::C3_R2 ] = 0.0f;
			
			offset.Append ( this->mLocalToWorldMtx );
			
			u32 emission = this->GetRandomEmission ();
			
			ZLVec3D particleLoc;
			ZLVec3D particleVec;
			
			for ( u32 i = 0; i < emission; ++i ) {
				
				this->GetRandomParticle ( particleLoc, particleVec );
				
				offset.Transform ( particleLoc );
				offset.TransformVec ( particleVec );
				
				this->mSystem->PushParticle (
					particleLoc.mX + ( this->mEmitLoc.mX - loc.mX ),
					particleLoc.mY + ( this->mEmitLoc.mY - loc.mY ),
					particleVec.mX,
					particleVec.mY
				);
			}
			
			ZLVec3D emitStep = moveVec;
			emitStep.Scale ( this->mEmitDistance );
			this->mEmitLoc.Add ( emitStep );
			
			dist -= this->mEmitDistance;
			this->mEmitDistance = this->GetRandomDistance ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIParticleDistanceEmitter::OnUpdate ( double step ) {
	UNUSED ( step );
	
	this->ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAIParticleDistanceEmitter::RegisterLuaClass ( MOAILuaState& state ) {

	this->MOAIParticleEmitter::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIParticleDistanceEmitter::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	this->MOAIParticleEmitter::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reset",					_reset },
		{ "setDistance",			_setDistance },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleDistanceEmitter::SetDistanceRange ( float min, float max ) {

	this->mMinDistance = min;
	this->mMaxDistance = max;
}
