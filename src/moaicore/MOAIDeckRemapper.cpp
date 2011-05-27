// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIDeckRemapper.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
int MOAIDeckRemapper::_reserve ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "U" )

	u32 size = state.GetValue < u32 >( 2, 0 );
	self->mRemap.Init ( size );
	
	for ( u32 i = 0; i < size; ++i ) {
		self->mRemap [ i ] = i + 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIDeckRemapper::_setBase ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "U" )
	
	self->mBase = state.GetValue < u32 >( 2, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIDeckRemapper::_setRemap ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "UN" )

	u32 idx		= state.GetValue < u32 >( 2, 1 ) - 1;
	u32 remap	= state.GetValue < u32 >( 3, idx );
	
	if ( idx < self->mRemap.Size ()) {
		self->mRemap [ idx ] = remap;
	}
	return 0;
}

//================================================================//
// MOAIDeckRemapper
//================================================================//

//----------------------------------------------------------------//
void MOAIDeckRemapper::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {
	UNUSED ( attrID );
	UNUSED ( attrOp );

	attrID -=1;
	if (( attrID >= this->mBase ) && ( attrID < this->mRemap.Size ())) {
		this->mRemap [ attrID ] = attrOp.Op ( this->mRemap [ attrID ]);
	}
}

//----------------------------------------------------------------//
MOAIDeckRemapper::MOAIDeckRemapper () :
	mBase ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckRemapper::~MOAIDeckRemapper () {
}

//----------------------------------------------------------------//
void MOAIDeckRemapper::RegisterLuaClass ( USLuaState& state ) {

	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeckRemapper::RegisterLuaFuncs ( USLuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserve",			_reserve },
		{ "setBase",			_setBase },
		{ "setRemap",			_setRemap },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
u32 MOAIDeckRemapper::Remap ( u32 idx ) {

	u32 code = ( idx & USTile::CODE_MASK ) - 1;

	if (( code >= this->mBase ) && ( code < this->mRemap.Size ())) {
	
		u32 flags = idx & USTile::FLAGS_MASK;
		return ( this->mRemap [ code ] ^ ( flags & USTile::FLIP_MASK )) | ( flags & USTile::HIDDEN );
	}
	return idx;
}

//----------------------------------------------------------------//
STLString MOAIDeckRemapper::ToString () {

	STLString repr;

	return repr;
}
