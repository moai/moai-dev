// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-vr/host.h>
#include <moai-ar/MOAIMarkerMgr.h>

//================================================================//
// host
//================================================================//

//----------------------------------------------------------------//
void AKUArAppFinalize () {
}

//----------------------------------------------------------------//
void AKUArAppInitialize () {

	#if MOAI_WITH_ARTOOLKIT
		//printf ( "GVR version: %s\n", gvr_get_version_string ());
	#endif
}

//----------------------------------------------------------------//
void AKUArContextInitialize () {

	#if MOAI_WITH_ARTOOLKIT
		MOAIMarkerMgr::Affirm ();
		REGISTER_LUA_CLASS ( MOAIMarkerMgr )
	#endif
}

//----------------------------------------------------------------//
void AKUArPause ( bool pause ) {
	UNUSED ( pause );
}
