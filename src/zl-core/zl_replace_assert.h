// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_ASSERT_H
#define ZL_ASSERT_H

#ifdef  __cplusplus
    extern "C" {
#endif

#include <setjmp.h>

#define zl_assert(x) (( void )( !( x ) && ( zl_handle_assert ( #x, __func__, __FILE__, __LINE__ ), 1 )))
#define zl_begin_assert_env() setjmp ( *zl_set_assert_jmp_buf (( jmp_buf* )calloc ( 1, sizeof ( jmp_buf ))))
#define zl_end_assert_env() zl_set_assert_jmp_buf ( 0 )

//================================================================//
// assert
//================================================================//

//----------------------------------------------------------------//
extern void                    zl_handle_assert            ( const char* condition, const char* funcname, const char* filename, int lineno );
extern jmp_buf*                zl_set_assert_jmp_buf        ( jmp_buf* env );

#ifdef  __cplusplus
    }
#endif

#endif

//================================================================//
// replace
//================================================================//

#undef __assert
#undef assert

#ifdef NDEBUG
    //for vs2015 currently break command line build
    //#define __assert(x) (( void )sizeof ( x ))
    //#define assert(x) (( void )sizeof ( x ))
    #ifndef ANDROID_NDK
        #define __assert(x) (( void ) 0 )
    #endif
	#define assert(x) (( void ) 0 )
#else
    #ifndef ANDROID_NDK
        #define __assert zl_assert
    #endif
    #define assert zl_assert
#endif