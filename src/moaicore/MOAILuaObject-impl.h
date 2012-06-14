// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAOBJECT_IMPL_H
#define	MOAILUAOBJECT_IMPL_H

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

	//----------------------------------------------------------------//
	void RegisterLuaClass ( MOAILuaState& state ) {
		
		luaL_Reg regTable [] = {
			{ "getClassName",			_getClassName },
			{ "new",					_new },
			{ NULL, NULL }
		};

		luaL_register ( state, 0, regTable );
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
