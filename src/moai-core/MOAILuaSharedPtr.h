// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUASHAREDPTR_H
#define	MOAILUASHAREDPTR_H

#include <moai-core/MOAILuaCanary.h>
#include <moai-core/MOAILuaObject.h>

//================================================================//
// MOAILuaSharedPtr
//================================================================//
template < typename TYPE >
class MOAILuaSharedPtr {
protected:

	TYPE*			mObject;
	MOAILuaCanary*	mCanary;
	
	//----------------------------------------------------------------//
	inline void operator = ( const MOAILuaSharedPtr < TYPE >& assign ) {
		UNUSED ( assign );
		assert ( false ); // unsupported
	};
	
	//----------------------------------------------------------------//
	inline TYPE* Get () {
		return this->GetObject ();
	}
	
	//----------------------------------------------------------------//
	inline const TYPE* Get () const {
		return this->GetObject ();
	}

	//----------------------------------------------------------------//
	inline TYPE* GetObject () const {
		if ( this->mCanary && this->mCanary->IsValid ()) {	
			return this->mObject;
		}
		return 0;
	}

	//----------------------------------------------------------------//
	MOAILuaSharedPtr ( const MOAILuaSharedPtr < TYPE >& assign ) :
		mObject ( 0 ),
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
	inline operator bool () const {
		return this->Get () != 0;
	};

	//----------------------------------------------------------------//
	inline TYPE& operator * () {
		return *this->Get ();
	};

	//----------------------------------------------------------------//
	inline const TYPE& operator * () const {
		return *this->Get ();
	};

	//----------------------------------------------------------------//
	inline TYPE* operator -> () {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline const TYPE* operator -> () const {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline operator TYPE* () {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline operator const TYPE* () const {
		return this->Get ();
	};

	//----------------------------------------------------------------//
	inline void Set ( MOAILuaObject& owner, TYPE* assign ) {

		if ( this->mObject != assign ) {
			
			owner.LuaRetain ( assign );
			
			if ( this->mCanary ) {
				owner.LuaRelease ( this->Get ());
				this->mCanary->Release ();
				this->mObject = 0;
				this->mCanary = 0;
			}
			
			if ( assign ) {
				this->mCanary = assign->AffirmCanary ();
				this->mCanary->Retain ();
				this->mObject = assign;
			}
		}
	}

	//----------------------------------------------------------------//
	MOAILuaSharedPtr () :
		mObject ( 0 ),
		mCanary ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~MOAILuaSharedPtr () {
		assert ( !this->mObject ); // must be manually cleared before destruction; use Set ( owner, 0 )
	}
};

#endif
