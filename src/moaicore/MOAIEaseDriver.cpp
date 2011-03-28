// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAINode.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>reserveForces ( self, total )</tt>\n
\n
	Reserve total forces.
	@param self (in)
	@param total (in)
*/
int MOAIEaseDriver::_reserveForces ( lua_State* L ) {
	LUA_SETUP ( MOAIEaseDriver, "UN" );
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveForces ( total );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setDelay ( self, delay )</tt>\n
\n
	Sets the delay (total duration of action).
	@param self (in)
	@param delay (in)
*/
int MOAIEaseDriver::_setDelay ( lua_State* L ) {
	LUA_SETUP ( MOAIEaseDriver, "UN" );
	
	float delay = state.GetValue < float >( 2, 0.0f );
	self->SetDelay ( delay );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setForce ( self, idx, target, attrID, force, mode )</tt>\n
\n
	Initializes a force to be applied to an attributue.
	@param self (in)
	@param idx (in)
	@param target (in)
	@param attrID (in)
	@param force (in)
	@param mode (in)
*/
int MOAIEaseDriver::_setForce ( lua_State* L ) {
	LUA_SETUP ( MOAIEaseDriver, "UNUNN" );
	
	MOAINode* target = state.GetLuaObject < MOAINode >( 3 );
	if ( !target ) return 0;
	
	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 attrID			= state.GetValue < u32 >( 4, 0 );
	float force			= state.GetValue < float >( 5, 0.0f );
	u32 mode			= state.GetValue < u32 >( 6, USInterpolate::kSmooth );
	
	self->SetForce ( idx, target, attrID, force, mode );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setMode ( self, mode )</tt>\n
\n
	Sets the interpolation mode.
	@param self (in)
	@param mode (in) Interpolation mode.
*/
int MOAIEaseDriver::_setMode ( lua_State* L ) {
	LUA_SETUP ( MOAIEaseDriver, "UN" );
	
	u32 mode = state.GetValue < u32 >( 2, USInterpolate::kSmooth );
	
	self->SetMode ( mode );
	
	return 0;
}

//================================================================//
// MOAIEaseDriver
//================================================================//

//----------------------------------------------------------------//
void MOAIEaseDriver::Apply ( float step ) {

	if ( step == 0.0f ) return;
	if ( this->mTime >= 1.0f ) return;

	float t0 = this->mTime;
	float t1 = t0 + step;
	
	if ( t1 > 1.0f ) {
		t1 = 1.0f;
	}

	USAttrAdder adder;

	u32 total = this->mForces.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIEaseAttr& forceAttr = this->mForces [ i ];
		if ( forceAttr.mTarget ) {
			
			u32 mode = forceAttr.mMode;
			
			float f0 = USInterpolate::Curve ( mode, t0 );
			float f1 = USInterpolate::Curve ( mode, t1 );
			
			float delta = ( forceAttr.mForce * f1 ) - ( forceAttr.mForce * f0 );
			
			if ( delta != 0.0f ) {
				adder.Set ( delta );
				forceAttr.mTarget->ApplyAttrOp ( forceAttr.mAttrID, adder );
				forceAttr.mTarget->ScheduleUpdate ();
			}
		}
	}
	
	this->mTime = t1;
}

//----------------------------------------------------------------//
bool MOAIEaseDriver::IsBusy () {

	return ( this->mTime < 1.0f );
}

//----------------------------------------------------------------//
MOAIEaseDriver::MOAIEaseDriver () :
	mTime ( 0.0f ),
	mDelay ( 0.0f ),
	mMode ( USInterpolate::kSmooth ) {
	
	RTTI_SINGLE ( MOAIAction )
}

//----------------------------------------------------------------//
MOAIEaseDriver::~MOAIEaseDriver () {
}

//----------------------------------------------------------------//
void MOAIEaseDriver::OnUpdate ( float step ) {
	
	this->Apply ( step / this->mDelay );
}

//----------------------------------------------------------------//
void MOAIEaseDriver::RegisterLuaClass ( USLuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIEaseDriver::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );
	
	LuaReg regTable [] = {
		{ "reserveForces",			_reserveForces },
		{ "setDelay",				_setDelay },
		{ "setForce",				_setForce },
		{ "setMode",				_setMode },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIEaseDriver::ReserveForces ( u32 total ) {

	this->mForces.Init ( total );
}

//----------------------------------------------------------------//
void MOAIEaseDriver::SetForce ( u32 idx, MOAINode* target, u32 attrID, float force, u32 mode ) {

	if ( idx < this->mForces.Size ()) {
		
		MOAIEaseAttr& forceAttr = this->mForces [ idx ];

		forceAttr.mTarget = target;
		forceAttr.mAttrID = attrID;
		forceAttr.mForce = force;
		forceAttr.mMode = mode;
	}
}

//----------------------------------------------------------------//
void MOAIEaseDriver::SetForce ( u32 mode ) {

	for ( u32 i = 0; i < this->mForces.Size (); ++i ) {
		this->mForces [ i ].mMode = mode;
	}
}

//----------------------------------------------------------------//
STLString MOAIEaseDriver::ToString () {

	STLString repr( MOAIAction::ToString () );

	const char *mode;

	switch ( mMode ) {
		case USInterpolate::kEaseIn:
			mode = "EASE_IN";
			break;
		case USInterpolate::kEaseOut:
			mode = "EASE_OUT";
			break;
		case USInterpolate::kFlat:
			mode = "FLAT";
			break;
		case USInterpolate::kLinear:
			mode = "LINEAR";
			break;
		case USInterpolate::kSmooth:
			mode = "SMOOTH";
			break;
		case USInterpolate::kSoftEaseIn:
			mode = "SOFT_EASE_IN";
			break;
		case USInterpolate::kSoftEaseOut:
			mode = "SOFT_EASE_OUT";
			break;
		case USInterpolate::kSoftSmooth:
			mode = "SOFT_SMOOTH";
			break;
		default:
			mode = "INVALID";
	}

	PRETTY_PRINT ( repr, mDelay )

	return repr;
}
