// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/host.h>
#include <moai-core/headers.h>
#include <moai-util/MOAIDataBuffer.h>
#include <moai-util/MOAILuaUtil.h>

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

int			_debugCallWithArgs		( MOAILuaState& state, int totalArgs, int asParams );
int			_loadContextFunc		( MOAILuaState& state );
int			_loadFuncFromBuffer		( MOAIDataBuffer& buffer, int compressed );
void		_pushArgOrParam			( MOAILuaState& state, int index, char* arg, int asParam );
void		_setupArgs				( MOAILuaState& state, char* exeName, char* scriptName, int asParams );

//----------------------------------------------------------------//
int _debugCallWithArgs ( MOAILuaState& state, int totalArgs, int asParams ) {

	int status;
	
	if ( asParams == AKU_AS_ARGS ) {
		state.Pop ( 1 );
		status = state.DebugCall ( 0, 0 );
	}
	else {
		status = state.DebugCall ( totalArgs, 0 );
	}
	
	return state.LogErrors ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, status ) ? AKU_ERROR : AKU_OK;
}

//----------------------------------------------------------------//
int _loadContextFunc ( MOAILuaState& state ) {

	if ( sContext && sContext->mLuaFunc ) {
		state.Push ( sContext->mLuaFunc );
		sContext->mLuaFunc.Clear ();
		return AKU_OK;
	}
	
	ZLLog_ErrorF ( ZLLog::CONSOLE, "Missing function to call; use AKULoadFunc* to load a function\n" );
	return AKU_ERROR;
}

//----------------------------------------------------------------//
int _loadFuncFromBuffer ( MOAIDataBuffer& buffer, int compressed ) {

	if ( compressed == AKU_DATA_ZIPPED ) {
		buffer.Inflate ( ZLDeflateWriter::DEFAULT_WBITS );
	}

	if ( !buffer.Size ()) return AKU_ERROR;

	void* data;
	size_t size;
	buffer.Lock ( &data, &size );
	
	bool isBytecode = false;
	
	if ( sizeof ( MOAILuaHeader ) <= size ) {
	
		MOAILuaHeader header;
		memcpy ( &header, data, sizeof ( MOAILuaHeader ));
		
		if ( header.IsBytecode ()) {
	
			isBytecode = true;
	
			MOAILuaHeader sysHeader;
			sysHeader.Init ();
			
			if ( !sysHeader.IsCompatible ( header )) {
				ZLLog_ErrorF ( ZLLog::CONSOLE, "ERROR: Attempt to load incompatible Lua bytecode.\n" );
				buffer.Unlock ();
				return AKU_ERROR;
			}
		}
	}
	
	int result = AKU_OK;

	if ( isBytecode ) {
	
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();;
	
		lua_getglobal ( state, "loadstring" );
		state.Push ( data, size );
		int status = state.DebugCall ( 1, 1 );
		if ( !state.LogErrors ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, status )) {
			sContext->mLuaFunc.SetRef ( state, -1 );
			result = AKU_ERROR;
		}
	}
	else {
		result = AKULoadFuncFromString (( cc8* )data, size );
	}
	
	buffer.Unlock ();
	return result;
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
int AKUCallFunc () {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( _loadContextFunc ( state ) != 0 ) return AKU_ERROR;
	int status = state.DebugCall ( 0, 0 );
	return state.LogErrors ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, status ) ? AKU_ERROR : AKU_OK;
}

//----------------------------------------------------------------//
// arg[-1] => argMinusOne - host binary (lua, luajit, moai-untz, ...)
// arg[0]  => argZero - first arg counting from offset (script name as passed to host binary)
// arg[1]  => next arg/option/script
// arg[2]  => next arg/option/script
	// ...
int AKUCallFuncWithArgArray ( char* exeName, char* scriptName, int argc, char** argv, int asParams ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( _loadContextFunc ( state ) != 0 ) return AKU_ERROR;

	_setupArgs ( state, exeName, scriptName, asParams );
	
	for ( int i = 0; i < argc; ++i ) {
		_pushArgOrParam ( state, i + 1, argv [ i ], asParams );
	}
	return _debugCallWithArgs ( state, argc, asParams );
}

//----------------------------------------------------------------//
int AKUCallFuncWithArgString ( char* exeName, char* scriptName, char* args, int asParams ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( _loadContextFunc ( state ) != 0 ) return AKU_ERROR;

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
	return _debugCallWithArgs ( state, numParams, asParams );
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
	MOAITestMgr::Affirm ();
	
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
int AKULoadFuncFromBuffer ( void* data, size_t size, int compressed ) {

	sContext->mLuaFunc.Clear ();

	MOAIDataBuffer buffer;
	buffer.Load ( data, size );
	
	return _loadFuncFromBuffer ( buffer, compressed );
}

//----------------------------------------------------------------//
int AKULoadFuncFromFile ( const char* filename ) {

	sContext->mLuaFunc.Clear ();

	if ( !ZLFileSys::CheckFileExists ( filename )) {
		ZLLog_ErrorF ( ZLLog::CONSOLE, "Could not find file %s \n", filename );
		return AKU_ERROR;
	}

	MOAIDataBuffer buffer;
	if ( !buffer.Load ( filename )) return AKU_ERROR;
	if ( !buffer.Size ()) return AKU_ERROR;
	
	return _loadFuncFromBuffer ( buffer, AKU_DATA_UNCOMPRESSED );
}

//----------------------------------------------------------------//
int AKULoadFuncFromString ( const char* script, size_t size ) {

	sContext->mLuaFunc.Clear ();

	// turns out Lua is really picky about trailing nulls
	while (( size > 1 ) && ( script [ size - 1 ] == 0 )) --size;

	if ( !size ) return AKU_OK;

	sContext->mLuaFunc.Clear ();

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	lua_getglobal ( state, "loadstring" );
	if ( !state.IsType ( -1, LUA_TFUNCTION )) {
		ZLLog_ErrorF ( ZLLog::CONSOLE, "Missing global Lua function 'loadstring'\n" );
	}
	
	state.Push (( void* )script, size );
	
	int status = state.DebugCall ( 1, 2 );
	if ( state.LogErrors ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, status )) return AKU_ERROR;
	
	if ( state.IsType ( -1, LUA_TSTRING )) {
	
		ZLLog_ErrorF ( ZLLog::CONSOLE, "Error loading script:\n" );
		ZLLog_ErrorF ( ZLLog::CONSOLE, "%s\n", state.GetValue < cc8* >( -1, "" ));
		return AKU_ERROR;
	}
	
	sContext->mLuaFunc.SetRef ( state, -2 );
	assert ( !sContext->mLuaFunc.IsNil ());
	return AKU_OK;
}

//----------------------------------------------------------------//
int AKUMountVirtualDirectory ( char const* virtualPath, char const* archive ) {

	int result = zl_mount_virtual ( virtualPath, archive );
	if ( result ) {
		ZLLog_ErrorF ( ZLLog::CONSOLE, "Error mounting %s at path %s\n", archive, virtualPath );
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
void AKUSetLogLevel ( int logLevel ) {

	ZLLog::SetLogLevel ( logLevel );
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
