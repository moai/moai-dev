#include <jni.h>
#include <time.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <moaicore/moaicore.h>
#include <aku/AKU.h>
#include <aku/AKU-untz.h>
#include <aku/AKU-luaext.h>

//----------------------------------------------------------------//
JavaVM* 		jvm;
jobject 		javaObject;
AKUContextID 	mAku;

jmethodID 		m_AKUEnterFullscreenModeFunc;
jmethodID 		m_AKUExitFullscreenModeFunc;
jmethodID 		m_AKUHideLoadingScreenFunc;
jmethodID 		m_AKUOpenWindowFunc;
jmethodID 		m_AKUShowLoadingScreenFunc;
jmethodID 		m_AKUShowSoftwareKeyboardFunc;
jmethodID 		m_AKUStartGameLoopFunc;
jmethodID 		m_GetConnectivityFunc;
jmethodID 		m_GenerateGuidFunc;

//----------------------------------------------------------------//
namespace MoaiInputDeviceID {
	enum {
		DEVICE,
		TOTAL,
	};
}

//----------------------------------------------------------------//
namespace MoaiInputDeviceSensorID {
	enum {
		COMPASS,
		LEVEL,
		LOCATION,
		TOUCH,
		TOTAL,
	};
}

//----------------------------------------------------------------//
enum {
	CONNECTION_TYPE_NONE,
	CONNECTION_TYPE_WIFI,
	CONNECTION_TYPE_WWAN
};

//================================================================//
// Utility macros
//================================================================//

	#define GET_STRING(jstr, cstr) \
		const char* cstr = env->GetStringUTFChars( jstr, NULL );

	#define RELEASE_STRING(jstr, cstr) \
		env->ReleaseStringUTFChars ( jstr, cstr );
		
	#define PRINT(str) \
		__android_log_write ( ANDROID_LOG_INFO, "MoaiLog", "str" );
		
//================================================================//
// AKU callbacks
//================================================================//

//----------------------------------------------------------------//
void _AKUEnterFullscreenModeFunc () {

	__android_log_write ( ANDROID_LOG_INFO, "MoaiJNI", "Enter Full Screen Callback Start." );
	JNIEnv* env;

	if ( jvm == NULL ) {
		return;
	}
	
	jvm->GetEnv ( ( void** ) &env, JNI_VERSION_1_4 );

	if ( env == NULL ) {
		return;
	}

	if ( m_AKUStartGameLoopFunc == NULL ) {
		__android_log_write ( ANDROID_LOG_INFO, "MoaiJNI", "Enter Full Screen Callback FAILURE.");
	}

	env->CallVoidMethod ( javaObject, m_AKUEnterFullscreenModeFunc );
}

//----------------------------------------------------------------//
void _AKUExitFullscreenModeFunc () {

	__android_log_write ( ANDROID_LOG_INFO, "MoaiJNI", "Exit Full Screen Callback Start." );
	JNIEnv* env;

	if ( jvm == NULL ) {
		return;
	}
	
	jvm->GetEnv ( ( void** ) &env, JNI_VERSION_1_4 );

	if ( env == NULL ) {
		return;
	}

  	if ( m_AKUStartGameLoopFunc == NULL ) {
		__android_log_write ( ANDROID_LOG_INFO, "MoaiJNI", "Exit Full Screen Callback FAILURE." );
	}
	
	env->CallVoidMethod ( javaObject, m_AKUExitFullscreenModeFunc );
}

//----------------------------------------------------------------//
void _AKUHideLoadingScreenFunc () {
	
	 __android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Hide Loading Screen Callback Start.");
	JNIEnv *env;

	if(jvm == NULL)
		return;

	
	jvm->GetEnv((void**)&env, JNI_VERSION_1_4);

	if(env == NULL)
		return;


	  if(m_AKUStartGameLoopFunc == NULL)
	  {
		  __android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Hide Loading Screen Callback FAILURE.");
	  }

     env->CallVoidMethod(javaObject, m_AKUHideLoadingScreenFunc);
}

