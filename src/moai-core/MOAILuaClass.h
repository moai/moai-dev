// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUACLASS_H
#define	MOAILUACLASS_H

#include <moai-core/MOAIGlobals.h>
#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAILuaObject.h>

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
// MOAILuaClass
//================================================================//
class MOAILuaClass :
	public MOAIGlobalClassBase {
protected:

	bool				mIsSingleton;
	
	MOAILuaStrongRef	mClassTable;			// global factory class for type
	MOAILuaStrongRef	mInterfaceTable;		// interface shared by all instances of type
	MOAILuaStrongRef	mRefTable;				// strong ref to member table for singletons

	//----------------------------------------------------------------//
	static int			_extendFactory				( lua_State* L );
	static int			_extendSingleton			( lua_State* L );
	static int			_get						( lua_State* L );
	static int			_getInterfaceTable			( lua_State* L );
	static int			_getUpvalue					( lua_State* L );
	static int			_new						( lua_State* L );

	//----------------------------------------------------------------//
	void				InitLuaFactoryClass			( MOAILuaObject& data, MOAILuaState& state );
	void				InitLuaSingletonClass		( MOAILuaObject& data, MOAILuaState& state );
	void				PushInterfaceTable			( MOAILuaState& state );
	void				PushRefTable				( MOAILuaState& state );
	virtual void		RegisterLuaClass			( MOAILuaState& state ) = 0;

public:

	friend class MOAILuaObject;

	//----------------------------------------------------------------//
	virtual MOAILuaObject*	GetSingleton			();
	bool					IsSingleton				();
	virtual void			Register				() = 0;
							MOAILuaClass			();
	virtual					~MOAILuaClass			();
};

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
		data->PushLuaUserdata ( state );
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
		MOAILuaFactoryClass* typeClass = MOAIGlobalsMgr::Get ()->GetGlobal < MOAILuaFactoryClass >();
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
	MOAILuaFactoryClass () {
		this->mIsSingleton = false;
	}

	//----------------------------------------------------------------//
	void Register () {
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
	MOAILuaSingletonClass () {
		this->mIsSingleton = true;
	}

	//----------------------------------------------------------------//
	void Register () {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		MOAILuaObject* type = this->GetSingleton ();
		this->InitLuaSingletonClass ( *type, state );
	}
};

#endif
