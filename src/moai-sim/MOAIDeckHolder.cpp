// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckHolder.h>
#include <moai-sim/MOAISpriteDeck2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getDeck
	@text	Get the deck.
               
	@in		MOAIDeckHolder self
	@out	MOAIDeck deck
*/
int MOAIDeckHolder::_getDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckHolder, "U" )

	if ( self->mDeck ) {
		self->mDeck->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDeck
	@text	Sets or clears the deck to be indexed by the prop.
	
	@in		MOAIDeckHolder self
	@opt	MOAIDeck deck		Default value is nil.
	@out	nil
*/
int MOAIDeckHolder::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckHolder, "U" )

	MOAIDeck* deck = MOAISpriteDeck2D::AffirmDeck ( state, 2 );

	self->mDeck.Set ( *self, deck );
	self->ScheduleUpdate ();
	
	if ( deck ) {
		state.Push ( deck );
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIDeckHolder
//================================================================//

//----------------------------------------------------------------//
//void MOAIDeckHolder::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
//	UNUSED ( sampler );

	//if ( !this->mDeck ) return;
	//
	//sampler.SetSourcePrim ( this );
	//
	//if ( this->mGrid ) {
	//	
	//	ZLRect localRect = sampler.GetLocalRect ();
	//	
	//	MOAICellCoord c0;
	//	MOAICellCoord c1;
	//	
	//	ZLRect deckBounds = this->mDeck->GetBounds ().GetRect( ZLBox::PLANE_XY );

	//	this->mGrid->GetBoundsInRect ( localRect, c0, c1, deckBounds );
	//	//this->mDeck->GatherSurfaces ( *this->mGrid, this->mRemapper, this->mGridScale, c0, c1, sampler );
	//}
	//else {
	//	//this->mDeck->GatherSurfaces ( MOAIDeckRemapper::Remap ( this->mRemapper, this->mIndex ), sampler );
	//}
//}

//----------------------------------------------------------------//
MOAIDeckHolder::MOAIDeckHolder () {
	
//	RTTI_BEGIN
//		RTTI_EXTEND ( MOAIPartitionHull )
//	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckHolder::~MOAIDeckHolder () {
	
	this->mDeck.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIDeckHolder::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDeckHolder::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getDeck",				_getDeck },
		{ "setDeck",				_setDeck },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeckHolder::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	this->mDeck.Set ( *this, serializer.MemberIDToObject < MOAIDeck >( state.GetFieldValue < cc8*, MOAISerializerBase::ObjID >( -1, "mDeck", 0 )));
}

//----------------------------------------------------------------//
void MOAIDeckHolder::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	
	state.SetField ( -1, "mDeck", serializer.AffirmMemberID ( this->mDeck ));
}
