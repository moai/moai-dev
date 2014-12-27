// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <zl-common/zl_platform.h>
#include <host-modules/aku_modules_android.h>

//================================================================//
// implementation
//================================================================//

//----------------------------------------------------------------//
void AKUModulesAndroidAppFinalize () {

	#if AKU_WITH_ANDROID_ADCOLONY
		AKUAndroidAdColonyAppFinalize ();
	#endif
	
	#if AKU_WITH_ANDROID_TWITTER
	  AKUAndroidTwitterAppFinalize ();
	#endif

	#if AKU_WITH_ANDROID_CHARTBOOST
		AKUAndroidChartboostAppFinalize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesAndroidAppInitialize () {
	
	#if AKU_WITH_IOS_ADCOLONY
		AKUAndroidAdColonyAppInitialize ();
	#endif
	
	#if AKU_WITH_ANDROID_TWITTER
		AKUAndroidTwitterAppInitialize ();
	#endif

	#if AKU_WITH_ANDROID_CHARTBOOST
		AKUAndroidChartboostAppInitialize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesAndroidContextInitialize () {
	
	#if AKU_WITH_IOS_ADCOLONY
		AKUAndroidAdColonyContextInitialize ();
	#endif
	
	#if AKU_WITH_ANDROID_TWITTER
		AKUAndroidTwitterContextInitialize ();
	#endif
	
	#if AKU_WITH_ANDROID_CHARTBOOST
		AKUAndroidChartboostContextInitialize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesAndroidPause ( bool pause ) {
	UNUSED ( pause );
}


//----------------------------------------------------------------//
void AKUModulesAndroidUpdate () {
}
