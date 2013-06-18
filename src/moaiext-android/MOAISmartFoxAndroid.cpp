// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_CHARTBOOST

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAISmartFoxAndroid.h>

#include "smartfox/Core/SFSBuddyEvent.h"

using namespace Sfs2X::Core;

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Smartfox.
	
	@in		string	appId			Available in ChartBoost dashboard settings.
	@in 	string	appSignature	Available in ChartBoost dashboard settings.
	@out 	nil
*/
int MOAISmartFoxAndroid::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

    bool debug = lua_toboolean( state, 1 );
    cc8* path = lua_tostring ( state, 2 );
	bool connectOnSuccess = lua_toboolean( state, 3 );
    
    if (path == NULL)
    {
        path = "config.xml";
    }
        
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( path, jpath);
	
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {

		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {

    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "init", "(ZLjava/lang/String;Z)V" );
    	if ( javaMethod == NULL ) {

			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "init" );
    	} else {

			env->CallStaticVoidMethod ( javaClass, javaMethod,debug, jpath,connectOnSuccess  );
		}
	}
			
	return 0;
}

int MOAISmartFoxAndroid::_initWithSocket ( lua_State* L ) {
	
	MOAILuaState state ( L );
    
    bool debug = lua_toboolean( state, 1 );
    cc8* ip = lua_tostring ( state, 2 );
	int port = state.GetValue < int >( 3, 9933 );
        
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( ip, jip);
	
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "initWithSocket", "(ZLjava/lang/String;I)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "initWithSocket" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, debug, jip, port  );
		}
	}
    
	return 0;
}



int MOAISmartFoxAndroid::_connect ( lua_State* L ) {
   
	MOAILuaState state ( L );
 
    cc8* ip = lua_tostring ( state, 1 );
	int port = state.GetValue < int >( 2, 9933 );
    
	JNI_GET_ENV ( jvm, env );
	JNI_GET_JSTRING ( ip, jip);	
    
    
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "connect", "(Ljava/lang/String;I)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "connect" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jip, port );
		}
	}
    
	return 0;
    
}

// todo add return params
int MOAISmartFoxAndroid::_isConnected ( lua_State* L ) {
   
	MOAILuaState state ( L );
    
	JNI_GET_ENV ( jvm, env );
	
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "isConnected", "()Z" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "isConnected" );
    	} else {
            
			bool connected = env->CallStaticObjectMethod ( javaClass, javaMethod );
        
            lua_pushnumber(state, connected);
            
            return 1;
		}
	}
    
    lua_pushnil ( state );
    
	return 0;
    
}

int MOAISmartFoxAndroid::_sendJoinRoomRequest ( lua_State* L ) {
  
	MOAILuaState state ( L );
   
 	cc8* roomId       = state.GetValue < cc8* >( 1, "" );
    bool   asSpectator     = state.GetValue < bool >( 2, false );
    
    // this makes sure you don't leave the prior room
    //NSNumber* roomIdToLeave   = [NSNumber numberWithInt:-1];
    int roomIdToLeave = -1;

    JNI_GET_ENV ( jvm, env );
    JNI_GET_JSTRING ( roomId, jroomId );
    
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendJoinRoomRequest", "(Ljava/lang/String;IZ)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendJoinRoomRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jroomId, roomIdToLeave, asSpectator );
		}
	}
    
	return 0;
    
}

int MOAISmartFoxAndroid::_login ( lua_State* L ) {
    
	MOAILuaState state ( L );

    cc8* loginName = lua_tostring ( state, 1 );
    cc8* password = lua_tostring ( state, 2 );
    cc8* zone = lua_tostring ( state, 3 );
    cc8* gameType = lua_tostring ( state, 4 );
    
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( loginName, jloginName );
	JNI_GET_JSTRING ( password, jpassword );
	JNI_GET_JSTRING ( zone, jzone );
	JNI_GET_JSTRING ( gameType, jgameType );    
    
    
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "login", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "login" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jloginName, jpassword, jzone, jgameType );
		}
	}
    
	return 0;
    
}

int MOAISmartFoxAndroid::_sendPublicMessageRequest ( lua_State* L ) {
    
	MOAILuaState state ( L );
    
    cc8* message = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( message, jmessage );

	
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendPublicMessageRequest", "(Ljava/lang/String;)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendPublicMessageRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jmessage );
		}
	}
    
	return 0;
    
}

int MOAISmartFoxAndroid::_sendLeaveRoomRequest ( lua_State* L ) {
 
    MOAILuaState state ( L );
    
	JNI_GET_ENV ( jvm, env );
	
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendLeaveRoomRequest", "()V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendLeaveRoomRequest" );
    	} else {
			env->CallStaticVoidMethod ( javaClass, javaMethod );
		}
	}
    
	return 0;
    
}

int MOAISmartFoxAndroid::_sendRoomVariablesRequest ( lua_State* L ) {
    
	return 0;    
}

int MOAISmartFoxAndroid::_sendQuickJoinGameRequest ( lua_State* L ) {
    
    MOAILuaState state ( L );
    
    cc8* name = lua_tostring ( state, 1 );
	int matchOperator = state.GetValue < int >( 2, 0 );
    
    ostringstream convert;   // stream used for the conversion
    
    if (matchOperator == BOOL_MATCH_EQUALS) {
		
		bool value = state.GetValue < bool >( 3, false );
        convert << value;
	}
	else if (matchOperator == BOOL_MATCH_NOT_EQUALS) {
        
		bool value = state.GetValue < bool >( 3, false );
        convert << value;		
		
	}
	else if (matchOperator == NUMBER_MATCH_EQUALS){
		
		int value = state.GetValue < int >( 3, 0 );
        convert << value;		
	}
	else if (matchOperator == NUMBER_MATCH_NOT_EQUALS){
        
        int value = state.GetValue < int >( 3, 0 );
        convert << value;
	}
	else if (matchOperator == NUMBER_MATCH_GREATER_THAN){
        
		int value = state.GetValue < int >( 3, 0 );
        convert << value;
	}
	else if (matchOperator == NUMBER_MATCH_GREATER_THAN_OR_EQUAL_TO){
		int value = state.GetValue < int >( 3, 0 );
        convert << value;
	}
	else if (matchOperator == NUMBER_MATCH_LESS_THAN){
		
		int value = state.GetValue < int >( 3, 0 );
        convert << value;
	}
	else if (matchOperator == NUMBER_MATCH_LESS_THAN_OR_EQUAL_TO){
        
		int value = state.GetValue < int >( 3, 0 );
        convert << value;
		
	}
	else if (matchOperator == STRING_MATCH_EQUALS){
        
		cc8* value = state.GetValue < cc8* >( 3, "" );
        convert << value;
		
	}
	else if (matchOperator == STRING_MATCH_NOT_EQUALS){
        
		cc8* value = state.GetValue < cc8* >( 3, "" );
        convert << value;
		
	}
	else if (matchOperator == STRING_MATCH_CONTAINS){
        
		cc8* value = state.GetValue < cc8* >( 3, "" );
        convert << value;
		
	}
	else if (matchOperator == STRING_MATCH_STARTS_WITH){
        
		cc8* value = state.GetValue < cc8* >( 3, "" );
        convert << value;
		
	}
	else if (matchOperator == STRING_MATCH_ENDS_WITH){
        
		cc8* value = state.GetValue < cc8* >( 3, "" );
        convert << value;
	}
	else {
		
		USLog::Print ("invalid match type %i", matchOperator);
	}
    
    string valueString = convert.str();
    
    cc8* groupName = lua_tostring ( state, 4 );

    JNI_GET_ENV ( jvm, env );
    JNI_GET_JSTRING ( name, jname );
	JNI_GET_JSTRING ( groupName, jgroupName );
	JNI_GET_JSTRING ( valueString.c_str(), jvalueString );
     
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendQuickJoinGameRequest", "(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendQuickJoinGameRequest" );
    	} else {
			env->CallStaticVoidMethod ( javaClass, javaMethod, jname, matchOperator, jvalueString, jgroupName);
		}
	}
    

	return 0;     
}

