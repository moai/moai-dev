// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUASHAREDPTR_H
#define	MOAILUASHAREDPTR_H

#include <moai-core/MOAILuaObject.h>

//================================================================//
// MOAILuaSharedPtr
//================================================================//
template < typename TYPE >
class MOAILuaSharedPtr {
protected:

	TYPE*			mObject;
	
	//----------------------------------------------------------------//
	inline TYPE* Get () {
		return this->mObject;
	}
	
	//----------------------------------------------------------------//
	inline const TYPE* Get () const {
		return this->mObject;
	}

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
	inline void operator = ( const MOAILuaSharedPtr < TYPE >& assign ) {
		assert ( !this->mObject );
		this->mObject = assign.mObject;
		(( MOAILuaSharedPtr < TYPE >& )assign ).mObject = 0;
	};

	//----------------------------------------------------------------//
	inline bool PushRef ( MOAILuaState& state ) {
	
		if ( this->mObject ) {
			state.Push ( this->mObject );
		}
		else {
			lua_pushnil ( state );
		}
		return !lua_isnil ( state, -1 );
	}

	//----------------------------------------------------------------//
	inline void Set ( MOAILuaObject& owner, TYPE* assign ) {

		if ( this->mObject != assign ) {
			owner.LuaRetain ( assign );
			owner.LuaRelease ( this->mObject );
			this->mObject = assign;
		}
	}

	//----------------------------------------------------------------//
	MOAILuaSharedPtr () :
		mObject ( 0 ) {
	}
	
	//----------------------------------------------------------------//
	~MOAILuaSharedPtr () {
		assert ( !this->mObject ); // must be manually cleared before destruction; use Set ( owner, 0 )
	}
	
	//----------------------------------------------------------------//
	MOAILuaSharedPtr ( const MOAILuaSharedPtr < TYPE >& assign ) :
		mObject ( assign.mObject ) {
		(( MOAILuaSharedPtr < TYPE >& )assign ).mObject = 0;
	};
};

#endif
