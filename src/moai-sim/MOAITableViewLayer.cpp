// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAITableViewLayer.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAITransform.h>

//================================================================//
// local
//==========================================================s======//

//----------------------------------------------------------------//

//================================================================//
// MOAITableViewLayer
//================================================================//

//----------------------------------------------------------------//
MOAITableViewLayer::MOAITableViewLayer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITableLayer )
		RTTI_EXTEND ( MOAIAbstractViewLayer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITableViewLayer::~MOAITableViewLayer () {
}

//----------------------------------------------------------------//
void MOAITableViewLayer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITableLayer::RegisterLuaClass ( state );
	MOAIAbstractViewLayer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITableViewLayer::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITableLayer::RegisterLuaFuncs ( state );
	MOAIAbstractViewLayer::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAITableViewLayer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAITableLayer::SerializeIn ( state, serializer );
	MOAIAbstractViewLayer::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAITableViewLayer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAITableLayer::SerializeOut ( state, serializer );
	MOAIAbstractViewLayer::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAIAbstractDrawable_Draw ( int subPrimID ) {

	this->MOAIAbstractViewLayer::MOAIAbstractDrawable_Draw ( subPrimID );
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAIAbstractViewLayer_Draw () {
	
	MOAIAbstractDrawable::Draw ( this->mRenderTable );
	
	if ( MOAIDebugLinesMgr::Get ().IsVisible () && this->mShowDebugLines ) {
		MOAIAbstractDrawable::Draw ( this->mRenderTable, true );
	}
}
