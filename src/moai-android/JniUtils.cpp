#include <moai-core/pch.h>
#include <moai-sim/pch.h>
#include <moai-android/JniUtils.h>

#include <jni.h>

extern JavaVM* jvm;

//================================================================//
// JniUtils
//================================================================//

//----------------------------------------------------------------//
jobjectArray JniUtils::ArrayFromLua ( lua_State* L, int index ) {
    MOAILuaState state ( L );

    int numEntries = 0;
    for ( int key = 1; ; ++key ) {
        state.GetField ( 1, key );
        cc8* value = state.GetValue < cc8* >( -1, 0 );
		
        lua_pop ( state, 1 );

        if ( !value ) {
            numEntries = key - 1;
            break;
        }
    }

    jobjectArray array = this->mEnv->NewObjectArray ( numEntries, this->mEnv->FindClass( "java/lang/String" ), 0 );
    for ( int key = 1; ; ++key ) {

        state.GetField ( 1, key );
        cc8* value = state.GetValue < cc8* >( -1, 0 );
        lua_pop ( state, 1 );

        if ( value ) {
            jstring jvalue = this->GetJString ( value );
            this->mEnv->SetObjectArrayElement ( array, key - 1, jvalue );
        }
        else {
            break;
        }
    }
    return array;
}

//----------------------------------------------------------------//
jobject JniUtils::BundleFromLua ( lua_State* L, int index ) {
    MOAILuaState state ( L );

	jclass clazz = this->mEnv->FindClass ( "android.os.Bundle" );
    jobject bundle = this->mEnv->NewObject ( clazz, this->mEnv->GetMethodID ( clazz, "<init>", "()V" ));
    jmethodID put = this->mEnv->GetMethodID ( clazz, "putString", "(Ljava/lang/String;Ljava/lang/String;)V" );

    // table is in the stack at index 'index'
    lua_pushnil ( state );  // first key
    while ( lua_next ( state, index ) != 0 ) {
        // use the 'key' (at index -2) and 'value' (at index -1)
        cc8* key = lua_tostring( state, -2 );

        if( key != NULL ) {
            cc8* value = lua_tostring( state, -1 );
            if ( value != NULL ) {
				
				jstring jkey = this->GetJString ( key );
				jstring jvalue = this->GetJString ( value );

                this->mEnv->CallObjectMethod( bundle, put, jkey, jvalue );
            }
        }
        // removes 'value'; keeps 'key' for next iteration
        lua_pop ( state, 1 );
    }
    return bundle;
}

//----------------------------------------------------------------//
bool JniUtils::CallStaticBooleanMethod ( jmethodID method, ... ) {

	va_list args;
	va_start ( args, method );
	bool result = ( bool )this->mEnv->CallStaticBooleanMethodV ( this->mClass, method, args );
	va_end ( args );
	
	return result;
}

//----------------------------------------------------------------//
long JniUtils::CallStaticLongMethod ( jmethodID method, ... ) {

	va_list args;
	va_start ( args, method );
	long result = ( long )this->mEnv->CallStaticLongMethod ( this->mClass, method, args );
	va_end ( args );
	
	return result;
}

//----------------------------------------------------------------//
jobject JniUtils::CallStaticObjectMethod ( jmethodID method, ... ) {
	
	va_list args;
	va_start ( args, method );
	jobject result = this->mEnv->CallStaticObjectMethodV ( this->mClass, method, args );
	va_end ( args );
	
	return result;
}

//----------------------------------------------------------------//
void JniUtils::CallStaticVoidMethod ( jmethodID method, ... ) {
	
	va_list args;
	va_start ( args, method );
	this->mEnv->CallStaticVoidMethodV ( this->mClass, method, args );
	va_end ( args );
}

//----------------------------------------------------------------//
void JniUtils::ClearException () {
	
	if ( this->mEnv->ExceptionCheck () == JNI_TRUE ) {
		this->mEnv->ExceptionClear ();
	}
}

//----------------------------------------------------------------//
jobject JniUtils::CreateObjectOfClass () {

     if ( !this->mClass ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAI JNI: Missing class; cannot create object" );
		assert ( false );
		return NULL;
    }
    jmethodID constructor = this->mEnv->GetMethodID ( this->mClass, "<init>", "()V" );
    return this->mEnv->NewObject ( this->mClass, constructor );
}

//----------------------------------------------------------------//
jclass JniUtils::GetClass ( cc8* className ) {
	
	jclass clazz = this->mEnv->FindClass ( className );
	if ( clazz == NULL ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAI JNI: Unable to find java class %s", className );
		this->ClearException ();
	}
	return clazz;
}

