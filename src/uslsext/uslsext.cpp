// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/uslsext.h>

//================================================================//
// uslsext
//================================================================//

//----------------------------------------------------------------//
void uslsext::InitGlobals ( USGlobals* globals ) {

	uslscore::InitGlobals ( globals );

	USUrlMgr::Get ();
	USGfxDevice::Get ();
	USVertexFormatMgr::Get ();
	USDrawBuffer::Get ();
}
