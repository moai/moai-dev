// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIIndexedPropBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getIndex
	@text	Gets the value of the deck indexer.
	
	@in		MOAIIndexedPropBase self
	@out	number index
*/
int MOAIIndexedPropBase::_getIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexedPropBase, "U" )

	state.Push ( MOAILuaIndex ( self->mIndex ));

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setIndex
	@text	Set the prop's index into its deck.
	
	@in		MOAIIndexedPropBase self
	@opt	number index		Default value is 1.
	@out	nil
*/
int MOAIIndexedPropBase::_setIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIIndexedPropBase, "U" )

	self->mIndex = state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	self->ScheduleUpdate ();

	return 0;
}

//================================================================//
// MOAIIndexedPropBase
//================================================================//

//----------------------------------------------------------------//
MOAIIndexedPropBase::MOAIIndexedPropBase () :
	mIndex ( ZLIndexOp::ZERO ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeckPropBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIIndexedPropBase::~MOAIIndexedPropBase () {
}

//----------------------------------------------------------------//
void MOAIIndexedPropBase::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIDeckPropBase::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_INDEX",					MOAIIndexedPropBaseAttr::Pack ( ATTR_INDEX ));
}

//----------------------------------------------------------------//
void MOAIIndexedPropBase::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIDeckPropBase::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getIndex",				_getIndex },
		{ "setIndex",				_setIndex },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIIndexedPropBase::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIDeckPropBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIIndexedPropBase::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIDeckPropBase::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIIndexedPropBase::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	if ( MOAIIndexedPropBaseAttr::Check ( attrID )) {
		
		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_INDEX:
				this->mIndex = attr.Apply ( this->mIndex, op, MOAIAttribute::ATTR_READ_WRITE );
				return true;
		}
	}
	return false;
}
