// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUACLASS_IMPL_H
#define	MOAILUACLASS_IMPL_H

#include <moai-core/MOAIGlobals.h>
#include <moai-core/MOAILuaClass.h>
#include <moai-core/MOAILuaRuntime.h>

//================================================================//
// MOAILuaFactoryClass
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
int MOAILuaFactoryClass < TYPE >::_new ( lua_State* L ) {
	
	MOAILuaState state ( L );
	MOAILuaObject* data = new TYPE ();
	data->PushLuaUserdata ( state );
	return 1;
}
	
//----------------------------------------------------------------//
template < typename TYPE >
MOAILuaFactoryClass < TYPE >& MOAILuaFactoryClass < TYPE >::Get () {
	MOAILuaFactoryClass < TYPE >* typeClass = MOAIGlobalsMgr::Get ()->GetGlobal < MOAILuaFactoryClass >();
	if ( !typeClass ) {
		typeClass = MOAIGlobalsMgr::Get ()->AffirmGlobal < MOAILuaFactoryClass >();
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		TYPE type;
		typeClass->InitLuaFactoryClass ( type, state );
	}
	assert ( typeClass );
	return *typeClass;
}

//----------------------------------------------------------------//
template < typename TYPE >
MOAILuaFactoryClass < TYPE >::MOAILuaFactoryClass () {
	this->mIsSingleton = false;
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaFactoryClass < TYPE >::Register () {
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaFactoryClass < TYPE >::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getClassName",			_getClassName },
		{ "new",					_new },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//================================================================//
// MOAILuaSingletonClass
//================================================================//
	
//----------------------------------------------------------------//
template < typename TYPE >
int MOAILuaSingletonClass < TYPE >::_getClassName ( lua_State* L ) {
	
	MOAILuaState state ( L );
	MOAILuaObject* singleton = MOAIGlobalsMgr::Get ()->GetGlobal < TYPE >();
	state.Push ( singleton->TypeName ());
	return 1;
}
	
//----------------------------------------------------------------//
template < typename TYPE >
MOAILuaSingletonClass < TYPE >& MOAILuaSingletonClass < TYPE >::Get () {
	return *MOAIGlobalsMgr::Get ()->AffirmGlobal < MOAILuaSingletonClass >();
}

//----------------------------------------------------------------//
template < typename TYPE >
MOAILuaObject* MOAILuaSingletonClass < TYPE >::GetSingleton () {
	return MOAIGlobalsMgr::Get ()->AffirmGlobal < TYPE >();
}

//----------------------------------------------------------------//
template < typename TYPE >
MOAILuaSingletonClass < TYPE >::MOAILuaSingletonClass () {
	this->mIsSingleton = true;
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaSingletonClass < TYPE >::Register () {
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	MOAILuaObject* type = this->GetSingleton ();
	this->InitLuaSingletonClass ( *type, state );
}

//----------------------------------------------------------------//
template < typename TYPE >
void MOAILuaSingletonClass < TYPE >::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getClassName",		_getClassName },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
