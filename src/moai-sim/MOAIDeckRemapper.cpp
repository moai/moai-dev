// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAITileFlags.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	reserve
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
		self->mRemap [ i ] = i;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBase
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
/**	@lua	setRemap
	@text	Remap a single index to a new value.
	
	@in		MOAIDeckRemapper self
	@in		number index		Index to remap.
	@opt	number remap		New value for index. Default value is index (i.e. remove the remap).
	@out	nil
*/
int MOAIDeckRemapper::_setRemap ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckRemapper, "UN" )

	u32 idx		= state.GetValue < u32 >( 2, 1 );
	u32 remap	= state.GetValue < u32 >( 3, idx );
	
	idx			= idx - 1;
	remap		= remap - 1;
	
	u32 code = idx - self->mBase;
	
	if ( code < self->mRemap.Size ()) {
		self->mRemap [ code ] = remap;
	}
	return 0;
}

//================================================================//
// MOAIDeckRemapper
//================================================================//

//----------------------------------------------------------------//
MOAIDeckRemapper::MOAIDeckRemapper () :
	mBase ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAINode )
		RTTI_EXTEND ( MOAIDeckProxy )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckRemapper::~MOAIDeckRemapper () {
}

//----------------------------------------------------------------//
void MOAIDeckRemapper::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );
	MOAIDeckProxy::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeckRemapper::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );
	MOAIDeckProxy::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "reserve",			_reserve },
		{ "setBase",			_setBase },
		{ "setRemap",			_setRemap },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
ZLIndex MOAIDeckRemapper::MOAIDeckProxy_Remap ( ZLIndex idx ) {

	u32 code = idx.mKey - this->mBase;
	
	return ZLIndex (( code < this->mRemap.Size ()) ? this->mRemap [ code ] : idx );
}

//----------------------------------------------------------------//
bool MOAIDeckRemapper::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	u32 code = attrID - this->mBase - 1;

	if ( code < this->mRemap.Size ()) {
		this->mRemap [ code ] = ZLFloat::ToIndex ( attr.Apply (( float )this->mRemap [ code ], op, MOAIAttribute::ATTR_READ_WRITE )) - 1;
		return true;
	}
	return false;
}
