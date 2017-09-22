// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-vr/host.h>
#include <moai-vr/MOAIHeadTransform.h>
#include <moai-vr/MOAIVrMgr.h>

//================================================================//
// host
//================================================================//

//----------------------------------------------------------------//
void AKUVrAppFinalize () {
}

//----------------------------------------------------------------//
void AKUVrAppInitialize () {

	#if MOAI_WITH_LIBGVR
		printf ( "GVR version: %s\n", gvr_get_version_string ());
	#endif
}

//----------------------------------------------------------------//
void AKUVrContextInitialize () {

	#if MOAI_WITH_LIBGVR
		MOAIVrMgr::Affirm ();
		REGISTER_LUA_CLASS ( MOAIHeadTransform )
	#endif
}

//----------------------------------------------------------------//
void AKUVrPause ( bool pause ) {
	UNUSED ( pause );
}

//----------------------------------------------------------------//
void AKUVrUpdate () {

	#if MOAI_WITH_LIBGVR
		MOAIVrMgr::Get ().Update ();
	#endif
}
