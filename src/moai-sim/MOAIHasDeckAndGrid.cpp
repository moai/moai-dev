// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIHasDeckAndGrid.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getGrid
	@text	Get the grid currently connected to the prop.
	
	@in		MOAIHasDeckAndGrid self
	@out	MOAIGrid grid		Current grid or nil.
*/
int MOAIHasDeckAndGrid::_getGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasDeckAndGrid, "U" )
	
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
	
	@in		MOAIHasDeckAndGrid self
	@opt	MOAIGrid grid		Default value is nil.
	@out	nil
*/
int MOAIHasDeckAndGrid::_setGrid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasDeckAndGrid, "U" )
	
	MOAIGrid* grid = state.GetLuaObject < MOAIGrid >( 2, true );
	if ( !grid ) return 0;
	
	self->mGrid.Set ( *self, grid );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGridScale
	@text	Scale applied to deck items before rendering to grid cell.
	
	@in		MOAIHasDeckAndGrid self
	@opt	number xScale		Default value is 1.
	@opt	number yScale		Default value is 1.
	@out	nil
*/
int MOAIHasDeckAndGrid::_setGridScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasDeckAndGrid, "U" )

	self->mGridScale.mX = state.GetValue < float >( 2, 1.0f );
	self->mGridScale.mY = state.GetValue < float >( 3, 1.0f );
	
	self->ScheduleUpdate ();
	
	return 0;
}

//================================================================//
// MOAIHasDeckAndGrid
//================================================================//

//----------------------------------------------------------------//
//void MOAIHasDeckAndGrid::GatherSurfaces ( MOAISurfaceSampler2D& sampler ) {
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
MOAIHasDeckAndGrid::MOAIHasDeckAndGrid () :
	mGridScale ( 1.0f, 1.0f ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIHasDeck )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasDeckAndGrid::~MOAIHasDeckAndGrid () {
	
	this->mGrid.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasDeckAndGrid::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeck, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIHasDeckAndGrid::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeck, MOAILuaObject_RegisterLuaFuncs ( composer, state ));

	luaL_Reg regTable [] = {
		{ "getGrid",				_getGrid },
		{ "setGrid",				_setGrid },
		{ "setGridScale",			_setGridScale },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIHasDeckAndGrid::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeck, MOAILuaObject_SerializeIn ( composer, state, serializer ));
	
	this->mGrid.Set ( *this, serializer.MemberIDToObject < MOAIGrid >( state.GetFieldValue < cc8*, MOAISerializerBase::ObjID >( -1, "mGrid", 0 )));
}

//----------------------------------------------------------------//
void MOAIHasDeckAndGrid::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAIHasDeck, MOAILuaObject_SerializeOut ( composer, state, serializer ));
	
	state.SetField ( -1, "mGrid", serializer.AffirmMemberID ( this->mGrid ));
}

