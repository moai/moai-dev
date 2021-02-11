// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUACLASS_IMPL_H
#define	MOAILUACLASS_IMPL_H

#include <moai-core/MOAILuaClass.h>
#include <moai-core/MOAILuaRuntime.h>

//================================================================//
// lua
//================================================================//
	
//----------------------------------------------------------------//
template < typename TYPE >
int MOAILuaFactoryClass < TYPE >::_getClassName ( lua_State* L ) {
	
	TYPE object;
	lua_pushstring ( L, object.TypeName ());
	return 1;
}

//----------------------------------------------------------------//
template < typename TYPE >
int MOAILuaFactoryClass < TYPE >::_getTypeID ( lua_State* L ) {
	
	MOAILuaState state ( L );
	state.Push ( ZLType::GetID < TYPE >());
	return 1;
}

//----------------------------------------------------------------//
template < typename TYPE >
int MOAILuaFactoryClass < TYPE >::_new ( lua_State* L ) {
	
	MOAILuaState state ( L );
	MOAILuaObject* data = new TYPE ();
	data->PushLuaUserdata ( state );
	
	// since we're creating this just to hand to Lua, we can safely
	// remove it from the runtime's global cache
	MOAILuaRuntime::Get ().PurgeUserdata ( state, -1 );
	
	return 1;
}

//================================================================//
// MOAILuaFactoryClass
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
MOAILuaFactoryClass < TYPE >& MOAILuaFactoryClass < TYPE >::Get () {
	return *ZLContextMgr::Get ()->AffirmGlobal < MOAILuaFactoryClass < TYPE > >();
}

//----------------------------------------------------------------//
template < typename TYPE >
MOAILuaFactoryClass < TYPE >::MOAILuaFactoryClass () {
	this->mIsSingleton = false;
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaFactoryClass < TYPE >::Register ( MOAILuaObject* instance ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	if ( instance ) {
		this->InitLuaFactoryClass ( *instance, state );
	}
	else {
		TYPE type;
		this->InitLuaFactoryClass ( type, state );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaFactoryClass < TYPE >::MOAILuaClass_RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getClassName",			_getClassName },
		{ "getTypeID",				_getTypeID },
		{ "new",					_new },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
	
	state.SetField ( -1, "typeID", ZLType::GetID < TYPE >());
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
int MOAILuaSingletonClass < TYPE >::_get ( lua_State* L ) {
	
	MOAILuaState state ( L );
	MOAILuaObject* singleton = ZLContextMgr::Get ()->GetGlobal < TYPE >();
	state.Push ( singleton );
	return 1;
}

//----------------------------------------------------------------//
template < typename TYPE >
int MOAILuaSingletonClass < TYPE >::_getClassName ( lua_State* L ) {
	
	MOAILuaState state ( L );
	MOAILuaObject* singleton = ZLContextMgr::Get ()->GetGlobal < TYPE >();
	state.Push ( singleton->TypeName ());
	return 1;
}

//----------------------------------------------------------------//
template < typename TYPE >
int MOAILuaSingletonClass < TYPE >::_getTypeID ( lua_State* L ) {
	
	MOAILuaState state ( L );
	state.Push ( ZLType::GetID < TYPE >());
	return 1;
}

//================================================================//
// MOAILuaSingletonClass
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
MOAILuaSingletonClass < TYPE >& MOAILuaSingletonClass < TYPE >::Get () {
	return *ZLContextMgr::Get ()->AffirmGlobal < MOAILuaSingletonClass < TYPE > >();
}

//----------------------------------------------------------------//
template < typename TYPE >
MOAILuaObject* MOAILuaSingletonClass < TYPE >::GetSingleton () {
	return ZLContextMgr::Get ()->GetGlobal < TYPE >();
}

//----------------------------------------------------------------//
template < typename TYPE >
MOAILuaSingletonClass < TYPE >::MOAILuaSingletonClass () {
	this->mIsSingleton = true;
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaSingletonClass < TYPE >::Register ( MOAILuaObject* instance ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	instance = instance ? instance : this->GetSingleton ();
	assert ( instance );
	this->InitLuaSingletonClass ( *instance, state );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaSingletonClass < TYPE >::MOAILuaClass_RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "get",				_get },
		{ "getClassName",		_getClassName },
		{ "getTypeID",			_getTypeID },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
