// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticleTimedEmitter.h>
#include <moaicore/MOAIParticleSystem.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setFrequency
	@text	Set timer frequency.
	
	@in		MOAIParticleTimedEmitter self
	@in		number min
	@opt	number max		Defaulkt value is min.
	@out	nil
*/
int MOAIParticleTimedEmitter::_setFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticleTimedEmitter, "UN" )

	float min = state.GetValue < float >( 2, 1.0f );
	float max = state.GetValue < float >( 3, min );

	self->SetFrequencyRange ( min, max );
	return 0;
}

//================================================================//
// MOAIParticleTimedEmitter
//================================================================//

//----------------------------------------------------------------//
float MOAIParticleTimedEmitter::GetRandomFrequency () {

	return USFloat::Rand ( this->mMinFrequency, this->mMaxFrequency );
}

//----------------------------------------------------------------//
MOAIParticleTimedEmitter::MOAIParticleTimedEmitter () :
	mTime ( 0.0f ),
	mEmitTime ( 0.0f ),
	mMinFrequency ( 1.0f ),
	mMaxFrequency ( 1.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIParticleEmitter )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIParticleTimedEmitter::~MOAIParticleTimedEmitter () {
}

//----------------------------------------------------------------//
void MOAIParticleTimedEmitter::OnUpdate ( float step ) {

	this->mTime += step;
	if ( !this->mSystem ) return;
	if ( this->mTime < this->mEmitTime ) return;
	
	u32 emission = this->GetRandomEmission ();
	this->Surge ( emission );
	
	this->mEmitTime = this->mTime + this->GetRandomFrequency ();
}

//----------------------------------------------------------------//
void MOAIParticleTimedEmitter::RegisterLuaClass ( MOAILuaState& state ) {

	this->MOAIParticleEmitter::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIParticleTimedEmitter::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	this->MOAIParticleEmitter::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setFrequency",		_setFrequency },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIParticleTimedEmitter::SetFrequencyRange ( float min, float max ) {

	this->mMinFrequency = min;
	this->mMaxFrequency = max;
}
