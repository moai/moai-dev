// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/ZLVertexBuffer.h>

//================================================================//
// ZLVertexBuffer
//================================================================//

//----------------------------------------------------------------//
ZLVertexBuffer::ZLVertexBuffer () {
	
	RTTI_SINGLE ( ZLGfxBuffer )
	
	this->mTarget = ZGL_BUFFER_TARGET_ARRAY;
}

//----------------------------------------------------------------//
ZLVertexBuffer::~ZLVertexBuffer () {
}

//----------------------------------------------------------------//
void ZLVertexBuffer::PrintVertices ( MOAIVertexFormat& vertexFormat ) {

	size_t cursor = this->GetCursor ();
	if ( cursor ) {
		this->SetCursor ( 0 );
		vertexFormat.PrintVertices ( *this, cursor );
		this->SetCursor ( cursor );
	}
}
