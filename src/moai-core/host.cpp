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
	MOAILuaStrongRef	mLuaFunc;
	
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

typedef STLMap < AKUContextID, AKUContext* >::iterator ContextMapIt;
typedef STLMap < AKUContextID, AKUContext* > ContextMap;

static ContextMap*		sContextMap = 0;
static AKUContextID		sContextIDCounter = 0;
static AKUContextID		sContextID = 0;
static AKUContext*		sContext = 0;

//================================================================//
// local
//================================================================//

void	_debugCallWithArgs	( MOAILuaState& state, int totalArgs, int asParams );
int		_loadContextFunc	( MOAILuaState& state );
void	_pushArgOrParam		( MOAILuaState& state, int index, char* arg, int asParam );
void	_setupArgs			( MOAILuaState& state, char* exeName, char* scriptName, int asParams );

//----------------------------------------------------------------//
void _debugCallWithArgs ( MOAILuaState& state, int totalArgs, int asParams ) {

	int status;
	
	if ( asParams == AKU_AS_ARGS ) {
		state.Pop ( 1 );
		status = state.DebugCall ( 0, 0 );
	}
	else {
		status = state.DebugCall ( totalArgs, 0 );
	}
	
	state.PrintErrors ( ZLLog::CONSOLE, status );
}

//----------------------------------------------------------------//
int _loadContextFunc ( MOAILuaState& state ) {

	if ( sContext && sContext->mLuaFunc ) {
		state.Push ( sContext->mLuaFunc );
		sContext->mLuaFunc.Clear ();
		return 0;
	}
	
	ZLLog::LogF ( ZLLog::CONSOLE, "Missing function to call; use AKULoadFunc* to load a function\n" );
	return 1;
}

//----------------------------------------------------------------//
void _pushArgOrParam ( MOAILuaState& state, int index, char* arg, int asParam ) {

	if ( asParam == AKU_AS_PARAMS ) {
		if ( arg ) {
			state.Push ( arg );
		}
		else {
			state.Push ();
		}
	}
	else {
		state.SetFieldByIndex ( -1, index, arg );
	}
}

//----------------------------------------------------------------//
void _setupArgs ( MOAILuaState& state, char* exeName, char* scriptName, int asParams ) {

	lua_newtable ( state );
	state.SetFieldByIndex ( -1, -1, exeName );
	state.SetFieldByIndex ( -1, 0, scriptName );
	lua_setglobal ( state, "arg" );

	if ( asParams == AKU_AS_ARGS ) {
		lua_getglobal ( state, "arg" );
	}
}

//================================================================//
// aku
//================================================================//

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
void AKUCallFunc () {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( _loadContextFunc ( state ) != 0 ) return;
	int status = state.DebugCall ( 0, 0 );
	state.PrintErrors ( ZLLog::CONSOLE, status );
}

//----------------------------------------------------------------//
// arg[-1] => argMinusOne - host binary (lua, luajit, moai-untz, ...)
// arg[0]  => argZero - first arg counting from offset (script name as passed to host binary)
// arg[1]  => next arg/option/script
// arg[2]  => next arg/option/script
	// ...
void AKUCallFuncWithArgArray ( char* exeName, char* scriptName, int argc, char** argv, int asParams ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( _loadContextFunc ( state ) != 0 ) return;

	_setupArgs ( state, exeName, scriptName, asParams );
	
	for ( int i = 0; i < argc; ++i ) {
		_pushArgOrParam ( state, i + 1, argv [ i ], asParams );
	}
	_debugCallWithArgs ( state, argc, asParams );
}

//----------------------------------------------------------------//
void AKUCallFuncWithArgString ( char* exeName, char* scriptName, char* args, int asParams ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( _loadContextFunc ( state ) != 0 ) return;

	_setupArgs ( state, exeName, scriptName, asParams );
	
	int numParams = 0; //The number of args
	if ( args ) {
		char* token = strtok ( args, " " );
		while ( token != NULL ) {
			++numParams;
			_pushArgOrParam ( state, numParams, token, asParams );
			token = strtok ( NULL, " " );
		}
	}
	_debugCallWithArgs ( state, numParams, asParams );
}

