// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <string.h>
#include <moai-core/headers.h>
#include <host-modules/aku_modules_android.h>

//================================================================//
// implementation
//================================================================//

//----------------------------------------------------------------//
void AKUModulesAndroidAppFinalize () {

	#if AKU_WITH_ANDROID
//		AKUAndroidAppFinalize ();
	#endif

	#if AKU_WITH_ANDROID_ADCOLONY
//		AKUAndroidAdColonyAppFinalize ();
	#endif

	#if AKU_WITH_ANDROID_BILLING
//		AKUAndroidBillingAppFinalize ();
	#endif

	#if AKU_WITH_ANDROID_CHARTBOOST
		AKUAndroidChartBoostAppFinalize ();
	#endif

	#if AKU_WITH_ANDROID_CRITTERCISM
//		AKUAndroidCrittercismAppFinalize ();
	#endif

	#if AKU_WITH_ANDROID_FACEBOOK
//		AKUAndroidFacebookAppFinalize ();
	#endif
	
	#if AKU_WITH_ANDROID_FLURRY
//		AKUAndroidFlurryAppFinalize ();
	#endif

	#if AKU_WITH_ANDROID_TAPJOY
//		AKUAndroidTapjoyAppFinalize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesAndroidAppInitialize () {

	#if AKU_WITH_ANDROID
//		AKUAndroidAppInitialize ();
	#endif

	#if AKU_WITH_ANDROID_ADCOLONY
//		AKUAndroidAdColonyAppInitialize ();
	#endif

	#if AKU_WITH_ANDROID_BILLING
//		AKUAndroidBillingAppInitialize ();
	#endif

	#if AKU_WITH_ANDROID_CHARTBOOST
		AKUAndroidChartBoostAppInitialize ();
	#endif

	#if AKU_WITH_ANDROID_CRITTERCISM
//		AKUAndroidCrittercismAppInitialize ();
	#endif

	#if AKU_WITH_ANDROID_FACEBOOK
//		AKUAndroidFacebookAppInitialize ();
	#endif
	
	#if AKU_WITH_ANDROID_FLURRY
//		AKUAndroidFlurryAppInitialize ();
	#endif

	#if AKU_WITH_ANDROID_TAPJOY
//		AKUAndroidTapjoyAppInitialize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesAndroidContextInitialize () {

	#if AKU_WITH_ANDROID
//		AKUAndroidContextInitialize ();
	#endif

	#if AKU_WITH_ANDROID_ADCOLONY
//		AKUAndroidAdColonyContextInitialize ();
	#endif

	#if AKU_WITH_ANDROID_BILLING
//		AKUAndroidBillingContextInitialize ();
	#endif

	#if AKU_WITH_ANDROID_CHARTBOOST
		AKUAndroidChartBoostContextInitialize ();
	#endif

	#if AKU_WITH_ANDROID_CRITTERCISM
//		AKUAndroidCrittercismContextInitialize ();
	#endif

	#if AKU_WITH_ANDROID_FACEBOOK
//		AKUAndroidFacebookContextInitialize ();
	#endif
	
	#if AKU_WITH_ANDROID_FLURRY
//		AKUAndroidFlurryContextInitialize ();
	#endif

	#if AKU_WITH_ANDROID_TAPJOY
//		AKUAndroidTapjoyContextInitialize ();
	#endif
	
}

//----------------------------------------------------------------//
void AKUModulesAndroidPause ( bool pause ) {
	UNUSED ( pause );
}


//----------------------------------------------------------------//
void AKUModulesAndroidUpdate () {
}
