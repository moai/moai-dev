// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/host.h>
#include <moai-core/headers.h>
#include <moai-util/MOAIDataBuffer.h>

//================================================================//
// AKUContext
//================================================================//
class AKUContext {
public:

	MOAIGlobals*		mGlobals;
	void*				mUserdata;
	
	//----------------------------------------------------------------//
	AKUContext () :
		mUserdata ( 0 ) {
		
		this->mGlobals = MOAIGlobalsMgr::Create ();
	}
	
	//----------------------------------------------------------------//
	~AKUContext () {
	
		if ( this->mGlobals ) {
			MOAIGlobalsMgr::Delete ( this->mGlobals );
			this->mGlobals = 0;
		}
	}
};


//================================================================//
// aku
//================================================================//

typedef STLMap < AKUContextID, AKUContext* >::iterator ContextMapIt;
typedef STLMap < AKUContextID, AKUContext* > ContextMap;

static ContextMap*		sContextMap = 0;
static AKUContextID		sContextIDCounter = 0;
static AKUContextID		sContextID = 0;
static AKUContext*		sContext = 0;

//----------------------------------------------------------------//
void AKUAppFinalize () {

	if ( sContextMap ) {

		ContextMapIt contextMapIt = sContextMap->begin ();
		for ( ; contextMapIt != sContextMap->end (); ++contextMapIt ) {
			AKUContext* context = contextMapIt->second;
			delete context;
		}
		delete sContextMap;
		sContextMap = 0;
	}
	
	zl_cleanup ();
}

//----------------------------------------------------------------//
void AKUAppInitialize () {
		
	// TODO: this should be part of the unit tests
	// make sure our fixed size typedefs are what we think
	// they are on the current platform/compiler
	assert ( sizeof ( cc8 )	== 1 );

	assert ( sizeof ( u8 )	== 1 );
	assert ( sizeof ( u16 )	== 2 );
	assert ( sizeof ( u32 )	== 4 );
	assert ( sizeof ( u64 )	== 8 );
	
	assert ( sizeof ( s8 )	== 1 );
	assert ( sizeof ( s16 )	== 2 );
	assert ( sizeof ( s32 )	== 4 );
	assert ( sizeof ( s64 )	== 8 );
	
	srand (( u32 )time ( 0 ));
	zl_init ();
	
	sContextMap = new ContextMap;
}

//----------------------------------------------------------------//
void AKUClearMemPool () {

	ZL_TLSF_POOL* pool = zl_tlsf_get_pool ();
	zl_tlsf_set_pool ( 0 );
	
	if ( pool ) {
		zl_tlsf_destroy_pool ( pool );
	}
}

//----------------------------------------------------------------//
AKUContextID AKUCreateContext () {

	sContext = new AKUContext;
	
	sContextID = ++sContextIDCounter;
	( *sContextMap )[ sContextID ] = sContext;

	MOAIGlobalsMgr::Set ( sContext->mGlobals  );

	MOAILuaRuntime::Affirm ();
	MOAILogMgr::Affirm ();
	
	MOAILuaRuntime& luaRuntime = MOAILuaRuntime::Get ();
	luaRuntime.Open ();
	luaRuntime.LoadLibs ();
	
	MOAILogMessages::RegisterDefaultLogMessages ();
	
	REGISTER_LUA_CLASS ( MOAILuaRuntime )
	REGISTER_LUA_CLASS ( MOAIDeserializer )
	REGISTER_LUA_CLASS ( MOAILogMgr )
	REGISTER_LUA_CLASS ( MOAISerializer )

	return sContextIDCounter;
}

//----------------------------------------------------------------//
void AKUDeleteContext ( AKUContextID contextID ) {
	
	AKUSetContext ( contextID );
	if ( !sContext ) return;
	
	sContextMap->erase ( contextID );
	delete sContext;
	
	AKUSetContext ( 0 );
}

//----------------------------------------------------------------//
AKUContextID AKUGetContext () {
	
	return sContextID;
}

//----------------------------------------------------------------//
void* AKUGetUserdata () {
	
	if ( sContext ) {
		return sContext->mUserdata;
	}
	return 0;
}

//----------------------------------------------------------------//
lua_State* AKUGetLuaState () {

	lua_State* lua_state = NULL;
	lua_state = MOAILuaRuntime::Get ().State ();

	return lua_state;
}

