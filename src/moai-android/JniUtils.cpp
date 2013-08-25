#include <moai-android/JniUtils.h>

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

extern JavaVM* jvm;

/**
 * Create a jobjectArray from the Lua stack
 * @param lua_state The lua Stack
 * @param index The index to convert
 */
jobjectArray JniUtils::arrayFromLua ( lua_State* L, int index ) {
    MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

    int numEntries = 0;
    for ( int key = 1; ; ++key ) {
        state.GetField ( 1, key );
        cc8* value = JniUtils::parseLuaTable ( state, -1 );
        lua_pop ( state, 1 );

        if ( !value ) {
            numEntries = key - 1;
            break;
        }
    }

    jobjectArray array = env->NewObjectArray ( numEntries, env->FindClass( "java/lang/String" ), 0 );
    for ( int key = 1; ; ++key ) {

        state.GetField ( 1, key );
        cc8* value = JniUtils::parseLuaTable ( state, -1 );
        lua_pop ( state, 1 );

        if ( value ) {
            JNI_GET_JSTRING ( value, jvalue );
            env->SetObjectArrayElement ( array, key - 1, jvalue );
        }
        else {
            break;
        }
    }
    return array;
}

/**
 * Create Android Bundle from the Lua stack
 * @param L The Lua stack
 * @param The index to convert
 */
jobject JniUtils::bundleFromLua ( lua_State* L, int index ) {
    MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );

    STLString className = "android.os.Bundle";
    jobject bundle = JniUtils::createObjectOfClass( className );
    jmethodID put = JniUtils::getMethod( className, "putString", "(Ljava/lang/String;Ljava/lang/String;)V" );

    // table is in the stack at index 'index'
    lua_pushnil ( state );  // first key
    while ( lua_next ( state, index ) != 0 ) {
        // use the 'key' (at index -2) and 'value' (at index -1)
        cc8* key = lua_tostring( state, -2 );

        if( key != NULL ) {
            cc8* value = lua_tostring( state, -1 );
            if ( value != NULL ) {
                JNI_GET_JSTRING ( key, jkey );
                JNI_GET_JSTRING ( value, jvalue );

                env->CallObjectMethod( bundle, put, jkey, jvalue );
            }
        }

        // removes 'value'; keeps 'key' for next iteration
        lua_pop ( state, 1 );
    }

    return bundle;
}

/**
 * Create an object of the provided class type.  The class must have a default constructor.
 * @param className The name of the class to create an instance of
 */
jobject JniUtils::createObjectOfClass ( STLString className ) {
	JNI_GET_ENV ( jvm, env );
    jclass Class = env->FindClass(className);
    jmethodID constructor = env->GetMethodID(Class, "<init>", "()V");
    return env->NewObject(Class, constructor);
}

/**
 * Get the method of the provided signature for the provided class.
 * @param className The name of the class that has the method
 * @param methodName The name of the method
 * @param methodSignature The JNI mehtod signature of the method.
 */
jmethodID JniUtils::getMethod ( STLString className, STLString methodName, STLString methodSignature ) {
	JNI_GET_ENV ( jvm, env );
    jclass Class = env->FindClass(className);
    return env->GetMethodID(Class, methodName, methodSignature);
}

cc8* JniUtils::parseLuaTable ( lua_State* L, int idx ) {
	switch ( lua_type ( L, idx )) {
		case LUA_TSTRING: {
			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}

	return NULL;
}
