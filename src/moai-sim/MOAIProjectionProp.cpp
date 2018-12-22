// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIProjectionProp.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIProjectionProp::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIProjectionProp, "UUUU" );
	
	MOAIGraphicsPropBase* sourceProp = state.GetLuaObject < MOAIGraphicsPropBase >( 2, true );
	if ( !sourceProp ) return 0;
	
	MOAIAbstractViewLayer* sourceLayer = state.GetLuaObject < MOAIAbstractViewLayer >( 3, true );
	if ( !sourceLayer ) return 0;
	
	MOAIAbstractViewLayer* destLayer = state.GetLuaObject < MOAIAbstractViewLayer >( 4, true );
	if ( !destLayer ) return 0;
	
	self->SetDependentMember ( self->mSourceProp, sourceProp );
	self->SetDependentMember ( self->mSourceLayer, sourceLayer );
	self->SetDependentMember ( self->mDestLayer, destLayer );
	
	return 0;
}

//================================================================//
// MOAIProjectionProp
//================================================================//

//----------------------------------------------------------------//
MOAIProjectionProp::MOAIProjectionProp () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAbstractProp )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIProjectionProp::~MOAIProjectionProp () {
}

//----------------------------------------------------------------//
void MOAIProjectionProp::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIAbstractProp::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIProjectionProp::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAbstractProp::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "init",					_init },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIProjectionProp::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	
	MOAIAbstractProp::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIProjectionProp::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	
	MOAIAbstractProp::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAIProjectionProp::MOAIAbstractDrawable_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );
}

//----------------------------------------------------------------//
void MOAIProjectionProp::MOAIAbstractDrawable_DrawDebug ( int subPrimID ) {
	UNUSED ( subPrimID );
}

//----------------------------------------------------------------//
ZLBounds MOAIProjectionProp::MOAIAbstractProp_GetModelBounds () {

//	return this->mDeck ? this->mDeck->GetBounds ( this->mIndex ) : ZLBounds::EMPTY;
	return ZLBounds::EMPTY;
}

//----------------------------------------------------------------//
void MOAIProjectionProp::MOAINode_Update () {

	MOAIAbstractProp::MOAINode_Update ();
}

//----------------------------------------------------------------//
u32 MOAIProjectionProp::MOAIPartitionHull_AffirmInterfaceMask ( MOAIPartition& partition ) {

	return partition.AffirmInterfaceMask < MOAIAbstractDrawable >();
}
