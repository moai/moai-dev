// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/host.h>
#include <moai-core/headers.h>
#include <moai-util/MOAIDataBuffer.h>
#include <moai-util/MOAILuaUtil.h>

//================================================================//
// AKUContext
//================================================================//
class AKUContext :
	public virtual ZLContextClass {
public:

	void*				mUserdata;
	MOAILuaStrongRef	mLuaFunc;
};

//================================================================//
// local
//================================================================//

int			_debugCallWithArgs		( MOAILuaState& state, int totalArgs, int asParams );
int			_loadContextFunc		( ZLContext& context, MOAILuaState& state );
int			_loadFuncFromBuffer		( ZLContext& context, MOAIDataBuffer& buffer, cc8* chunkname, int compressed );
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
int _loadContextFunc ( ZLContext& context, MOAILuaState& state ) {

	AKUContext& akuContext = context.Get < AKUContext >();

	if ( akuContext.mLuaFunc ) {
		state.Push ( akuContext.mLuaFunc );
		akuContext.mLuaFunc.Clear ();
		return AKU_OK;
	}
	
	context.Get < ZLLog >().LogF ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, "Missing function to call; use AKULoadFunc* to load a function\n" );
	return AKU_ERROR;
}

//----------------------------------------------------------------//
int _loadFuncFromBuffer ( ZLContext& context, MOAIDataBuffer& buffer, cc8* chunkname, int compressed ) {

	if ( compressed == AKU_DATA_ZIPPED ) {
		buffer.Inflate ( ZLDeflateWriter::DEFAULT_WBITS );
	}

	if ( !buffer.Size ()) return AKU_ERROR;

	ZLLog& log = context.Get < ZLLog >();

	u8* data;
	size_t size;
	
	MOAIDataBufferScopedLock lock ( buffer, ( void** )&data, &size  );
	
	#ifndef MOAI_WITH_LUAJIT
	MOAILuaHeader header;
	header.Read ( data, size );
		
	if ( header.IsBytecode ()) {
		
		MOAILuaHeader sysHeader;
		sysHeader.Init ();
		
		if ( !sysHeader.IsCompatible ( header )) {
			log.LogF ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, "ERROR: Attempt to load incompatible Lua bytecode.\n" );
			return AKU_ERROR;
		}
	}
	else {
		// trim trailing nulls from non-bytecode
		while (( size > 1 ) && ( data [ size - 1 ] == 0 )) --size;
	}
	#else
		// trim trailing nulls from non-bytecode
		while (( size > 1 ) && ( data [ size - 1 ] == 0 )) --size;
	#endif
	
	MOAIScopedLuaState state = context.Get < MOAILuaRuntime >().State ();

	lua_getglobal ( state, "loadstring" );
	if ( !state.IsType ( -1, LUA_TFUNCTION )) {
		log.LogF ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, "Missing global Lua function 'loadstring'\n" );
	}
	
	state.Push ( data, size );
	
	if ( chunkname ) {
		state.Push ( chunkname );
	}

	int status = state.DebugCall ( state.GetLocalTop () - 1, 2 );
	if ( state.LogErrors ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, status )) return AKU_ERROR;
	
	if ( state.IsType ( -1, LUA_TSTRING )) {
	
		log.LogF ( ZLLog::LOG_ERROR,  ZLLog::CONSOLE, "Error loading script:\n" );
		log.LogF ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, "%s\n", state.GetValue < cc8* >( -1, "" ));
		return AKU_ERROR;
	}
	
	AKUContext& akuContext = context.Get < AKUContext >();
	akuContext.mLuaFunc.SetRef ( state, -2 );
	assert ( !akuContext.mLuaFunc.IsNil ());
	return AKU_OK;
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
		state.SetField ( -1, index, arg );
	}
}

//----------------------------------------------------------------//
void _setupArgs ( MOAILuaState& state, char* exeName, char* scriptName, int asParams ) {

	lua_newtable ( state );
	state.SetField ( -1, -1, exeName );
	state.SetField ( -1, 0, scriptName );
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

//	if ( sContextMap ) {
//
//		ContextMapIt contextMapIt = sContextMap->begin ();
//		for ( ; contextMapIt != sContextMap->end (); ++contextMapIt ) {
//			AKUContext* context = contextMapIt->second;
//			delete context;
//		}
//		delete sContextMap;
//		sContextMap = 0;
//	}
	
//	ZLContextMgr::Finalize ();
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
	
	//sContextMap = new ContextMap;
}

//----------------------------------------------------------------//
int AKUCallFunc ( AKUContextID contextID ) {

	assert ( contextID );
	ZLContext& context = *( ZLContext* )contextID;

	MOAIScopedLuaState state = context.Get < MOAILuaRuntime >().State ();
	if ( _loadContextFunc ( context, state ) != 0 ) return AKU_ERROR;
	int status = state.DebugCall ( 0, 0 );
	return state.LogErrors ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, status ) ? AKU_ERROR : AKU_OK;
}

