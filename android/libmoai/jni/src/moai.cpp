#include <jni.h>
#include <time.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <moaicore/moaicore.h>
#include <moaiext-android/moaiext-android.h>
#include <aku/AKU.h>
#include <aku/AKU-untz.h>
#include <aku/AKU-luaext.h>

//================================================================//
// Utility macros
//================================================================//

	#define GET_ENV() 	\
		JNIEnv* env; 	\
		jvm->GetEnv (( void** )&env, JNI_VERSION_1_4 );

	#define GET_CSTRING(jstr, cstr) \
		const char* cstr = env->GetStringUTFChars ( jstr, NULL );

	#define RELEASE_CSTRING(jstr, cstr) \
		env->ReleaseStringUTFChars ( jstr, cstr );
		
	#define GET_JSTRING(cstr, jstr) \
		jstring jstr = env->NewStringUTF (( const char* )cstr );
		
	#define PRINT(str) \
		__android_log_write ( ANDROID_LOG_INFO, "MoaiLog", str );
		
//================================================================//
// JNI set up
//================================================================//

	JavaVM* 		jvm;

	jobject			mMoaiView;
	jmethodID 		mGenerateGuidFunc;
	
	jobject			mMoaiActivity;
	jmethodID 		mCheckBillingSupportedFunc;
	jmethodID		mRequestPurchaseFunc;
	jmethodID		mConfirmNotificationFunc;
	jmethodID		mRestoreTransactionsFunc;
	jmethodID		mSetMarketPublicKeyFunc;
	
	//----------------------------------------------------------------//
	int JNI_OnLoad ( JavaVM* vm, void* reserved ) {
    
		jvm = vm;
		return JNI_VERSION_1_4;
	}
		
//================================================================//
// In-App Billing callbacks
//================================================================//

	bool CheckBillingSupported () {
		// get environment
		GET_ENV ();

		bool retVal = ( bool )env->CallObjectMethod ( mMoaiActivity , mCheckBillingSupportedFunc );

		return retVal;
	}
	
	bool RequestPurchase ( const char * identifier ) {
		// get environment
		GET_ENV ();

		GET_JSTRING(identifier, jstr);

		bool retVal = ( bool )env->CallObjectMethod ( mMoaiActivity , mRequestPurchaseFunc, jstr );

		return retVal;
	}	

	bool ConfirmNotification ( const char * notification ) {
		// get environment
		GET_ENV ();

		GET_JSTRING(notification, jstr);

		bool retVal = ( bool )env->CallObjectMethod ( mMoaiActivity , mConfirmNotificationFunc, jstr );

		return retVal;
	}
		
	bool RestoreTransactions () {
		// get environment
		GET_ENV ();

		bool retVal = ( bool )env->CallObjectMethod ( mMoaiActivity , mRestoreTransactionsFunc );

		return retVal;
	}
	
	void SetMarketPublicKey ( const char * key ) {
		// get environment
		GET_ENV ();

		GET_JSTRING(key, jstr);

		env->CallObjectMethod ( mMoaiActivity , mSetMarketPublicKeyFunc, jstr );
	}
	
//================================================================//
// Generate GUID callback
//================================================================//

	//----------------------------------------------------------------//
	const char* GenerateGUID () {

		// get environment
		GET_ENV ();

	    // call generate guid method in java
		jstring jguid = ( jstring )env->CallObjectMethod ( mMoaiView, mGenerateGuidFunc );

		// convert jstring to cstring
		GET_CSTRING ( jguid, guid );

		char buf [ 512 ];
		strcpy ( buf, guid );
		const char* retVal = buf;
		
		RELEASE_CSTRING ( jguid, guid );

		// return guid string
		return retVal;
	}

