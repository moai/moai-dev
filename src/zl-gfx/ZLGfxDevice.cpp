// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/headers.h>
#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLGfxDevice.h>

//================================================================//
// ZLGfxDevice
//================================================================//

//----------------------------------------------------------------//
void ZLGfxDevice::Begin () {

	zglBegin ();
}

//----------------------------------------------------------------//
void ZLGfxDevice::End () {

	zglEnd ();
}

//----------------------------------------------------------------//
void ZLGfxDevice::Finalize () {

	zglFinalize ();
}

//----------------------------------------------------------------//
u32 ZLGfxDevice::GetCap ( u32 cap ) {

	return zglGetCap ( cap );
}

//----------------------------------------------------------------//
u32 ZLGfxDevice::GetError () {

	return zglGetError ();
}

//----------------------------------------------------------------//
cc8* ZLGfxDevice::GetErrorString ( u32 error ) {

	return zglGetErrorString ( error );
}

//----------------------------------------------------------------//
cc8* ZLGfxDevice::GetString ( u32 stringID ) {

	return zglGetString ( stringID );
}

//----------------------------------------------------------------//
void ZLGfxDevice::Initialize () {

	zglInitialize ();
}
