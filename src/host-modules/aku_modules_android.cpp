// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <zl-common/zl_platform.h>
#include <host-modules/aku_modules_android.h>

//================================================================//
// implementation
//================================================================//

//----------------------------------------------------------------//
void AKUModulesAndroidAppFinalize () {

	#if AKU_WITH_ANDROID
		AKUAndroidAppFinalize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesAndroidAppInitialize () {

	#if AKU_WITH_ANDROID
		AKUAndroidAppInitialize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesAndroidContextInitialize () {

	#if AKU_WITH_ANDROID
		AKUAndroidContextInitialize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesAndroidPause ( bool pause ) {
	UNUSED ( pause );
}


//----------------------------------------------------------------//
void AKUModulesAndroidUpdate () {
}
