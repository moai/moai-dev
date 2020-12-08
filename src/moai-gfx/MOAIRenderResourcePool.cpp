// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIRenderResourcePool.h>

//================================================================//
// MOAIRenderResource
//================================================================//

//----------------------------------------------------------------//
bool MOAIRenderResource::IsInPool () {

	return ( this->mPoolType != NOT_IN_POOL );
}

//----------------------------------------------------------------//
MOAIRenderResource::MOAIRenderResource () :
	mPoolType ( NOT_IN_POOL ) {
}

//----------------------------------------------------------------//
MOAIRenderResource::~MOAIRenderResource () {
}

//----------------------------------------------------------------//
void MOAIRenderResource::SetPoolType ( u32 poolType ) {

	this->mPoolType = poolType;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderResource::ZLRefCountedObjectBase_OnRelease ( u32 refCount ) {

	if ( this->IsInPool () && ( refCount < 2 )) {
		if ( refCount == 0 ) {
			this->Retain ();
		}
		MOAIRenderResourcePool::Get ().Remit ( this );
	}
	MOAILuaObject::ZLRefCountedObjectBase_OnRelease ( refCount );
}

//----------------------------------------------------------------//
void MOAIRenderResource::MOAIRenderResource_OnRemit () {
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderResourcePool::_addFactory ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIRenderResourcePool, "" )

	u32 typeID = MOAIRenderResource::NOT_IN_POOL;

	if ( state.IsType ( 1, LUA_TFUNCTION )) {
		typeID = self->mMaxID++;
		MOAIRenderResourceFactory& factory = self->mFactory [ typeID ];
		factory.mFactory.SetRef ( state, 1 );
		factory.mOnProvision.SetRef ( state, 2 );
		factory.mOnRemit.SetRef ( state, 3 );
	}
	state.Push ( typeID );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderResourcePool::_provision ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIRenderResourcePool, "" )

	u32 typeID = state.GetValue < u32 >( 1, MOAIRenderResource::NOT_IN_POOL );
	MOAIRenderResource* resource = self->Provision ( typeID );
	assert ( resource );

	if ( resource ) {
		state.Push ( resource );
		assert ( state.IsType ( -1, LUA_TUSERDATA ));
		return 1;
	}
	return 0;
}

//================================================================//
// MOAIRenderResourcePool
//================================================================//

//----------------------------------------------------------------//
MOAIRenderResource* MOAIRenderResourcePool::Provision ( u32 poolType ) {

	if ( poolType == MOAIRenderResource::NOT_IN_POOL ) return NULL;
	if ( !this->mFactory.contains ( poolType )) return NULL;

	MOAIRenderResourceFactory& factory = this->mFactory [ poolType ];

	MOAIRenderResource* resource = NULL;

	STLSet < MOAIRenderResource* >& pool = this->mAvailableResourcesByType [ poolType ];
	if ( pool.size ()) {
		STLSet < MOAIRenderResource* >::iterator resourceIt = pool.begin ();
		resource = *resourceIt;
		assert ( resource );
		pool.erase ( resourceIt );
	}
	
	if ( !resource ) {
	
		resource = factory.Create ();
		if ( !resource ) return NULL;
		
		assert ( !resource->IsInPool ());
		assert ( resource->GetRefCount () == 0 );
		
		resource->mPoolType = poolType;
		this->LuaRetain ( resource );
		this->mResources.insert ( resource );
	}
	
	factory.OnProvision ( resource );
	return resource;
}

//----------------------------------------------------------------//
void MOAIRenderResourcePool::Remit ( MOAIRenderResource* resource ) {

	assert ( resource );
	assert ( resource->IsInPool ());
	assert ( this->mFactory.contains ( resource->mPoolType ));
		
	this->mFactory [ resource->mPoolType ].OnRemit ( resource );
	resource->MOAIRenderResource_OnRemit ();
	
	STLSet < MOAIRenderResource* >& pool = this->mAvailableResourcesByType [ resource->mPoolType ];
	pool.insert ( resource );
}

//----------------------------------------------------------------//
MOAIRenderResourcePool::MOAIRenderResourcePool () :
	mMaxID ( 0 ) {
	
	RTTI_BEGIN ( MOAIRenderResourcePool )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIRenderResourcePool >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIRenderResourcePool::~MOAIRenderResourcePool () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIRenderResourcePool::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

//	state.SetField ( -1, "DECK2D_SHADER",			( u32 )MOAIShaderPresetEnum::DECK2D_SHADER );

	luaL_Reg regTable [] = {
		{ "addFactory",				_addFactory },
		{ "provision",				_provision },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRenderResourcePool::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIRenderResourcePool::ZLContextClass_Finalize () {

	STLSet < MOAIRenderResource* >::iterator resourceIt = this->mResources.begin ();
	for ( ; resourceIt != this->mResources.end (); ++resourceIt ) {
		MOAIRenderResource* resource = *resourceIt;
		resource->SetPoolType ();
		this->LuaRelease ( resource );
	}
	this->mResources.clear ();
}