int MOAISmartFoxAndroid::_sendUserVariablesRequest ( lua_State* L ) {
    
 	return 0;    
}

int MOAISmartFoxAndroid::_sendObjectMessageRequest ( lua_State* L ) {
    
	MOAILuaState state ( L );

    cc8* cmd = lua_tostring ( state, 1 );
    cc8* message = lua_tostring ( state, 2 );
    
	JNI_GET_ENV ( jvm, env );

    JNI_GET_JSTRING ( cmd, jcmd );
	JNI_GET_JSTRING ( message, jmessage );

	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendObjectMessageRequest", "(Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendObjectMessageRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jcmd, jmessage );
		}
	}
    
	return 0;
    
}

int MOAISmartFoxAndroid::_sendExtensionRequest ( lua_State* L ) {
    
	MOAILuaState state ( L );
    
    cc8* cmd = lua_tostring ( state, 1 );
    cc8* message = lua_tostring ( state, 2 );
    
	JNI_GET_ENV ( jvm, env );
    
    JNI_GET_JSTRING ( cmd, jcmd );
	JNI_GET_JSTRING ( message, jmessage );
    
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendExtensionRequest", "(Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendExtensionRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jcmd, jmessage );
		}
	}
    
	return 0;
    
}

int MOAISmartFoxAndroid::_sendInitBuddyListRequest ( lua_State* L ) {
 
	MOAILuaState state ( L );
    
	JNI_GET_ENV ( jvm, env );

	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendInitBuddyListRequest", "()V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendInitBuddyListRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod );
		}
	}
    
	return 0;
    
}

int MOAISmartFoxAndroid::_sendAddBuddyRequest ( lua_State* L ) {

	MOAILuaState state ( L );
    
    cc8* name = lua_tostring ( state, 1 );
    
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( name, jname);
    
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendAddBuddyRequest", "(Ljava/lang/String;)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendAddBuddyRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jname );
		}
	}
    
	return 0;
    
}

int MOAISmartFoxAndroid::_sendRemoveBuddyRequest ( lua_State* L ) {
 
	MOAILuaState state ( L );
    
    cc8* name = lua_tostring ( state, 1 );
    
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( name, jname);
    
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendRemoveBuddyRequest", "(Ljava/lang/String;)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendRemoveBuddyRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jname );
		}
	}
    
	return 0;
    
}

int MOAISmartFoxAndroid::_sendBlockBuddyRequest ( lua_State* L ) {

	MOAILuaState state ( L );
    
    cc8* name = lua_tostring ( state, 1 );
	bool block = lua_toboolean( state, 2 );
    
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( name, jname);
    
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendBlockBuddyRequest", "(Ljava/lang/String;Z)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendBlockBuddyRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jname, block );
		}
	}
    
	return 0;
    
    
}

int MOAISmartFoxAndroid::_sendGoOnlineRequest ( lua_State* L ) {

	MOAILuaState state ( L );
    
	bool online = lua_toboolean( state, 1 );
    
	JNI_GET_ENV ( jvm, env );
	
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendGoOnlineRequest", "(Z)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendGoOnlineRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, online );
		}
	}
    
	return 0;
    
    
}

int MOAISmartFoxAndroid::_sendSetBuddyVariablesRequest ( lua_State* L ) {
    
	return 0;    
}

int MOAISmartFoxAndroid::_sendSpectatorToPlayerRequest ( lua_State* L ) {
    
	return 0;    
}

int MOAISmartFoxAndroid::_sendPlayerToSpectatorRequest ( lua_State* L ) {
    
 	return 0;   
}

int MOAISmartFoxAndroid::_sendFindRoomsRequest ( lua_State* L ) {
    
	return 0;    
}

int MOAISmartFoxAndroid::_sendSubscribeRoomGroupRequest ( lua_State* L ) {
    
 	MOAILuaState state ( L );
    
    cc8* name = lua_tostring ( state, 1 );
    
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( name, jname);
    
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendSubscribeRoomGroupRequest", "(Ljava/lang/String;)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendSubscribeRoomGroupRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jname );
		}
	}
    
	return 0;
}

int MOAISmartFoxAndroid::_sendUnsubscribeRoomGroupRequest ( lua_State* L ) {
    
	MOAILuaState state ( L );
    
    cc8* name = lua_tostring ( state, 1 );
    
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( name, jname);
    
	jclass javaClass = env->FindClass ( "com/ziplinegames/moai/MoaiSmartFox" );
    if ( javaClass == NULL ) {
        
		USLog::Print ( "MOAISmartFoxAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiSmartFox" );
    } else {
        
    	jmethodID javaMethod = env->GetStaticMethodID ( javaClass, "sendUnsubscribeRoomGroupRequest", "(Ljava/lang/String;)V" );
    	if ( javaMethod == NULL ) {
            
			USLog::Print ( "MOAISmartFoxAndroid: Unable to find static java method %s", "sendUnsubscribeRoomGroupRequest" );
    	} else {
            
			env->CallStaticVoidMethod ( javaClass, javaMethod, jname );
		}
	}
    
	return 0;
}

//================================================================//
// MOAISmartFoxAndroid
//================================================================//

//----------------------------------------------------------------//
MOAISmartFoxAndroid::MOAISmartFoxAndroid () {
    
	RTTI_SINGLE ( MOAILuaObject )
	RTTI_SINGLE ( MOAIGlobalEventSource )
    
}

//----------------------------------------------------------------//
MOAISmartFoxAndroid::~MOAISmartFoxAndroid () {
    
}

