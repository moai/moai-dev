// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIPool.h>
#include <moai-core/MOAIScope.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPool::_addFactory ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIPool, "" )

	u32 typeID = MOAILuaObject::NOT_IN_POOL;

	if ( state.IsType ( 1, LUA_TFUNCTION )) {
		typeID = self->mMaxID++;
		
		MOAIPooledObjectLuaFactory* factory = new MOAIPooledObjectLuaFactory ();
		
		factory->mFactory.SetRef ( state, 1 );
		factory->mOnProvision.SetRef ( state, 2 );
		factory->mOnRemit.SetRef ( state, 3 );
		
		self->mFactory [ typeID ] = factory;
	}
	state.Push ( typeID );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPool::_provision ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIPool, "" )

	u32 typeID = MOAILuaObject::NOT_IN_POOL;
	
	if ( state.IsType ( 1, LUA_TNUMBER )) {
		typeID = state.GetValue < u32 >( 1, MOAILuaObject::NOT_IN_POOL );
	}

	if ( state.IsType ( 1, LUA_TTABLE )) {
		typeID = state.GetFieldValue < cc8*, u32 >( 1, "typeID", MOAILuaObject::NOT_IN_POOL );
	}

	MOAIScope* scope = state.GetLuaObject < MOAIScope >( 2, true );
		
	MOAILuaObject* resource = self->Provision ( typeID, scope );
	assert ( resource );

	if ( resource ) {
		state.Push ( resource );
		assert ( state.IsType ( -1, LUA_TUSERDATA ));
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIPool::_remit ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIPool, "U" )

	MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( 1, true );

	if ( object ) {
		self->Remit ( object );
	}
	return 0;
}

//================================================================//
// MOAIPool
//================================================================//

//----------------------------------------------------------------//
MOAIPool::MOAIPool ( ZLContext& context ) :
	ZLHasContext ( context ),
	ZLContextClass ( context ),
	MOAILuaObject ( context ),
	mMaxID ( STARTING_ID ) {
	
	RTTI_BEGIN ( MOAIPool )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIPool >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIPool::~MOAIPool () {
}

//----------------------------------------------------------------//
MOAILuaObject* MOAIPool::Provision ( u32 poolType, MOAIScope* scope ) {

	if ( poolType == MOAILuaObject::NOT_IN_POOL ) return NULL;
	if ( !this->mFactory.contains ( poolType )) return NULL;

	MOAIAbstractPooledObjectFactory* factory = this->mFactory [ poolType ];
	assert ( factory );

	MOAILuaObject* resource = NULL;

	STLSet < MOAILuaObject* >& pool = this->mAvailableResourcesByType [ poolType ];
	if ( pool.size ()) {
		STLSet < MOAILuaObject* >::iterator resourceIt = pool.begin ();
		resource = *resourceIt;
		assert ( resource );
		pool.erase ( resourceIt );
	}
	
	if ( !resource ) {
		
		resource = factory->Create ( this->mContext );
		if ( !resource ) return NULL;
		
		assert ( !resource->IsInPool ());
		assert ( resource->GetRefCount () == 0 );
		
		this->LuaRetain ( resource );
		
		resource->mPoolType = poolType;
		this->mResources.insert ( resource );
	}
	
	if ( scope && resource ) {
		scope->AffirmObject ( *resource );
	}
	factory->OnProvision ( *resource, scope );
	return resource;
}

//----------------------------------------------------------------//
void MOAIPool::Remit ( MOAILuaObject* resource ) {

	assert ( resource );
	assert ( resource->IsInPool ());
	assert ( this->mFactory.contains ( resource->mPoolType ));
	
	this->mFactory [ resource->mPoolType ]->OnRemit ( *resource );
	resource->MOAILuaObject_OnPooledRemit ();
	
	STLSet < MOAILuaObject* >& pool = this->mAvailableResourcesByType [ resource->mPoolType ];
	pool.insert ( resource );
}

//----------------------------------------------------------------//
void MOAIPool::PurgeAll () {

	STLSet < MOAILuaObject* >::iterator resourceIt = this->mResources.begin ();
	for ( ; resourceIt != this->mResources.end (); ++resourceIt ) {
		MOAILuaObject* resource = *resourceIt;
		resource->mPoolType = MOAILuaObject::NOT_IN_POOL;
		this->LuaRelease ( resource );
	}
	this->mResources.clear ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIPool::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "addFactory",				_addFactory },
		{ "provision",				_provision },
		{ "remit",					_remit },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPool::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIPool::ZLContextClass_Finalize () {

	this->PurgeAll ();
}
