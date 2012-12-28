// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_MOBCLIX

#include "pch.h"


#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIMobclixAndroid.h>

extern JavaVM* jvm;


//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Mobclix.
	
	@in		string appId			Available in Mobclix dashboard settings.
	@out	nil
*/
int MOAIMobclixAndroid::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* appId = lua_tostring ( state, 1 );

    
	JNI_GET_ENV ( jvm, env );
	JNI_GET_JSTRING ( appId, jappId );
	
	jclass mobclix = env->FindClass ( "com/ziplinegames/moai/MoaiMobclix" );
	if ( mobclix == NULL ) {
        
		USLog::Print ( "MOAIMobclixAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiMobclix" );
	} else {
        
		jmethodID init = env->GetStaticMethodID ( mobclix, "init", "(Ljava/lang/String;)V" );
		if ( init == NULL ) {
            
			USLog::Print ( "MOAIMoxblixAndroid: Unable to find static java method %s", "init" );
		} else {
			USLog::Print ( "MOAIMobclixAndroid: calling mobclix" );
			env->CallStaticVoidMethod ( mobclix, init, jappId );
		}
	}
    

	return 0;
}

//----------------------------------------------------------------//
/**	@name	showAds
	@text	Forces a crash and crittercism report, can be used for internal errors or debugging crittercism
 
	@out	nil 
 */
int	MOAIMobclixAndroid::_showAds ( lua_State* L ) {
	MOAILuaState state ( L );
    
	JNI_GET_ENV ( jvm, env );
	
	jclass mobclix = env->FindClass ( "com/ziplinegames/moai/MoaiMobclix" );
	if ( mobclix == NULL ) {
        
		USLog::Print ( "MOAIMobclixAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiMobclix" );
	} else {
        
		jmethodID showAds = env->GetStaticMethodID ( mobclix, "showAds", "()V" );
		if ( showAds == NULL ) {
            
			USLog::Print ( "MOAIMoxblixAndroid: Unable to find static java method %s", "showAds" );
		} else {
			USLog::Print ( "MOAIMobclixAndroid: calling mobclix" );
			env->CallStaticVoidMethod ( mobclix, showAds );
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	leaveBreadcrumb
	@text	Leave a breadcrumb (log statement) to trace execution.
	
	@in		string breadcrumb		A string describing the code location.
	@out	nil
*/
int MOAIMobclixAndroid::_hideAds ( lua_State* L ) {
	
	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );
	
	jclass mobclix = env->FindClass ( "com/ziplinegames/moai/MoaiMobclix" );
	if ( mobclix == NULL ) {
        
		USLog::Print ( "MOAIMobclixAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiMobclix" );
	} else {
        
		jmethodID hideAds = env->GetStaticMethodID ( mobclix, "hideAds", "()V" );
		if ( hideAds == NULL ) {
            
			USLog::Print ( "MOAIMoxblixAndroid: Unable to find static java method %s", "hideAds" );
		} else {
			USLog::Print ( "MOAIMobclixAndroid: calling mobclix" );
			env->CallStaticVoidMethod ( mobclix, hideAds );
		}
	}
    
			
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUser
 @text	Sets the username
 
 @in	string username
 @out	nil
*/
int MOAIMobclixAndroid::_dealloc ( lua_State* L ) {
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	jclass mobclix = env->FindClass ( "com/ziplinegames/moai/MoaiMobclix" );
	if ( mobclix == NULL ) {
        
		USLog::Print ( "MOAIMobclixAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiMobclix" );
	} else {
        
		jmethodID dealloc = env->GetStaticMethodID ( mobclix, "dealloc", "()V" );
		if ( dealloc == NULL ) {
            
			USLog::Print ( "MOAIMoxblixAndroid: Unable to find static java method %s", "dealloc" );
		} else {
			USLog::Print ( "MOAIMobclixAndroid: calling mobclix" );
			env->CallStaticVoidMethod ( mobclix, dealloc );
		}
	}
    
	return 0;
}


//================================================================//
// MOAIMobclixIOS
//================================================================//

//----------------------------------------------------------------//
MOAIMobclixAndroid::MOAIMobclixAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
	

}

//----------------------------------------------------------------//
MOAIMobclixAndroid::~MOAIMobclixAndroid () {

}

//----------------------------------------------------------------//
void MOAIMobclixAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "init",				_init },
		{ "showAds",            _showAds },
		{ "hideAds",            _hideAds },
		{ "dealloc",			_dealloc },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

/*

#pragma mark -
#pragma mark MobclixAdViewDelegate Methods
@implementation MoaiMobclixDelegate

@synthesize mAdView;

- (void)adViewDidFinishLoad:(MobclixAdView*)adView {
	NSLog(@"Ad Loaded: %@.", NSStringFromCGSize(adView.frame.size));
}

- (void)adView:(MobclixAdView*)adView didFailLoadWithError:(NSError*)error {
	NSLog(@"Ad Failed: %@.", NSStringFromCGSize(adView.frame.size));
}

- (void)adViewWillTouchThrough:(MobclixAdView*)adView {
	NSLog(@"Ad Will Touch Through: %@.", NSStringFromCGSize(adView.frame.size));
}

- (void)adViewDidFinishTouchThrough:(MobclixAdView*)adView {
	NSLog(@"Ad Did Finish Touch Through: %@.", NSStringFromCGSize(adView.frame.size));
}

@end
 
*/

#endif