//----------------------------------------------------------------//
void MOAISmartFoxAndroid::RegisterLuaClass ( MOAILuaState& state ) {
    
    USLog::Print ( "MOAISmartAndroid: registering lua class ");
   
	state.SetField ( -1, "ON_CONNECTION",       ( u32 )ON_CONNECTION );
	state.SetField ( -1, "ON_CONNECTION_LOST",	( u32 )ON_CONNECTION_LOST );
	state.SetField ( -1, "ON_CONNECTION_RETRY",       ( u32 )ON_CONNECTION_RETRY );
	state.SetField ( -1, "ON_CONNECTION_RESUME",	( u32 )ON_CONNECTION_RESUME );
	state.SetField ( -1, "ON_LOGIN",				( u32 )ON_LOGIN );
	state.SetField ( -1, "ON_LOGOUT",				( u32 )ON_LOGOUT );
	state.SetField ( -1, "ON_ROOM_JOIN",			( u32 )ON_ROOM_JOIN );
	state.SetField ( -1, "ON_ROOM_JOIN_ERROR",		( u32 )ON_ROOM_JOIN_ERROR );
	state.SetField ( -1, "ON_USER_ENTER_ROOM",		( u32 )ON_USER_ENTER_ROOM );
	state.SetField ( -1, "ON_USER_EXIT_ROOM",		( u32 )ON_USER_EXIT_ROOM );
	state.SetField ( -1, "ON_USER_COUNT_CHANGE",	( u32 )ON_USER_COUNT_CHANGE );
	state.SetField ( -1, "ON_ROOM_ADD",				( u32 )ON_ROOM_ADD );
	state.SetField ( -1, "ON_ROOM_REMOVE",			( u32 )ON_ROOM_REMOVE );
	state.SetField ( -1, "ON_PUBLIC_MESSAGE",		( u32 )ON_PUBLIC_MESSAGE );
	state.SetField ( -1, "ON_PRIVATE_MESSAGE",		( u32 )ON_PRIVATE_MESSAGE );
	state.SetField ( -1, "ON_ROOM_CREATION_ERROR",	( u32 )ON_ROOM_CREATION_ERROR );
	state.SetField ( -1, "ON_ROOM_VARIABLES_UDATE",	( u32 )ON_ROOM_VARIABLES_UDATE );
	state.SetField ( -1, "ON_OBJECT_MESSAGE",		( u32 )ON_OBJECT_MESSAGE );
	state.SetField ( -1, "ON_EXTENSION_RESPONSE",	( u32 )ON_EXTENSION_RESPONSE );
    
	state.SetField ( -1, "ON_ROOM_FIND_RESULT",		( u32 )ON_ROOM_FIND_RESULT );
    
    state.SetField ( -1, "ON_INVITATION",	( u32 )ON_INVITATION );
	state.SetField ( -1, "ON_INVITATION_REPLY",	( u32 )ON_INVITATION_REPLY );
	state.SetField ( -1, "ON_INVITATION_REPLY_ERROR",	( u32 )ON_INVITATION_REPLY_ERROR );
    
	state.SetField ( -1, "ON_BUDDY_MESSAGE",	( u32 )ON_BUDDY_MESSAGE );
	state.SetField ( -1, "ON_BUDDY_LIST_INIT",	( u32 )ON_BUDDY_LIST_INIT );
	state.SetField ( -1, "ON_BUDDY_ADD",	( u32 )ON_BUDDY_ADD );
	state.SetField ( -1, "ON_BUDDY_REMOVE",	( u32 )ON_BUDDY_REMOVE );
	state.SetField ( -1, "ON_BUDDY_BLOCK",	( u32 )ON_BUDDY_BLOCK );
	state.SetField ( -1, "ON_BUDDY_ONLINE_STATUS_UPDATE",	( u32 )ON_BUDDY_ONLINE_STATUS_UPDATE );
	state.SetField ( -1, "ON_BUDDY_VARIABLE_UPDATE",	( u32 )ON_BUDDY_VARIABLE_UPDATE );
	state.SetField ( -1, "ON_BUDDY_ERROR",	( u32 )ON_BUDDY_ERROR );
    
	state.SetField ( -1, "ON_SPECTATOR_TO_PLAYER",	( u32 )ON_SPECTATOR_TO_PLAYER );
	state.SetField ( -1, "ON_SPECTATOR_TO_PLAYER_ERROR",	( u32 )ON_SPECTATOR_TO_PLAYER_ERROR );
	state.SetField ( -1, "ON_PLAYER_TO_SPECTATOR",	( u32 )ON_PLAYER_TO_SPECTATOR );
	state.SetField ( -1, "ON_PLAYER_TO_SPECTATOR_ERROR",	( u32 )ON_PLAYER_TO_SPECTATOR_ERROR );
    
	state.SetField ( -1, "ON_SUBSCRIBE_ROOM_GROUP",	( u32 )ON_SUBSCRIBE_ROOM_GROUP );
	state.SetField ( -1, "ON_SUBSCRIBE_ROOM_GROUP_ERROR",	( u32 )ON_SUBSCRIBE_ROOM_GROUP_ERROR );
	state.SetField ( -1, "ON_UNSUBSCRIBE_ROOM_GROUP",	( u32 )ON_UNSUBSCRIBE_ROOM_GROUP );
	state.SetField ( -1, "ON_UNSUBSCRIBE_ROOM_GROUP_ERROR",	( u32 )ON_UNSUBSCRIBE_ROOM_GROUP_ERROR );
    
	state.SetField ( -1, "ON_SMARTFOX_EVENT",       ( u32 )ON_SMARTFOX_EVENT ); 
    
    
	state.SetField ( -1, "BOOL_MATCH_EQUALS",		( u32 )BOOL_MATCH_EQUALS );
	state.SetField ( -1, "BOOL_MATCH_NOT_EQUALS",	( u32 )BOOL_MATCH_NOT_EQUALS );
	state.SetField ( -1, "NUMBER_MATCH_EQUALS",		( u32 )NUMBER_MATCH_EQUALS );
	state.SetField ( -1, "NUMBER_MATCH_NOT_EQUALS",		( u32 )NUMBER_MATCH_NOT_EQUALS );
	state.SetField ( -1, "NUMBER_MATCH_GREATER_THAN",	( u32 )NUMBER_MATCH_GREATER_THAN );
	state.SetField ( -1, "NUMBER_MATCH_GREATER_THAN_OR_EQUAL_TO",	( u32 )NUMBER_MATCH_GREATER_THAN_OR_EQUAL_TO );
	state.SetField ( -1, "NUMBER_MATCH_LESS_THAN",		( u32 )NUMBER_MATCH_LESS_THAN );
	state.SetField ( -1, "NUMBER_MATCH_LESS_THAN_OR_EQUAL_TO",	( u32 )NUMBER_MATCH_LESS_THAN_OR_EQUAL_TO );
	state.SetField ( -1, "STRING_MATCH_EQUALS",	( u32 )STRING_MATCH_EQUALS );
	state.SetField ( -1, "STRING_MATCH_NOT_EQUALS",		( u32 )STRING_MATCH_NOT_EQUALS );
	state.SetField ( -1, "STRING_MATCH_CONTAINS",	( u32 )STRING_MATCH_CONTAINS );
	state.SetField ( -1, "STRING_MATCH_STARTS_WITH",	( u32 )STRING_MATCH_STARTS_WITH );
	state.SetField ( -1, "STRING_MATCH_ENDS_WITH",	( u32 )STRING_MATCH_ENDS_WITH );
	
	
	luaL_Reg regTable[] = {
		{ "init",					_init },
		{ "initWithSocket",			_initWithSocket },
		{ "login",					_login },
		{ "connect",				_connect },
		{ "isConnected",			_isConnected },
		{ "sendPublicMessageRequest",		_sendPublicMessageRequest},
		{ "sendJoinRoomRequest",	_sendJoinRoomRequest},
		{ "setListener",			&MOAIGlobalEventSource::_setListener < MOAISmartFoxAndroid > },
		{ "sendRoomVariablesRequest",      _sendRoomVariablesRequest},
		{ "sendUserVariablesRequest",      _sendUserVariablesRequest},
		{ "sendObjectMessageRequest", _sendObjectMessageRequest},
		{ "sendQuickJoinGameRequest", _sendQuickJoinGameRequest},
		{ "sendLeaveRoomRequest", _sendLeaveRoomRequest},
		{ "sendExtensionRequest", _sendExtensionRequest},
		{ "sendInitBuddyListRequest", _sendInitBuddyListRequest},
		{ "sendAddBuddyRequest",	_sendAddBuddyRequest},
		{ "sendRemoveBuddyRequest", _sendRemoveBuddyRequest},
		{ "sendBlockBuddyRequest",	_sendBlockBuddyRequest},
		{ "sendGoOnlineRequest",	_sendGoOnlineRequest},
		{ "sendSetBuddyVariablesRequest",	_sendSetBuddyVariablesRequest},
		{ "sendPlayerToSpectatorRequest", _sendPlayerToSpectatorRequest},
		{ "sendSpectatorToPlayerRequest", _sendSpectatorToPlayerRequest},
		{ "sendFindRoomsRequest", _sendFindRoomsRequest},
		{ "sendSubscribeRoomGroupRequest", _sendSubscribeRoomGroupRequest},
		{ "sendUnsubscribeRoomGroupRequest", _sendUnsubscribeRoomGroupRequest},
		{ NULL, NULL }
	};
    
	luaL_register ( state, 0, regTable );
}

