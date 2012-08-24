// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_FLURRY

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIFlurryAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

cc8* MOAIFlurryAndroid::_luaParseTable ( lua_State* L, int idx ) {
    
	switch ( lua_type ( L, idx )) {
            
		case LUA_TSTRING: {
            
			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}
    
	return NULL;
}

jobject MOAIFlurryAndroid::_luaReadMap( lua_State * L, int idx ) {
    
	MOAILuaState state ( L );
	JNI_GET_ENV ( jvm, env );
    
	if ( state.IsType( idx, LUA_TTABLE ) ) {
        
		// table is in the stack at index 'idx'
		lua_pushnil ( state );  // first key
		int numEntries = 0;
		while ( lua_next ( state, idx ) != 0 ) {
			
			// use the 'key' (at index -2) and 'value' (at index -1)
			cc8* key = _luaParseTable ( state, -2 );
			if ( key ) {
				
				cc8* value = _luaParseTable ( state, -1 );
				if ( value ) {
					++numEntries;
				}
			}
			
			// removes 'value'; keeps 'key' for next iteration
			lua_pop ( state, 1 );
		}
        
		jclass mapClass = env->FindClass ( "java/util/HashMap" );
		jmethodID init = env->GetMethodID( mapClass, "<init>", "()V" );
		jobject hashMap = env->NewObject( mapClass, init );
        
		jmethodID put = env->GetMethodID( mapClass, "put",
                                         "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		
		// table is in the stack at index 'idx'
		lua_pushnil ( state );  // first key
		int curidx = 0;
		while ( lua_next ( state, idx ) != 0 ) {
			
			// use the 'key' (at index -2) and 'value' (at index -1)
			cc8* key = _luaParseTable ( state, -2 );
			if ( key ) {
				
				cc8* value = _luaParseTable ( state, -1 );
				if ( value ) {
                    
					JNI_GET_JSTRING ( value, jvalue );
					JNI_GET_JSTRING ( key, jkey );
                    
					env->CallObjectMethod( hashMap, put, jkey, jvalue );
					
					++curidx;
				}
			}
			
			// removes 'value'; keeps 'key' for next iteration
			lua_pop ( state, 1 );
		}
		return hashMap;
	}
	return NULL;
}

//----------------------------------------------------------------//

//----------------------------------------------------------------//
/**	@name	startSession
 @text	Start up Flurry Analytics.
 
 @in		string	apikey			The Flurry API key.
 @out 	nil
 */
int MOAIFlurryAndroid::_startSession ( lua_State* L ) {
    
	USLog::Print ( "MOAIFlurryAndroid: startSession" );
	MOAILuaState state ( L );
	
	cc8* apikey = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( apikey, japikey );
	
	jclass flurry = env->FindClass ( "com/ziplinegames/moai/MoaiFlurry" );
	if ( flurry == NULL ) {
        
		USLog::Print ( "MOAIFlurryAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFlurry" );
	} else {
        
		jmethodID startSession = env->GetStaticMethodID ( flurry, "startSession", "(Ljava/lang/String;)V" );
		if ( startSession == NULL ) {
            
			USLog::Print ( "MOAIFlurryAndroid: Unable to find static java method %s", "startSession" );
		} else {
			USLog::Print ( "MOAIFlurryAndroid: calling flurry" );
			env->CallStaticVoidMethod ( flurry, startSession, japikey );
		}
	}
	
	return 0;
}



//----------------------------------------------------------------//
/**	@name	logEvent
 @text	Log an event.
 
 @in		string	event	 The name for the event.
 @in   	table   extra	(optional)  Extra data to send with the event.
 @out 	nil
 */
int MOAIFlurryAndroid::_logEvent ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
	cc8* eventName = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( eventName, jeventName );
    
	jobject map = _luaReadMap( L, 2 );
    
	jclass flurry = env->FindClass ( "com/ziplinegames/moai/MoaiFlurry" );
	if ( flurry == NULL ) {
        
		USLog::Print ( "MOAIFlurryAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFlurry" );
	} else {
        
		if ( map == NULL ) {
			jmethodID logEvent = env->GetStaticMethodID ( flurry, "logEvent", "(Ljava/lang/String;)V" );
			if ( logEvent == NULL ) {
                
				USLog::Print ( "MOAIFlurryAndroid: Unable to find static java method %s", "logEvent" );
			} else {
				USLog::Print ( "MOAIFlurryAndroid: calling flurry event name = %s", eventName );
				env->CallStaticVoidMethod ( flurry, logEvent, jeventName );
			}
		}
		else {
			jmethodID logEvent = env->GetStaticMethodID ( flurry, "logEvent", "(Ljava/lang/String;Ljava/util/Map;)V" );
			if ( logEvent == NULL ) {
                
				USLog::Print ( "MOAIFlurryAndroid: Unable to find static java method %s", "logEvent" );
			} else {
				USLog::Print ( "MOAIFlurryAndroid: calling flurry event name = %s", eventName );
				env->CallStaticVoidMethod ( flurry, logEvent, jeventName, map );
			}
			
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	logTimedEvent
 @text	Log an event and start a timer.  You can end the event with a call to endTimedEvent
 
 @in		string	event	 The name for the event.
 @in   	table   extra	(optional)  Extra data to send with the event.
 @out 	nil
 */
int MOAIFlurryAndroid::_logTimedEvent ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
	cc8* eventName = lua_tostring ( state, 1 );
	bool timed = lua_toboolean( state, 2 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( eventName, jeventName );
    
	jobject map = _luaReadMap( L, 3 );
    
	jclass flurry = env->FindClass ( "com/ziplinegames/moai/MoaiFlurry" );
	if ( flurry == NULL ) {
        
		USLog::Print ( "MOAIFlurryAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFlurry" );
	} else {
        
		if ( map == NULL ) {
			jmethodID logEvent = env->GetStaticMethodID ( flurry, "logEvent", "(Ljava/lang/String;Z)V" );
			if ( logEvent == NULL ) {
                
				USLog::Print ( "MOAIFlurryAndroid: Unable to find static java method %s", "logEvent" );
			} else {
                
				env->CallStaticVoidMethod ( flurry, logEvent, jeventName , timed );
			}
		}
		else {
			jmethodID logEvent = env->GetStaticMethodID ( flurry, "logEvent", "(Ljava/lang/String;Ljava/util/Map;Z)V" );
			if ( logEvent == NULL ) {
                
				USLog::Print ( "MOAIFlurryAndroid: Unable to find static java method %s", "logEvent" );
			} else {
                
				env->CallStaticVoidMethod ( flurry, logEvent, jeventName, map , timed );
			}
			
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	endTimedEvent
 @text	End a timed event and send off to Flurry.
 
 @in		string	event	The event name you wish to stop the timer on.  Will match an
 earlier call to logTimedEvent.
 @out 	nil
 */
int MOAIFlurryAndroid::_endTimedEvent ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
	cc8* eventName = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( eventName, jeventName );
    
	jclass flurry = env->FindClass ( "com/ziplinegames/moai/MoaiFlurry" );
	if ( flurry == NULL ) {
        
		USLog::Print ( "MOAIFlurryAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiFlurry" );
	} else {
        
		jmethodID logEvent = env->GetStaticMethodID ( flurry, "endTimedEvent", "(Ljava/lang/String;)V" );
		if ( logEvent == NULL ) {
            
			USLog::Print ( "MOAIFlurryAndroid: Unable to find static java method %s", "logEvent" );
		} else {
            
			env->CallStaticVoidMethod ( flurry, logEvent, jeventName );
		}
	}
	return 0;
}
//================================================================//
// MOAIFlurryAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIFlurryAndroid::MOAIFlurryAndroid () {
    
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIFlurryAndroid::~MOAIFlurryAndroid () {
    
}

//----------------------------------------------------------------//
void MOAIFlurryAndroid::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "startSession",	_startSession },
		{ "logEvent",		_logEvent },
		{ "logTimedEvent",	_logTimedEvent },
		{ "endTimedEvent",	_endTimedEvent },
		{ NULL, NULL }
	};
    
	luaL_register ( state, 0, regTable );
}

#endif