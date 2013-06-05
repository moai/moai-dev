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

struct lua_State;
typedef int AKUContextID;

enum {
	AKU_DATA_BYTECODE,
	AKU_DATA_STRING,
	AKU_DATA_ZIPPED,
	AKU_DATA_UNCOMPRESSED,
};

// callbacks
typedef void ( *AKUErrorTracebackFunc )         ( const char* message, struct lua_State* L, int level );

// context api
AKU_API void			AKUClearMemPool					();
AKU_API AKUContextID	AKUCreateContext				();
AKU_API void			AKUDeleteContext				( AKUContextID context );
AKU_API AKUContextID	AKUGetContext					();
AKU_API void*			AKUGetUserdata					();
AKU_API void			AKUFinalize						();
AKU_API void			AKUInitMemPool					( size_t sizeInBytes );
AKU_API void			AKUSetContext					( AKUContextID context );
AKU_API void			AKUSetUserdata					( void* user );

// management api
AKU_API lua_State*		AKUGetLuaState					();
AKU_API char*			AKUGetMoaiVersion				( char* buffer, size_t length );
AKU_API char*			AKUGetWorkingDirectory			( char* buffer, size_t length );
AKU_API int				AKUMountVirtualDirectory		( char const* virtualPath, char const* archive );
AKU_API void			AKURunData						( void* data, size_t size, int dataType, int compressed );
AKU_API void			AKURunScript					( const char* filename );
AKU_API void			AKURunString					( const char* script );
AKU_API int				AKUSetWorkingDirectory			( char const* path );
AKU_API void			AKUSetArgv						( char **argv );

// callback management
AKU_API void			AKUSetFunc_ErrorTraceback		( AKUErrorTracebackFunc func );

#endif
