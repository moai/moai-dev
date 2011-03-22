// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	LUADATA_IMPL_H
#define	LUADATA_IMPL_H

/** @addtogroup LuaLib */

#include <uslscore/USGlobals.h>

#define DECL_LUA_DATA(type) \
	USLuaDataType* GetLuaDataType () { return &USLuaDataTypeImp < type >::Get (); } \
	static void RegisterLuaType () { USLuaDataTypeImp < type >::Get ().Register (); } \
	cc8* TypeName () const { return #type; }

#define DECL_LUA_SINGLETON(type) \
	USLuaDataType* GetLuaDataType () { return &USLuaSingletonTypeImp < type >::Get (); } \
	static void RegisterLuaType () { USLuaSingletonTypeImp < type >::Get ().Register (); } \
	cc8* TypeName () const { return #type; }

#define REGISTER_LUA_TYPE(type) \
	type::RegisterLuaType ();

//================================================================//
// USLuaDataTypeImp
//================================================================//
/**	@brief Convenience template class for implementing Lua data types. Spares
	the class author the onerous chore of implementing _new ().
*/
template < typename TYPE >
class USLuaDataTypeImp :
	public USLuaDataType {
private:

	//----------------------------------------------------------------//
	static int _new ( lua_State* L ) {

		USLuaState state ( L );
		USLuaData* data = new TYPE ();
		
		data->SetLuaInstance ( state, 1 );
		data->PushLuaInstance ( state );

		return 1;
	}

	//----------------------------------------------------------------//
	void RegisterLuaClass ( USLuaState& state ) {
		
		LuaReg regTable [] = {
			{ "new",					_new },
			{ NULL, NULL }
		};

		luaL_register ( state, 0, regTable );
	}

public:

	//----------------------------------------------------------------//
	static USLuaDataTypeImp& Get () {
		return *USGlobals::Get ()->GetGlobal < USLuaDataTypeImp >();
	}

	//----------------------------------------------------------------//
	void Register () {
		
		USLuaStateHandle state = USLuaRuntime::Get ().State ();

		TYPE type;
		this->InitLuaData ( type, state );
	}
};

//================================================================//
// USLuaSingletonTypeImp
//================================================================//
/**	@brief Convenience template class for implementing Lua data types for. 
	singletons. Spares the class author the onerous chore of overloading GetInstance ().
*/
template < typename TYPE >
class USLuaSingletonTypeImp :
	public USLuaDataType {
private:

	//----------------------------------------------------------------//
	static int _get ( lua_State* L ) {
		
		USLuaState state ( L );
		
		USLuaData* object = USGlobals::Get ()->GetGlobal < TYPE >();
		object->PushLuaInstance ( state );

		return 1;
	}

	//----------------------------------------------------------------//
	void RegisterLuaClass ( USLuaState& state ) {
		
		LuaReg regTable [] = {
			{ "get",					_get },
			{ NULL, NULL }
		};

		luaL_register ( state, 0, regTable );
	}

public:
	
	//----------------------------------------------------------------//
	static USLuaSingletonTypeImp& Get () {
		return *USGlobals::Get ()->GetGlobal < USLuaSingletonTypeImp >();
	}

	//----------------------------------------------------------------//
	USLuaData* GetSingleton () {
		return USGlobals::Get ()->GetGlobal < TYPE >();
	}

	//----------------------------------------------------------------//
	void Register () {

		USLuaStateHandle state = USLuaRuntime::Get ().State ();
		
		USLuaData* type = this->GetSingleton ();
		this->InitLuaSingleton ( *type, state );
	}
};

#endif
