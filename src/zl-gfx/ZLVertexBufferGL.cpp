// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/ZLGfxEnum.h>
#include <zl-gfx/ZLVertexBufferGL.h>
#include <zl-gfx/ZLVertexFormatGL.h>

//================================================================//
// ZLVertexBufferGL
//================================================================//

//----------------------------------------------------------------//
void ZLVertexBufferGL::PrintVertices ( ZLVertexFormatGL& vertexFormat ) {

	size_t cursor = this->GetCursor ();
	if ( cursor ) {
		this->SetCursor ( 0 );
		vertexFormat.PrintVertices ( *this, cursor );
		this->SetCursor ( cursor );
	}
}

//----------------------------------------------------------------//
ZLVertexBufferGL::ZLVertexBufferGL () {
	
	RTTI_SINGLE ( ZLGfxBufferGL )
	
	this->mTarget = ZGL_BUFFER_TARGET_ARRAY;
}

//----------------------------------------------------------------//
ZLVertexBufferGL::~ZLVertexBufferGL () {
}