//----------------------------------------------------------------//
jclass JniUtils::GetClassViaLoader ( cc8* className ) {

	jstring jclassName = this->GetJString ( className );
	
	jclass contextClass = this->GetClass ( "android/content/Context" );
	jmethodID getClassLoader = this->GetMethod ( contextClass, "getClassLoader", "()Ljava/lang/ClassLoader;" );
	
	jclass classLoaderClass = this->GetClass ( "java/lang/ClassLoader" );
	jmethodID loadClass = this->GetMethod ( classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;" );
	
	jobject classLoader = this->mEnv->CallObjectMethod ( this->mActivity, getClassLoader );
	jclass clazz = ( jclass )( this->mEnv->CallObjectMethod ( classLoader, loadClass, jclassName ));
	
    if ( clazz == NULL ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAI JNI: Missing class; cannot find java class %d", className );
		this->ClearException ();
    }
	return clazz;
}

//----------------------------------------------------------------//
cc8* JniUtils::GetCString ( jstring jstr ) {
	
	return ( jstr != NULL ) ? this->mEnv->GetStringUTFChars ( jstr, NULL ) : NULL;
}

//----------------------------------------------------------------//
jstring JniUtils::GetJString ( cc8* cstr ) {
	
	return ( cstr != NULL ) ? this->mEnv->NewStringUTF (( const char* )cstr ) : NULL;
}

//----------------------------------------------------------------//
jmethodID JniUtils::GetMethod ( cc8* methodName, cc8* methodSignature ) {

	this->GetMethod ( this->mClass, methodName, methodSignature );
}

//----------------------------------------------------------------//
jmethodID JniUtils::GetMethod ( jclass clazz, cc8* methodName, cc8* methodSignature ) {
	
    if ( !clazz ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAI JNI: Missing class; cannot find java method %d", methodName );
		this->ClearException ();
		return NULL;
    }
	
    jmethodID method = this->mEnv->GetMethodID ( clazz, methodName, methodSignature );
	
	if ( method == NULL ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAI JNI: Unable to find java method %s", methodName );
		this->ClearException ();
	}
	return method;
}

//----------------------------------------------------------------//
jmethodID JniUtils::GetStaticMethod ( cc8* methodName, cc8* methodSignature ) {

	this->GetStaticMethod ( this->mClass, methodName, methodSignature );
}

//----------------------------------------------------------------//
jmethodID JniUtils::GetStaticMethod ( jclass clazz, cc8* methodName, cc8* methodSignature ) {
	
    if ( !clazz ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAI JNI: Missing class; cannot find static java method %d", methodName );
		this->ClearException ();
		return NULL;
    }
	
    jmethodID method = this->mEnv->GetStaticMethodID ( clazz, methodName, methodSignature );
	
	if ( method == NULL ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "MOAI JNI: Unable to find static java method %s", methodName );
		this->ClearException ();
	}
	return method;
}

//----------------------------------------------------------------//
JniUtils::JniUtils () :
	mEnv ( 0 ),
	mClass ( 0 ) {
	
	jvm->GetEnv (( void** )&this->mEnv, JNI_VERSION_1_4 );
	
	jclass clazz = this->GetClass ( "com.ziplinegames.moai.Moai" );
    jmethodID getActivity = this->mEnv->GetStaticMethodID ( clazz, "getActivity", "()Landroid/app/Activity;" );
	this->mActivity = this->mEnv->CallStaticObjectMethod ( clazz, getActivity );

	assert ( this->mActivity );
}

//----------------------------------------------------------------//
JniUtils::~JniUtils () {
}

//----------------------------------------------------------------//
void JniUtils::ReleaseCString ( jstring jstr, cc8* cstr ) {
	
	if ( jstr && cstr ) {
		this->mEnv->ReleaseStringUTFChars ( jstr, cstr );
	}
}

//----------------------------------------------------------------//
bool JniUtils::SetClass ( cc8* className ) {
	
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAI JNI: set class %s", className );
	
	this->mClass = this->GetClass ( className );
	return this->mClass != NULL;
}

//----------------------------------------------------------------//
bool JniUtils::SetClassViaLoader ( cc8* className ) {
	
	ZLLog::LogF ( ZLLog::CONSOLE, "MOAI JNI: set class via loader %s", className );
	
	this->mClass = this->GetClassViaLoader ( className );
	return this->mClass != NULL;
}
