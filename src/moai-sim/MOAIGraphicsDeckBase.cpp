// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIGraphicsDeckBase.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIGraphicsDeckBase
//================================================================//

//----------------------------------------------------------------//
MOAIGraphicsDeckBase::MOAIGraphicsDeckBase () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIDeck )
		RTTI_EXTEND ( MOAIMaterialBatch )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGraphicsDeckBase::~MOAIGraphicsDeckBase () {
}

//----------------------------------------------------------------//
void MOAIGraphicsDeckBase::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaClass ( state );
	MOAIMaterialBatch::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIGraphicsDeckBase::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIDeck::RegisterLuaFuncs ( state );
	MOAIMaterialBatch::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIGraphicsDeckBase::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAIDeck::SerializeIn ( state, serializer );
	MOAIMaterialBatch::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIGraphicsDeckBase::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAIDeck::SerializeOut ( state, serializer );
	MOAIMaterialBatch::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialBatch* MOAIGraphicsDeckBase::MOAIDeck_GetMaterialBatch () {

	return this;
}

