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
		MOAIObject* data = new TYPE ();
		
		// make sure a suitable table is at the top of the stack
		if ( state.IsType ( 1, LUA_TTABLE )) {
			lua_pushvalue ( state, 1 ); // copy the instance table (or nil) to the top of the stack
		}
		else {
			lua_newtable ( state ); // push an empty instance table
		}
		
		data->BindToLuaWithTable ( state );
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
		return *USGlobalsMgr::Get ()->AffirmGlobal < MOAILuaFactoryClass >();
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
	void RegisterLuaClass ( MOAILuaState& state ) {
		UNUSED ( state );
	}

public:
	
	//----------------------------------------------------------------//
	static MOAILuaSingletonClass& Get () {
		return *USGlobalsMgr::Get ()->AffirmGlobal < MOAILuaSingletonClass >();
	}

	//----------------------------------------------------------------//
	MOAIObject* GetSingleton () {
		return USGlobalsMgr::Get ()->AffirmGlobal < TYPE >();
	}

	//----------------------------------------------------------------//
	void Register () {

		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		
		MOAIObject* type = this->GetSingleton ();
		this->InitLuaSingletonClass ( *type, state );
	}
};

#endif
