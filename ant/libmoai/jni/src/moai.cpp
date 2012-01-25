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


template <class T>
class LockingQueue {
public:
	pthread_mutex_t mutex;
	int tail;
	int num;

	static const int kMaxMessages = 100;

	T messages [ kMaxMessages ];

	//----------------------------------------------------------------//
	void Push ( const T &message ) {

		pthread_mutex_lock( &mutex );

		if ( num >= LockingQueue::kMaxMessages ) {
			printf ("ERROR: g_MessageQueue, kMaxMessages (%d) exceeded\n", LockingQueue::kMaxMessages );
		} 
		else {
			int head = ( tail + num) % LockingQueue::kMaxMessages;

			 messages [ head ] = message;
			++num;

			if ( num >= LockingQueue::kMaxMessages )  {
				 num -= LockingQueue::kMaxMessages;
			}

		}

		pthread_mutex_unlock( &mutex );
	}

	int PopMessage ( T &message ) {

		pthread_mutex_lock( &mutex );

		int result = 0;

		if ( num > 0) {

			result = 1;
			message = messages [ tail ];
			++tail;

			if ( tail >=  LockingQueue::kMaxMessages) {
				tail -=  LockingQueue::kMaxMessages;
			}
			--num;
		}

		pthread_mutex_unlock( &mutex );

		return result;
	}
};

struct InputEvent {

	enum {
		INPUTEVENT_LEVEL,
		INPUTEVENT_COMPASS,
		INPUTEVENT_LOCATION,
		INPUTEVENT_TOUCH,
	};

	int m_type;
	int m_deviceId;
	int m_sensorId;

	float m_x;
	float m_y;
	float m_z;


	//compass
	int m_heading;

	//touch
	int m_touchId;
	bool m_down;
	int m_tapCount;
	
	//location
	double m_longitude;
	double m_latitude;
	double m_altitude;
	float m_hAccuracy;
	float m_vAccuracy;
	float m_speed;
};

LockingQueue<InputEvent> *g_InputQueue = NULL;

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
	JNIEnv* 		gEnv;

	jobject			mMoaiActivity;
	jobject			mMoaiView;

	jmethodID 		mCheckBillingSupportedFunc;
	jmethodID		mConfirmNotificationFunc;
	jmethodID 		mGenerateGuidFunc;
	jmethodID		mOpenURLFunc;
	jmethodID		mRequestPurchaseFunc;
	jmethodID		mRestoreTransactionsFunc;
	jmethodID		mSetMarketPublicKeyFunc;
	jmethodID		mShowDialogFunc;
	jmethodID		mShareFunc;
	
	//----------------------------------------------------------------//
	int JNI_OnLoad ( JavaVM* vm, void* reserved ) {
    
		jvm = vm;		
		return JNI_VERSION_1_4;
	}
		
//================================================================//
// In-App Billing callbacks
//================================================================//

	//----------------------------------------------------------------//
	bool CheckBillingSupported () {

		GET_ENV ();

		bool retVal = env->CallBooleanMethod ( mMoaiActivity , mCheckBillingSupportedFunc );
		return retVal;
	}

	//----------------------------------------------------------------//
	bool ConfirmNotification ( const char* notification ) {

		GET_ENV ();
		GET_JSTRING ( notification, jstr );
		
		bool retVal = ( bool )env->CallBooleanMethod ( mMoaiActivity , mConfirmNotificationFunc, jstr );
		return retVal;
	}

	//----------------------------------------------------------------//
	bool RequestPurchase ( const char* identifier, const char* payload ) {

		GET_ENV ();
		GET_JSTRING ( identifier, jidentifier );
		GET_JSTRING ( payload, jpayload );

		bool retVal = ( bool )env->CallBooleanMethod ( mMoaiActivity , mRequestPurchaseFunc, jidentifier, jpayload );
		return retVal;
	}	
		
	//----------------------------------------------------------------//
	bool RestoreTransactions () {

		GET_ENV ();
		
		bool retVal = ( bool )env->CallBooleanMethod ( mMoaiActivity , mRestoreTransactionsFunc );
		return retVal;
	}
	
	//----------------------------------------------------------------//
	void SetMarketPublicKey ( const char* key ) {

		GET_ENV ();
		GET_JSTRING ( key, jstr );
		
		env->CallVoidMethod ( mMoaiActivity, mSetMarketPublicKeyFunc, jstr );
	}
	
	//----------------------------------------------------------------//
	void ShowDialog ( const char* title , const char* message , const char* positive , const char* neutral , const char* negative , bool cancelable ) {

		GET_ENV ();

		GET_JSTRING ( title, jtitle );
		GET_JSTRING ( message, jmessage );
		GET_JSTRING ( positive, jpositive );
		GET_JSTRING ( neutral, jneutral );
		GET_JSTRING ( negative, jnegative );

		env->CallVoidMethod ( mMoaiActivity , mShowDialogFunc, jtitle, jmessage, jpositive, jneutral, jnegative, cancelable );
	}
	
	//----------------------------------------------------------------//
	void Share ( const char* prompt , const char* subject , const char* text ) {

		GET_ENV ();

		GET_JSTRING ( prompt, jprompt );
		GET_JSTRING ( subject, jsubject );
		GET_JSTRING ( text, jtext );

		env->CallVoidMethod ( mMoaiActivity , mShareFunc, jprompt, jsubject, jtext );
	}
	
