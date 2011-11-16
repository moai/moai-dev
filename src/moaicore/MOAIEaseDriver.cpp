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
	MOAI_LUA_SETUP ( MOAIEaseDriver, "UNUN" );
	
	MOAINode* dest = state.GetLuaObject < MOAINode >( 3 );
	if ( !dest ) return 0;
	
	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 destAttrID		= state.GetValue < u32 >( 4, 0 );
	
	MOAINode* source = state.GetLuaObject < MOAINode >( 5 );
	
	if ( source ) {
	
		u32 sourceAttrID	= state.GetValue < u32 >( 6, MOAINode::NULL_ATTR );
		u32 mode			= state.GetValue < u32 >( 7, USInterpolate::kSmooth );
		
		self->SetLink ( idx, dest, destAttrID, source, sourceAttrID, mode );
	}
	else {
	
		float v1			= state.GetValue < float >( 5, 0.0f );
		u32 mode			= state.GetValue < u32 >( 6, USInterpolate::kSmooth );
		
		self->SetLink ( idx, dest, destAttrID, v1, mode );
	}
	return 0;
}

//================================================================//
// MOAIEaseDriver
//================================================================//

//----------------------------------------------------------------//
void MOAIEaseDriver::Apply ( float step ) {

	if ( step == 0.0f ) return;
	if ( this->mTime >= 1.0f ) return;
	
	this->mTime += step;
	
	if ( this->mTime > 1.0f ) {
		this->mTime = 1.0f;
	}

	MOAIAttrOp adder;

	u32 total = this->mLinks.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIEaseDriverLink& link = this->mLinks [ i ];
		if ( link.mDest ) {
			
			if ( link.mSource ) {
				link.mV1 = link.mSource->GetAttributeValue ( link.mSourceAttrID, link.mV1 );
			}
			
			float value = USInterpolate::Interpolate ( link.mMode, link.mV0, link.mV1, this->mTime );
			float delta = value - link.mValue;
			link.mValue = value;
			
			if ( delta != 0.0f ) {
				adder.SetValue ( delta );
				link.mDest->ApplyAttrOp ( link.mDestAttrID, adder, MOAIAttrOp::ADD );
				link.mDest->ScheduleUpdate ();
			}
		}
	}
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
void MOAIEaseDriver::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIEaseDriver::RegisterLuaFuncs ( MOAILuaState& state ) {
	
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
void MOAIEaseDriver::SetLink ( u32 idx, MOAINode* dest, u32 destAttrID, float v1, u32 mode  ) {

	if ( idx < this->mLinks.Size ()) {
		
		MOAIEaseDriverLink& link = this->mLinks [ idx ];

		link.mSource		= 0;
		link.mSourceAttrID	= MOAINode::NULL_ATTR;

		link.mDest			= dest;
		link.mDestAttrID	= destAttrID;
		
		link.mV0			= 0.0f;
		link.mV1			= v1;
		link.mMode			= mode;
		
		link.mValue			= link.mV0;
	}
}

//----------------------------------------------------------------//
void MOAIEaseDriver::SetLink ( u32 idx, MOAINode* dest, u32 destAttrID, MOAINode* source, u32 sourceAttrID, u32 mode ) {

	if ( idx < this->mLinks.Size ()) {
		
		MOAIEaseDriverLink& link = this->mLinks [ idx ];

		link.mSource		= source;
		link.mSourceAttrID	= sourceAttrID;

		link.mDest			= dest;
		link.mDestAttrID	= destAttrID;
		
		link.mV0			= dest->GetAttributeValue ( destAttrID, 0.0f );
		link.mV1			= source->GetAttributeValue ( sourceAttrID, 0.0f );
		link.mMode			= mode;
		
		link.mValue			= link.mV0;
	}
}
