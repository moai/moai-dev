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
// lua
//==========================================================s======//

//----------------------------------------------------------------//

//================================================================//
// MOAITableViewLayer
//================================================================//

//----------------------------------------------------------------//
MOAITableViewLayer::MOAITableViewLayer () {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAITableViewLayer )
		RTTI_EXTEND ( MOAITableLayer )
		RTTI_EXTEND ( MOAIAbstractViewLayer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITableViewLayer::~MOAITableViewLayer () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAIAbstractViewLayer_Draw () {
	
	MOAIDrawable::Draw ( this->mRenderTable );
	
	if ( MOAIDebugLinesMgr::Get ().IsVisible () && this->mShowDebugLines ) {
		MOAIDrawable::Draw ( this->mRenderTable, true );
	}
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAIDrawable_Draw ( int subPrimID ) {

	this->MOAIAbstractViewLayer::MOAIDrawable_Draw ( subPrimID );
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAILuaObject_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAILuaObject_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
}
