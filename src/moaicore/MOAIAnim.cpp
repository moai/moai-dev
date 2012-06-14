// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIAnim.h>
#include <moaicore/MOAIAnimCurveBase.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	apply
	@text	Apply the anim at a given time or time step.

	@overload	Apply the anim at time t0.

		@in		MOAIAnim self
		@opt	number t0 - Default value is 0.
		@out	nil
	
	@overload	Apply the anim for the step t0 to t1.

		@in		MOAIAnim self
		@in		number t0
		@in		number t1
		@out	nil
*/
int MOAIAnim::_apply ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnim, "U" );

	float t0 = state.GetValue < float >( 2, 0.0f );
	float t1 = state.GetValue < float >( 3, t0 );

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
	@in		MOAIAnimCurveBase curve
	@in		MOAINode target - Target node.
	@in		number attrID - Attribute of the target node to be driven by the curve.
	@opt	boolean asDelta - 'true' to apply the curve as a delta instead of an absolute. Default value is false.
	@out	nil
*/
int	MOAIAnim::_setLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnim, "UNUUN" );
	
	MOAINode* target = state.GetLuaObject < MOAINode >( 4, true );
	if ( !target ) return 0;
	
	u32 linkID					= state.GetValue < u32 >( 2, 1 ) - 1;
	MOAIAnimCurveBase* curve	= state.GetLuaObject < MOAIAnimCurveBase >( 3, true );
	u32 attrID					= state.GetValue < u32 >( 5, 0 );
	bool relative				= state.GetValue < bool >( 6, false );
	
	self->SetLink ( linkID, curve, target, attrID, relative );
	
	return 0;
}

//================================================================//
// MOAIAnim
//================================================================//

//----------------------------------------------------------------//
void MOAIAnim::Apply ( float t ) {
	
	MOAIAttrOp attrOp;
	
	u32 total = this->mLinks.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIAnimLink& link = this->mLinks [ i ];
		MOAIAnimCurveBase* curve = link.mCurve;
		MOAINode* target = link.mTarget;
		
		if ( curve && target ) {
			
			if ( !link.mRelative ) {
				curve->GetValue ( attrOp, t );
				target->ApplyAttrOp ( link.mAttrID, attrOp, MOAIAttrOp::SET );
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
	
	MOAIAttrOp attrOp;
	
	u32 total = this->mLinks.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIAnimLink& link = this->mLinks [ i ];
		MOAIAnimCurveBase* curve = link.mCurve;
		MOAINode* target = link.mTarget;
		
		if ( curve && target ) {
			
			if ( link.mRelative ) {
				curve->GetDelta ( attrOp, t0, t1 );
				target->ApplyAttrOp ( link.mAttrID, attrOp, MOAIAttrOp::ADD );
			}
			else {
				curve->GetValue ( attrOp, t1 );
				target->ApplyAttrOp ( link.mAttrID, attrOp, MOAIAttrOp::SET );
			}
			target->ScheduleUpdate ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnim::Clear () {

	this->ClearLinks ();
	this->mLength = 0.0f;
}

//----------------------------------------------------------------//
void MOAIAnim::ClearLinks () {

	for ( u32 i = 0; i < this->mLinks.Size (); ++i ) {
		MOAIAnimLink& link = this->mLinks [ i ];
		link.mCurve.Set ( *this, 0 );
	}
	this->mLinks.Clear ();
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

	float t0 = this->GetTime ();
	this->DoStep ( step );
	float t1 = this->GetTime ();
	
	this->Apply ( t0, t1 );
}

//----------------------------------------------------------------//
void MOAIAnim::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITimer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAnim::RegisterLuaFuncs ( MOAILuaState& state ) {

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

	this->ClearLinks ();
	this->mLinks.Init ( totalLinks );
}

//----------------------------------------------------------------//
void MOAIAnim::SetLink ( u32 linkID, MOAIAnimCurveBase* curve, MOAINode* target, u32 attrID, bool relative ) {

	if ( linkID >= this->mLinks.Size ()) return;
	if ( !target ) return;
	if ( !target->CheckAttrExists ( attrID )) return;

	MOAIAnimLink& link = this->mLinks [ linkID ];
	link.mCurve.Set ( *this, curve );
	link.mTarget	= target;
	link.mAttrID	= attrID;
	link.mRelative	= relative;
	
	float length = curve->GetLength ();

	if ( this->mLength < length ) {
		this->mLength = length;
	}
	this->mEndTime = this->mLength;
}

