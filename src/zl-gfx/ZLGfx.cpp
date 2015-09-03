// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfx.h>

//================================================================//
// ZLGfxListener
//================================================================//

//----------------------------------------------------------------//
void ZLGfxListener::OnGfxEvent ( u32 event, void* userdata ) {
	UNUSED ( event );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
void ZLGfxListener::OnUniformLocation ( u32 addr, void* userdata ) {
	UNUSED ( addr );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
ZLGfxListener::ZLGfxListener () {
}

//----------------------------------------------------------------//
ZLGfxListener::~ZLGfxListener () {
}
