// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfx.h>

//================================================================//
// ZLGfxListener
//================================================================//

//----------------------------------------------------------------//
void ZLGfxListener::ZLGfxListener_OnGfxEvent ( u32 event, void* userdata ) {
	UNUSED ( event );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
void ZLGfxListener::ZLGfxListener_OnReadPixels ( const ZLCopyOnWrite& copyOnWrite, void* userdata ) {
	UNUSED ( copyOnWrite );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
void ZLGfxListener::ZLGfxListener_OnUniformLocation ( u32 addr, void* userdata ) {
	UNUSED ( addr );
	UNUSED ( userdata );
}

//----------------------------------------------------------------//
ZLGfxListener::ZLGfxListener () {
}

//----------------------------------------------------------------//
ZLGfxListener::~ZLGfxListener () {
}
