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
	
	RTTI_BEGIN ( MOAITableViewLayer )
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
void MOAITableViewLayer::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAITableLayer, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractViewLayer, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	
	MOAI_CALL_SUPER_ONCE ( composer, MOAITableLayer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractViewLayer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAITableLayer, MOAILuaObject_SerializeIn ( composer, state, serializer ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractViewLayer, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {

	MOAI_CALL_SUPER_ONCE ( composer, MOAITableLayer, MOAILuaObject_SerializeOut ( composer, state, serializer ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIAbstractViewLayer, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}