//----------------------------------------------------------------//
int AKUCheckContext ( AKUContextID contextID ) {
	
	return sContextMap->contains ( contextID ) ? 0 : -1;
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
int AKUCountContexts () {

	return sContextMap ? ( int )sContextMap->size () : 0;
}

//----------------------------------------------------------------//
AKUContextID AKUCreateContext () {

	sContext = new AKUContext;
	
	sContextID = ++sContextIDCounter;
	( *sContextMap )[ sContextID ] = sContext;

	MOAIGlobalsMgr::Set ( sContext->mGlobals  );

	MOAILuaRuntime::Affirm ();
	MOAITrace::Affirm ();
	MOAILogMgr::Affirm ();
	
	MOAILuaRuntime& luaRuntime = MOAILuaRuntime::Get ();
	luaRuntime.Open ();
	luaRuntime.LoadLibs ();
	
	MOAIEnvironment::Affirm ();
	
	MOAILogMessages::RegisterDefaultLogMessages ();
	
	REGISTER_LUA_CLASS ( MOAILuaRuntime )
	REGISTER_LUA_CLASS ( MOAIEnvironment )
	REGISTER_LUA_CLASS ( MOAIDeserializer )
	REGISTER_LUA_CLASS ( MOAILogMgr )
	REGISTER_LUA_CLASS ( MOAISerializer )
	REGISTER_LUA_CLASS ( MOAITrace )
	REGISTER_LUA_CLASS ( MOAITestMgr )

	MOAIEnvironment::Get ().DetectEnvironment ();

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
void AKULoadFuncFromBuffer ( void* data, size_t size, int dataType, int compressed ) {

	sContext->mLuaFunc.Clear ();

	if ( !size ) return;

	MOAIDataBuffer buffer;
	buffer.Load ( data, size );
	
	if ( compressed == AKU_DATA_ZIPPED ) {
		buffer.Inflate ( ZLDeflateWriter::DEFAULT_WBITS );
	}

	buffer.Lock ( &data, &size );
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	if ( dataType == AKU_DATA_BYTECODE ) {
		lua_getglobal ( state, "loadstring" );
		state.Push ( data, size );
		int status = state.DebugCall ( 1, 1 );
		if ( !state.PrintErrors ( ZLLog::CONSOLE, status )) {
			sContext->mLuaFunc.SetRef ( state, -1 );
		}
	}
	
	if ( dataType == AKU_DATA_STRING ) {
		AKULoadFuncFromString (( cc8* )data );
	}
	
	buffer.Unlock ();
}

//----------------------------------------------------------------//
void AKULoadFuncFromFile ( const char* filename ) {

	sContext->mLuaFunc.Clear ();

	if ( !ZLFileSys::CheckFileExists ( filename )) {
		ZLLog::LogF ( ZLLog::CONSOLE, "Could not find file %s \n", filename );
		return;
	}

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	//int top = state.GetTop ();
	
	lua_getglobal ( state, "loadfile" );
	if ( !state.IsType ( -1, LUA_TFUNCTION )) {
		ZLLog::LogF ( ZLLog::CONSOLE, "Missing global Lua function 'loadfile'\n" );
	}
	
	state.Push ( filename );
	
	int status = state.DebugCall ( 1, 2 );
	if ( !state.PrintErrors ( ZLLog::CONSOLE, status )) {
		if ( state.IsNil ( -2 )) {
			cc8* msg = state.GetValue < cc8* >( -1, "loafile returned 'nil'" );
			printf ( "%s\n", msg );
		}
		else {
			sContext->mLuaFunc.SetRef ( state, -2 );
		}
	}
}

//----------------------------------------------------------------//
void AKULoadFuncFromString ( const char* script ) {

	sContext->mLuaFunc.Clear ();

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	lua_getglobal ( state, "loadstring" );
	if ( !state.IsType ( -1, LUA_TFUNCTION )) {
		ZLLog::LogF ( ZLLog::CONSOLE, "Missing global Lua function 'loadstring'\n" );
	}
	
	state.Push ( script );
	
	int status = state.DebugCall ( 1, 1 );
	if ( !state.PrintErrors ( ZLLog::CONSOLE, status )) {
		sContext->mLuaFunc.SetRef ( state, -1 );
	}
}

//----------------------------------------------------------------//
int AKUMountVirtualDirectory ( char const* virtualPath, char const* archive ) {

	int result = zl_mount_virtual ( virtualPath, archive );
	if ( result ) {
		ZLLog::LogF ( ZLLog::CONSOLE, "Error mounting %s at path %s\n", archive, virtualPath );
	}
	return result;
}

//----------------------------------------------------------------//
int AKUSetContext ( AKUContextID contextID ) {
	
	if ( sContextID != contextID ) {
		
		sContextID = contextID;
		sContext = sContextMap->value_for_key ( contextID );
		
		if ( sContext ) {
			MOAIGlobalsMgr::Set ( sContext->mGlobals );
			return 0;
		}
		MOAIGlobalsMgr::Set ( 0 );
		return -1;
	}
	return 0;
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
