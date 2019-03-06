// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfxEnum.h>
#include <zl-gfx/ZLVertexBuffer.h>
#include <zl-gfx/ZLVertexFormat.h>

//================================================================//
// ZLVertexBuffer
//================================================================//

//----------------------------------------------------------------//
void ZLVertexBuffer::PrintVertices ( ZLVertexFormat& vertexFormat ) {

	size_t cursor = this->GetCursor ();
	if ( cursor ) {
		this->SetCursor ( 0 );
		vertexFormat.PrintVertices ( *this, cursor );
		this->SetCursor ( cursor );
	}
}

//----------------------------------------------------------------//
ZLVertexBuffer::ZLVertexBuffer () {
	
	RTTI_SINGLE ( ZLGfxBuffer )
	
	this->mTarget = ZGL_BUFFER_TARGET_ARRAY;
}

//----------------------------------------------------------------//
ZLVertexBuffer::~ZLVertexBuffer () {
}
