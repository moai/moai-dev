// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USLUASHAREDPTR_H
#define	USLUASHAREDPTR_H

#include <uslscore/USCanary.h>

//================================================================//
// USLuaSharedPtr
//================================================================//
template < typename TYPE >
class USLuaSharedPtr {
protected:

	TYPE*			mObject;
	
	//----------------------------------------------------------------//
	inline void operator = ( const USLuaSharedPtr < TYPE >& assign ) {
		UNUSED ( assign );
		assert ( false ); // unsupported
	};
	
	//----------------------------------------------------------------//
	inline TYPE* Get () {
	
		return this->mObject;
	}

	//----------------------------------------------------------------//
	USLuaSharedPtr ( const USLuaSharedPtr < TYPE >& assign ) :
		mObject ( 0 ) {
		UNUSED ( assign );
		assert ( false ); // unsupported
	};

public:

	//----------------------------------------------------------------//
	inline operator bool () {
		return this->Get () != 0;
	};

	//----------------------------------------------------------------//
	inline TYPE& operator * () {
		return *this->Get ();
	};

	//----------------------------------------------------------------//
	inline TYPE* operator -> () {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline operator TYPE* () {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline void Set ( USLuaObject& owner, TYPE* assign ) {

		if ( this->mObject != assign ) {

			if ( assign ) {
				owner.LuaRetain ( *assign );
			}

			if ( this->mObject ) {
				owner.LuaRelease ( *this->mObject );
			}
			
			this->mObject = assign;
		}
	}

	//----------------------------------------------------------------//
	USLuaSharedPtr () :
		mObject ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~USLuaSharedPtr () {
		assert ( !this->mObject ); // must be manually cleared before destruction; use Set ( owner, 0 )
	}
};

#endif