//-------------------------------------------------------------
void MOAISmartFoxAndroid::Connection(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
    
	if ( this->PushListener ( ON_CONNECTION, state )) {
        
        /*
        if ([[evt.params objectForKey:@"success"] boolValue])
        {
            NSLog(@"Connection was established");
        }
        else
        {
            NSLog(@"Connection failed");
        }
        
        state.Push ( [[evt.params objectForKey:@"success"] boolValue] );
        */
        
        //state.Push ( success );
		state.DebugCall ( 1, 0 );
	}
	
}

void MOAISmartFoxAndroid::ConnectionLost(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_CONNECTION_LOST, state )) {
		/*
        NSLog(@"Connection was lost, Reason: %@", [evt.params objectForKey:@"reason"]);
        
        NSString* reason = [evt.params objectForKey:@"reason"];
        
        state.Push ( [ reason UTF8String ] );
         */
		state.DebugCall ( 1, 0 );
	}
	
}

void MOAISmartFoxAndroid::ConnectionRetry(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_CONNECTION_RETRY, state )) {
		
        // state.Push ( [ reason UTF8String ] );
		state.DebugCall ( 0, 0 );
	}
	
}

void MOAISmartFoxAndroid::ConnectionResume(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_CONNECTION_RESUME, state )) {
		
        //state.Push ( [ reason UTF8String ] );
		state.DebugCall ( 0, 0 );
	}
	
}

void MOAISmartFoxAndroid::Login(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_LOGIN, state )) {
		
		
		//NSLog(@"Hi, I have just logged in as: %@", [[evt.params objectForKey:@"user"] name]);
		//NSLog(@"Hi, I am in zone: %@", [evt.params objectForKey:@"zone"]);
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxAndroid::Logout(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_LOGOUT, state )) {
		
		
		//NSLog(@"Hi, I have just logged in as: %@", [[evt.params objectForKey:@"user"] name]);
		//NSLog(@"Hi, I am in zone: %@", [evt.params objectForKey:@"zone"]);
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxAndroid::LoginError(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_LOGIN_ERROR, state )) {
		
        //NSLog(@"Login Failed. Reason: %@" ,[evt.params objectForKey:@"errorMessage"]);
        
		state.DebugCall ( 0, 0 );
	}
}




void MOAISmartFoxAndroid::RoomJoin(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	/*
	if ( this->PushListener ( ON_ROOM_JOIN, state )) {
        
		lua_newtable ( state );
		
		SFSRoom *room = [evt.params objectForKey:@"room"];
		NSLog(@"The Room %@ was successfully joined!", room.name);
		
		// save the room reference for later logout
		MOAISmartFoxAndroid::Get ().mCurrentRoom = room;
		
		NSArray* players = [room userList];
		
		if ( [room isGame] )
		{
			NSLog(@"is game");
		}
		else{
			NSLog(@"is not game");
		}
		
		state.SetField ( -1, "name", [ room.name UTF8String ]);
		state.SetField ( -1, "groupId", [ room.groupId	UTF8String ]);
		state.SetField ( -1, "id", room.id);
        
		NSLog(@"about to count players %i", [room userCount]);
		
		lua_pushstring ( state, "userList" );
		lua_newtable ( state );
		
		// interate through the player list
		int count = 1;
		for ( SFSUser* player in players ) {
            
			lua_pushnumber ( state, count++ );
			lua_newtable ( state );
            
			//state.SetField ( -1, "id", [ player  ]);
            
            if ( [room isGame] ){
                state.SetField ( -1, "playerId", player.playerId);
            }
            
			state.SetField ( -1, "name", [ player.name UTF8String ]);
			state.SetField ( -1, "id",  player.id  );
			state.SetField ( -1, "isItMe", player.isItMe);
			state.SetField ( -1, "isSpectator", player.isSpectator);
			
            lua_pushstring ( state, "userVariables" );
            lua_newtable ( state );
            
            // get the variables for this player
            NSArray* userVariables = [player getVariables];
            
            int variableCount = 1;
            for ( SFSUserVariable* variable in userVariables ) {
                
                NSLog(@"about to get user variable named %@", variable.name);
                
                //lua_newtable ( state );
                //lua_pushstring ( state, [variable.name UTF8String ]);
                
                if (variable.type == @"String"){
                    
                    NSLog(@"got value for variable %@", [variable getStringValue]);
                    
                    //lua_pushstring(state, [[variable getStringValue] UTF8String]);
                    state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
                }
                
                if (variable.type == @"Int"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
                }
                
                if (variable.type == @"Bool"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
                }
                
                if (variable.type == @"Double"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
                }
                
                //lua_settable ( state, -3 );
                
            }
            
            // end user variables table
            lua_settable ( state, -3 );
            
            
			printf("set table");
			
            // end player table
			lua_settable ( state, -3 );
			
		}
		
		lua_settable ( state, -3 );
		
		NSArray* variables = [room getVariables];
        
        lua_pushstring ( state, "roomVariables" );
        lua_newtable ( state );
        
		int count2 = 1;
		for ( SFSRoomVariable* variable in variables ) {
			
            NSLog(@"about to get room variable named %@", variable.name);
            
            //lua_newtable ( state );
            //lua_pushstring ( state, [variable.name UTF8String ]);
            
            if (variable.type == @"String"){
                
                NSLog(@"got value for variable %@", [variable getStringValue]);
                
                //lua_pushstring(state, [[variable getStringValue] UTF8String]);
                state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
            }
            
            if (variable.type == @"Int"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
            }
            
            if (variable.type == @"Bool"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
            }
            
            if (variable.type == @"Double"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
            }
		}
		
        // end room variable table
        lua_settable ( state, -3 );
        
		NSLog(@"calling back with count %i", count);
		
		state.DebugCall ( 1, 0 );
	}
    */
}

