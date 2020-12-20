// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPropWithDeckAndIndex.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getIndex
	@text	Gets the value of the deck indexer.
	
	@in		MOAIPropWithDeckAndIndex self
	@out	number index
*/
int MOAIPropWithDeckAndIndex::_getIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPropWithDeckAndIndex, "U" )

	state.Push ( MOAILuaIndex ( self->mIndex ));

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setIndex
	@text	Set the prop's index into its deck.
	
	@in		MOAIPropWithDeckAndIndex self
	@opt	number index		Default value is 1.
	@out	nil
*/
int MOAIPropWithDeckAndIndex::_setIndex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPropWithDeckAndIndex, "U" )

	self->mIndex = state.GetValue < MOAILuaIndex >( 2, 0 );
	self->ScheduleUpdate ();

	MOAI_LUA_RETURN_SELF
}

//================================================================//
// MOAIPropWithDeckAndIndex
//================================================================//

//----------------------------------------------------------------//
MOAIPropWithDeckAndIndex::MOAIPropWithDeckAndIndex () :
	mIndex ( 0 ) {
	
	RTTI_BEGIN ( MOAIPropWithDeckAndIndex )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIPropWithDeckAndIndex >)
		RTTI_EXTEND ( MOAIHasDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIPropWithDeckAndIndex::~MOAIPropWithDeckAndIndex () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPropWithDeckAndIndex::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	state.SetField ( -1, "ATTR_INDEX", AttrID::Pack ( ATTR_INDEX ).ToRaw ());
}

//----------------------------------------------------------------//
void MOAIPropWithDeckAndIndex::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getIndex",				_getIndex },
		{ "setIndex",				_setIndex },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
ZLBounds MOAIPropWithDeckAndIndex::MOAIAbstractProp_GetModelBounds () {
	
	return this->mDeck ? this->mDeck->GetBounds ( this->mIndex ) : ZLBounds::EMPTY;
}


//----------------------------------------------------------------//
MOAIPickResult MOAIPropWithDeckAndIndex::MOAIAbstractProp_PickByPoint ( ZLVec3D loc ) {
	
	return this->mDeck ? this->mDeck->PickByPoint ( this->mIndex, loc ) : MOAIPickResult ();
}

//----------------------------------------------------------------//
MOAIPickResult MOAIPropWithDeckAndIndex::MOAIAbstractProp_PickByRay ( ZLVec3D loc, ZLVec3D normal ) {
	
	return this->mDeck ? this->mDeck->PickByRay ( this->mIndex, loc, normal ) : MOAIPickResult ();
}

//----------------------------------------------------------------//
void MOAIPropWithDeckAndIndex::MOAIAbstractRenderNode_Render ( u32 renderPhase ) {

	switch ( renderPhase ) {
		
		case MOAIAbstractRenderNode::RENDER_PHASE_DRAW:
			this->mDeck->Draw ( this->mIndex );
			break;
		
		case MOAIAbstractRenderNode::RENDER_PHASE_DRAW_DEBUG:
			this->DrawDebug ();
			break;
	}
}

//----------------------------------------------------------------//
bool MOAIPropWithDeckAndIndex::MOAINode_ApplyAttrOp ( ZLAttrID attrID, ZLAttribute& attr, u32 op ) {
	
	if ( AttrID::Check ( attrID )) {
		
		switch ( attrID.Unpack ()) {
			case ATTR_INDEX:
				this->mIndex = attr.Apply ( this->mIndex, op, ZLAttribute::ATTR_READ_WRITE );
				return true;
		}
	}
	
	if ( MOAIAbstractProp::MOAINode_ApplyAttrOp ( attrID, attr, op )) return true;
	return false;
}
