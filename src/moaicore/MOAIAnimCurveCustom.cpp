//
//  MOAIAnimCurveCustom.cpp
//  libmoai
//
//  Created by Aaron Barrett on 5/29/13.
//
//

#include "pch.h"
#include <moaicore/MOAIAnimCurveCustom.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**  @name  setCallback
  + @text  Sets or clears the custom interpolation callback.
  +
  + @in    MOAICustomAnimCurve self
  + @opt  function callback    Default value is nil.
  + @out  nil
  + */
int MOAIAnimCurveCustom::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimCurveCustom, "U" )

	self->mCallback.SetStrongRef ( state, 2 );

	return 0;
}

//================================================================//
// MOAIAnimCurveCustom
//================================================================//

//----------------------------------------------------------------//
float MOAIAnimCurveCustom::GetValue ( const MOAIAnimKeySpan& span ) {
	
	MOAIAnimKey& key = this->mKeys [ span.mKeyID ];
	float v0 = this->mSamples [ span.mKeyID ];
	
	if ( span.mTime > 0.0f ) {
		v0 = MOAIAnimCurveCustom::InterpolateCustom(v0, this->mSamples[span.mKeyID + 1], span.mTime, key.mWeight);
	}
	return v0 + ( this->GetCurveDelta () * span.mCycle );
}

float MOAIAnimCurveCustom::InterpolateCustom(float x0, float x1, float t, float weight) {
	float v0 = t; // quicker way of doing: USInterpolate::Curve( USInterpolate::kLinear, t);
	float v1 = 0.0f; // use custom function

	if (this->mCallback) {
		MOAILuaStateHandle state = this->mCallback.GetSelf();
		lua_pushnumber(state, t); // add parameter variable t to stack
		state.DebugCall(1, 1); // the function should take one argument and return one number

		int top = state.GetTop();
		v1 = state.GetValue < float >( top, 0.0f); // store the return value of the function in v1
	  }

	float s = USInterpolate::Interpolate(USInterpolate::kLinear, v0, v1, weight);

	return x0 + ((x1 - x0) * s);
}

//----------------------------------------------------------------//
MOAIAnimCurveCustom::MOAIAnimCurveCustom () {
	
	RTTI_SINGLE ( MOAIAnimCurve )
}

//----------------------------------------------------------------//
MOAIAnimCurveCustom::~MOAIAnimCurveCustom () {
}

//----------------------------------------------------------------//
void MOAIAnimCurveCustom::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIAnimCurve::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAnimCurveCustom::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAnimCurve::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setCallback",    _setCallback },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}