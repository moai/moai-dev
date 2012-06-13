// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_REPLACE_NEW_DELETE_H
#define ZL_REPLACE_NEW_DELETE_H

#include <zlcore/zlcore.h>

//================================================================//
// new/delete operators
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