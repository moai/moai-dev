// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLFACTORY_H
#define	ZLFACTORY_H

#include <zl-util/STLMap.h>

//================================================================//
// ZLDeallocator
//================================================================//
class ZLDeallocator {
public:

	//----------------------------------------------------------------//
	virtual void Delete ( void* mem ) = 0;

	//----------------------------------------------------------------//
	virtual ~ZLDeallocator	() {
	}
};

//================================================================//
// ZLAbstractCreator
//================================================================//
class ZLAbstractCreator :
	public ZLDeallocator {
public:

	//----------------------------------------------------------------//
	virtual	void*		New				() = 0;
	virtual	size_t		SizeOf			() = 0;
};

//================================================================//
// ZLConcreteCreator
//================================================================//
template < typename TYPE, typename CAST_TYPE >
class ZLConcreteCreator :
	public ZLAbstractCreator {
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
	size_t SizeOf () {
		return sizeof ( TYPE );
	}
};

//================================================================//
// ZLFactory
//================================================================//
template < typename KEY_TYPE, typename PRODUCT_TYPE >
class ZLFactory {
private:

	STLMap < KEY_TYPE, ZLAbstractCreator* > mCreatorMap;

public:

	typedef	typename STLMap < KEY_TYPE, ZLAbstractCreator* >::iterator iterator;

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
	ZLAbstractCreator* GetCreator ( KEY_TYPE key ) {
	
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
	
		static ZLConcreteCreator < TYPE, PRODUCT_TYPE > creator;
		this->mCreatorMap [ key ] = &creator;
	};
	
	//----------------------------------------------------------------//
	ZLFactory () {
	};
	
	//----------------------------------------------------------------//
	virtual ~ZLFactory () {
	};
};

#endif