//----------------------------------------------------------------//
void _AKUOpenWindowFunc ( const char* title, int width, int height ) {
}

//----------------------------------------------------------------//
void _AKUShowLoadingScreenFunc () {

	 __android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Show Loading Screen Callback Start.");
	JNIEnv *env;

	if(jvm == NULL)
		return;

	
	jvm->GetEnv((void**)&env, JNI_VERSION_1_4);

	if(env == NULL)
		return;


	  if(m_AKUStartGameLoopFunc == NULL)
	  {
		  __android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Show Loading Screen Callback FAILURE.");
	  }

     env->CallVoidMethod(javaObject, m_AKUShowLoadingScreenFunc);
}

//----------------------------------------------------------------//
void _AKUShowSoftwareKeyboardFunc () {

	 __android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Software Keyboard Callback Start.");
	JNIEnv *env;

	if(jvm == NULL)
		return;

	
	jvm->GetEnv((void**)&env, JNI_VERSION_1_4);

	if(env == NULL)
		return;


	  if(m_AKUStartGameLoopFunc == NULL)
	  {
		  __android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Software Keyboard Callback FAILURE.");
	  }
	
     env->CallVoidMethod(javaObject, m_AKUShowSoftwareKeyboardFunc);
}

//----------------------------------------------------------------//
void _AKUStartGameLoopFunc () {
	
	 __android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Game Loop Callback Start.");
	JNIEnv *env;

	if(jvm == NULL)
		return;

	
	jvm->GetEnv((void**)&env, JNI_VERSION_1_4);

	if(env == NULL)
		return;


	  if(m_AKUStartGameLoopFunc == NULL)
	  {
		  __android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Game Loop Callback FAILURE.");
	  }

     env->CallVoidMethod(javaObject, m_AKUStartGameLoopFunc);


}

// -------------------------------------------------------------//
long _GetConnectivity () {

	JNIEnv *env;
	if(jvm == NULL)
		return NULL;
	
	jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
	if(env == NULL)
		return NULL;

	if(m_AKUStartGameLoopFunc == NULL)
	{
		__android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Get connection Callback FAILURE.");
	}

    jstring conn = (jstring)env->CallObjectMethod(javaObject, m_GetConnectivityFunc);
	char buf[512];
    const char *str, *ret;
    str = env->GetStringUTFChars(conn, NULL);
    if (str == NULL) {
        return NULL; /* OutOfMemoryError already thrown */
    }
	strcpy(buf, str);
	ret = buf;
    env->ReleaseStringUTFChars(conn, str);
    
    if ( strcmp ( buf, "WIFI" ))
    	return ( long )CONNECTION_TYPE_WIFI;
    else if (strcmp ( buf, "MOBILE" ))
    	return ( long )CONNECTION_TYPE_WWAN;
	else
		return ( long )CONNECTION_TYPE_NONE;
}

// -------------------------------------------------------------//
const char* _GenerateGUID () {

	JNIEnv *env;
	if(jvm == NULL)
		return NULL;
	
	jvm->GetEnv((void**)&env, JNI_VERSION_1_4);
	if(env == NULL)
		return NULL;

	if(m_AKUStartGameLoopFunc == NULL)
	{
		__android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Generate GUID Callback FAILURE.");
	}

    jstring guid = (jstring)env->CallObjectMethod(javaObject, m_GenerateGuidFunc);
	char buf[512];
    const char *str, *ret;
    str = env->GetStringUTFChars(guid, NULL);
    if (str == NULL) {
        return NULL; /* OutOfMemoryError already thrown */
    }
	strcpy(buf, str);
	ret = buf;
    env->ReleaseStringUTFChars(guid, str);
	return ret;
}


// -------------------------------------------------------------//

int JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jvm = vm;
	
	return JNI_VERSION_1_4;
}

