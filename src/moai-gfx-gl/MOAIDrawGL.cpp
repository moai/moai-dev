// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIDrawGL.h>

//================================================================//
// MOAIAbstractDrawingObjectGL
//================================================================//

//----------------------------------------------------------------//
MOAIDrawGL::MOAIDrawGL () {
}

//----------------------------------------------------------------//
MOAIDrawGL::~MOAIDrawGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDrawGL::MOAIAbstractDrawingAPI_RetainObject ( ZLRefCountedObject* object ) {
	UNUSED ( object );
}

//----------------------------------------------------------------//
void MOAIDrawGL::MOAIAbstractDrawingAPI_SubmitCommand ( MOAIDrawingAPIEnum::_ cmd, const void* param, ZLSize size ) {

	MOAIDrawingAPI::Execute ( 0, cmd, param );
}
