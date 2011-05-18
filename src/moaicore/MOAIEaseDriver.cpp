// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAINode.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	reserveLinks
	@text	Reserve links.

	@in		MOAIEaseDriver self
	@in		number nLinks
	@out	nil
*/
int MOAIEaseDriver::_reserveLinks ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIEaseDriver, "UN" );
	
	u32 total = state.GetValue < u32 >( 2, 0 );
	self->ReserveLinks ( total );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLength
	@text	Sets the length (duration) of the ease.

	@in		MOAIEaseDriver self
	@in		number length
	@out	nil
*/
int MOAIEaseDriver::_setLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIEaseDriver, "UN" );
	
	float length = state.GetValue < float >( 2, 0.0f );
	self->SetLength ( length );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLink
	@text	Set the ease for a target node attribute.

	@in		MOAIEaseDriver self
	@in		number idx				Index of the link;
	@in		MOAINode target			Target node.
	@in		number attrID			Index of the attribute to be driven.
	@in		number value			Value for attribute at the end of the ease.
	@in		number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
									MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
	@out	nil
*/
int MOAIEaseDriver::_setLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIEaseDriver, "UNUNN" );
	
	MOAINode* target = state.GetLuaObject < MOAINode >( 3 );
	if ( !target ) return 0;
	
	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 attrID			= state.GetValue < u32 >( 4, 0 );
	float force			= state.GetValue < float >( 5, 0.0f );
	u32 mode			= state.GetValue < u32 >( 6, USInterpolate::kSmooth );
	
	self->SetLink ( idx, target, attrID, force, mode );
	
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

	u32 total = this->mLinks.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIEaseDriverLink& link = this->mLinks [ i ];
		if ( link.mTarget ) {
			
			u32 mode = link.mMode;
			
			float f0 = USInterpolate::Curve ( mode, t0 );
			float f1 = USInterpolate::Curve ( mode, t1 );
			
			float delta = ( link.mValue * f1 ) - ( link.mValue * f0 );
			
			if ( delta != 0.0f ) {
				adder.Set ( delta );
				link.mTarget->ApplyAttrOp ( link.mAttrID, adder );
				link.mTarget->ScheduleUpdate ();
			}
		}
	}
	
	this->mTime = t1;
}

//----------------------------------------------------------------//
bool MOAIEaseDriver::IsDone () {

	return ( this->mTime >= 1.0f );
}

//----------------------------------------------------------------//
MOAIEaseDriver::MOAIEaseDriver () :
	mTime ( 0.0f ),
	mLength ( 0.0f ) {
	
	RTTI_SINGLE ( MOAIAction )
}

//----------------------------------------------------------------//
MOAIEaseDriver::~MOAIEaseDriver () {
}

//----------------------------------------------------------------//
void MOAIEaseDriver::OnUpdate ( float step ) {
	
	this->Apply ( step / this->mLength );
}

//----------------------------------------------------------------//
void MOAIEaseDriver::RegisterLuaClass ( USLuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIEaseDriver::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserveLinks",			_reserveLinks },
		{ "setLength",				_setLength },
		{ "setLink",				_setLink },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIEaseDriver::ReserveLinks ( u32 total ) {

	this->mLinks.Init ( total );
}

//----------------------------------------------------------------//
void MOAIEaseDriver::SetLink ( u32 idx, MOAINode* target, u32 attrID, float value, u32 mode ) {

	if ( idx < this->mLinks.Size ()) {
		
		MOAIEaseDriverLink& link = this->mLinks [ idx ];

		link.mTarget = target;
		link.mAttrID = attrID;
		link.mValue = value;
		link.mMode = mode;
	}
}

//----------------------------------------------------------------//
STLString MOAIEaseDriver::ToString () {

	STLString repr( MOAIAction::ToString ());
	return repr;
}