extern "C"
void Java_@PACKAGE_UNDERSCORED@_MoaiView_RestartAku
(JNIEnv *env, jclass obj, jobject thizz) 
{
	jclass classic = env->GetObjectClass(javaObject);

	mAku = AKUCreateContext ();
		
	AKUSetInputConfigurationName ( "Android" );

	AKUReserveInputDevices			( MoaiInputDeviceID::TOTAL );
	AKUSetInputDevice				( MoaiInputDeviceID::DEVICE, "device" );
		
	AKUReserveInputDeviceSensors	( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::TOTAL );
	AKUSetInputDeviceCompass		( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::COMPASS,		"compass" );
	AKUSetInputDeviceLevel			( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::LEVEL,		"level" );
	AKUSetInputDeviceLocation		( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::LOCATION,		"location" );
	AKUSetInputDeviceTouch			( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::TOUCH,		"touch" );
	
	AKUUntzInit ();

	AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc );
	AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc );
	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );

	m_AKUEnterFullscreenModeFunc  = env->GetMethodID(classic,"AKUEnterFullscreenModeFunc", "()V");
	m_AKUExitFullscreenModeFunc  = env->GetMethodID(classic,"AKUExitFullscreenModeFunc", "()V");
	m_AKUHideLoadingScreenFunc  = env->GetMethodID(classic,"AKUHideLoadingScreenFunc", "()V");
	//m_AKUOpenWindowFunc;
	m_AKUShowLoadingScreenFunc  = env->GetMethodID(classic,"AKUShowLoadingScreenFunc", "()V");
	m_AKUShowSoftwareKeyboardFunc  = env->GetMethodID(classic,"AKUShowSoftwareKeyboardFunc", "()V");
	m_AKUStartGameLoopFunc = env->GetMethodID(classic,"AKUStartGameLoopFunc", "()V");
	
	//Device properties
	m_GetConnectivityFunc = env->GetMethodID(classic,"GetConnectivity","()Ljava/lang/String;");
	m_GenerateGuidFunc = env->GetMethodID(classic,"GenerateGUID","()Ljava/lang/String;");

	__android_log_write(ANDROID_LOG_INFO,"MoaiJNI","Aku Successfully Initialized");
}
extern "C"
void Java_@PACKAGE_UNDERSCORED@_MoaiView_InitializeAku
  (JNIEnv *env, jclass obj, jobject thizz) 
{

	javaObject = (jobject)env->NewGlobalRef(thizz);
	
	Java_@PACKAGE_UNDERSCORED@_MoaiView_RestartAku(env, obj, thizz);
	
}

//----------------------------------------------------------------//
extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_Run ( JNIEnv* env, jclass obj, jstring jfilename, jint width, jint height ) {
	
	GET_STRING ( jfilename, filename );

	AKUSetContext ( mAku );
	AKUSetScreenSize ( width, height );
	AKURunScript ( filename );
	
	RELEASE_STRING ( jfilename, filename );
}

//----------------------------------------------------------------//
extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUFinalize	( JNIEnv* env, jclass obj ) {
	
	AKUFinalize ();
}

//----------------------------------------------------------------//
extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUDeleteContext ( JNIEnv* env, jclass obj ) {
	
	AKUDeleteContext ( mAku );
}

//----------------------------------------------------------------//
extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_DetectAkuContext ( JNIEnv* env, jclass obj ) {
	
	AKUDetectGfxContext ();
}

//----------------------------------------------------------------//
extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_onDraw ( JNIEnv* env, jclass obj, jint width, jint height ) {
	
	AKUSetContext ( mAku );
	AKUSetScreenSize ( width, height);
	AKURender();
}

//----------------------------------------------------------------//
extern "C" 
void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUEnqueueLevelEvent ( JNIEnv* env, jclass obj, jfloat x, jfloat y, jfloat z ) {
	
	AKUEnqueueLevelEvent (
		MoaiInputDeviceID::DEVICE,
		MoaiInputDeviceSensorID::LEVEL,
		x,
		y,
		z
	);
}

//----------------------------------------------------------------//
extern "C" 
void Java_@PACKAGE_UNDERSCORED@_MoaiView_onUpdateAnim ( JNIEnv* env, jclass obj ) {
	
	AKUSetContext ( mAku );
	AKUUpdate ();
}

