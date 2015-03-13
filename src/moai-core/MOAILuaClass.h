// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUACLASS_H
#define	MOAILUACLASS_H

#include <moai-core/MOAIGlobals.h>
#include <moai-core/MOAILuaObject.h>

class MOAILuaState;

#define DECL_LUA_ABSTRACT(type)									\
MOAILuaClass* GetLuaClass () { return 0; }						\
cc8* TypeName () const { return #type; }

#define DECL_LUA_FACTORY(type) \
MOAILuaClass* GetLuaClass () { return &MOAILuaFactoryClass < type >::Get (); }			\
static void RegisterLuaType () { MOAILuaFactoryClass < type >::Get ().Register (); }	\
cc8* TypeName () const { return #type; }

#define DECL_LUA_SINGLETON(type) \
MOAILuaClass* GetLuaClass () { return &MOAILuaSingletonClass < type >::Get (); }		\
static void RegisterLuaType () { MOAILuaSingletonClass < type >::Get ().Register (); }	\
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
	static int						_getClassName			( lua_State* L );
	static int						_getTypeID				( lua_State* L );
	static int						_new					( lua_State* L );
	
	//----------------------------------------------------------------//
	void							RegisterLuaClass		( MOAILuaState& state );
	
public:
	
	//----------------------------------------------------------------//
	static MOAILuaFactoryClass&		Get						();
									MOAILuaFactoryClass		();
	void							Register				();
};

//================================================================//
// MOAILuaSingletonClass
//================================================================//
template < typename TYPE >
class MOAILuaSingletonClass :
public MOAILuaClass {
private:
	
	//----------------------------------------------------------------//
	static int						_getClassName			( lua_State* L );
	static int						_getTypeID				( lua_State* L );
	
	//----------------------------------------------------------------//
	void							RegisterLuaClass		( MOAILuaState& state );

	
public:
	
	//----------------------------------------------------------------//
	static MOAILuaSingletonClass&	Get						();
	MOAILuaObject*					GetSingleton			();
									MOAILuaSingletonClass	();
	void							Register				();

};

#endif
