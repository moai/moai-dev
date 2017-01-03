// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckPropBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getDeck
	@text	Get the deck.
               
	@in		MOAIDeckPropBase self
	@out	MOAIDeck deck
*/
int MOAIDeckPropBase::_getDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckPropBase, "U" )

	if ( self->mDeck ) {
		self->mDeck->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDeck
	@text	Sets or clears the deck to be indexed by the prop.
	
	@in		MOAIDeckPropBase self
	@opt	MOAIDeck deck		Default value is nil.
	@out	nil
*/
int MOAIDeckPropBase::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckPropBase, "U" )

	self->mDeck.Set ( *self, state.GetLuaObject < MOAIDeck >( 2, true ));
	self->ScheduleUpdate ();
	return 0;
}

//================================================================//
// MOAIDeckPropBase
//================================================================//

//----------------------------------------------------------------//
//void MOAIDeckPropBase::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
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
MOAIDeckPropBase::MOAIDeckPropBase () {
	
//	RTTI_BEGIN
//		RTTI_EXTEND ( MOAIPartitionHull )
//	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckPropBase::~MOAIDeckPropBase () {
	
	this->mDeck.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIDeckPropBase::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDeckPropBase::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getDeck",				_getDeck },
		{ "setDeck",				_setDeck },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeckPropBase::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	this->mDeck.Set ( *this, serializer.MemberIDToObject < MOAIDeck >( state.GetFieldValue < MOAISerializerBase::ObjID >( -1, "mDeck", 0 )));
}

//----------------------------------------------------------------//
void MOAIDeckPropBase::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	state.SetField ( -1, "mDeck", serializer.AffirmMemberID ( this->mDeck ));
}