void MOAISmartFoxAndroid::RoomJoinError(SFSEvent *evt)
{
    
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	// get all the users from the
	if ( this->PushListener ( ON_ROOM_JOIN_ERROR, state )) {
        
        
        //NSLog(@"Join Room Failure:  %@", [evt.params objectForKey:@"errorMessage"]);
        
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxAndroid::UserEnterRoom(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	/*
	if ( this->PushListener ( ON_USER_ENTER_ROOM, state )) {
        
		SFSRoom *room = [evt.params objectForKey:@"room"];
		
		lua_newtable ( state );
		
		//lua_pushnumber ( state, 1 );
		//lua_newtable ( state );
		
		SFSUser *user = [evt.params objectForKey:@"user"];
		
		// interate through the player list
		
		if ( [room isGame] ){
			state.SetField ( -1, "playerId", user.playerId);
            NSLog(@"playerid: %i", user.playerId);
		}
		
		state.SetField ( -1, "name", [ user.name UTF8String ]);
		state.SetField ( -1, "id",  user.id  );
		
		NSLog(@"id: %i isitme: %i", user.id, user.isItMe);
		
		state.SetField ( -1, "isItMe", user.isItMe);
		state.SetField ( -1, "isSpectator", user.isSpectator);
		
		NSLog(@"User: %@ has just joined Room: %@", user.name, room.name);
		
        lua_pushstring ( state, "userVariables" );
        lua_newtable ( state );
        
        // get the variables for this player
        NSArray* userVariables = [user getVariables];
        
        int variableCount = 1;
        for ( SFSUserVariable* variable in userVariables ) {
            
            NSLog(@"about to get variable named %@", variable.name);
            
            //lua_newtable ( state );
            //lua_pushstring ( state, [variable.name UTF8String ]);
            
            if (variable.type == @"String"){
                
                NSLog(@"got value for variable %@", [variable getStringValue]);
                
                //lua_pushstring(state, [[variable getStringValue] UTF8String]);
                state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
            }
            
            if (variable.type == @"Int"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
            }
            
            if (variable.type == @"Bool"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
            }
            
            if (variable.type == @"Double"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
            }
            
            //lua_settable ( state, -3 );
            
        }
        
        // end user variables table
        lua_settable ( state, -3 );
		
		NSLog(@"about to do debug call");
		
		state.DebugCall ( 1, 0 );
	}
     */
}

void MOAISmartFoxAndroid::UserExitRoom(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
    /*
	if ( this->PushListener ( ON_USER_EXIT_ROOM, state )) {
        
		SFSRoom *room = [evt.params objectForKey:@"room"];
		
		lua_newtable ( state );
		//lua_pushstring ( state, "user" );
		//lua_newtable ( state );
		
		SFSUser *user = [evt.params objectForKey:@"user"];
        
		
		if ( [room isGame] ){
			state.SetField ( -1, "playerId", user.playerId);
            NSLog(@"playerid: %i", user.playerId);
        }
        
		// interate through the player list
		state.SetField ( -1, "name", [ user.name UTF8String ]);
		state.SetField ( -1, "id",  user.id  );
		
		
		NSLog(@"id: %i isitme: %i", user.id, user.isItMe);
		
		state.SetField ( -1, "isItMe", user.isItMe);
		state.SetField ( -1, "isSpectator", user.isSpectator);
		
		//lua_settable ( state, -3 );
		//lua_settable ( state, -3 );
		
		NSLog(@"User: %@ has just left Room: %@", user.name, room.name);
        
        lua_pushstring ( state, "userVariables" );
        lua_newtable ( state );
        
        // get the variables for this player
        NSArray* userVariables = [user getVariables];
        
        int variableCount = 1;
        for ( SFSUserVariable* variable in userVariables ) {
            
            NSLog(@"about to get variable named %@", variable.name);
            
            //lua_newtable ( state );
            //lua_pushstring ( state, [variable.name UTF8String ]);
            
            if (variable.type == @"String"){
                
                NSLog(@"got value for variable %@", [variable getStringValue]);
                
                //lua_pushstring(state, [[variable getStringValue] UTF8String]);
                state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
            }
            
            if (variable.type == @"Int"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
            }
            
            if (variable.type == @"Bool"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
            }
            
            if (variable.type == @"Double"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
            }
            
            //lua_settable ( state, -3 );
            
        }
        
        // end user variables table
        lua_settable ( state, -3 );
        
		state.DebugCall ( 1, 0 );
	}
     */
}

void MOAISmartFoxAndroid::UserCountChange(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_USER_COUNT_CHANGE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxAndroid::RoomAdd(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_ROOM_ADD, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxAndroid::RoomRemove(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_ROOM_REMOVE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxAndroid::PublicMessage(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	/*
	SFSUser *sender = [evt.params objectForKey:@"sender"];
    NSLog(@"%@: %@", sender.name, [evt.params objectForKey:@"message"]);
	
	NSString* name = sender.name;
	NSString* message = [evt.params objectForKey:@"message"];
	
	if ( this->PushListener ( ON_PUBLIC_MESSAGE, state )) {
		
		state.Push ( [ name UTF8String ] );
		state.Push ( [ message UTF8String] );
		state.DebugCall ( 2, 0 );
	}
     */
}

void MOAISmartFoxAndroid::PrivateMessage(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
    /*
	SFSUser *sender = [evt.params objectForKey:@"sender"];
    NSLog(@"%@: %@", sender.name, [evt.params objectForKey:@"message"]);
	
	NSString* name = sender.name;
	NSString* message = [evt.params objectForKey:@"message"];
	
	
	if ( this->PushListener ( ON_PRIVATE_MESSAGE, state )) {
		
		state.Push ( [ name UTF8String ] );
		state.Push ( [ message UTF8String] );
		state.DebugCall ( 2, 0 );
	}
     */
}

void MOAISmartFoxAndroid::RoomCreationError(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_ROOM_CREATION_ERROR, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxAndroid::RoomVariablesUpdate(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( ON_ROOM_VARIABLES_UDATE, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxAndroid::ObjectMessage(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

	if ( this->PushListener ( ON_OBJECT_MESSAGE, state )) {
    
        //state.Push ( cmd);
        //state.Push ( message );
        
        //state.DebugCall ( 2, 0 );
    }
    
    /*
	if ( this->PushListener ( ON_OBJECT_MESSAGE, state )) {
		
		SFSObject *obj = [evt.params objectForKey:@"message"];
		
		
		// NSLog(@"Received data: %@", [obj getDump]);
		
		//NSArray* keys = [obj getKeys];
		
        //	for (NSString* object in keys) {
        // do something with object
        //		NSLog(@"object key %@", object);
		
        //	}
		
		NSString* cmd = [obj getUtfString:@"cmd"];
		NSString* message = [obj getUtfString:@"message"];
        
		// NSLog(@"command and message %@: %@", cmd, message);
		
		state.Push ( [ cmd UTF8String ] );
		state.Push ( [ message UTF8String] );
        
		state.DebugCall ( 2, 0 );
	}
     */
}

void MOAISmartFoxAndroid::ExtensionResponse(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	/*
	if ( this->PushListener ( ON_EXTENSION_RESPONSE, state )) {
		
		
		NSString* cmd = [evt.params objectForKey:@"cmd"];
		SFSObject *obj = [evt.params objectForKey:@"params"];
		NSString* message = [obj getUtfString:@"message"];
		
        
		state.Push ( [ cmd UTF8String ] );
		state.Push ( [ message UTF8String] );
		
		state.DebugCall ( 2, 0 );
	}
     */
}

void MOAISmartFoxAndroid::Invitation(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
    
	if ( this->PushListener ( ON_INVITATION, state )) {
		
	}
	
}

void MOAISmartFoxAndroid::InvitationReply(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_INVITATION_REPLY, state )) {
		
	}
}

void MOAISmartFoxAndroid::InvitationReplyError(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_INVITATION_REPLY_ERROR, state )) {
		
	}
}

void MOAISmartFoxAndroid::BuddyMessage(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_BUDDY_MESSAGE, state )) {
		
	}
}

void MOAISmartFoxAndroid::BuddyListInit(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
    /*
    if ( this->PushListener ( ON_BUDDY_LIST_INIT, state )) {
        
		lua_newtable ( state );
        
        lua_pushstring ( state, "buddyList" );
        lua_newtable ( state );
        
        // get the variables for this player
        NSArray* buddyList = [evt.params objectForKey:@"buddyList"];
        
        int count = 1;
        for ( SFSBuddy* buddy in buddyList ) {
            
			lua_pushnumber ( state, count++ );
			lua_newtable ( state );
            
            state.SetField ( -1, "name", [ buddy.name UTF8String ]);
            state.SetField ( -1, "state", [ buddy.state UTF8String ]);
            
            state.SetField ( -1, "isOnline", buddy.isOnline);
            state.SetField ( -1, "isBlocked", buddy.isBlocked);
            state.SetField ( -1, "isTemp", buddy.isTemp);
            
			lua_pushstring ( state, "userVariables" );
			lua_newtable ( state );
			
			// get the variables for this player
			NSArray* userVariables = [buddy variables];
			
			int variableCount = 1;
			for ( SFSUserVariable* variable in userVariables ) {
				
				NSLog(@"about to get variable named %@", variable.name);
				
				//lua_newtable ( state );
				//lua_pushstring ( state, [variable.name UTF8String ]);
				
				if (variable.type == @"String"){
					
					NSLog(@"got value for variable %@", [variable getStringValue]);
					
					//lua_pushstring(state, [[variable getStringValue] UTF8String]);
					state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
				}
				
				if (variable.type == @"Int"){
					
					state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
				}
				
				if (variable.type == @"Bool"){
					
					state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
				}
				
				if (variable.type == @"Double"){
					
					state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
				}
				
				//lua_settable ( state, -3 );
			}
			
			// end user variables table
			lua_settable ( state, -3 );
            
			// end buddy
            lua_settable ( state, -3 );
        }
        
        // end buddy list
        lua_settable ( state, -3 );
        
        state.DebugCall ( 1, 0 );
	}
     */
}

void MOAISmartFoxAndroid::BuddyAdd(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_BUDDY_ADD, state )) {
		
	}
}

