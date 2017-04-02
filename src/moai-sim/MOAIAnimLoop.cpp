// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIAnimLoop.h>
#include <moai-sim/MOAIAnimMixer.h>
#include <moai-sim/MOAIAnimCurveBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getLength
	@text	Return the length of the animation.
	
	@in		MOAIAnimLoop self
	@out	number length
*/
int	MOAIAnimLoop::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimLoop, "U" )
	
	lua_pushnumber ( state, self->GetLength ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	reserveCurves
	@text	Reserves a specified number of curves for the loop.
	
	@in		MOAIAnimLoop self
	@in		number nLinks
	@out	nil
*/
int	MOAIAnimLoop::_reserveCurves ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimLoop, "UN" );
	
	u32 nCurves = state.GetValue < u32 >( 2, 0 );
	self->ReserveCurves ( nCurves );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIAnimLoop::_setCurve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimLoop, "U" );
	
	u32 curveID					= state.GetValue < u32 >( 2, 1 ) - 1;
	MOAIAnimCurveBase* curve	= state.GetLuaObject < MOAIAnimCurveBase >( 3, true );
	u32 mapping					= state.GetValue < u32 >( 4, curveID + 1 ) - 1;
	bool delta					= state.GetValue < bool >( 5, false );
	
	self->SetCurve ( curveID, curve, mapping, delta );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int	MOAIAnimLoop::_setLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnimLoop, "U" )
	
	return 1;
}

//================================================================//
// MOAIAnimLoop
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimLoop::Apply ( MOAIAnimMixer& mixer, float t, float weight ) {

	MOAIAttribute attr;
	
	u32 total = ( u32 )this->mCurves.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIAnimLoopChannel& srcChannel = this->mCurves [ i ];
		
		MOAIAnimCurveBase* curve = srcChannel.mCurve;
		MOAIAnimChannel* dstChannel = mixer.GetChannel ( srcChannel.mMapping );
		
		if ( curve && dstChannel ) {
			
			curve->Accumulate ( *dstChannel, t, weight );
		}
	}
}

//----------------------------------------------------------------//
//void MOAIAnimLoop::Apply ( float t0, float t1 ) {
//	
//	if ( t0 == t1 ) {
//		this->Apply ( t0 );
//		return;
//	}
//	
//	MOAIAttribute attr;
//	
//	u32 total = ( u32 )this->mLinks.Size ();
//	for ( u32 i = 0; i < total; ++i ) {
//		
//		MOAIAnimLoopLink& link = this->mLinks [ i ];
//		MOAIAnimCurveBase* curve = link.mCurve;
//		MOAINode* target = link.mTarget;
//		
//		if ( curve && target ) {
//			
//			if ( link.mRelative ) {
//				curve->GetDelta ( attr, t0, t1 );
//				target->ApplyAttrOp ( link.mAttrID, attr, MOAIAttribute::ADD );
//			}
//			else {
//				curve->GetValue ( attr, t1 );
//				target->ApplyAttrOp ( link.mAttrID, attr, MOAIAttribute::SET );
//			}
//			target->ScheduleUpdate ();
//		}
//	}
//}

//----------------------------------------------------------------//
MOAIAnimLoop::MOAIAnimLoop () :
	mLength ( 0.0f ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIAnimLoop::~MOAIAnimLoop () {
}

//----------------------------------------------------------------//
void MOAIAnimLoop::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIAnimLoop::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getLength",				_getLength },
		{ "reserveCurves",			_reserveCurves },
		{ "setCurve",				_setCurve },
		{ "setLength",				_setLength },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimLoop::ReserveCurves ( u32 nCurves ) {

	//this->Clear ();
	this->mCurves.Init ( nCurves );
}

//----------------------------------------------------------------//
void MOAIAnimLoop::SetCurve ( u32 curveID, MOAIAnimCurveBase* curve, u32 mapping, bool delta ) {

	this->mCurves.Grow ( curveID + 1 );

	MOAIAnimLoopChannel& channel = this->mCurves [ curveID ];
	channel.mCurve		= curve;
	channel.mMapping	= mapping;
	channel.mDelta		= delta;
	
	if ( curve ) {
		float length = curve->GetLength ();

		if ( this->mLength < length ) {
			this->mLength = length;
		}
	}
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//

