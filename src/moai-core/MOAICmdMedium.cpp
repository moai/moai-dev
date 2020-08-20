// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractCmdAPI.h>
#include <moai-core/MOAIAbstractCmdStream.h>
#include <moai-core/MOAICmdMedium.h>

//================================================================//
// MOAICmdLuaInterfaceMgr
//================================================================//
class MOAICmdLuaInterfaceMgr :
	public ZLContextClass < MOAICmdLuaInterfaceMgr > {
public:

	int		mRefID;

public:

	//----------------------------------------------------------------//
	MOAICmdLuaInterfaceMgr () :
		mRefID ( LUA_NOREF ) {
	}
};

//================================================================//
// MOAIAbstractCmdMedium
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractCmdMedium::MOAIAbstractCmdMedium () :
	mInternal ( NULL ) {
}

//----------------------------------------------------------------//
MOAIAbstractCmdMedium::MOAIAbstractCmdMedium ( MOAIAbstractCmdMediumInternal& internal ) :
	mInternal ( &internal ) {
}

//----------------------------------------------------------------//
MOAIAbstractCmdMedium::~MOAIAbstractCmdMedium () {
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::RetainObject ( MOAILuaObject* object ) {

	this->MOAIAbstractCmdMedium_RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::SetHandler ( MOAIAbstractCmdHandler& handler ) {

	handler.InitializeMedium ( *this );
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {

	this->MOAIAbstractCmdMedium_SubmitCommand ( cmd, param, paramSize );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::MOAIAbstractCmdMedium_RetainObject ( MOAILuaObject* object ) {

	assert ( this->mInternal );
	this->mInternal->mStream.RetainObject ( object );
}

//----------------------------------------------------------------//
void MOAIAbstractCmdMedium::MOAIAbstractCmdMedium_SubmitCommand ( u32 cmd, const void* param, ZLSize paramSize ) {

	assert ( this->mInternal );
	this->mInternal->mStream.SubmitCommand ( *this->mInternal->mHandler, cmd, param, paramSize );
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAICmdMedium::_getInterface ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( state.IsType ( 1, LUA_TUSERDATA )) {
		MOAICmdMedium* self = ( MOAICmdMedium* )state.GetPtrUserData ( 1 );
		return self->PushWithHandler ( state, 2 );
	}
	return 0;
}

//================================================================//
// MOAICmdMedium
//================================================================//

//----------------------------------------------------------------//
MOAICmdMedium::MOAICmdMedium ( MOAIAbstractCmdStream& stream ) :
	MOAIAbstractCmdMediumInternal ( stream ),
	MOAIAbstractCmdMedium ( *( MOAIAbstractCmdMediumInternal* )this ) {
}

//----------------------------------------------------------------//
MOAICmdMedium::~MOAICmdMedium () {
}

//----------------------------------------------------------------//
void MOAICmdMedium::PushInterface ( MOAILuaState& state ) {

	assert ( this->mInternal );
	assert ( this->mInternal->mAPI );

	MOAILuaStrongRef& luaInterface = this->mInternal->mLuaInterface;

	if ( luaInterface ) {
		state.Push ( luaInterface );
	}
	else {
		state.PushPtrUserData ( this );
		luaInterface.SetRef ( state, -1 );
	}
	this->PushLuaAPI ( state );
	lua_setmetatable ( state, -2 );
}

//----------------------------------------------------------------//
void MOAICmdMedium::PushLuaAPI ( MOAILuaState& state ) {

	ZLType::RAW_ID rawAPITypeID = ZLType::ToRaw ( this->mInternal->mAPITypeID );

	MOAICmdLuaInterfaceMgr& mgr = MOAICmdLuaInterfaceMgr::Affirm ();

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
	this->mInternal->mAPI->MOAIAbstractCmdAPI_RegisterLuaAPI ( state );
	this->MOAICmdMedium_ExtendLuaAPI ( state );
	
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
int MOAICmdMedium::PushWithHandler ( MOAILuaState& state, int idx ) {

	MOAIAbstractCmdHandler* handler = NULL;

	if ( lua_istable ( state, idx )) {
		handler = MOAILuaRuntime::Get ().GetSingletonForClass < MOAIAbstractCmdHandler >( state, idx );
	}
	else {
		handler = state.GetLuaObject < MOAIAbstractCmdHandler >( 2, false );
		if ( handler ) {
			this->RetainObject ( handler );
		}
	}
	
	if ( handler ) {
		this->PushWithHandler ( state, *handler );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
void MOAICmdMedium::PushWithHandler ( MOAILuaState& state, MOAIAbstractCmdHandler& handler ) {

	this->SetHandler ( handler );
	this->PushInterface ( state );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void* MOAICmdMedium::MOAIAbstractCmdMedium_GetConcrete ( ZLTypeID apiTypeID ) {

	return ( this->mAPITypeID == apiTypeID ) ? ( void* )this->mBuffer.GetBuffer () : NULL;
}

//----------------------------------------------------------------//
void MOAICmdMedium::MOAICmdMedium_ExtendLuaAPI ( MOAILuaState& state ) {
	UNUSED ( state );
}


