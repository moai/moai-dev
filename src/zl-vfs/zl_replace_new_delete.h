// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_REPLACE_NEW_DELETE_H
#define ZL_REPLACE_NEW_DELETE_H

#include <zl-vfs/headers.h>

//================================================================//
// new/delete operators
//================================================================//

// TODO:
// this is giving problems with untz
// unclear at this time how to selectively overload new/delete for specific libraries

#ifdef __cplusplus

	#include <new>

//	//----------------------------------------------------------------//
//	void* operator new ( size_t size ) throw ( std::bad_alloc ) {
//		void* mem = malloc ( size );
//		#if 0
//			if ( mem ) return mem;
//			throw std::bad_alloc();
//		#else
//			return mem;
//		#endif
//	}
//
//	//----------------------------------------------------------------//
//	void* operator new ( std::size_t size, const std::nothrow_t& ) throw () {
//		return malloc ( size );
//	}
//
//	//----------------------------------------------------------------//
//	void* operator new []( size_t size ) throw ( std::bad_alloc ) {
//		void* mem = malloc ( size );
//		#if 0
//			if ( mem ) return mem;
//			throw std::bad_alloc();
//		#else
//			return mem;
//		#endif
//	}
//
//	//----------------------------------------------------------------//
//	void* operator new []( std::size_t size, const std::nothrow_t& ) throw () {
//		return malloc ( size );
//	}
//
//	//----------------------------------------------------------------//
//	void operator delete ( void* ptr ) throw() {
//		free ( ptr );
//	}
//
//	//----------------------------------------------------------------//
//	void operator delete []( void* ptr ) throw() {
//		free ( ptr );
//	}

#endif
#endif
