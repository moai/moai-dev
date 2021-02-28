// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractCmdAPI.h>
#include <moai-core/MOAIAbstractCmdStream.h>
#include <moai-core/MOAICmdMedium.h>
#include <moai-core/MOAICmdMediumPolymorphic.h>

//================================================================//
// MOAICmdLuaInterfaceMgr
//================================================================//
class MOAICmdLuaInterfaceMgr :
	public virtual ZLContextClass {
public:

	int		mRefID;

public:

	//----------------------------------------------------------------//
	MOAICmdLuaInterfaceMgr ( ZLContext& context ) :
		ZLHasContext ( context ),
		ZLContextClass ( context ),
		mRefID ( LUA_NOREF ) {
	}
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractCmdMedium::_getInterface ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( state.IsType ( 1, LUA_TUSERDATA )) {
		MOAIAbstractCmdMedium* self = ( MOAIAbstractCmdMedium* )state.GetPtrUserData ( 1 );
		return self->PushCmdInterfaceWithHandler ( state, 2 );
	}
	return 0;
}

//================================================================//
// MOAIAbstractCmdMedium
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractCmdMedium::MOAIAbstractCmdMedium () :
	mAdapter ( NULL ) {
}

//----------------------------------------------------------------//
MOAIAbstractCmdMedium::~MOAIAbstractCmdMedium () {
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::RetainObject ( MOAILuaObject* object ) {

	this->MOAIAbstractCmdMedium_RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::PushCmdInterface ( MOAILuaState& state ) {

	assert ( this->mAdapter );
	MOAILuaStrongRef& luaInterface = this->mAdapter->mLuaInterface;

	if ( luaInterface ) {
		state.Push ( luaInterface );
	}
	else {
		state.PushPtrUserData ( this );
		luaInterface.SetRef ( state, -1 );
	}
	this->PushLuaCmdAPI ( state );
	lua_setmetatable ( state, -2 );
}

//----------------------------------------------------------------//
int MOAIAbstractCmdMedium::PushCmdInterfaceWithHandler ( MOAILuaState& state, int idx ) {

	MOAIAbstractCmdHandler* handler = NULL;

	if ( lua_istable ( state, idx )) {
		handler = state.Get < MOAILuaRuntime >().GetSingletonForClass < MOAIAbstractCmdHandler >( state, idx );
	}
	else {
		handler = state.GetLuaObject < MOAIAbstractCmdHandler >( 2, false );
		if ( handler ) {
			this->RetainObject ( handler );
		}
	}
	
	if ( handler ) {
		this->PushCmdInterfaceWithHandler ( state, *handler );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::PushCmdInterfaceWithHandler ( MOAILuaState& state, MOAIAbstractCmdHandler& handler ) {

	this->SetHandler ( handler );
	this->PushCmdInterface ( state );
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::PushLuaCmdAPI ( MOAILuaState& state ) {

	assert ( this->mAdapter );
	MOAICmdMediumAdapter& adapter = *this->mAdapter;

	ZLType::RAW_ID rawAPITypeID = ZLType::ToRaw ( adapter.mAPITypeID );

	MOAICmdLuaInterfaceMgr& mgr = state.GetContext ().Affirm < MOAICmdLuaInterfaceMgr >();

	// affirm the table of interfaces
	if ( mgr.mRefID == LUA_NOREF ) {
		lua_newtable ( state );
		mgr.mRefID = luaL_ref ( state, LUA_REGISTRYINDEX );
	}

	// get the interface
	lua_rawgeti ( state, LUA_REGISTRYINDEX, mgr.mRefID ); // table of interfaces
	
	lua_pushinteger ( state, rawAPITypeID );
	lua_rawget ( state, -2 );
	
	// if we have the interface, replace the registry and return
	if ( lua_istable ( state, -1 )) {
		lua_replace ( state, -2 );
		return;
	}
	
	// pop off the nil value
	lua_pop ( state, 1 );
	
	// create and initialize the interface table
	lua_newtable ( state );
	
	luaL_Reg regTable [] = {
		{ "_",							_getInterface },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
	adapter.mAPI->MOAIAbstractCmdAPI_RegisterLuaAPI ( state );
	this->MOAIAbstractCmdMedium_ExtendLuaAPI ( state );
	
	// interface is its own metatable
	lua_pushstring ( state, "__index" );
	lua_pushvalue ( state, -2 );
	lua_rawset ( state, -3 );
	
	// store it for future use
	lua_pushinteger ( state, rawAPITypeID );
	lua_pushvalue ( state, -2 );
	lua_rawset ( state, -4 );
	
	// replace the registry and we're done
	lua_replace ( state, -2 );
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::SetHandler ( MOAIAbstractCmdHandler& handler ) {

	assert ( this->mAdapter );
	handler.InitializePolymorphic ( this->mAdapter->GetPolymorphic ());
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {

	this->MOAIAbstractCmdMedium_SubmitCommand ( cmd, param, paramSize );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::MOAIAbstractCmdMedium_ExtendLuaAPI ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::MOAIAbstractCmdMedium_RetainObject ( MOAILuaObject* object ) {

	assert ( this->mAdapter );
	this->mAdapter->RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::MOAIAbstractCmdMedium_SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {

	assert ( this->mAdapter );
	this->mAdapter->SubmitCommand ( cmd, param, paramSize );
}
