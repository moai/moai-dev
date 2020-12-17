// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPOOLEDOBJECTFACTORY_H
#define	MOAIPOOLEDOBJECTFACTORY_H

#include <moai-core/MOAILua.h>
#include <moai-core/MOAIScope.h>

class MOAIScope;

//================================================================//
// MOAIAbstractPooledObjectFactory
//================================================================//
class MOAIAbstractPooledObjectFactory :
	public virtual ZLRefCountedObject {
protected:
	
	//----------------------------------------------------------------//
	virtual MOAILuaObject*			MOAIAbstractPooledObjectFactory_Create 				() = 0;
	virtual void 					MOAIAbstractPooledObjectFactory_OnProvision 		( MOAILuaObject& object, MOAIScope* scope ) = 0;
	virtual void 					MOAIAbstractPooledObjectFactory_OnRemit 			( MOAILuaObject& object ) = 0;

public:

	//----------------------------------------------------------------//
	MOAILuaObject* Create () {
		return MOAIAbstractPooledObjectFactory_Create ();
	}

	//----------------------------------------------------------------//
	MOAIAbstractPooledObjectFactory () {
	}
	
	//----------------------------------------------------------------//
	virtual ~MOAIAbstractPooledObjectFactory () {
	}
	
	//----------------------------------------------------------------//
	void OnProvision ( MOAILuaObject& object, MOAIScope* scope ) {
		return MOAIAbstractPooledObjectFactory_OnProvision ( object, scope );
	}
	
	//----------------------------------------------------------------//
	void OnRemit ( MOAILuaObject& object ) {
		return MOAIAbstractPooledObjectFactory_OnRemit ( object );
	}
};

//================================================================//
// MOAIPooledObjectLuaFactory
//================================================================//
class MOAIPooledObjectLuaFactory :
	public virtual MOAIAbstractPooledObjectFactory {
protected:

	friend class MOAIPool;

	MOAILuaStrongRef		mFactory;
	MOAILuaStrongRef		mOnProvision;
	MOAILuaStrongRef		mOnRemit;
	
	//----------------------------------------------------------------//
	MOAILuaObject* MOAIAbstractPooledObjectFactory_Create () {
		
		assert ( this->mFactory );
		
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mFactory );
		state.DebugCall ( 0, 1 );
		MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( -1, false );

		return object;
	}

	//----------------------------------------------------------------//
	void MOAIAbstractPooledObjectFactory_OnProvision ( MOAILuaObject& object, MOAIScope* scope ) {

		if ( !( this->mOnProvision )) return;
		
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mOnProvision );
		state.Push ( object );
		if ( scope ) {
			state.Push ( scope );
			state.DebugCall ( 2, 0 );
		}
		else {
			state.DebugCall ( 1, 0 );
		}
	}

	//----------------------------------------------------------------//
	void MOAIAbstractPooledObjectFactory_OnRemit ( MOAILuaObject& object ) {

		if ( !( this->mOnRemit )) return;
		
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mOnRemit );
		state.Push ( &object );
		state.DebugCall ( 1, 0 );
	}

public:
	
	//----------------------------------------------------------------//
	MOAIPooledObjectLuaFactory () {
	}
};

//================================================================//
// MOAIPooledObjectFactory
//================================================================//
template < typename TYPE >
class MOAIPooledObjectFactory :
	public virtual MOAIAbstractPooledObjectFactory {
protected:
	
	//----------------------------------------------------------------//
	MOAILuaObject* MOAIAbstractPooledObjectFactory_Create () {
		return new TYPE ();
	}

	//----------------------------------------------------------------//
	void MOAIAbstractPooledObjectFactory_OnProvision ( MOAILuaObject& object, MOAIScope* scope ) {
		UNUSED ( object );
		UNUSED ( scope );
	}

	//----------------------------------------------------------------//
	void MOAIAbstractPooledObjectFactory_OnRemit ( MOAILuaObject& object ) {
		UNUSED ( object );
	}

public:
	
	//----------------------------------------------------------------//
	MOAIPooledObjectFactory () {
	}
};

#endif
