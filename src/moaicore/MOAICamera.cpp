// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAICamera.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// MOAICamera
//================================================================//

//----------------------------------------------------------------//
USMatrix4x4 MOAICamera::GetViewMtx () {

	USMatrix4x4 mtx;
	mtx.Init ( this->GetWorldToLocalMtx ());
	return mtx;
}
	
//----------------------------------------------------------------//
MOAICamera::MOAICamera () {

	RTTI_SINGLE ( MOAITransform )
}

//----------------------------------------------------------------//
MOAICamera::~MOAICamera () {
}

//----------------------------------------------------------------//
void MOAICamera::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITransform::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAICamera::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAITransform::RegisterLuaFuncs ( state );
}
