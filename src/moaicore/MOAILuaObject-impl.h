// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAOBJECT_IMPL_H
#define	MOAILUAOBJECT_IMPL_H

#include <luaconf.h>

#define DECL_LUA_FACTORY(type) \
	MOAILuaClass* GetLuaClass () { return &MOAILuaFactoryClass < type >::Get (); } \
	static void RegisterLuaType () { MOAILuaFactoryClass < type >::Get ().Register (); } \
	cc8* TypeName () const { return #type; }

#define DECL_LUA_SINGLETON(type) \
	MOAILuaClass* GetLuaClass () { return &MOAILuaSingletonClass < type >::Get (); } \
	static void RegisterLuaType () { MOAILuaSingletonClass < type >::Get ().Register (); } \
	cc8* TypeName () const { return #type; }

#define REGISTER_LUA_CLASS(type) \
	type::RegisterLuaType ();

//================================================================//
// MOAILuaFactoryClass
//================================================================//
template < typename TYPE >
class MOAILuaFactoryClass :
	public MOAILuaClass {
private:

	//----------------------------------------------------------------//
	static int _getClassName ( lua_State* L ) {

		TYPE object;
		lua_pushstring ( L, object.TypeName ());
		return 1;
	}

	//----------------------------------------------------------------//
	static int _new ( lua_State* L ) {

		MOAILuaState state ( L );
		MOAILuaObject* data = new TYPE ();
		
		data->BindToLua ( state );
		MOAILuaRuntime::Get ().SetObjectStackTrace ( data );

		return 1;
	}

#if !defined(LUA_PURE)
	//----------------------------------------------------------------//
	static int _prenew ( lua_State* L ) {
		
		// Get the arguments out of the metatable.
		lua_getmetatable ( L, 1 );
		lua_getfield ( L, -1, "args" );
		int args = lua_gettop ( L );

		// Load the _new C function and call it.
		lua_pushcfunction ( L, _new );
		int count = 0;
		for (size_t i = 1; i < lua_objlen ( L, args ); i++)
		{
			lua_rawgeti ( L, args, (int)i );
			count++;
		}
		lua_call ( L, count, 1 );
		return 1;
	}

	//----------------------------------------------------------------//
	static int _call ( lua_State* L ) {
		
		int argcount = lua_gettop ( L ) - 1;

		// Push a table that uses the __new metamethod to offer
		// a constructable preinstance.
		lua_newtable ( L );
		int tbl = lua_gettop ( L );
		lua_newtable ( L );
		int meta = lua_gettop ( L );
		lua_pushcfunction ( L, _prenew );
		lua_setfield ( L, meta, "__new" );

		// Push all arguments into a table and set in the
		// metatable.
		lua_newtable ( L );
		int args = lua_gettop ( L );
		lua_pushnil ( L ); // "self" argument to new (will this cause problems?)
		lua_rawseti ( L, args, 1 );
		for (int i = 2; i < argcount + 2; i++)
		{
			lua_pushvalue ( L, i );
			lua_rawseti ( L, args, i );
		}
		lua_setfield ( L , meta, "args" );

		// Set the metatable.
		lua_setmetatable ( L, tbl );

		return 1;
	}
#endif

	//----------------------------------------------------------------//
	void RegisterLuaClass ( MOAILuaState& state ) {

		TYPE object;
		
		luaL_Reg regTable [] = {
			{ "getClassName",			_getClassName },
			{ "new",					_new },
			{ NULL, NULL }
		};

		luaL_register ( state, 0, regTable );

#if !defined(LUA_PURE)
		// Set __type and __call metamethods.
		lua_newtable ( state );
		int meta = lua_gettop ( state );
		lua_pushstring ( state, object.TypeName ());
		lua_setfield ( state, meta, "__type" );
		lua_pushcfunction ( state, _call );
		lua_setfield ( state, meta, "__call" );
		lua_setmetatable ( state, -2 );
#endif
	}

public:

	//----------------------------------------------------------------//
	static MOAILuaFactoryClass& Get () {
		return *MOAIGlobalsMgr::Get ()->AffirmGlobal < MOAILuaFactoryClass >();
	}

	//----------------------------------------------------------------//
	void Register () {
		
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

		TYPE type;
		this->InitLuaFactoryClass ( type, state );
	}
};

//================================================================//
// MOAILuaSingletonClass
//================================================================//
template < typename TYPE >
class MOAILuaSingletonClass :
	public MOAILuaClass {
private:

	//----------------------------------------------------------------//
	static int _getClassName ( lua_State* L ) {
		
		MOAILuaState state ( L );
		MOAILuaObject* singleton = MOAIGlobalsMgr::Get ()->GetGlobal < TYPE >();
		state.Push ( singleton->TypeName ());
		return 1;
	}

	//----------------------------------------------------------------//
	void RegisterLuaClass ( MOAILuaState& state ) {
		
		luaL_Reg regTable [] = {
			{ "getClassName",		_getClassName },
			{ NULL, NULL }
		};

		luaL_register ( state, 0, regTable );
	}

public:
	
	//----------------------------------------------------------------//
	static MOAILuaSingletonClass& Get () {
		return *MOAIGlobalsMgr::Get ()->AffirmGlobal < MOAILuaSingletonClass >();
	}

	//----------------------------------------------------------------//
	MOAILuaObject* GetSingleton () {
		return MOAIGlobalsMgr::Get ()->AffirmGlobal < TYPE >();
	}

	//----------------------------------------------------------------//
	void Register () {

		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		
		MOAILuaObject* type = this->GetSingleton ();
		this->InitLuaSingletonClass ( *type, state );
	}
};

#endif
