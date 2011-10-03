// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USLUAOBJECT_IMPL_H
#define	USLUAOBJECT_IMPL_H

#include <uslscore/USGlobals.h>

#define DECL_LUA_FACTORY(type) \
	USLuaClass* GetLuaClass () { return &USLuaFactoryClass < type >::Get (); } \
	static void RegisterLuaType () { USLuaFactoryClass < type >::Get ().Register (); } \
	cc8* TypeName () const { return #type; }

#define DECL_LUA_SINGLETON(type) \
	USLuaClass* GetLuaClass () { return &USLuaSingletonClass < type >::Get (); } \
	static void RegisterLuaType () { USLuaSingletonClass < type >::Get ().Register (); } \
	cc8* TypeName () const { return #type; }

#define REGISTER_LUA_CLASS(type) \
	type::RegisterLuaType ();

//================================================================//
// USLuaFactoryClass
//================================================================//
template < typename TYPE >
class USLuaFactoryClass :
	public USLuaClass {
private:

	//----------------------------------------------------------------//
	static int _getClassName ( lua_State* L ) {

		TYPE object;
		
		lua_pushstring ( L, object.TypeName ());
		return 1;
	}

	//----------------------------------------------------------------//
	static int _new ( lua_State* L ) {

		USLuaState state ( L );
		USLuaObject* data = new TYPE ();
		
		// make sure a suitable table is at the top of the stack
		if ( state.IsType ( 1, LUA_TTABLE )) {
			lua_pushvalue ( state, 1 ); // copy the instance table (or nil) to the top of the stack
		}
		else {
			lua_newtable ( state ); // push an empty instance table
		}
		
		data->BindToLuaWithTable ( state );
		data->PushLuaUserdata ( state );
		USLuaRuntime::Get ().SetObjectStackTrace ( data );

		return 1;
	}

	//----------------------------------------------------------------//
	void RegisterLuaClass ( USLuaState& state ) {
		
		luaL_Reg regTable [] = {
			{ "getClassName",			_getClassName },
			{ "new",					_new },
			{ NULL, NULL }
		};

		luaL_register ( state, 0, regTable );
	}

public:

	//----------------------------------------------------------------//
	static USLuaFactoryClass& Get () {
		return *USGlobalsMgr::Get ()->AffirmGlobal < USLuaFactoryClass >();
	}

	//----------------------------------------------------------------//
	void Register () {
		
		USLuaStateHandle state = USLuaRuntime::Get ().State ();

		TYPE type;
		this->InitLuaFactoryClass ( type, state );
	}
};

//================================================================//
// USLuaSingletonClass
//================================================================//
template < typename TYPE >
class USLuaSingletonClass :
	public USLuaClass {
private:

	//----------------------------------------------------------------//
	void RegisterLuaClass ( USLuaState& state ) {
		UNUSED ( state );
	}

public:
	
	//----------------------------------------------------------------//
	static USLuaSingletonClass& Get () {
		return *USGlobalsMgr::Get ()->AffirmGlobal < USLuaSingletonClass >();
	}

	//----------------------------------------------------------------//
	USLuaObject* GetSingleton () {
		return USGlobalsMgr::Get ()->AffirmGlobal < TYPE >();
	}

	//----------------------------------------------------------------//
	void Register () {

		USLuaStateHandle state = USLuaRuntime::Get ().State ();
		
		USLuaObject* type = this->GetSingleton ();
		this->InitLuaSingletonClass ( *type, state );
	}
};

#endif
