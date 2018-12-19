// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIDeckHolderWithIndex.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getIndex
	@text	Gets the value of the deck indexer.
	
	@in		MOAIDeckHolderWithIndex self
	@out	number index
*/
int MOAIDeckHolderWithIndex::_getIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckHolderWithIndex, "U" )

	state.Push ( MOAILuaIndex ( self->mIndex ));

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setIndex
	@text	Set the prop's index into its deck.
	
	@in		MOAIDeckHolderWithIndex self
	@opt	number index		Default value is 1.
	@out	nil
*/
int MOAIDeckHolderWithIndex::_setIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckHolderWithIndex, "U" )

	self->mIndex = state.GetValue < MOAILuaIndex >( 2, ZLIndexOp::ZERO );
	self->ScheduleUpdate ();

	return 0;
}

//================================================================//
// MOAIDeckHolderWithIndex
//================================================================//

//----------------------------------------------------------------//
MOAIDeckHolderWithIndex::MOAIDeckHolderWithIndex () :
	mIndex ( ZLIndexOp::ZERO ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeckHolder )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckHolderWithIndex::~MOAIDeckHolderWithIndex () {
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithIndex::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIDeckHolder::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_INDEX", AttrID::Pack ( ATTR_INDEX ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithIndex::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIDeckHolder::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getIndex",				_getIndex },
		{ "setIndex",				_setIndex },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithIndex::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIDeckHolder::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithIndex::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIDeckHolder::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAIDeckHolderWithIndex::MOAINode_ApplyAttrOp ( MOAIAttrID attrID, MOAIAttribute& attr, u32 op ) {

	if ( AttrID::Check ( attrID )) {
		
		switch (  attrID.Unpack ()) {
			case ATTR_INDEX:
				this->mIndex = attr.Apply ( this->mIndex, op, MOAIAttribute::ATTR_READ_WRITE );
				return true;
		}
	}
	return false;
}
