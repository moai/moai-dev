// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITraits.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setTraitMask
	@text	Mask which traits will be inherited. See class documentation
			for a list of trait bitmasks.
	
	@in		MOAITraits self
	@opt	MOAITransformBase mask	Default value is MOAITraits.INHERIT_TRANSFORM + MOAITraits.INHERIT_COLOR + MOAITraits.INHERIT_VISIBLE.
	@out	nil
*/
int MOAITraits::_setTraitMask ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITraits, "U" )
	
	self->mTraitMask = state.GetValue < u32 >( 2, DEFAULT_MASK );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTraitSource
	@text	Sets or clears the trait source.
	
	@in		MOAITraits self
	@opt	MOAITransformBase traitSource	Default value is nil.
	@out	nil
*/
int MOAITraits::_setTraitSource ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITraits, "U" )
	
	MOAITraits* traitSource = state.GetLuaObject < MOAITraits >( 2 );
	self->SetTraitSource ( traitSource );
	
	return 0;
}


//================================================================//
// MOAITraits
//================================================================//

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
MOAIPartition* MOAITraits::GetPartitionTrait () {
	return 0;
}

//----------------------------------------------------------------//
MOAIShader* MOAITraits::GetShaderTrait () {
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
MOAITraits::MOAITraits () :
	mTraitMask ( DEFAULT_MASK ) {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAITraits::~MOAITraits () {
}

//----------------------------------------------------------------//
void MOAITraits::RegisterLuaClass ( USLuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
	
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
		{ "setTraitMask",			_setTraitMask },
		{ "setTraitSource",			_setTraitSource },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITraits::SetTraitSource ( MOAITraits* traitSource ) {

	this->SetDependentMember < MOAITraits >( this->mTraitSource, traitSource );
}

//----------------------------------------------------------------//
STLString MOAITraits::ToString () {

	STLString repr;
	return repr;
}