//----------------------------------------------------------------//
extern "C"
void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUEnqueueCompassEvent ( JNIEnv* env, jclass obj, jint heading ) {
	
	AKUEnqueueCompassEvent (
		MoaiInputDeviceID::DEVICE,
		MoaiInputDeviceSensorID::COMPASS,
		heading
	);
}

//----------------------------------------------------------------//
extern "C"	
void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUEnqueueLocationEvent ( JNIEnv* env, jclass obj, jint longitude, jint latitude, jint altitude, jfloat hAccuracy, jfloat vAccuracy, jfloat speed ) {
	
	AKUEnqueueLocationEvent (
		MoaiInputDeviceID::DEVICE,
		MoaiInputDeviceSensorID::LOCATION,
		longitude,
		latitude,
		altitude,
		hAccuracy,
		vAccuracy,
		speed
	);
}

//----------------------------------------------------------------//
extern "C"
void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUEnqueueTouchEvent ( JNIEnv* env, jclass obj, jint touchId, jboolean down, jint x, jint y, jint tapCount ) {

	AKUEnqueueTouchEvent (
		MoaiInputDeviceID::DEVICE,
		MoaiInputDeviceSensorID::TOUCH,
		touchId,
		down,
		x,
		y,
		tapCount
	);
}

//----------------------------------------------------------------//
extern "C" 
void Java_@PACKAGE_UNDERSCORED@_MoaiView_setWorkingDirectory ( JNIEnv* env, jclass obj, jstring jpath ) {

	GET_STRING ( jpath, path );

	USFileSys::SetCurrentPath ( path );
	MOAILuaRuntime::Get ().SetPath ( path );
	
	RELEASE_STRING ( jpath, path );
}

//----------------------------------------------------------------//
extern "C"
void Java_@PACKAGE_UNDERSCORED@_MoaiView_setDeviceProperties ( JNIEnv* env, jclass obj, jstring jappName, jstring jabi,	jstring jdevBrand, jstring jdevName, jstring jdevManufacturer, jstring jdevModel,	jstring jdevProduct, jstring josBrand, jstring josVersion, jstring jUDID ) {

	// get the environment
	MOAIEnvironment& moaiEnv = MOAIEnvironment::Get ();
	
	// set environment callbacks
	moaiEnv.SetGUIDFunc ( &_GenerateGUID );
	moaiEnv.SetConnectivityFunc ( &_GetConnectivity );

	// convert jstrings to cstrings
	GET_STRING ( jappName, appName );
	GET_STRING ( jabi, abi );
	GET_STRING ( jdevBrand, devBrand );
	GET_STRING ( jdevName, devName );
	GET_STRING ( jdevManufacturer, devManufacturer );
	GET_STRING ( jdevModel, devModel );
	GET_STRING ( jdevProduct, devProduct );
	GET_STRING ( josBrand, osBrand );
	GET_STRING ( josVersion, osVersion );
	GET_STRING ( jUDID, UDID );
	
	// set environment properties
	moaiEnv.SetAppDisplayName 	( appName );
	moaiEnv.SetCPUABI 			( abi );
	moaiEnv.SetDevBrand 		( devBrand );
	moaiEnv.SetDevName 			( devName );
	moaiEnv.SetDevManufacturer	( devManufacturer );
	moaiEnv.SetDevModel			( devModel );
	moaiEnv.SetDevProduct		( devProduct );
	moaiEnv.SetOSBrand			( osBrand );
	moaiEnv.SetOSVersion		( osVersion );
	moaiEnv.SetUDID				( UDID );

	// release jstrings
	RELEASE_STRING ( jappName, appName );
	RELEASE_STRING ( jabi, abi );
	RELEASE_STRING ( jdevBrand, devBrand );
	RELEASE_STRING ( jdevName, devName );
	RELEASE_STRING ( jdevManufacturer, devManufacturer );
	RELEASE_STRING ( jdevModel, devModel );
	RELEASE_STRING ( jdevProduct, devProduct );
	RELEASE_STRING ( josBrand, osBrand );
	RELEASE_STRING ( josVersion, osVersion );
	RELEASE_STRING ( jUDID, UDID );
}
