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
	u32 SizeOf () {
		return sizeof ( TYPE );
	}
};

//================================================================//
// USFactory
//================================================================//
template < typename KEY_TYPE, typename PRODUCT_TYPE >
class USFactory {
private:

	STLMap < KEY_TYPE, USAbstractCreator* > mCreatorMap;

public:

	typedef	typename STLMap < KEY_TYPE, USAbstractCreator* >::iterator iterator;

	//----------------------------------------------------------------//
	iterator Begin () {
		return this->mCreatorMap.begin ();
	}

	//----------------------------------------------------------------//
	PRODUCT_TYPE* Create ( KEY_TYPE key ) {
		
		iterator itr =	this->mCreatorMap.find ( key );
		if ( itr == this->mCreatorMap.end ()) return 0;
		return ( PRODUCT_TYPE* )itr->second->New ();
	};
	
	//----------------------------------------------------------------//
	iterator End () {
		return this->mCreatorMap.end ();
	}
	
	//----------------------------------------------------------------//
	USAbstractCreator* GetCreator ( KEY_TYPE key ) {
	
		iterator itr =	this->mCreatorMap.find ( key );
		if ( itr == this->mCreatorMap.end ()) return 0;
		return itr->second;
	}

	//----------------------------------------------------------------//
	bool IsValidKey	( KEY_TYPE key ) {
	
		if ( this->mCreatorMap.find ( key ) == this->mCreatorMap.end ()) {
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
