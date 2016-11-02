//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAI_CORE_HOST_H
#define MOAI_CORE_HOST_H

#include <stddef.h>

#ifdef WIN32

	#ifdef AKU_EXPORT
		#define AKU_API __declspec(dllexport)
	#endif

	#ifdef AKU_DLL
		#define AKU_API __declspec(dllimport)
	#else
		#define AKU_API
	#endif

#else
	#ifdef MOAI_OS_HTML
		#define AKU_API extern "C"
	#else
		#define AKU_API
	#endif
#endif

#ifndef AKU_CONTEXT_ID_TYPE
	#define AKU_CONTEXT_ID_TYPE void*
#endif

struct lua_State;
typedef struct lua_State lua_State;
typedef AKU_CONTEXT_ID_TYPE AKUContextID;

enum {
	AKU_AS_ARGS		= 0,
	AKU_AS_PARAMS	= 1,
};

enum {
	AKU_DATA_ZIPPED,
	AKU_DATA_UNCOMPRESSED,
};

enum {
	AKU_LOG_DEBUG,
	AKU_LOG_STATUS,
	AKU_LOG_WARNING,
	AKU_LOG_ERROR,
	AKU_LOG_FATAL,
	AKU_LOG_NONE,
};

enum {
	AKU_OK,
	AKU_ERROR		= -1,
};

// callbacks
typedef void ( *AKUErrorTracebackFunc )         ( const char* message, struct lua_State* L, int level );

// context api
AKU_API void			AKUAppFinalize					();
AKU_API void			AKUAppInitialize				();
AKU_API int				AKUCheckContext					( AKUContextID context );
AKU_API void			AKUClearMemPool					();
AKU_API int				AKUCountContexts				();
AKU_API AKUContextID	AKUCreateContext				();
AKU_API void			AKUDeleteContext				( AKUContextID context );
AKU_API AKUContextID	AKUGetContext					();
AKU_API void*			AKUGetUserdata					();

AKU_API void			AKUInitMemPool					( size_t sizeInBytes );
AKU_API int				AKUSetContext					( AKUContextID context );
AKU_API void			AKUSetLogLevel					( int logLevel );
AKU_API void			AKUSetUserdata					( void* user );

// management api
AKU_API int				AKUCallFunc						();
AKU_API int				AKUCallFuncWithArgArray			( char* exeName, char* scriptName, int argc, char** argv, int asParams );
AKU_API int				AKUCallFuncWithArgString		( char* exeName, char* scriptName, char* args, int asParams );
AKU_API lua_State*		AKUGetLuaState					();
AKU_API char*			AKUGetMoaiVersion				( char* buffer, size_t length );
AKU_API char*			AKUGetWorkingDirectory			( char* buffer, size_t length );
AKU_API int				AKULoadFuncFromBuffer			( void* data, size_t size, const char* chunkname, int compressed );
AKU_API int				AKULoadFuncFromFile				( const char* filename );
AKU_API int				AKULoadFuncFromString			( const char* script, size_t size, const char* chunkname );
AKU_API int				AKUMountVirtualDirectory		( char const* virtualPath, char const* archive );
AKU_API int				AKUSetWorkingDirectory			( char const* path );

// callback management
AKU_API void			AKUSetFunc_ErrorTraceback		( AKUErrorTracebackFunc func );

#endif
