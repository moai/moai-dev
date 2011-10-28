// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFS_REPLACE_H
#define ZIPFS_REPLACE_H

#ifdef  __cplusplus
	extern "C" {
#endif

#include <zipfs/zipfs.h>

//================================================================//
// stdlib
//================================================================//

#undef calloc
#define calloc zipfs_calloc

#undef free
#define free zipfs_free

#undef malloc
#define malloc zipfs_malloc

#undef realloc
#define realloc zipfs_realloc

#ifdef  __cplusplus
	}
#endif

//================================================================//
// cpp
//================================================================//

// TODO:
// this is giving problems with untz
// unclear at this time how to selectively overload new/delete for specific libraries

#ifdef __cplusplus

	#include <new>

	//----------------------------------------------------------------//
	inline void* operator new ( size_t size ) throw ( std::bad_alloc ) {
		void* mem = malloc ( size );
		if ( mem ) return mem;
		throw std::bad_alloc();
	}

	//----------------------------------------------------------------//
	inline void* operator new ( std::size_t size, const std::nothrow_t& ) throw () {
		return malloc ( size );
	}

	//----------------------------------------------------------------//
	inline void* operator new []( size_t size ) throw ( std::bad_alloc ) {
		void* mem = malloc ( size );
		if ( mem ) return mem;
		throw std::bad_alloc();
	}

	//----------------------------------------------------------------//
	inline void* operator new []( std::size_t size, const std::nothrow_t& ) throw () {
		return malloc ( size );
	}

	//----------------------------------------------------------------//
	inline void operator delete ( void* ptr ) throw() {
		free ( ptr );
	}

	//----------------------------------------------------------------//
	inline void operator delete []( void* ptr ) throw() {
		free ( ptr );
	}

#endif

#endif