//----------------------------------------------------------------//
// arg[-1] => argMinusOne - host binary (lua, luajit, moai-untz, ...)
// arg[0]  => argZero - first arg counting from offset (script name as passed to host binary)
// arg[1]  => next arg/option/script
// arg[2]  => next arg/option/script
// ...
int AKUCallFuncWithArgArray ( AKUContextID contextID, char* exeName, char* scriptName, int argc, char** argv, int asParams ) {

	assert ( contextID );
	ZLContext& context = *( ZLContext* )contextID;

	MOAIScopedLuaState state = context.Get < MOAILuaRuntime >().State ();
	if ( _loadContextFunc ( context, state ) != 0 ) return AKU_ERROR;

	_setupArgs ( state, exeName, scriptName, asParams );
	
	for ( int i = 0; i < argc; ++i ) {
		_pushArgOrParam ( state, i + 1, argv [ i ], asParams );
	}
	return _debugCallWithArgs ( state, argc, asParams );
}

//----------------------------------------------------------------//
int AKUCallFuncWithArgString ( AKUContextID contextID, char* exeName, char* scriptName, char* args, int asParams ) {

	assert ( contextID );
	ZLContext& context = *( ZLContext* )contextID;

	MOAIScopedLuaState state = context.Get < MOAILuaRuntime >().State ();
	if ( _loadContextFunc ( context, state ) != 0 ) return AKU_ERROR;

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

////----------------------------------------------------------------//
//int AKUCheckContext ( AKUContextID contextID ) {
//
//	//return sContextMap->contains ( contextID ) ? 0 : -1;
//	return ZLContextMgr::Check (( ZLContext* )contextID );
//}

//----------------------------------------------------------------//
void AKUClearMemPool () {

	ZL_TLSF_POOL* pool = zl_tlsf_get_pool ();
	zl_tlsf_set_pool ( 0 );
	
	if ( pool ) {
		zl_tlsf_destroy_pool ( pool );
	}
}

////----------------------------------------------------------------//
//int AKUCountContexts () {
//
//	return ( int )ZLContextMgr::CountContexts ();
//}

//----------------------------------------------------------------//
AKUContextID AKUCreateContext () {

	assert ( sizeof ( void* ) <= sizeof ( AKU_CONTEXT_ID_TYPE ));

	ZLContext* context = new ZLContext ();

	context->Affirm < AKUContext >();

	context->Affirm < MOAILuaRuntime >();
	context->Affirm < MOAITrace >();
	context->Affirm < MOAILogMgr >();
//	context->Affirm < MOAITestMgr >();
	
	MOAILuaRuntime& luaRuntime = context->Get < MOAILuaRuntime >();
	luaRuntime.Open ();
	luaRuntime.LoadLibs ();
	
	context->Affirm < MOAIPool >();
	context->Affirm < MOAIActionMgr >();
	context->Affirm < MOAIFooMgr >();
	context->Affirm < MOAIEaseType >();
	context->Affirm < MOAIEnvironment >();
	context->Affirm < MOAINodeMgr >();
	
	REGISTER_LUA_CLASS ( context, MOAIAction )
	REGISTER_LUA_CLASS ( context, MOAIActionMgr )
	REGISTER_LUA_CLASS ( context, MOAIActionTree )
	REGISTER_LUA_CLASS ( context, MOAIAnim )
	REGISTER_LUA_CLASS ( context, MOAIAnimCurveBone )
	REGISTER_LUA_CLASS ( context, MOAIAnimCurveFloat )
	REGISTER_LUA_CLASS ( context, MOAIAnimCurveIndex )
	REGISTER_LUA_CLASS ( context, MOAIAnimCurveQuat )
	REGISTER_LUA_CLASS ( context, MOAIAnimCurveVec )
	REGISTER_LUA_CLASS ( context, MOAICanary )
	REGISTER_LUA_CLASS ( context, MOAIEaseDriver )
	REGISTER_LUA_CLASS ( context, MOAIEaseType )
	REGISTER_LUA_CLASS ( context, MOAIFoo )
	REGISTER_LUA_CLASS ( context, MOAIImmediateCmdStream )
	REGISTER_LUA_CLASS ( context, MOAILuaRuntime )
	REGISTER_LUA_CLASS ( context, MOAIEnvironment )
	REGISTER_LUA_CLASS ( context, MOAIDeserializer )
	REGISTER_LUA_CLASS ( context, MOAILogMgr )
	REGISTER_LUA_CLASS ( context, MOAINode )
	REGISTER_LUA_CLASS ( context, MOAINodeMgr )
	REGISTER_LUA_CLASS ( context, MOAIPool )
	REGISTER_LUA_CLASS ( context, MOAIScope )
	REGISTER_LUA_CLASS ( context, MOAIRetainedCmdStream )
	REGISTER_LUA_CLASS ( context, MOAISerializer )
	REGISTER_LUA_CLASS ( context, MOAITimer )
	REGISTER_LUA_CLASS ( context, MOAITrace )
//	REGISTER_LUA_CLASS ( context, MOAITestMgr )

	context->Get < MOAIEnvironment >().DetectEnvironment ();

	return ( AKUContextID )context;
}

//----------------------------------------------------------------//
void AKUDeleteContext ( AKUContextID contextID ) {
	
	if ( !contextID ) return;
	ZLContext* context = ( ZLContext* )contextID;
	
	// MOAILusRuntime needs to clean up first; release all of the lua state and lua-bound objects
	context->Get < MOAILuaRuntime >().Close (); // call this ahead of everything to purge all the Lua bindings
	
	delete context;
}

////----------------------------------------------------------------//
//AKUContextID AKUGetContext () {
//	
//	return ( AKUContextID )ZLContextMgr::Get ();
//}
//
////----------------------------------------------------------------//
//void* AKUGetUserdata () {
//	
//	if ( AKUContext::IsValid ()) {
//		return AKUContext::Get ().mUserdata;
//	}
//	return 0;
//}

//----------------------------------------------------------------//
lua_State* AKUGetLuaState ( AKUContextID contextID ) {

	return (( ZLContext* )contextID )->Get < MOAILuaRuntime >().State ();
}

//----------------------------------------------------------------//
char* AKUGetMoaiAuthor ( char* buffer, size_t length ) {

	STLString version = MOAIVersion::GetAuthorString ();
	strncpy ( buffer, version.c_str (), length - 1 );
	buffer [ length - 1 ] = 0;
	return buffer;
}

//----------------------------------------------------------------//
char* AKUGetMoaiCommit ( char* buffer, size_t length ) {

	STLString version = MOAIVersion::GetCommitString ();
	strncpy ( buffer, version.c_str (), length - 1 );
	buffer [ length - 1 ] = 0;
	return buffer;
}

//----------------------------------------------------------------//
char* AKUGetMoaiVersion ( char* buffer, size_t length ) {

	STLString version = MOAIVersion::GetVersionString ();
	strncpy ( buffer, version.c_str (), length - 1 );
	buffer [ length - 1 ] = 0;
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
int AKULoadFuncFromBuffer ( AKUContextID contextID, void* data, size_t size, const char* chunkname, int compressed ) {

	(( ZLContext* )contextID )->Get < AKUContext >().mLuaFunc.Clear ();

	MOAIDataBuffer buffer;
	buffer.Load ( data, size );
	
	return _loadFuncFromBuffer ( *( ZLContext* )contextID, buffer, chunkname, compressed );
}

//----------------------------------------------------------------//
int AKULoadFuncFromFile ( AKUContextID contextID, const char* filename ) {

	(( ZLContext* )contextID )->Get < AKUContext >().mLuaFunc.Clear ();

	if ( !ZLFileSys::CheckFileExists ( filename )) {
		ZLLog_ErrorF ( ZLLog::CONSOLE, "Could not find file %s \n", filename );
		return AKU_ERROR;
	}

	MOAIDataBuffer buffer;
	if ( !buffer.Load ( filename )) return AKU_ERROR;
	if ( !buffer.Size ()) return AKU_ERROR;
	
	return _loadFuncFromBuffer ( *( ZLContext* )contextID, buffer, filename, AKU_DATA_UNCOMPRESSED );
}

//----------------------------------------------------------------//
int AKULoadFuncFromString ( AKUContextID contextID, const char* script, size_t size, const char* chunkname ) {

	return AKULoadFuncFromBuffer ( contextID, ( void* )script, size, chunkname, AKU_DATA_UNCOMPRESSED );
}

//----------------------------------------------------------------//
int AKUMountVirtualDirectory ( char const* virtualPath, char const* archive ) {

	int result = zl_mount_virtual ( virtualPath, archive );
	if ( result ) {
		ZLLog_ErrorF ( ZLLog::CONSOLE, "Error mounting %s at path %s\n", archive, virtualPath );
	}
	return result;
}

////----------------------------------------------------------------//
//int AKUSetContext ( AKUContextID contextID ) {
//	
////	if ( sContextID != contextID ) {
////		
////		sContextID = contextID;
////		sContext = sContextMap->value_for_key ( contextID );
////		
////		if ( sContext ) {
////			ZLContextMgr::Set ( sContext->mGlobals );
////			return 0;
////		}
////		ZLContextMgr::Set ( 0 );
////		return -1;
////	}
//	
//	// TODO: restore check for valid context (once work is done to share ZLContextMgr's
//	// context set across multiple threads).
//    ZLContextMgr::Set (( ZLContext* )contextID );
//	return 0;
//}

//----------------------------------------------------------------//
void AKUSetFunc_ErrorTraceback ( AKUContextID contextID, AKUErrorTracebackFunc func ) {

	(( ZLContext* )contextID )->Get < MOAILuaRuntime >().SetTracebackFunc ( func );
}

//----------------------------------------------------------------//
void AKUSetLogLevel ( int logLevel ) {

	ZLLog::Get ()->SetLogLevel ( logLevel );
}

//----------------------------------------------------------------//
void AKUSetUserdata ( AKUContextID contextID, void* userdata ) {

	AKUContext& aku = (( ZLContext* )contextID )->Get < AKUContext >();

	if ( aku.IsValid ()) {
		aku.mUserdata = userdata;
	}
}

//----------------------------------------------------------------//
int AKUSetWorkingDirectory ( char const* path ) {

	return zl_chdir ( path );
}
