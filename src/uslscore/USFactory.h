// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	STD_FACTORY_H
#define	STD_FACTORY_H

#include <uslscore/STLMap.h>

//================================================================//
// USDeallocator
//================================================================//
class USDeallocator {
public:

	//----------------------------------------------------------------//
	virtual void Delete ( void* mem ) = 0;

	//----------------------------------------------------------------//
	virtual ~USDeallocator	() {
	}
};

//================================================================//
// USAbstractCreator
//================================================================//
class USAbstractCreator :
	public USDeallocator {
public:

	//----------------------------------------------------------------//
	virtual	void*		New				() = 0;
	virtual	void*		Place			( void* mem ) = 0;
	virtual	u32			SizeOf			() = 0;
};

//================================================================//
// USConcreteCreator
//================================================================//
template < typename TYPE, typename CAST_TYPE >
class USConcreteCreator :
	public USAbstractCreator {
public:
	
	//----------------------------------------------------------------//
	void Delete ( void* mem ) {
	
		delete ( TYPE* )mem;
	}
	
	//----------------------------------------------------------------//
	void* New () {
		
		// create the type
		TYPE* type = new TYPE ();
		assert ( type );
		
		// pointer arithmetic to match expected return type
		CAST_TYPE* cast = type;
		return cast;
	};
	
	//----------------------------------------------------------------//
	void* Place ( void* mem ) {
		return new ( mem ) TYPE ();
	}
	
	//----------------------------------------------------------------//
	u32 SizeOf () {
		return sizeof ( TYPE );
	}
};

//================================================================//
// USAllocator
//================================================================//
class USAllocator :
	public USDeallocator {
public:

	//----------------------------------------------------------------//
	void* Alloc ( u32 size ) {
		return malloc ( size );
	}

	//----------------------------------------------------------------//
	void Delete ( void* mem ) {
		free ( mem );
	}
};

//================================================================//
// USConcreteArrayCreator
//================================================================//
template < typename TYPE, typename CAST_TYPE = TYPE >
class USConcreteArrayCreator :
	public USConcreteCreator < TYPE, CAST_TYPE > {
public:
	
	//----------------------------------------------------------------//
	void Delete ( void* mem ) {
		delete []( TYPE* )mem;
	}
	
	//----------------------------------------------------------------//
	void* New ( u32 nElements ) {
	
		assert ( nElements >= 1 );
		TYPE* mem = new TYPE [ nElements ];
		assert ( mem );
		return mem;
	};
};

//================================================================//
// USFactory
//================================================================//
template < typename KEY_TYPE, typename PRODUCT_TYPE >
class USFactory {
private:

	typedef	typename STLMap < KEY_TYPE, USAbstractCreator* >::iterator CreatorMapIt;
	STLMap < KEY_TYPE, USAbstractCreator* > mCreatorMap;

public:

	//----------------------------------------------------------------//
	PRODUCT_TYPE* Create ( KEY_TYPE key ) {
		
		CreatorMapIt creatorMapIt =	mCreatorMap.find ( key );
		if ( creatorMapIt == mCreatorMap.end ()) return	0;
		return ( PRODUCT_TYPE* )creatorMapIt->second->New ();
	};
	
	//----------------------------------------------------------------//
	USAbstractCreator* GetCreator ( KEY_TYPE key ) {
	
		CreatorMapIt creatorMapIt =	mCreatorMap.find ( key );
		if ( creatorMapIt == mCreatorMap.end ()) return	0;
		return creatorMapIt->second;
	}

	//----------------------------------------------------------------//
	bool IsValidKey	( KEY_TYPE key ) {
	
		if ( mCreatorMap.find ( key ) == mCreatorMap.end ()) {
			return false;
		}
		return true;
	};

	//----------------------------------------------------------------//
	template < class TYPE >
	void Register ( KEY_TYPE key ) {
	
		static USConcreteCreator < TYPE, PRODUCT_TYPE > creator;
		this->mCreatorMap [ key ] = &creator;
	};
	
	//----------------------------------------------------------------//
	USFactory () {
	};
	
	//----------------------------------------------------------------//
	virtual ~USFactory () {
	};
};

#endif
