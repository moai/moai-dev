// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIVertexArray.h>

//================================================================//
// ZLShaderGL
//================================================================//

//----------------------------------------------------------------//
MOAIVertexBuffer* MOAIVertexArray::GetVertexBuffer ( ZLIndex idx ) {

	return MOAIVertexArray_GetVertexBuffer ( idx );
}

//----------------------------------------------------------------//
MOAIVertexFormat* MOAIVertexArray::GetVertexFormat ( ZLIndex idx ) {

	return MOAIVertexArray_GetVertexFormat ( idx );
}

//----------------------------------------------------------------//
MOAIVertexArray::MOAIVertexArray () {

	RTTI_SINGLE ( MOAIGfxResource )
}

//----------------------------------------------------------------//
MOAIVertexArray::~MOAIVertexArray () {
}

//----------------------------------------------------------------//
void MOAIVertexArray::SetVertexBuffer ( ZLIndex idx, MOAIVertexBuffer* vtxBuffer, MOAIVertexFormat* vtxFormat ) {

	this->MOAIVertexArray_SetVertexBuffer ( idx, vtxBuffer, vtxFormat );
}
