// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <Foundation/Foundation.h>
#include <string.h>
#include <zl-common/zl_platform.h>
#include <host-modules/aku_modules_ios.h>

//================================================================//
// implementation
//================================================================//

//----------------------------------------------------------------//
void AKUModulesIosAppFinalize () {

	#if AKU_WITH_IOS
		AKUIosAppFinalize ();
	#endif
	
    #if AKU_WITH_IOS_MOTION
		AKUIosMotionAppFinalize ();
	#endif

	#if AKU_WITH_IOS_MOVIE
		AKUIosMovieAppFinalize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesIosAppInitialize () {

	#if AKU_WITH_IOS
		AKUIosAppInitialize ();
	#endif
	
	#if AKU_WITH_IOS_MOTION
		AKUIosMotionAppInitialize ();
	#endif
	
	#if AKU_WITH_IOS_MOVIE
		AKUIosMovieAppInitialize ();
	#endif
}

//----------------------------------------------------------------//
BOOL AKUModulesIosApplicationDidFinishLaunchingWithOptions ( UIApplication* application, NSDictionary* launchOptions ) {

    BOOL status = YES;
    
    return status;
}

//----------------------------------------------------------------//
BOOL AKUModulesIosApplicationOpenURL ( UIApplication* application,  NSURL* url, NSString* sourceApplication, id annotation ) {

    return NO;
}

//----------------------------------------------------------------//
void AKUModulesIosContextInitialize () {

	#if AKU_WITH_IOS
		AKUIosContextInitialize ();
	#endif
	
	#if AKU_WITH_IOS_MOTION
		AKUIosMotionContextInitialize ();
	#endif
	
	#if AKU_WITH_IOS_MOVIE
		AKUIosMovieContextInitialize ();
	#endif
}

//----------------------------------------------------------------//
void AKUModulesIosPause ( bool pause ) {
	UNUSED ( pause );
}

//----------------------------------------------------------------//
void AKUModulesIosUpdate () {
}
