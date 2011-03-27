// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIOscillator.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>setAmplitude ( self, amp )</tt>\n
\n
	Sets the amplitude of the wave.
	@param self (in)
	@param amp (in)
*/
int MOAIOscillator::_setAmplitude ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UN" )) return 0;
	
	MOAIOscillator* self = state.GetLuaData < MOAIOscillator >( 1 );
	if ( !self ) return 0;

	self->mAmplitude = state.GetValue ( 2, self->mAmplitude );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setBounce ( self, bounce )</tt>\n
\n
	Limits the wave sign to be pos/neg (no bounce) or pos/pos (bounce).
	@param self (in)
	@param bounce (in) 'true' or 'false.'
*/
int MOAIOscillator::_setBounce ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UB" )) return 0;
	
	MOAIOscillator* self = state.GetLuaData < MOAIOscillator >( 1 );
	if ( !self ) return 0;

	self->mBounce = state.GetValue ( 2, self->mBounce );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setFrequency ( self, frequency )</tt>\n
\n
	Sets the frequency of the wave.
	@param self (in)
	@param frequency (in)
*/
int MOAIOscillator::_setFrequency ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UN" )) return 0;
	
	MOAIOscillator* self = state.GetLuaData < MOAIOscillator >( 1 );
	if ( !self ) return 0;

	self->mFrequency = state.GetValue ( 2, self->mFrequency );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setPeriod ( self, period )</tt>\n
\n
	Sets the period of the wave.
	@param self (in)
	@param period (in)
*/
int MOAIOscillator::_setPeriod ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UN" )) return 0;
	
	MOAIOscillator* self = state.GetLuaData < MOAIOscillator >( 1 );
	if ( !self ) return 0;

	self->mPeriod = state.GetValue ( 2, self->mPeriod );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setPhase ( self, phase )</tt>\n
\n
	Set the phase of the wave.
	@param self (in)
	@param phase (in)
*/
int MOAIOscillator::_setPhase ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UN" )) return 0;
	
	MOAIOscillator* self = state.GetLuaData < MOAIOscillator >( 1 );
	if ( !self ) return 0;

	self->mPhase = state.GetValue ( 2, self->mPhase );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setPower ( self, power )</tt>\n
\n
	Controls the 'chunkiness' of the wave.
	@param self (in)
	@param power (in)
*/
int MOAIOscillator::_setPower ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UN" )) return 0;
	
	MOAIOscillator* self = state.GetLuaData < MOAIOscillator >( 1 );
	if ( !self ) return 0;

	self->mPower = state.GetValue ( 2, self->mPower );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setSlide ( self, duration, targetAmp, targetFreq )</tt>\n
\n
	Sets up an interpolation of amp and freq over duration.
	@param self (in)
	@param duration (in)
	@param targetAmp (in)
	@param targetFreq (in)
*/
int MOAIOscillator::_setSlide ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UNNN" )) return 0;
	
	MOAIOscillator* self = state.GetLuaData < MOAIOscillator >( 1 );
	if ( !self ) return 0;

	self->mDuration			= state.GetValue ( 2, self->mDuration );
	self->mTargetAmplitude	= state.GetValue ( 3, self->mTargetAmplitude );
	self->mTargetFrequency	= state.GetValue ( 4, self->mTargetFrequency );

	return 0;
}

//================================================================//
// MOAIOscillator
//================================================================//

//----------------------------------------------------------------//
void MOAIOscillator::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch ( attrID ) {
		case ATTR_TIME:
			this->mTime = attrOp.Op ( this->mTime );
			return;
		case ATTR_VALUE:
			attrOp.Op ( this->mValue );
			return;
	}
}

//----------------------------------------------------------------//
u32 MOAIOscillator::CountAttributes () {

	return MOAIOscillator::TOTAL_ATTR;
}

//----------------------------------------------------------------//
MOAIOscillator::MOAIOscillator () :
	mTime ( 0.0f ),
	mValue ( 0.0f ),
	mDuration ( 0.0f ),
	mTargetAmplitude ( 1.0f ),
	mTargetFrequency ( 1.0f ),
	mPower ( 2.0f ),
	mAmplitude ( 1.0f ),
	mPeriod ( 1.0f ),
	mFrequency ( 1.0f ),
	mPhase ( 0.0f ),
	mBounce ( false ) {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIOscillator::~MOAIOscillator () {
}

//----------------------------------------------------------------//
void MOAIOscillator::OnDepNodeUpdate () {
	
	float t = this->mTime;
	float a = this->mAmplitude;
	float f = this->mFrequency;
	
	if ( this->mDuration > 0.0f ) {
	
		t = USFloat::Mod ( t, this->mDuration );
		float mod = t / this->mDuration;
		a = USInterpolate::EaseIn ( a, this->mTargetAmplitude, mod );
		f = USInterpolate::EaseIn ( f, this->mTargetFrequency, mod );
	}
	
	// bring time into curve space
	t = (( t * f ) / this->mPeriod ) + this->mPhase;
	
	int i = ( int )t;
	float x = t - ( float )i;
	
	// negate 'a' on odd cycles
	if ( this->mBounce ) {
		a = -a;
	}
	else {
		a = ( i & 0x01 ) ? a : -a;
	}
	
	float p = this->mPower;
	float b = this->mPower - p;
	
	if ( b == 0.0f ) {
		this->mValue = ( USFloat::Abs ( powf ((( x * 2.0f ) - 1.0f ), p )) - 1 ) * a;
	}
	else {
		
		float y0 = ( USFloat::Abs ( powf ((( x * 2.0f ) - 1 ), p )) - 1 ) * a;
		float y1 = ( USFloat::Abs ( powf ((( x * 2.0f ) - 1 ), p + 1.0f )) - 1 ) * a;
		
		this->mValue = USInterpolate::Linear ( y0, y1, b );
	}
}

//----------------------------------------------------------------//
void MOAIOscillator::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "ATTR_TIME", ( u32 )ATTR_TIME );
	state.SetField ( -1, "ATTR_VALUE", ( u32 )ATTR_VALUE );
}

//----------------------------------------------------------------//
void MOAIOscillator::RegisterLuaFuncs ( USLuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "setAmplitude",	_setAmplitude },
		{ "setBounce",		_setBounce },
		{ "setFrequency",	_setFrequency	},
		{ "setPeriod",		_setPeriod },
		{ "setPhase",		_setPhase },
		{ "setPower",		_setPower },
		{ "setSlide",		_setSlide },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIOscillator::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mTime )
	PRETTY_PRINT ( repr, mValue )
	PRETTY_PRINT ( repr, mDuration )
	PRETTY_PRINT ( repr, mTargetAmplitude )
	PRETTY_PRINT ( repr, mTargetFrequency )
	PRETTY_PRINT ( repr, mPower )
	PRETTY_PRINT ( repr, mAmplitude )
	PRETTY_PRINT ( repr, mPeriod )
	PRETTY_PRINT ( repr, mPhase )
	PRETTY_PRINT ( repr, mBounce )

	return repr;
}
