// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIDeckHolderWithGrid.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getGrid
	@text	Get the grid currently connected to the prop.
	
	@in		MOAIDeckHolderWithGrid self
	@out	MOAIGrid grid		Current grid or nil.
*/
int MOAIDeckHolderWithGrid::_getGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckHolderWithGrid, "U" )
	
	if ( self->mGrid ) {
		self->mGrid->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGrid
	@text	Sets or clears the prop's grid indexer. The grid indexer (if any)
			will override the standard indexer.
	
	@in		MOAIDeckHolderWithGrid self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIDeckHolderWithGrid::_setGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckHolderWithGrid, "U" )
	
	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2, true );
	if ( !grid ) return 0;
	
	self->mGrid.Set ( *self, grid );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGridScale
	@text	Scale applied to deck items before rendering to grid cell.
	
	@in		MOAIDeckHolderWithGrid self
	@opt	number xScale		Default value is 1.
	@opt	number yScale		Default value is 1.
	@out	nil
*/
int MOAIDeckHolderWithGrid::_setGridScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeckHolderWithGrid, "U" )

	self->mGridScale.mX = state.GetValue < float >( 2, 1.0f );
	self->mGridScale.mY = state.GetValue < float >( 3, 1.0f );
	
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAIDeckHolderWithGrid
//================================================================//

//----------------------------------------------------------------//
//void MOAIDeckHolderWithGrid::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
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
MOAIDeckHolderWithGrid::MOAIDeckHolderWithGrid () :
	mGridScale ( 1.0f, 1.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeckHolder )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeckHolderWithGrid::~MOAIDeckHolderWithGrid () {
	
	this->mGrid.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithGrid::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIDeckHolder::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithGrid::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIDeckHolder::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getGrid",				_getGrid },
		{ "setGrid",				_setGrid },
		{ "setGridScale",			_setGridScale },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithGrid::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIDeckHolder::SerializeIn ( state, serializer );
	
	this->mGrid.Set ( *this, serializer.MemberIDToObject < MOAIGrid >( state.GetFieldValue < cc8*, MOAISerializerBase::ObjID >( -1, "mGrid", 0 )));
}

//----------------------------------------------------------------//
void MOAIDeckHolderWithGrid::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIDeckHolder::SerializeOut ( state, serializer );
	
	state.SetField ( -1, "mGrid", serializer.AffirmMemberID ( this->mGrid ));
}

