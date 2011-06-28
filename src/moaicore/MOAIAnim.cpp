// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnim.h>
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	apply
	@text	Apply the anim at a given time or time step.

	@overload	Apply the anim at time t0.

		@in		MOAIAnim self
		@in		number t0 - Default value is 0.
		@out	nil
	
	@overload	Apply the anim for the step t0 to t1.

		@in		MOAIAnim self
		@in		number t0 - Default value is 0.
		@in		number t1 - Default value is 0.
		@out	nil
*/
int MOAIAnim::_apply ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnim, "U" );

	float t0 = state.GetValue < float >( 2, 0.0f );
	float t1 = state.GetValue < float >( 3, 0.0f );

	self->Apply ( t0, t1 );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	getLength
	@text	Return the length of the animation.
	
	@in		MOAIAnim self
	@out	number length
*/
int	MOAIAnim::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnim, "U" )
	
	lua_pushnumber ( state, self->GetLength ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	reserveLinks
	@text	Reserves a specified number of links for the animation.
	
	@in		MOAIAnim self
	@in		number nLinks
	@out	nil
*/
int	MOAIAnim::_reserveLinks ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnim, "UN" );
	
	u32 totalLinks = state.GetValue < u32 >( 2, 0 );
	self->ReserveLinks ( totalLinks );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLink
	@text	Connect a curve to a given node attribute.
	
	@in		MOAIAnim self
	@in		number linkID
	@in		MOAIAnimCurve curve
	@in		MOAINode target - Target node.
	@in		number attrID - Attribute of the target node to be driven by the curve.
	@in		boolean asDelta - 'true' to apply the curve as a delta instead of an absolute.
	@out	nil
*/
int	MOAIAnim::_setLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnim, "UNUUN" );
	
	MOAINode* target = state.GetLuaObject < MOAINode >( 4 );
	if ( !target ) return 0;
	
	u32 linkID				= state.GetValue < u32 >( 2, 1 ) - 1;
	MOAIAnimCurve* curve	= state.GetLuaObject < MOAIAnimCurve >( 3 );
	u32 attrID				= state.GetValue < u32 >( 5, 0 );
	bool relative			= state.GetValue < bool >( 6, false );
	
	self->SetLink ( linkID, curve, target, attrID, relative );
	
	return 0;
}

//================================================================//
// MOAIAnim
//================================================================//

//----------------------------------------------------------------//
void MOAIAnim::Apply ( float t ) {
	
	u32 total = this->mLinks.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIAnimLink& link = this->mLinks [ i ];
		MOAIAnimCurve* curve = link.mCurve;
		MOAINode* target = link.mTarget;
		
		if ( curve && target ) {
			
			if ( !link.mRelative ) {
				float value = curve->GetFloatValue ( t );
				target->SetAttributeValue < float >( link.mAttrID, value );
			}
			target->ScheduleUpdate ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnim::Apply ( float t0, float t1 ) {
	
	if ( t0 == t1 ) {
		this->Apply ( t0 );
		return;
	}
	
	USAttrAdder adder;
	
	u32 total = this->mLinks.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIAnimLink& link = this->mLinks [ i ];
		MOAIAnimCurve* curve = link.mCurve;
		MOAINode* target = link.mTarget;
		
		if ( curve && target ) {
			
			if ( link.mRelative ) {
				float value = curve->GetFloatDelta ( t0, t1 );
				adder.Set ( value );
				target->ApplyAttrOp ( link.mAttrID, adder );
			}
			else {
				float value = curve->GetFloatValue ( t1 );
				target->SetAttributeValue < float >( link.mAttrID, value );
			}
			target->ScheduleUpdate ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnim::Clear () {

	this->mLinks.Clear ();
	this->mLength = 0.0f;
}

//----------------------------------------------------------------//
MOAIAnim::MOAIAnim () :
	mLength ( 0.0f ) {
	
	RTTI_SINGLE ( MOAITimer )
}

//----------------------------------------------------------------//
MOAIAnim::~MOAIAnim () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIAnim::OnUpdate ( float step ) {

	float t0 = this->mTime;
	float t1 = t0 + this->DoStep ( step );
	
	this->Apply ( t0, t1 );
}

//----------------------------------------------------------------//
void MOAIAnim::RegisterLuaClass ( USLuaState& state ) {

	MOAITimer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAnim::RegisterLuaFuncs ( USLuaState& state ) {

	MOAITimer::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "apply",				_apply },
		{ "getLength",			_getLength },
		{ "reserveLinks",		_reserveLinks },
		{ "setLink",			_setLink },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnim::ReserveLinks ( u32 totalLinks ) {

	this->mLinks.Init ( totalLinks );
}

//----------------------------------------------------------------//
void MOAIAnim::SetLink ( u32 linkID, MOAIAnimCurve* curve, MOAINode* target, u32 attrID, bool relative ) {

	if ( linkID >= this->mLinks.Size ()) return;
	if ( !target ) return;
	if ( !target->AttrExists ( attrID )) return;

	MOAIAnimLink& link = this->mLinks [ linkID ];
	link.mCurve		= curve;
	link.mTarget	= target;
	link.mAttrID	= attrID;
	link.mRelative	= relative;
	
	float length = curve->GetLength ();

	if ( this->mLength < length ) {
		this->mLength = length;
	}
	this->mEndTime = this->mLength;
}

//----------------------------------------------------------------//
STLString MOAIAnim::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mLength )
	PRETTY_PRINT ( repr, mLinks )

	return repr;
}
