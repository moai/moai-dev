// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAISim.h>
#include <moaicore/MOAIAnim.h>
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIAnimPlayer.h>

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
int MOAIAnimPlayer::_apply ( lua_State* L ) {
	LUA_SETUP ( MOAIAnimPlayer, "U" );

	float t = state.GetValue < float >( 2, self->mTime );
	self->Apply ( t );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>reserveLinks ( self, totalLinks )</tt>\n
\n
	Reserves links for the AnimPlayer.
	@param self (in)
	@param totalLinks (in) Total number of links to be reserved.
*/
int	MOAIAnimPlayer::_reserveLinks ( lua_State* L ) {
	LUA_SETUP ( MOAIAnimPlayer, "UN" );
	
	u32 totalLinks = state.GetValue < u32 >( 2, 0 );
	self->ReserveLinks ( totalLinks );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setAnim ( self, anim )</tt>\n
\n
	Sets the anim for this anim player.
	@param self (in)
	@param anim (in) The MOAIAnim object to be bound.
*/
int MOAIAnimPlayer::_setAnim ( lua_State* L ) {
	LUA_SETUP ( MOAIAnimPlayer, "UU" );

	MOAIAnim* anim = state.GetLuaData < MOAIAnim >( 2 );
	if ( !anim ) return 0;

	self->mAnim = anim;
	self->mStartTime = 0.0f;
	self->mEndTime = anim->mLength;

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setLink ( self, linkID, curveID, target, attrID )</tt>\n
\n
	Sets a link from a curve to an attribute (must be bound with the bind function).
	@param self (in)
	@param linkID (in) ID of the link index.
	@param curveID (in) ID of the curve.
	@param target (in) Target object to animate.
	@param attrID (in) ID of the attribute.
*/
int	MOAIAnimPlayer::_setLink ( lua_State* L ) {
	LUA_SETUP ( MOAIAnimPlayer, "UNNUN" );
	
	MOAINode* target = state.GetLuaData < MOAINode >( 4 );
	if ( !target ) return 0;
	
	u32 linkID		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 curveID		= state.GetValue < u32 >( 3, 1 ) - 1;
	u32 attrID		= state.GetValue < u32 >( 5, 0 );
	bool relative	= state.GetValue < bool >( 6, false );
	
	self->SetLink ( linkID, curveID, target, attrID, relative );
	
	return 0;
}

//================================================================//
// MOAIAnimPlayer
//================================================================//

//----------------------------------------------------------------//
void MOAIAnimPlayer::Apply ( float t ) {
	
	MOAIAnim* anim = this->mAnim;
	if ( !anim ) return;
	
	u32 totalCurves = anim->mCurves.Size ();
	if ( !totalCurves ) return;
	
	u32 total = this->mLinks.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIAnimLink& link = this->mLinks [ i ];
		MOAINode* target = link.mTarget;
		
		if ( target && ( link.mCurveID < totalCurves ) && anim->mCurves [ link.mCurveID ] ) {
			
			if ( !link.mRelative ) {
				float value = anim->mCurves [ link.mCurveID ]->GetFloatValue ( t );
				target->SetAttributeValue < float >( link.mAttrID, value );
			}
			target->ScheduleUpdate ();
		}
	}
}

//----------------------------------------------------------------//
MOAIAnimPlayer::MOAIAnimPlayer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITimer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAnimPlayer::~MOAIAnimPlayer () {
}

//----------------------------------------------------------------//
void MOAIAnimPlayer::OnUpdate ( float step ) {

	float t0 = this->mTime;
	float t1 = t0 + this->DoStep ( step );
	
	MOAIAnim* anim = this->mAnim;
	if ( !anim ) return;
	
	u32 totalCurves = anim->mCurves.Size ();
	if ( !totalCurves ) return;
	
	USAttrAdder adder;
	
	u32 total = this->mLinks.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIAnimLink& link = this->mLinks [ i ];
		MOAINode* target = link.mTarget;
		
		if ( target && ( link.mCurveID < totalCurves ) && anim->mCurves [ link.mCurveID ] ) {
			
			if ( link.mRelative ) {
				float value = anim->mCurves [ link.mCurveID ]->GetFloatDelta ( t0, t1 );
				adder.Set ( value );
				target->ApplyAttrOp ( link.mAttrID, adder );
			}
			else {
				float value = anim->mCurves [ link.mCurveID ]->GetFloatValue ( this->mTime );
				target->SetAttributeValue < float >( link.mAttrID, value );
			}
			target->ScheduleUpdate ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIAnimPlayer::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIAnimPlayer::RegisterLuaFuncs ( USLuaState& state ) {

	MOAITimer::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "apply",				_apply },
		{ "reserveLinks",		_reserveLinks },
		{ "setAnim",			_setAnim },
		{ "setLink",			_setLink },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAnimPlayer::ReserveLinks ( u32 totalLinks ) {

	this->mLinks.Init ( totalLinks );
}

//----------------------------------------------------------------//
void MOAIAnimPlayer::SetLink ( u32 linkID, u32 curveID, MOAINode* target, u32 attrID, bool relative ) {

	if ( linkID >= this->mLinks.Size ()) return;
	if ( !target ) return;
	if ( !target->AttrExists ( attrID )) return;

	MOAIAnimLink& link = this->mLinks [ linkID ];
	link.mCurveID	= curveID;
	link.mTarget	= target;
	link.mAttrID	= attrID;
	link.mRelative	= relative;
}

//----------------------------------------------------------------//
STLString MOAIAnimPlayer::ToString () {

	STLString repr( MOAITimer::ToString ());

	PRETTY_PRINT ( repr, mAnim );
	PRETTY_PRINT ( repr, mLinks );

	return repr;
}
