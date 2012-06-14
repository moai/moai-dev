// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAITileFlags.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	reserve
	@text	The total number of indices to remap. Index remaps will be
			initialized from 1 to N.
	
	@in		MOAIDeckRemapper self
	@opt	number size		Default value is 0.
	@out	nil
*/
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
/**	@name	setBase
	@text	Set the base offset for the range of indices to remap.
			Used when remapping only a portion of the indices in
			the original deck.
	
	@in		MOAIDeckRemapper self
	@opt	number base		Default value is 0.
	@out	nil
*/
int MOAIDeckRemapper::_setBase ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "U" )
	
	self->mBase = state.GetValue < u32 >( 2, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRemap
	@text	Remap a single index to a new value.
	
	@in		MOAIDeckRemapper self
	@in		number index		Index to remap.
	@opt	number remap		New value for index. Default value is index (i.e. remove the remap).
	@out	nil
*/
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
bool MOAIDeckRemapper::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {
	UNUSED ( attrID );
	UNUSED ( attrOp );

	attrID -=1;
	if (( attrID >= this->mBase ) && ( attrID < this->mRemap.Size ())) {
		this->mRemap [ attrID ] = USFloat::ToIndex ( attrOp.Apply (( float )this->mRemap [ attrID ], op, MOAIAttrOp::ATTR_READ_WRITE ));
		return true;
	}
	return false;
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
void MOAIDeckRemapper::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeckRemapper::RegisterLuaFuncs ( MOAILuaState& state ) {

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

	u32 code = ( idx & MOAITileFlags::CODE_MASK ) - 1;

	if (( code >= this->mBase ) && ( code < this->mRemap.Size ())) {
	
		u32 flags = idx & MOAITileFlags::FLAGS_MASK;
		return ( this->mRemap [ code ] ^ ( flags & MOAITileFlags::FLIP_MASK )) | ( flags & MOAITileFlags::HIDDEN );
	}
	return idx;
}
