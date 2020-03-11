// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIHasDeck.h>
#include <moai-sim/MOAISpriteDeck2D.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getDeck
	@text	Get the deck.
               
	@in		MOAIHasDeck self
	@out	MOAIDeck deck
*/
int MOAIHasDeck::_getDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasDeck, "U" )

	if ( self->mDeck ) {
		self->mDeck->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDeck
	@text	Sets or clears the deck to be indexed by the prop.
	
	@in		MOAIHasDeck self
	@opt	MOAIDeck deck		Default value is nil.
	@out	nil
*/
int MOAIHasDeck::_setDeck ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasDeck, "U" )

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
// MOAIHasDeck
//================================================================//

//----------------------------------------------------------------//
//void MOAIHasDeck::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
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
MOAIHasDeck::MOAIHasDeck () {
		
	RTTI_BEGIN ( MOAIHasDeck )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHasDeck >)
		RTTI_VISITOR ( MOAIAbstractLuaSerializationVisitor, MOAILuaSerializationVisitor < MOAIHasDeck >)
		RTTI_EXTEND ( MOAINode )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasDeck::~MOAIHasDeck () {
	
	this->mDeck.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasDeck::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIHasDeck::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
	
	luaL_Reg regTable [] = {
		{ "getDeck",				_getDeck },
		{ "setDeck",				_setDeck },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIHasDeck::MOAILuaObject_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
	
	this->mDeck.Set ( *this, serializer.MemberIDToObject < MOAIDeck >( state.GetFieldValue < cc8*, MOAISerializerBase::ObjID >( -1, "mDeck", 0 )));
}

//----------------------------------------------------------------//
void MOAIHasDeck::MOAILuaObject_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
	
	state.SetField ( -1, "mDeck", serializer.AffirmMemberID ( this->mDeck ));
}