void MOAISmartFoxAndroid::BuddyRemove(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_BUDDY_REMOVE, state )) {
		
	}
}

void MOAISmartFoxAndroid::BuddyBlock(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_BUDDY_BLOCK, state )) {
		
	}
}

void MOAISmartFoxAndroid::BuddyOnlineStatusUpdate(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_BUDDY_ONLINE_STATUS_UPDATE, state )) {
		
	}
}

void MOAISmartFoxAndroid::BuddyVariablesUpdate(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_BUDDY_VARIABLE_UPDATE, state )) {
		
	}
}

void MOAISmartFoxAndroid::BuddyError(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_BUDDY_ERROR, state )) {
		
	}
}

void MOAISmartFoxAndroid::SpectatorToPlayer(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
    /*
    if ( this->PushListener ( ON_SPECTATOR_TO_PLAYER, state )) {
        
		lua_newtable ( state );
		
		SFSRoom *room = [evt.params objectForKey:@"room"];
		NSLog(@"The Room %@ was successfully joined!", room.name);
		
		// save the room reference for later logout
		MOAISmartFoxAndroid::Get ().mCurrentRoom = room;
		
		NSArray* players = [room userList];
		
		if ( [room isGame] )
		{
			NSLog(@"is game");
		}
		else{
			NSLog(@"is not game");
		}
		
		state.SetField ( -1, "name", [ room.name UTF8String ]);
		state.SetField ( -1, "groupId", [ room.groupId	UTF8String ]);
		state.SetField ( -1, "id", room.id);
		
		NSLog(@"about to count players %i", [room userCount]);
		
		lua_pushstring ( state, "userList" );
		lua_newtable ( state );
		
		// interate through the player list
		int count = 1;
		for ( SFSUser* player in players ) {
			
			lua_pushnumber ( state, count++ );
			lua_newtable ( state );
			
			//state.SetField ( -1, "id", [ player  ]);
            
            if ( [room isGame] ){
                state.SetField ( -1, "playerId", player.playerId);
            }
			
			state.SetField ( -1, "name", [ player.name UTF8String ]);
			state.SetField ( -1, "id",  player.id  );
			state.SetField ( -1, "isItMe", player.isItMe);
			state.SetField ( -1, "isSpectator", player.isSpectator);
			
            lua_pushstring ( state, "userVariables" );
            lua_newtable ( state );
            
            // get the variables for this player
            NSArray* userVariables = [player getVariables];
            
            int variableCount = 1;
            for ( SFSUserVariable* variable in userVariables ) {
                
                NSLog(@"about to get user variable named %@", variable.name);
                
                //lua_newtable ( state );
                //lua_pushstring ( state, [variable.name UTF8String ]);
                
                if (variable.type == @"String"){
                    
                    NSLog(@"got value for variable %@", [variable getStringValue]);
                    
                    //lua_pushstring(state, [[variable getStringValue] UTF8String]);
                    state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
                }
                
                if (variable.type == @"Int"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
                }
                
                if (variable.type == @"Bool"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
                }
                
                if (variable.type == @"Double"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
                }
                
                //lua_settable ( state, -3 );
                
            }
            
            // end user variables table
            lua_settable ( state, -3 );
            
            
			printf("set table");
			
            // end player table
			lua_settable ( state, -3 );
			
		}
		
		lua_settable ( state, -3 );
		
		NSArray* variables = [room getVariables];
		
        lua_pushstring ( state, "roomVariables" );
        lua_newtable ( state );
        
		int count2 = 1;
		for ( SFSRoomVariable* variable in variables ) {
			
            NSLog(@"about to get room variable named %@", variable.name);
            
            //lua_newtable ( state );
            //lua_pushstring ( state, [variable.name UTF8String ]);
            
            if (variable.type == @"String"){
                
                NSLog(@"got value for variable %@", [variable getStringValue]);
                
                //lua_pushstring(state, [[variable getStringValue] UTF8String]);
                state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
            }
            
            if (variable.type == @"Int"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
            }
            
            if (variable.type == @"Bool"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
            }
            
            if (variable.type == @"Double"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
            }
		}
		
        // end room variable table
        lua_settable ( state, -3 );
        
		NSLog(@"calling back with count %i", count);
		
		state.DebugCall ( 1, 0 );
		
	}
     */
}

void MOAISmartFoxAndroid::SpectatorToPlayerError(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_SPECTATOR_TO_PLAYER_ERROR, state )) {
        
		//NSLog(@"Spectator to player error:  %@", [evt.params objectForKey:@"errorMessage"]);
		state.DebugCall ( 0, 0 );
	}
}

void MOAISmartFoxAndroid::PlayerToSpectator(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	/*
    if ( this->PushListener ( ON_PLAYER_TO_SPECTATOR, state )) {
        
		lua_newtable ( state );
		
		SFSRoom *room = [evt.params objectForKey:@"room"];
		NSLog(@"The Room %@ was successfully joined!", room.name);
		
		// save the room reference for later logout
		MOAISmartFoxAndroid::Get ().mCurrentRoom = room;
		
		NSArray* players = [room userList];
		
		if ( [room isGame] )
		{
			NSLog(@"is game");
		}
		else{
			NSLog(@"is not game");
		}
		
		state.SetField ( -1, "name", [ room.name UTF8String ]);
		state.SetField ( -1, "groupId", [ room.groupId	UTF8String ]);
		state.SetField ( -1, "id", room.id);
		
		NSLog(@"about to count players %i", [room userCount]);
		
		lua_pushstring ( state, "userList" );
		lua_newtable ( state );
		
		// interate through the player list
		int count = 1;
		for ( SFSUser* player in players ) {
			
			lua_pushnumber ( state, count++ );
			lua_newtable ( state );
			
			//state.SetField ( -1, "id", [ player  ]);
            
            if ( [room isGame] ){
                state.SetField ( -1, "playerId", player.playerId);
            }
			
			state.SetField ( -1, "name", [ player.name UTF8String ]);
			state.SetField ( -1, "id",  player.id  );
			state.SetField ( -1, "isItMe", player.isItMe);
			state.SetField ( -1, "isSpectator", player.isSpectator);
			
            lua_pushstring ( state, "userVariables" );
            lua_newtable ( state );
            
            // get the variables for this player
            NSArray* userVariables = [player getVariables];
            
            int variableCount = 1;
            for ( SFSUserVariable* variable in userVariables ) {
                
                NSLog(@"about to get user variable named %@", variable.name);
                
                //lua_newtable ( state );
                //lua_pushstring ( state, [variable.name UTF8String ]);
                
                if (variable.type == @"String"){
                    
                    NSLog(@"got value for variable %@", [variable getStringValue]);
                    
                    //lua_pushstring(state, [[variable getStringValue] UTF8String]);
                    state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
                }
                
                if (variable.type == @"Int"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
                }
                
                if (variable.type == @"Bool"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
                }
                
                if (variable.type == @"Double"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
                }
                
                //lua_settable ( state, -3 );
                
            }
            
            // end user variables table
            lua_settable ( state, -3 );
            
            
			printf("set table");
			
            // end player table
			lua_settable ( state, -3 );
			
		}
		
		lua_settable ( state, -3 );
		
		NSArray* variables = [room getVariables];
		
        lua_pushstring ( state, "roomVariables" );
        lua_newtable ( state );
        
		int count2 = 1;
		for ( SFSRoomVariable* variable in variables ) {
			
            NSLog(@"about to get room variable named %@", variable.name);
            
            //lua_newtable ( state );
            //lua_pushstring ( state, [variable.name UTF8String ]);
            
            if (variable.type == @"String"){
                
                NSLog(@"got value for variable %@", [variable getStringValue]);
                
                //lua_pushstring(state, [[variable getStringValue] UTF8String]);
                state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
            }
            
            if (variable.type == @"Int"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
            }
            
            if (variable.type == @"Bool"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
            }
            
            if (variable.type == @"Double"){
                
                state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
            }
		}
		
        // end room variable table
        lua_settable ( state, -3 );
        
		NSLog(@"calling back with count %i", count);
		
		state.DebugCall ( 1, 0 );
	}
     */
}

