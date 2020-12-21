// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAnim.h>
#include <moai-core/MOAIAnimCurve.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	apply
	@text	Apply the animation at a given time or time step.

	@overload	Apply the animation at time t0.

		@in		MOAIAnim self
		@opt	number t0		Default value is 0.
		@out	nil
	
	@overload	Apply the animation for the step t0 to t1.

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
/**	@lua	getLength
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
/**	@lua	reserveLinks
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
/**	@lua	setLink
	@text	Connect a curve to a given node attribute.
	
	@in		MOAIAnim self
	@in		number linkID
	@in		MOAIAnimCurve curve
	@in		MOAINode target				Target node.
	@in		number attrID				Attribute of the target node to be driven by the curve.
	@opt	boolean asDelta				'true' to apply the curve as a delta instead of an absolute. Default value is false.
	@out	nil
*/
int	MOAIAnim::_setLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAnim, "UNUUN" );
	
	MOAINode* target = state.GetLuaObject < MOAINode >( 4, true );
	if ( !target ) return 0;
	
	ZLIndex linkID				= state.GetValue < MOAILuaIndex >( 2, 0 );
	MOAIAnimCurve* curve		= state.GetLuaObject < MOAIAnimCurve >( 3, true );
	ZLAttrID attrID			= ZLAttrID::FromRaw ( state.GetValue < u32 >( 5, 0 ));
	bool relative				= state.GetValue < bool >( 6, false );
	
	self->SetLink ( linkID, curve, target, attrID, relative );
	
	return 0;
}

//================================================================//
// MOAIAnim
//================================================================//

//----------------------------------------------------------------//
void MOAIAnim::Apply ( float t ) {
	
	ZLAttribute attr;
	
	ZLSize total = this->mLinks.Size ();
	for ( ZLIndex i = 0; i < total; ++i ) {
		
		MOAIAnimLink& link = this->mLinks [ i ];
		MOAIAnimCurve* curve = link.mCurve;
		MOAINode* target = link.mTarget;
		
		if ( curve && target ) {
			
			if ( !link.mRelative ) {
				curve->GetValue ( attr, t );
				target->ApplyAttrOp ( link.mAttrID, attr, ZLAttribute::SET );
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
	
	ZLAttribute attr;
	
	u32 total = ( u32 )this->mLinks.Size ();
	for ( ZLIndex i = 0; i < total; ++i ) {
		
		MOAIAnimLink& link = this->mLinks [ i ];
		MOAIAnimCurve* curve = link.mCurve;
		MOAINode* target = link.mTarget;
		
		if ( curve && target ) {
			
			if ( link.mRelative ) {
				curve->GetDelta ( attr, t0, t1 );
				target->ApplyAttrOp ( link.mAttrID, attr, ZLAttribute::ADD );
			}
			else {
				curve->GetValue ( attr, t1 );
				target->ApplyAttrOp ( link.mAttrID, attr, ZLAttribute::SET );
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

	for ( ZLIndex i = 0; i < this->mLinks.Size (); ++i ) {
		MOAIAnimLink& link = this->mLinks [ i ];
		link.mCurve.Set ( *this, 0 );
		link.mTarget.Set ( *this, 0 );
	}
	this->mLinks.Clear ();
}

//----------------------------------------------------------------//
MOAIAnim::MOAIAnim () :
	mLength ( 0.0f ) {
		
	RTTI_BEGIN ( MOAIAnim )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAnim >)
		RTTI_EXTEND ( MOAITimer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAnim::~MOAIAnim () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIAnim::ReserveLinks ( u32 totalLinks ) {

	this->ClearLinks ();
	this->mLinks.Init ( totalLinks );
}

//----------------------------------------------------------------//
void MOAIAnim::SetLink ( ZLIndex linkID, MOAIAnimCurve* curve, MOAINode* target, ZLAttrID attrID, bool relative ) {

	if ( linkID >= this->mLinks.Size ()) return;
	if ( !target ) return;
	if ( !target->CheckAttrExists ( attrID )) return;

	MOAIAnimLink& link = this->mLinks [ linkID ];
	link.mCurve.Set ( *this, curve );
	link.mTarget.Set ( *this, target );
	link.mAttrID	= attrID;
	link.mRelative	= relative;
	
	float length = curve->GetLength ();

	if ( this->mLength < length ) {
		this->mLength = length;
	}
	this->mEndTime = this->mLength;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAnim::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAnim::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

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
void MOAIAnim::MOAIAction_Update ( double step ) {

	float t0 = this->GetTime ();
	this->DoStep (( float )step );
	float t1 = this->GetTime ();
	
	this->Apply ( t0, t1 );
}