//----------------------------------------------------------------//
char* AKUGetMoaiVersion ( char* buffer, size_t length ) {

	STLString version = MOAIVersion::GetVersionString ();
	if ( version.length () < length ) {
		strcpy ( buffer, version.c_str ());
	}
	else {
		strncpy ( buffer, version.c_str (), length - 1 );
		buffer [ length - 1 ] = 0;
	}
	return buffer;
}

//----------------------------------------------------------------//
char* AKUGetWorkingDirectory ( char* buffer, size_t length ) {

	return zl_getcwd ( buffer, length );
}

//----------------------------------------------------------------//
void AKUInitMemPool ( size_t bytes ) {

	assert ( !zl_tlsf_get_pool ());

	ZL_TLSF_POOL* pool = zl_tlsf_create_pool ( bytes );
	zl_tlsf_set_pool ( pool );
}

//----------------------------------------------------------------//
int AKUMountVirtualDirectory ( char const* virtualPath, char const* archive ) {

	return zl_mount_virtual ( virtualPath, archive );
}

//----------------------------------------------------------------//
void AKURunData ( void* data, size_t size, int dataType, int compressed ) {

	if ( !size ) return;

	MOAIDataBuffer buffer;
	buffer.Load ( data, size );
	
	if ( compressed == AKU_DATA_ZIPPED ) {
		buffer.Inflate ( ZLDeflateReader::DEFAULT_WBITS );
	}

	buffer.Lock ( &data, &size );
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	if ( dataType == AKU_DATA_BYTECODE ) {
		state.Run ( data, size, 0, 0 );
	}
	
	if ( dataType == AKU_DATA_STRING ) {
		int status = luaL_loadstring ( state, ( cc8* )data );
		if ( !state.PrintErrors ( ZLLog::CONSOLE, status )) {
			state.DebugCall ( 0, 0 );
		}
	}
	
	buffer.Unlock ();
}

//----------------------------------------------------------------//
void AKURunScript ( const char* filename ) {

	if ( !ZLFileSys::CheckFileExists ( filename ))
	{
		ZLLog::PrintFile ( ZLLog::CONSOLE, "Script does not exist: '%s'\n", filename );
		return;
	}

	int status;
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	status = luaL_loadfile ( state, filename );

	if ( state.PrintErrors ( ZLLog::CONSOLE, status )) return;
	
	state.DebugCall ( 0, 0 );
}

//----------------------------------------------------------------//
void AKURunString ( const char* script ) {

	int status;
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	status = luaL_loadstring ( state, script );
	if ( state.PrintErrors ( ZLLog::CONSOLE, status )) return;
	
	state.DebugCall ( 0, 0 );
}

//----------------------------------------------------------------//
void AKUSetContext ( AKUContextID contextID ) {
	
	if ( sContextID != contextID ) {
		
		sContextID = contextID;
		sContext = sContextMap->value_for_key ( contextID );
		
		if ( sContext ) {
			MOAIGlobalsMgr::Set ( sContext->mGlobals );
		}
		else {
			MOAIGlobalsMgr::Set ( 0 );
		}
	}
}

//----------------------------------------------------------------//
void AKUSetFunc_ErrorTraceback ( AKUErrorTracebackFunc func ) {

	MOAILuaRuntime::Get ().SetTracebackFunc ( func );
}

//----------------------------------------------------------------//
void AKUSetUserdata ( void* userdata ) {

	if ( sContext ) {
		sContext->mUserdata = userdata;
	}
}

//----------------------------------------------------------------//
int AKUSetWorkingDirectory ( char const* path ) {

	return zl_chdir ( path );
}

//----------------------------------------------------------------//
void AKUSetArgv ( char **argv ) {

	int i;
	int argc = 0;

	lua_State* L = AKUGetLuaState ();

	// count argv
	while ( argv[argc] ) argc++;

	lua_createtable ( L, argc, 0 );
	int newTable = lua_gettop ( L );

	// arg[-1] => host binary (lua, luajit, moai-untz, ...)
	// arg[0]  => first arg (script name as passed to host binary)
	// arg[1]  => next arg/option/script
	// arg[2]  => next arg/option/script
	// ...
	for ( i=0; i < argc; i++ ) {
		lua_pushstring ( L, argv[i] );
		lua_rawseti ( L, newTable, i - 1 );
	}

	// same as lua global 'arg'
	lua_setglobal ( L, "arg" );
}