void MOAISmartFoxAndroid::PlayerToSpectatorError(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( ON_PLAYER_TO_SPECTATOR_ERROR, state )) {
		
		//NSLog(@"Player to Spectator error:  %@", [evt.params objectForKey:@"errorMessage"]);
		state.DebugCall ( 0, 0 );
		
	}
}

void MOAISmartFoxAndroid::SubscribeRoomGroup(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
    /*
    if ( this->PushListener ( ON_SUBSCRIBE_ROOM_GROUP, state )) {
        
		NSArray* newRooms = [evt.params objectForKey:@"newRooms"];
        
		for ( SFSRoom* room in newRooms ) {
            
            NSLog(@"The Room %@ was successfully joined!", room.name);
            
            NSArray* users = [room userList];
            NSArray* players = [room playerList];
            
        }
	}
     */
}

void MOAISmartFoxAndroid::SubscribeRoomGroupError(SFSEvent *evt)
{
    
}

void MOAISmartFoxAndroid::UnsubscribeRoomGroup(SFSEvent *evt)
{
    
}

void MOAISmartFoxAndroid::UnsubscribeRoomGroupError(SFSEvent *evt)
{
    
}



void MOAISmartFoxAndroid::RoomFindResult(SFSEvent *evt)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
    /*
    if ( this->PushListener ( ON_ROOM_FIND_RESULT, state )) {
        
		NSArray* rooms = [evt.params objectForKey:@"rooms"];
        
        lua_newtable ( state );
        
		lua_pushstring ( state, "rooms" );
		lua_newtable ( state );
		
        int roomCount = 1;
		for ( SFSRoom* room in rooms ) {
            
			lua_pushnumber ( state, roomCount++ );
			lua_newtable ( state );
            
            NSLog(@"The Room %@ was successfully joined!", room.name);
            
            NSArray* players = [room userList];
            
            if ( [room isGame] )
            {
                NSLog(@"is game");
            }
            else{
                NSLog(@"is not game");
            }
            
            state.SetField ( -1, "name", [ room.name UTF8String ]);
            state.SetField ( -1, "groupId", [ room.groupId	UTF8String ]);
            state.SetField ( -1, "id", room.id);
            
            NSLog(@"about to count players %i", [room userCount]);
            
            lua_pushstring ( state, "userList" );
            lua_newtable ( state );
            
            // interate through the player list
            int count = 1;
            for ( SFSUser* player in players ) {
                
                lua_pushnumber ( state, count++ );
                lua_newtable ( state );
                
                //state.SetField ( -1, "id", [ player  ]);
                
                if ( [room isGame] ){
                    state.SetField ( -1, "playerId", player.playerId);
                }
                
                state.SetField ( -1, "name", [ player.name UTF8String ]);
                state.SetField ( -1, "id",  player.id  );
                state.SetField ( -1, "isItMe", player.isItMe);
                state.SetField ( -1, "isSpectator", player.isSpectator);
                
                lua_pushstring ( state, "userVariables" );
                lua_newtable ( state );
                
                // get the variables for this player
                NSArray* userVariables = [player getVariables];
                
                int variableCount = 1;
                for ( SFSUserVariable* variable in userVariables ) {
                    
                    NSLog(@"about to get user variable named %@", variable.name);
                    
                    //lua_newtable ( state );
                    //lua_pushstring ( state, [variable.name UTF8String ]);
                    
                    if (variable.type == @"String"){
                        
                        NSLog(@"got value for variable %@", [variable getStringValue]);
                        
                        //lua_pushstring(state, [[variable getStringValue] UTF8String]);
                        state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
                    }
                    
                    if (variable.type == @"Int"){
                        
                        state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
                    }
                    
                    if (variable.type == @"Bool"){
                        
                        state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
                    }
                    
                    if (variable.type == @"Double"){
                        
                        state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
                    }
                    
                    //lua_settable ( state, -3 );
                    
                }
                
                // end user variables table
                lua_settable ( state, -3 );
                
                
                printf("set table");
                
                // end player table
                lua_settable ( state, -3 );
                
            }
            
            // end player list table
            lua_settable ( state, -3 );
            
            NSArray* variables = [room getVariables];
            
            lua_pushstring ( state, "roomVariables" );
            lua_newtable ( state );
            
            int count2 = 1;
            for ( SFSRoomVariable* variable in variables ) {
                
                NSLog(@"about to get room variable named %@", variable.name);
                
                //lua_newtable ( state );
                //lua_pushstring ( state, [variable.name UTF8String ]);
                
                if (variable.type == @"String"){
                    
                    NSLog(@"got value for variable %@", [variable getStringValue]);
                    
                    //lua_pushstring(state, [[variable getStringValue] UTF8String]);
                    state.SetField ( -1, [variable.name UTF8String ], [[variable getStringValue] UTF8String]);
                }
                
                if (variable.type == @"Int"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getIntValue]);
                }
                
                if (variable.type == @"Bool"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getBoolValue]);
                }
                
                if (variable.type == @"Double"){
                    
                    state.SetField ( -1, [variable.name UTF8String ], [variable getDoubleValue]);
                }
            }
            
            // end room variables table
            lua_settable ( state, -3 );
            
            //end room table
            lua_settable ( state, -3 );
        }
        
        // end rooms table
		lua_settable ( state, -3 );
        
        NSLog(@"about to do debug call");
		state.DebugCall ( 1, 0 );
        
	}
     */
}

void MOAISmartFoxAndroid::SmartFoxEvent(const char* type,const char* arguments)
{
    MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

    	//USLog::Print ( "smartfox event c++" );
 		//USLog::Print ( type );
 		//USLog::Print ( arguments );

    if ( this->PushListener ( ON_SMARTFOX_EVENT, state )) {
     
        MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
        
        state.Push ( type );
        state.Push ( arguments );
        
        state.DebugCall ( 2, 0 );

    }
    
}
//================================================================//
// SmartFox JNI methods
//================================================================//

//----------------------------------------------------------------//

