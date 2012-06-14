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
/**	@name	setLink
	@text	Set the ease for a target node attribute.

	@overload	Target is a number.

		@in		MOAIEaseDriver self
		@in		number idx				Index of the link;
		@in		MOAINode target			Target node.
		@in		number attrID			Index of the attribute to be driven.
		@opt	number value			Value for attribute at the end of the ease. Default is 0.
		@opt	number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
										MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
		@out	nil
	
	@overload	Target is a node.

		@in		MOAIEaseDriver self
		@in		number idx				Index of the link;
		@in		MOAINode target			Target node.
		@in		number attrID			Index of the attribute to be driven.
		@in		MOAINode source			Node that you are linking to target.
		@in		number sourceAttrID		Index of the attribute being linked.
		@opt	number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
										MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
		@out	nil

	
*/
int MOAIEaseDriver::_setLink ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIEaseDriver, "UNUN" );
	
	MOAINode* dest = state.GetLuaObject < MOAINode >( 3, true );
	if ( !dest ) return 0;
	
	u32 idx				= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 destAttrID		= state.GetValue < u32 >( 4, 0 );
	
	MOAINode* source = state.GetLuaObject < MOAINode >( 5, true );
	
	if ( source ) {
	
		u32 sourceAttrID	= state.GetValue < u32 >( 6, MOAIAttrOp::NULL_ATTR );
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
MOAIEaseDriver::MOAIEaseDriver () {
	
	RTTI_SINGLE ( MOAITimer )
}

//----------------------------------------------------------------//
MOAIEaseDriver::~MOAIEaseDriver () {
}

//----------------------------------------------------------------//
void MOAIEaseDriver::OnUpdate ( float step ) {
	
	float c0 = this->GetCycle ();
	float t0 = USFloat::Clamp ( this->GetNormalizedTime () - c0, 0.0f, 1.0f );
	
	MOAITimer::OnUpdate ( step );
	if ( step == 0.0f ) return;
	
	float c1 = this->GetCycle ();
	float t1 = USFloat::Clamp ( this->GetNormalizedTime () - c1, 0.0f, 1.0f );

	MOAIAttrOp adder;

	u32 total = this->mLinks.Size ();
	for ( u32 i = 0; i < total; ++i ) {
		
		MOAIEaseDriverLink& link = this->mLinks [ i ];
		if ( link.mDest ) {
			
			float delta = 0.0f;
			
			if ( link.mSource ) {
				if ( this->mDirection > 0.0f ) {
				
					t0 = c0 >= 1.0f ? 1.0f : t0;
					t1 = c1 >= 1.0f ? 1.0f : t1;
					
					float v0 = USInterpolate::Interpolate ( link.mMode, link.mV0, link.mV1, t0 );
					
					link.mV1 = link.mSource->GetAttributeValue ( link.mSourceAttrID, link.mV1 );
					
					float v1 = USInterpolate::Interpolate ( link.mMode, link.mV0, link.mV1, t1 );
					
					delta = v1 - v0;
				}
			}
			else {
				
				float magnitude = ( link.mV1 - link.mV0 );
				if ( magnitude == 0.0f ) continue;
				
				float v0 = link.mV0 + ( magnitude * c0 ) + USInterpolate::Interpolate ( link.mMode, 0.0f, magnitude, t0 );
				float v1 = link.mV0 + ( magnitude * c1 ) + USInterpolate::Interpolate ( link.mMode, 0.0f, magnitude, t1 );
				
				delta = v1 - v0;
			}
			
			if ( delta != 0.0f ) {
				adder.SetValue ( delta );
				link.mDest->ApplyAttrOp ( link.mDestAttrID, adder, MOAIAttrOp::ADD );
				link.mDest->ScheduleUpdate ();
			}
		}
	}
}

//----------------------------------------------------------------//
u32 MOAIEaseDriver::ParseForMove ( MOAILuaState& state, int idx, MOAINode* dest, u32 total, int mode, ... ) {

	float* params = ( float* )alloca ( total * sizeof ( float ));
	u32* destAttrIDs = ( u32* )alloca ( total * sizeof ( u32 ));
	
	va_list args;
	va_start ( args, mode );
	
	u32 count = 0;
	for ( u32 i = 0; i < total; ++i ) {
	
		destAttrIDs [ i ] = ( u32 )va_arg ( args, int );
		float defaultValue = ( float )va_arg ( args, double );
	
		params [ i ] = state.GetValue < float >( idx + i, defaultValue );
		if ( params [ i ] != 0.0f ) {
			count++;
		}
	}
	va_end ( args );

	if ( count ) {
		
		this->ReserveLinks ( count );
		
		u32 linkID = 0;
		for ( u32 i = 0; i < total; ++i ) {
		
			u32 destAttrID = destAttrIDs [ i ];
			float v1 = params [ i ];
			
			if ( v1 != 0.0f ) {
				this->SetLink ( linkID++, dest, destAttrID, v1, ( u32 )mode );
			}
		}
	}
	return count;
}

//----------------------------------------------------------------//
u32 MOAIEaseDriver::ParseForSeek ( MOAILuaState& state, int idx, MOAINode* dest, u32 total, int mode, ... ) {

	float* params = ( float* )alloca ( total * sizeof ( float ));
	u32* destAttrIDs = ( u32* )alloca ( total * sizeof ( u32 ));
	
	va_list args;
	va_start ( args, mode );
	
	u32 count = 0;
	for ( u32 i = 0; i < total; ++i ) {
	
		destAttrIDs [ i ] = ( u32 )va_arg ( args, int );
		float attrValue =  ( float )va_arg ( args, double );
		float defaultValue =  ( float )va_arg ( args, double );
		
		params [ i ] = state.GetValue < float >( idx + i, defaultValue ) - attrValue;
		if ( params [ i ] != 0.0f ) {
			count++;
		}
	}
	va_end ( args );

	if ( count ) {
		
		this->ReserveLinks ( count );
		
		u32 linkID = 0;
		for ( u32 i = 0; i < total; ++i ) {
		
			u32 destAttrID = destAttrIDs [ i ];
			float v1 = params [ i ];
			
			if ( v1 != 0.0f ) {
				this->SetLink ( linkID++, dest, destAttrID, v1, ( u32 )mode );
			}
		}
	}
	return count;
}

//----------------------------------------------------------------//
void MOAIEaseDriver::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITimer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIEaseDriver::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITimer::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserveLinks",			_reserveLinks },
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
		link.mSourceAttrID	= MOAIAttrOp::NULL_ATTR;

		link.mDest			= dest;
		link.mDestAttrID	= destAttrID;
		
		link.mV0			= 0.0f;
		link.mV1			= v1;
		link.mMode			= mode;
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
	}
}
