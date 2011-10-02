// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITraits.h>

//================================================================//
// MOAITraitsBuffer
//================================================================//

//----------------------------------------------------------------//
MOAIBlendMode MOAITraitsBuffer::GetBlendModeTrait () {

	if ( this->mMask & MOAITraits::INHERIT_BLEND_MODE ) {
		return this->mSources [ SOURCE_BLEND_MODE ]->GetBlendModeTrait ();
	}
	MOAIBlendMode blendMode;
	return blendMode;
}

//----------------------------------------------------------------//
USColorVec MOAITraitsBuffer::GetColorTrait () {

	if ( this->mMask & MOAITraits::INHERIT_COLOR ) {
		return this->mSources [ SOURCE_COLOR ]->GetColorTrait ();
	}
	static USColorVec color ( 1.0f, 1.0f, 1.0f, 1.0f );
	return color;
}

//----------------------------------------------------------------//
USRect* MOAITraitsBuffer::GetFrameTrait () {

	return this->mMask & MOAITraits::INHERIT_FRAME ? this->mSources [ SOURCE_FRAME ]->GetFrameTrait () : 0;
}

//----------------------------------------------------------------//
const USAffine2D* MOAITraitsBuffer::GetLocTrait () {

	return this->mMask & MOAITraits::INHERIT_LOC ? this->mSources [ SOURCE_LOC ]->GetLocTrait () : 0;
}

//----------------------------------------------------------------//
MOAIPartition* MOAITraitsBuffer::GetPartitionTrait () {

	return this->mMask & MOAITraits::INHERIT_PARTITION ? this->mSources [ SOURCE_PARTITION ]->GetPartitionTrait () : 0;
}

//----------------------------------------------------------------//
MOAIShader* MOAITraitsBuffer::GetShaderTrait () {

	return this->mMask & MOAITraits::INHERIT_SHADER ? this->mSources [ SOURCE_SHADER ]->GetShaderTrait () : 0;
}

//----------------------------------------------------------------//
const USAffine2D* MOAITraitsBuffer::GetTransformTrait () {

	return this->mMask & MOAITraits::INHERIT_TRANSFORM ? this->mSources [ SOURCE_TRANSFORM ]->GetTransformTrait () : 0;
}

//----------------------------------------------------------------//
bool MOAITraitsBuffer::GetVisibleTrait () {

	return this->mMask & MOAITraits::INHERIT_VISIBLE ? this->mSources [ SOURCE_VISIBLE ]->GetVisibleTrait () : true;
}

//----------------------------------------------------------------//
bool MOAITraitsBuffer::HasTrait ( u32 mask ) {

	return (( this->mMask & mask ) != 0 );
}

//----------------------------------------------------------------//
bool MOAITraitsBuffer::HasTraits () {

	return ( this->mMask != 0 );
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setTraitSource
	@text	Sets or clears the trait source.
	
	@in		MOAITraits self
	@opt	MOAITransformBase traitSource	Default value is nil.
	@opt	number mask						Default value is 0xffffffff (all traits).
	@out	nil
*/
int MOAITraits::_setTraitSource ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITraits, "U" )
	
	MOAITraits* traitSource	= state.GetLuaObject < MOAITraits >( 2 );
	u32 mask				= state.GetValue < u32 >( 3, ALL_TRAITS );

	self->SetTraitSource ( traitSource, mask );
	
	return 0;
}

//================================================================//
// MOAITraits
//================================================================//