/*
extern "C" void Java_com_ziplinegames_moai_MoaiSmartFox_AKUOnObjectMessage ( JNIEnv* env, jclass jobj, jstring cmd, jstring message) {

    JNI_GET_CSTRING ( jcmd, cmd );
    JNI_GET_CSTRING ( jmessage, message );
    
    MOAISmartFoxAndroid::Get ().ObjectMessage ( cmd, message );
    
    JNI_RELEASE_CSTRING ( jcmd, cmd );
    JNI_RELEASE_CSTRING ( jmessage, message );
}

extern "C" void Java_com_ziplinegames_moai_MoaiSmartFox_AKUOnConnection( JNIEnv* env, jclass jobj, jboolean success) {
    
    MOAISmartFoxAndroid::Get ().Connection(bool success);
    
}

extern "C" void Java_com_ziplinegames_moai_MoaiSmartFox_AKUOnLogin ( JNIEnv* env, jclass jobj) {
    
    MOAISmartFoxAndroid::Get ().Login();
    
}

extern "C" void Java_com_ziplinegames_moai_MoaiSmartFox_AKUOnLoginError ( JNIEnv* env, jclass jobj) {
    
    MOAISmartFoxAndroid::Get ().Login();
    
}

*/


extern "C" void Java_com_ziplinegames_moai_MoaiSmartFox_AKUOnSmartFoxEvent ( JNIEnv* env, jclass jobj, jstring jtype, jstring jarguments ) {


    // tons of translation stuff here
    //SFSEvent* evt = [[SFSEvent alloc] init];
    //evt.type = type;
    JNI_GET_CSTRING ( jtype, type );
    JNI_GET_CSTRING ( jarguments, arguments );

    
    USLog::Print ( "MOAISmartFoxAndroid: AKUOnSmartFoxEvent %s", type );
    
    
    //SFSEvent* event = new SFSEvent(type);
    //std::string stype(type);
    MOAISmartFoxAndroid::Get ().SmartFoxEvent ( type, arguments );

    /*
    if (stype == *(SFSEvent::CONNECTION)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSEvent::CONNECTION_LOST)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSEvent::CONNECTION_RETRY)) {
        MOAISmartFoxAndroid::Get ().ConnectionRetry ( event );
    }
    else if (stype == *(SFSEvent::CONNECTION_RESUME)) {
        MOAISmartFoxAndroid::Get ().ConnectionResume ( event );
    }
    else if (stype == *(SFSEvent::LOGIN)) {
        MOAISmartFoxAndroid::Get ().Login ( event );
    }
    else if (stype == *(SFSEvent::LOGIN_ERROR)) {
        MOAISmartFoxAndroid::Get ().LoginError ( event );
    }
    else if (stype == *(SFSEvent::LOGOUT)) {
        MOAISmartFoxAndroid::Get ().Logout ( event );
    }
    else if (stype == *(SFSEvent::ROOM_JOIN)) {
        MOAISmartFoxAndroid::Get ().RoomJoin ( event );
    }
    else if (stype == *(SFSEvent::ROOM_JOIN_ERROR)) {
        MOAISmartFoxAndroid::Get ().RoomJoinError ( event );
    }
    else if (stype == *(SFSEvent::USER_ENTER_ROOM)) {
        MOAISmartFoxAndroid::Get ().UserEnterRoom ( event );
    }
    else if (stype == *(SFSEvent::USER_EXIT_ROOM )) {
        MOAISmartFoxAndroid::Get ().UserExitRoom ( event );
    }
    else if (stype == *(SFSEvent::USER_COUNT_CHANGE)) {
        MOAISmartFoxAndroid::Get ().UserCountChange ( event );
    }
    else if (stype == *(SFSEvent::PUBLIC_MESSAGE)) {
        MOAISmartFoxAndroid::Get ().PublicMessage ( event );
    }
    else if (stype == *(SFSEvent::OBJECT_MESSAGE)) {
        MOAISmartFoxAndroid::Get ().ObjectMessage ( event );
    }
    else if (stype == *(SFSEvent::EXTENSION_RESPONSE)) {
        MOAISmartFoxAndroid::Get ().ExtensionResponse ( event );
    }
    else if (stype == *(SFSEvent::ROOM_VARIABLES_UPDATE)) {
        MOAISmartFoxAndroid::Get ().RoomVariablesUpdate ( event );
    }
    else if (stype == *(SFSEvent::USER_VARIABLES_UPDATE)) {
       // MOAISmartFoxAndroid::Get ().UserVariable ( event );
    }
    else if (stype == *(SFSEvent::ROOM_GROUP_SUBSCRIBE)) {
        //MOAISmartFoxAndroid::Get ().RoomGroupSubscibe ( event );
    }
    else if (stype == *(SFSEvent::ROOM_GROUP_UNSUBSCRIBE)) {
        //MOAISmartFoxAndroid::Get ().RoomGroupUnsubscribe ( event );
    }
    else if (stype == *(SFSEvent::ROOM_GROUP_SUBSCRIBE_ERROR)) {
        //MOAISmartFoxAndroid::Get ().Room ( event );
    }
    else if (stype == *(SFSEvent::ROOM_GROUP_UNSUBSCRIBE_ERROR)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSEvent::SPECTATOR_TO_PLAYER)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSEvent::PLAYER_TO_SPECTATOR)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSEvent::SPECTATOR_TO_PLAYER_ERROR)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSEvent::PLAYER_TO_SPECTATOR_ERROR)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSEvent::ROOM_FIND_RESULT)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSEvent::DEBUG_MESSAGE)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }    
    else {
        
        // this is an error
        
    }
        
    //  else if (stype == SFSEvent::CONNECTION_LOST) {
  //      MOAISmartFoxAndroid::Get ().Connection ( event );
  //  }
    */
    //JNI_RELEASE_CSTRING ( jtype, type );
    //JNI_RELEASE_CSTRING ( jarguments, arguments );
    
	//MOAIChartBoostAndroid::Get ().NotifyInterstitialDismissed ();
}

/*
extern "C" void Java_com_ziplinegames_moai_MoaiSmartFox_AKUOnConnection ( JNIEnv* env, jclass jobj, bool success) {

    SFSEvent* event = new SFSEvent(type);
    
    MOAISmartFoxAndroid::Get ().Connection ( event );
    
}
 */

extern "C" void Java_com_ziplinegames_moai_MoaiSmartFox_AKUOnSmartFoxBuddyEvent ( JNIEnv* env, jclass jobj, jstring jtype, jstring jarguments ) {

    // tons of translation stuff here
    //SFSEvent* evt = [[SFSEvent alloc] init];
    //evt.type = type;
    JNI_GET_CSTRING ( jtype, type );
    JNI_GET_CSTRING ( jarguments, arguments );
    
    //SFSEvent* event = new SFSEvent(type);
    //std::string stype(type);
    MOAISmartFoxAndroid::Get ().SmartFoxEvent ( type, arguments );
    
    // take the json from the arguments and make it into an object that
    // we can pass around the system.  SFSObject would be best but may be a pain
    // to get compiled
    /*
    if (stype == *(SFSBuddyEvent::BUDDY_LIST_INIT)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSBuddyEvent::BUDDY_ADD)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSBuddyEvent::BUDDY_REMOVE)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSBuddyEvent::BUDDY_BLOCK)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSBuddyEvent::BUDDY_ERROR)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSBuddyEvent::BUDDY_ONLINE_STATE_UPDATE)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSBuddyEvent::BUDDY_VARIABLES_UPDATE)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    else if (stype == *(SFSBuddyEvent::BUDDY_MESSAGE)) {
        MOAISmartFoxAndroid::Get ().Connection ( event );
    }
    
    else {
        
        // this is an error
        
    }
    */
    //  else if (stype == SFSEvent::CONNECTION_LOST) {
    //      MOAISmartFoxAndroid::Get ().Connection ( event );
    //  }
    
    //JNI_RELEASE_CSTRING ( jtype, type );
    //JNI_RELEASE_CSTRING ( jarguments, arguments );
    
}

#endif