//================================================================//
// Generate GUID callback
//================================================================//

	//----------------------------------------------------------------//
	const char* GenerateGUID () {

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
// Open Url External callback
//================================================================//

	//----------------------------------------------------------------//
	void OpenURL ( const char* url ) {

		GET_ENV ();
		GET_JSTRING ( url, jstr );
		
		env->CallVoidMethod ( mMoaiActivity, mOpenURLFunc, jstr );
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
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUAppWillEndSession ( JNIEnv* env, jclass obj ) {
		MOAIApp::Get ().WillEndSession ();
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUEnqueueCompassEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jint heading ) {

		InputEvent ievent;

		ievent.m_type = InputEvent::INPUTEVENT_COMPASS;

		ievent.m_deviceId = deviceId;
		ievent.m_sensorId = sensorId;

		ievent.m_heading = heading;

		g_InputQueue->Push ( ievent );
		//AKUEnqueueCompassEvent ( deviceId, sensorId, heading );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUEnqueueLevelEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jfloat x, jfloat y, jfloat z ) {
		
		InputEvent ievent;

		ievent.m_type = InputEvent::INPUTEVENT_LEVEL;

		ievent.m_deviceId = deviceId;
		ievent.m_sensorId = sensorId;

		ievent.m_x = x;
		ievent.m_y = y;
		ievent.m_z = z;

		g_InputQueue->Push ( ievent );
		//AKUEnqueueLevelEvent ( deviceId, sensorId, x, y, z );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUEnqueueLocationEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jdouble longitude, jdouble latitude, jdouble altitude, jfloat hAccuracy, jfloat vAccuracy, jfloat speed ) {
		
		InputEvent ievent;

		ievent.m_type = InputEvent::INPUTEVENT_LOCATION;

		ievent.m_deviceId = deviceId;
		ievent.m_sensorId = sensorId;

		ievent.m_longitude = longitude;
		ievent.m_latitude = latitude;
		ievent.m_altitude = altitude;
		ievent.m_hAccuracy = hAccuracy;
		ievent.m_vAccuracy = vAccuracy;
		ievent.m_speed = speed ;
		
		g_InputQueue->Push ( ievent );
		//AKUEnqueueLocationEvent ( deviceId, sensorId, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUFinalize	( JNIEnv* env, jclass obj ) {
		AKUFinalize ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUEnqueueTouchEvent ( JNIEnv* env, jclass obj, jint deviceId, jint sensorId, jint touchId, jboolean down, jint x, jint y, jint tapCount ) {
		InputEvent ievent;

		ievent.m_type = InputEvent::INPUTEVENT_TOUCH;

		ievent.m_deviceId = deviceId;
		ievent.m_sensorId = sensorId;

		ievent.m_touchId = touchId;
		ievent.m_down = down;
		ievent.m_x = x;
		ievent.m_y = y;
		ievent.m_tapCount = tapCount;

		g_InputQueue->Push ( ievent );
		//AKUEnqueueTouchEvent ( deviceId, sensorId, touchId, down, x, y, tapCount );
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
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUInit ( JNIEnv* env, jclass obj, jobject moaiView, jobject moaiActivity ) {

		// create MOAIApp class
		MOAIApp::Affirm ();
		REGISTER_LUA_CLASS ( MOAIApp );

#ifndef DISABLE_TAPJOY
		MOAITapjoy::Affirm ();
		REGISTER_LUA_CLASS ( MOAITapjoy );
#endif

		// register callbacks into Java
		mMoaiView = ( jobject ) env->NewGlobalRef ( moaiView );
		jclass moaiViewClass = env->GetObjectClass ( mMoaiView );
		
		g_InputQueue = new LockingQueue<InputEvent> ();
		mGenerateGuidFunc = env->GetMethodID ( moaiViewClass, "getGUID", "()Ljava/lang/String;" );

		MOAIApp::Get ().SetCheckBillingSupportedFunc( &CheckBillingSupported );
		MOAIApp::Get ().SetConfirmNotificationFunc( &ConfirmNotification );
		MOAIApp::Get ().SetOpenURLFunc( &OpenURL );
		MOAIApp::Get ().SetMarketPublicKeyFunc( &SetMarketPublicKey );
		MOAIApp::Get ().SetRequestPurchaseFunc( &RequestPurchase );
		MOAIApp::Get ().SetRestoreTransactionsFunc( &RestoreTransactions );
		MOAIApp::Get ().SetShowDialogFunc( &ShowDialog );
		MOAIApp::Get ().SetShareFunc( &Share );

		mMoaiActivity = ( jobject ) env->NewGlobalRef ( moaiActivity );
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );

		mCheckBillingSupportedFunc = env->GetMethodID ( moaiActivityClass, "checkBillingSupported", "()Z" );
		mConfirmNotificationFunc = env->GetMethodID ( moaiActivityClass, "confirmNotification", "(Ljava/lang/String;)Z" );
		mOpenURLFunc = env->GetMethodID ( moaiActivityClass, "openURL", "(Ljava/lang/String;)V" );
		mRequestPurchaseFunc = env->GetMethodID ( moaiActivityClass, "requestPurchase", "(Ljava/lang/String;Ljava/lang/String;)Z" );
		mRestoreTransactionsFunc = env->GetMethodID ( moaiActivityClass, "restoreTransactions", "()Z" );
		mSetMarketPublicKeyFunc = env->GetMethodID ( moaiActivityClass, "setMarketPublicKey", "(Ljava/lang/String;)V" );
		mShowDialogFunc = env->GetMethodID ( moaiActivityClass, "showDialog", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V" );
		mShareFunc = env->GetMethodID ( moaiActivityClass, "share", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
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
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyBillingSupported ( JNIEnv* env, jclass obj, jboolean supported ) {
		MOAIApp::Get ().NotifyBillingSupported ( supported );
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyPurchaseResponseReceived ( JNIEnv* env, jclass obj, jstring jidentifier, jint code ) {
		GET_CSTRING ( jidentifier, identifier );

		MOAIApp::Get ().NotifyPurchaseResponseReceived ( identifier, code );

		RELEASE_CSTRING ( jidentifier, identifier );
	}

	//----------------------------------------------------------------//
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
		
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyRestoreResponseReceived ( JNIEnv* env, jclass obj, jint code ) {
		MOAIApp::Get ().NotifyRestoreResponseReceived ( code );
	}

	//----------------------------------------------------------------//
	extern "C" bool Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyBackButtonPressed ( JNIEnv* env, jclass obj ) {
		return MOAIApp::Get ().NotifyBackButtonPressed ();
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyDialogDismissed ( JNIEnv* env, jclass obj, jint code ) {
		MOAIApp::Get ().NotifyDialogDismissed ( code );
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyVideoAdReady ( JNIEnv* env, jclass obj ) {
		MOAITapjoy::Get ().NotifyVideoAdReady ();
	}

	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyVideoAdError ( JNIEnv* env, jclass obj, jint code ) {
		MOAITapjoy::Get ().NotifyVideoAdError ( code );
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiActivity_AKUNotifyVideoAdClose ( JNIEnv* env, jclass obj ) {
		MOAITapjoy::Get ().NotifyVideoAdClose ();
	}
	
	//----------------------------------------------------------------//
	extern "C" void Java_@PACKAGE_UNDERSCORED@_MoaiView_AKUPause ( JNIEnv* env, jclass obj, jboolean paused ) {
		AKUPause ( paused );

		if ( paused ) {
		
			AKUUntzSuspend ();
		} else {
		
			AKUUntzResume ();
		}		
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

		InputEvent ievent;
		while ( g_InputQueue->PopMessage ( ievent )) {
			switch ( ievent.m_type ) {
			case InputEvent::INPUTEVENT_TOUCH:
				AKUEnqueueTouchEvent ( ievent.m_deviceId, ievent.m_sensorId, ievent.m_touchId, ievent.m_down, ievent.m_x, ievent.m_y, ievent.m_tapCount );
				break;
			case InputEvent::INPUTEVENT_LEVEL:
				AKUEnqueueLevelEvent ( ievent.m_deviceId, ievent.m_sensorId, ievent.m_x, ievent.m_y, ievent.m_z );
				break;
			case InputEvent::INPUTEVENT_COMPASS:
				AKUEnqueueCompassEvent ( ievent.m_deviceId, ievent.m_sensorId, ievent.m_heading );
				break;
			case InputEvent::INPUTEVENT_LOCATION:
				AKUEnqueueLocationEvent ( ievent.m_deviceId, ievent.m_sensorId, ievent.m_longitude, ievent.m_latitude, ievent.m_altitude, ievent.m_hAccuracy, ievent.m_vAccuracy, ievent.m_speed );
				break;
			}
		}

		AKUUpdate ();
	}