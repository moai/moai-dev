// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLMERCATOR_H
#define ZLMERCATOR_H

#include <zl-util/ZLVec2D.h>

//================================================================//
// ZLAllocator
//================================================================//
class ZLAllocator {
public:

	//----------------------------------------------------------------//
	virtual void*		Alloc		( size_t size ) = 0;
	virtual void		Free		( void* mem ) = 0;

	//----------------------------------------------------------------//
	ZLAllocator () {
	}

	//----------------------------------------------------------------//
	virtual ~ZLAllocator () {
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	void Delete ( TYPE* type ) {
		
		if ( type ) {
			type->TYPE::~TYPE ();
			this->Free ( type );
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* New () {
		
		TYPE* type = ( TYPE* )this->Alloc ( sizeof ( TYPE ));
		return type ? new ( type ) TYPE () : 0;
	}
};

//================================================================//
// ZLSimpleAllocator
//================================================================//
class ZLSimpleAllocator :
	ZLAllocator {
public:

	//----------------------------------------------------------------//
	void* Alloc ( size_t size ) {
	
		return malloc ( size );
	}
	
	//----------------------------------------------------------------//
	void Free ( void* mem ) {
	
		if ( mem ) {
			free ( mem );
		}
	}

	//----------------------------------------------------------------//
	ZLSimpleAllocator () {
	}

	//----------------------------------------------------------------//
	virtual ~ZLSimpleAllocator () {
	}
};

#endif