//----------------------------------------------------------------//
void MOAITraits::AccumulateSources ( MOAITraitsBuffer& buffer, u32 traitsMask ) {

	buffer.mMask = 0;
	
	if ( this->mSourceList ) {
		
		for ( u32 i = 0; i < MOAITraitsBuffer::TOTAL_SOURCES; ++i ) {
			
			MOAITraitLink* link = this->mSourceList;
			for ( ; link; link = link->mNext ) {
				
				if ( link->mSource ) {
					
					u32 mask = (( link->mMask & traitsMask ) & MOAITraitsBuffer::MASK_TO_8_BITS);
					buffer.mMask |= mask;
					
					for ( u32 i = 0; mask; i++, mask = mask >> 1 ) {
					
						if ( mask & 0x01 ) {
							buffer.mSources [ i ] = link->mSource;	
						}
					}
				}
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAITraits::ClearTraitSources () {

	MOAITraitLink* cursor = this->mSourceList;
	this->mSourceList = 0;
	
	for ( ; cursor;  ) {
		MOAITraitLink* link = cursor;
		cursor = cursor->mNext;
		
		this->SetDependentMember < MOAITraits >( link->mSource, 0 );
		delete link;
	}
}

//----------------------------------------------------------------//
MOAIBlendMode MOAITraits::GetBlendModeTrait () {

	MOAIBlendMode blendMode;
	return blendMode;
}

//----------------------------------------------------------------//
USColorVec MOAITraits::GetColorTrait () {

	static USColorVec color ( 1.0f, 1.0f, 1.0f, 1.0f );
	return color;
}

//----------------------------------------------------------------//
USRect* MOAITraits::GetFrameTrait () {
	return 0;
}

//----------------------------------------------------------------//
const USAffine2D* MOAITraits::GetLocTrait () {
	return 0;
}

//----------------------------------------------------------------//
MOAIPartition* MOAITraits::GetPartitionTrait () {
	return 0;
}

//----------------------------------------------------------------//
MOAIShader* MOAITraits::GetShaderTrait () {
	return 0;
}

//----------------------------------------------------------------//
MOAITraits* MOAITraits::GetTraitSource ( u32 mask ) {

	MOAITraitLink* link = this->mSourceList;
	for ( ; link; link = link->mNext ) {
		if ( link->mMask & mask ) return link->mSource;
	}
	return 0;
}

//----------------------------------------------------------------//
const USAffine2D* MOAITraits::GetTransformTrait () {
	return 0;
}

//----------------------------------------------------------------//
bool MOAITraits::GetVisibleTrait () {

	return true;
}

//----------------------------------------------------------------//
bool MOAITraits::HasTraitsSource () {

	return this->mSourceList != 0;
}

//----------------------------------------------------------------//
MOAITraits::MOAITraits () :
	mSourceList ( 0 ) {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAITraits::~MOAITraits () {

	MOAITraitLink* cursor = this->mSourceList;
	this->mSourceList = 0;
	
	for ( ; cursor;  ) {
		MOAITraitLink* link = cursor;
		cursor = cursor->mNext;

		delete link;
	}
}

//----------------------------------------------------------------//
void MOAITraits::RegisterLuaClass ( USLuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
	
	state.SetField ( -1, "INHERIT_ALL", ( u32 )ALL_TRAITS );
	state.SetField ( -1, "INHERIT_LOC", ( u32 )INHERIT_LOC );
	state.SetField ( -1, "INHERIT_TRANSFORM", ( u32 )INHERIT_TRANSFORM );
	state.SetField ( -1, "INHERIT_COLOR", ( u32 )INHERIT_COLOR );
	state.SetField ( -1, "INHERIT_FRAME", ( u32 )INHERIT_FRAME );
	state.SetField ( -1, "INHERIT_PARTITION", ( u32 )INHERIT_PARTITION );
	state.SetField ( -1, "INHERIT_SHADER", ( u32 )INHERIT_SHADER );
	state.SetField ( -1, "INHERIT_VISIBLE", ( u32 )INHERIT_VISIBLE );
}

//----------------------------------------------------------------//
void MOAITraits::RegisterLuaFuncs ( USLuaState& state ) {
	
	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "setTraitSource",			_setTraitSource },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITraits::SetTraitSource ( MOAITraits* source, u32 mask ) {

	bool foundLink = false;

	// clear out any links that match mask
	MOAITraitLink* cursor = this->mSourceList;
	this->mSourceList = 0;
	
	for ( ; cursor;  ) {
		MOAITraitLink* link = cursor;
		cursor = cursor->mNext;
		
		if ( link->mSource ) {
			if ( link->mSource == source ) {
				link->mMask |= mask; // set mask
				foundLink = true;
			}
			else {
				link->mMask &= ~mask; // clear mask
			}
		}
		else {
			link->mMask = 0;
		}
		
		if ( link->mMask ) {
			link->mNext = this->mSourceList;
			this->mSourceList = link;
		}
		else {
			this->SetDependentMember < MOAITraits >( link->mSource, 0 );
			delete link;
		}
	}

	if ( source && !foundLink ) {
		
		MOAITraitLink* link = new MOAITraitLink ();
		this->SetDependentMember < MOAITraits >( link->mSource, source );
		link->mMask = mask;
		
		link->mNext = this->mSourceList;
		this->mSourceList = link;
	}

	this->ForceUpdate ();
}

//----------------------------------------------------------------//
STLString MOAITraits::ToString () {

	STLString repr;
	return repr;
}