//================================================================//
// JNI Functions
//================================================================//

	//----------------------------------------------------------------//
	extern "C" int Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUCreateContext ( JNIEnv* env, jclass obj ) {
		return AKUCreateContext ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUDeleteContext ( JNIEnv* env, jclass obj, jint akuContextId ) {
		AKUDeleteContext ( akuContextId );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUDetectGfxContext ( JNIEnv* env, jclass obj ) {
		AKUDetectGfxContext ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUAppDidStartSession ( JNIEnv* env, jclass obj ) {
		MOAIApp::Get ().DidStartSession ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUEnqueueCompassEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jint heading ) {
		AKUEnqueueCompassEvent ( deviceId, sensorId, heading );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUEnqueueLevelEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jfloat x, jfloat y, jfloat z ) {
		AKUEnqueueLevelEvent ( deviceId, sensorId, x, y, z );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUEnqueueLocationEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jint longitude, jint latitude, jint altitude, jfloat hAccuracy, jfloat vAccuracy, jfloat speed ) {
		AKUEnqueueLocationEvent ( deviceId, sensorId, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUEnqueueTouchEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jint touchId, jboolean down, jint x, jint y, jint tapCount ) {
		AKUEnqueueTouchEvent ( deviceId, sensorId, touchId, down, x, y, tapCount );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUExtLoadLuacrypto ( JNIEnv* env, jclass obj ) {
		AKUExtLoadLuacrypto ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUExtLoadLuacurl ( JNIEnv* env, jclass obj ) {
		AKUExtLoadLuacurl ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUExtLoadLuasocket ( JNIEnv* env, jclass obj ) {
		AKUExtLoadLuasocket ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUExtLoadLuasql ( JNIEnv* env, jclass obj ) {
		AKUExtLoadLuasql ();
	}


	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUFinalize	( JNIEnv* env, jclass obj ) {
		AKUFinalize ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUInit ( JNIEnv* env, jclass obj, jobject moaiView, jobject moaiActivity ) {

		// create MOAIApp class
		MOAIApp::Affirm ();
		REGISTER_LUA_CLASS ( MOAIApp );

		// register callbacks into Java
		mMoaiView = ( jobject ) env->NewGlobalRef ( moaiView );
		jclass moaiViewClass = env->GetObjectClass ( mMoaiView );
		
		mGenerateGuidFunc = env->GetMethodID ( moaiViewClass, "getGUID", "()Ljava/lang/String;" );

		MOAIApp::Get ().SetCheckBillingSupportedFunc( &CheckBillingSupported );
		MOAIApp::Get ().SetRequestPurchaseFunc( &RequestPurchase );
		MOAIApp::Get ().SetConfirmNotificationFunc( &ConfirmNotification );
		MOAIApp::Get ().SetRestoreTransactionsFunc( &RestoreTransactions );
		MOAIApp::Get ().SetMarketPublicKeyFunc( &SetMarketPublicKey );

		mMoaiActivity = ( jobject ) env->NewGlobalRef ( moaiActivity );
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );

		mCheckBillingSupportedFunc = env->GetMethodID ( moaiActivityClass, "checkBillingSupported", "()Z" );
		mRequestPurchaseFunc = env->GetMethodID ( moaiActivityClass, "requestPurchase", "(Ljava/lang/String;)Z" );
		mConfirmNotificationFunc = env->GetMethodID ( moaiActivityClass, "confirmNotification", "(Ljava/lang/String;)Z" );
		mRestoreTransactionsFunc = env->GetMethodID ( moaiActivityClass, "restoreTransactions", "()Z" );
		mSetMarketPublicKeyFunc = env->GetMethodID ( moaiActivityClass, "setMarketPublicKey", "(Ljava/lang/String;)V" );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUMountVirtualDirectory ( JNIEnv* env, jclass obj, jstring jvirtualPath, jstring jarchive ) {
		GET_CSTRING ( jvirtualPath, virtualPath );
		GET_CSTRING ( jarchive, archive );
		AKUMountVirtualDirectory ( virtualPath, archive );
		RELEASE_CSTRING ( jvirtualPath, virtualPath );
		RELEASE_CSTRING ( jarchive, archive );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUPause ( JNIEnv* env, jclass obj, jboolean paused ) {
		AKUPause ( paused );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKURender ( JNIEnv* env, jclass obj ) {
		AKURender ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUReserveInputDevices ( JNIEnv* env, jclass obj, jint total ) {
		AKUReserveInputDevices ( total );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUReserveInputDeviceSensors ( JNIEnv* env, jclass obj, jint deviceId, jint total ) {
		AKUReserveInputDeviceSensors ( deviceId, total );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKURunScript ( JNIEnv* env, jclass obj, jstring jfilename ) {
		GET_CSTRING ( jfilename, filename );
		AKURunScript ( filename );
		RELEASE_CSTRING ( jfilename, filename );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUSetConnectionType ( JNIEnv* env, jclass obj, jlong connectionType ) {
		MOAIEnvironment::Get ().SetConnectionType ( connectionType );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetContext ( JNIEnv* env, jclass obj, jint akuContextId ) {
		AKUSetContext ( akuContextId );
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetDeviceProperties ( JNIEnv* env, jclass obj, jstring jappName, jstring jappId, jstring jappVersion, jstring jabi, jstring jdevBrand, jstring jdevName, jstring jdevManufacturer, jstring jdevModel, jstring jdevProduct, jstring josBrand, jstring josVersion, jstring judid ) {

		// get the environment
		MOAIEnvironment& moaiEnv = MOAIEnvironment::Get ();
	
		// set up environment callbacks
		moaiEnv.SetGUIDFunc ( &GenerateGUID );

		// convert jstrings to cstrings
		GET_CSTRING ( jappName, appName );
		GET_CSTRING ( jappId, appId );
		GET_CSTRING ( jappVersion, appVersion );
		GET_CSTRING ( jabi, abi );
		GET_CSTRING ( jdevBrand, devBrand );
		GET_CSTRING ( jdevName, devName );
		GET_CSTRING ( jdevManufacturer, devManufacturer );
		GET_CSTRING ( jdevModel, devModel );
		GET_CSTRING ( jdevProduct, devProduct );
		GET_CSTRING ( josBrand, osBrand );
		GET_CSTRING ( josVersion, osVersion );
		GET_CSTRING ( judid, udid );
	
		// set environment properties
		moaiEnv.SetAppDisplayName 	( appName );
		moaiEnv.SetAppID 			( appId );
		moaiEnv.SetAppVersion		( appVersion );
		moaiEnv.SetCPUABI 			( abi );
		moaiEnv.SetDevBrand 		( devBrand );
		moaiEnv.SetDevName 			( devName );
		moaiEnv.SetDevManufacturer	( devManufacturer );
		moaiEnv.SetDevModel			( devModel );
		moaiEnv.SetDevProduct		( devProduct );
		moaiEnv.SetOSBrand			( osBrand );
		moaiEnv.SetOSVersion		( osVersion );
		moaiEnv.SetUDID				( udid );

		// release jstrings
		RELEASE_CSTRING ( jappName, appName );
		RELEASE_CSTRING ( jappId, appId );
		RELEASE_CSTRING ( jappVersion, appVersion );
		RELEASE_CSTRING ( jabi, abi );
		RELEASE_CSTRING ( jdevBrand, devBrand );
		RELEASE_CSTRING ( jdevName, devName );
		RELEASE_CSTRING ( jdevManufacturer, devManufacturer );
		RELEASE_CSTRING ( jdevModel, devModel );
		RELEASE_CSTRING ( jdevProduct, devProduct );
		RELEASE_CSTRING ( josBrand, osBrand );
		RELEASE_CSTRING ( josVersion, osVersion );
		RELEASE_CSTRING ( judid, udid );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUSetDocumentDirectory ( JNIEnv* env, jclass obj, jstring jpath ) {
		GET_CSTRING ( jpath, path );
		MOAIEnvironment::Get ().SetDocumentDirectory ( path );
		RELEASE_CSTRING ( jpath, path );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputConfigurationName ( JNIEnv* env, jclass obj, jstring jname ) {
		GET_CSTRING ( jname, name );
		AKUSetInputConfigurationName ( name );
		RELEASE_CSTRING ( jname, name );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputDevice ( JNIEnv* env, jclass obj, jint deviceId, jstring jname ) {
		GET_CSTRING ( jname, name );
		AKUSetInputDevice ( deviceId, name );
		RELEASE_CSTRING ( jname, name );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputDeviceCompass ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jstring jname ) {
		GET_CSTRING ( jname, name );
		AKUSetInputDeviceCompass ( deviceId, sensorId, name );
		RELEASE_CSTRING ( jname, name );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputDeviceLevel ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jstring jname ) {
		GET_CSTRING ( jname, name );
		AKUSetInputDeviceLevel ( deviceId, sensorId, name );
		RELEASE_CSTRING ( jname, name );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputDeviceLocation ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jstring jname ) {
		GET_CSTRING ( jname, name );
		AKUSetInputDeviceLocation ( deviceId, sensorId, name );
		RELEASE_CSTRING ( jname, name );
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetInputDeviceTouch ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jstring jname ) {
		GET_CSTRING ( jname, name );
		AKUSetInputDeviceTouch ( deviceId, sensorId, name );
		RELEASE_CSTRING ( jname, name );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetScreenSize ( JNIEnv* env, jclass obj, jint width, jint height ) {
		AKUSetScreenSize ( width, height );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetViewSize ( JNIEnv* env, jclass obj, jint width, jint height ) {
		AKUSetViewSize ( width, height );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUSetWorkingDirectory ( JNIEnv* env, jclass obj, jstring jpath ) {

		GET_CSTRING ( jpath, path );

		USFileSys::SetCurrentPath ( path );
		MOAILuaRuntime::Get ().SetPath ( path );
	
		RELEASE_CSTRING ( jpath, path );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUUntzInit ( JNIEnv* env, jclass obj ) {
		AKUUntzInit ();
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUUpdate ( JNIEnv* env, jclass obj ) {
		AKUUpdate ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUAppWillEndSession ( JNIEnv* env, jclass obj ) {
		MOAIApp::Get ().WillEndSession ();
	}
	
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyBillingSupported ( JNIEnv* env, jclass obj, jboolean supported ) {
		MOAIApp::Get ().NotifyBillingSupported ( supported );
	}
	
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyPurchaseResponseReceived ( JNIEnv* env, jclass obj, jstring jidentifier, jint code ) {
		GET_CSTRING ( jidentifier, identifier );

		MOAIApp::Get ().NotifyPurchaseResponseReceived ( identifier, code );

		RELEASE_CSTRING ( jidentifier, identifier );
	}

	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyPurchaseStateChanged ( JNIEnv* env, jclass obj, jstring jidentifier, jint code, jstring jorder, jstring jnotification, jstring jpayload ) {
		GET_CSTRING ( jidentifier, identifier );
		GET_CSTRING ( jorder, order );
		GET_CSTRING ( jnotification, notification );
		GET_CSTRING ( jpayload, payload );
		
		MOAIApp::Get ().NotifyPurchaseStateChanged ( identifier, code, order, notification, payload );

		RELEASE_CSTRING ( jidentifier, identifier );
		RELEASE_CSTRING ( jorder, order );
		RELEASE_CSTRING ( jnotification, notification );
		RELEASE_CSTRING ( jpayload, payload );
	}
		
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyRestoreResponseReceived ( JNIEnv* env, jclass obj, jint code ) {
		MOAIApp::Get ().NotifyRestoreResponseReceived ( code );
	}
