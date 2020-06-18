// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIHasDeckAndIndex.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getIndex
	@text	Gets the value of the deck indexer.
	
	@in		MOAIHasDeckAndIndex self
	@out	number index
*/
int MOAIHasDeckAndIndex::_getIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasDeckAndIndex, "U" )

	state.Push ( MOAILuaIndex ( self->mIndex ));

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setIndex
	@text	Set the prop's index into its deck.
	
	@in		MOAIHasDeckAndIndex self
	@opt	number index		Default value is 1.
	@out	nil
*/
int MOAIHasDeckAndIndex::_setIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasDeckAndIndex, "U" )

	self->mIndex = state.GetValue < MOAILuaIndex >( 2, 0 );
	self->ScheduleUpdate ();

	return 0;
}

//================================================================//
// MOAIHasDeckAndIndex
//================================================================//

//----------------------------------------------------------------//
MOAIHasDeckAndIndex::MOAIHasDeckAndIndex () :
	mIndex ( 0 ) {
	
	RTTI_BEGIN ( MOAIHasDeckAndIndex )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasDeckAndIndex >)
		RTTI_EXTEND ( MOAIHasDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasDeckAndIndex::~MOAIHasDeckAndIndex () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasDeckAndIndex::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	state.SetField ( -1, "ATTR_INDEX", AttrID::Pack ( ATTR_INDEX ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAIHasDeckAndIndex::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getIndex",				_getIndex },
		{ "setIndex",				_setIndex },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIPickResult MOAIHasDeckAndIndex::MOAIAbstractPickable_PickByPoint ( ZLVec3D loc ) {

	return this->mDeck ? this->mDeck->PickByPoint ( this->mIndex, loc ) : MOAIPickResult ();
}

//----------------------------------------------------------------//
MOAIPickResult MOAIHasDeckAndIndex::MOAIAbstractPickable_PickByRay ( ZLVec3D loc, ZLVec3D normal ) {

	return this->mDeck ? this->mDeck->PickByRay ( this->mIndex, loc, normal ) : MOAIPickResult ();
}

//----------------------------------------------------------------//
bool MOAIHasDeckAndIndex::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {

	if ( AttrID::Check ( attrID )) {
		
		switch ( attrID.Unpack ()) {
			case ATTR_INDEX:
				this->mIndex = attr.Apply ( this->mIndex, op, ZLAttribute::ATTR_READ_WRITE );
				return true;
		}
	}
	return false;
}
