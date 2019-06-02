// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIDeckHolderWithIndex.h>

//================================================================//
// lua
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

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIDeckHolderWithIndex::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	if ( AttrID::Check ( attrID )) {
		
		switch ( attrID.Unpack ()) {
			case ATTR_INDEX:
				this->mIndex = attr.Apply ( this->mIndex, op, ZLAttribute::ATTR_READ_WRITE );
				return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithIndex::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIDeckHolder, MOAILuaObject_RegisterLuaClass ( composer, state ));
	
	state.SetField ( -1, "ATTR_INDEX", AttrID::Pack ( ATTR_INDEX ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithIndex::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIDeckHolder, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "getIndex",				_getIndex },
		{ "setIndex",				_setIndex },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithIndex::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIDeckHolder, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithIndex::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAIDeckHolder, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}
