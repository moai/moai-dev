// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUACLASS_H
#define	MOAILUACLASS_H

#include <moai-core/MOAILuaObject.h>

class MOAILuaState;

//================================================================//
// MOAILuaClass
//================================================================//
class MOAILuaClass :
	public virtual ZLContextClass {
protected:

	friend MOAILuaRuntime;

	bool				mIsSingleton;
	
	MOAILuaStrongRef	mClassTable;			// global factory class for type
	MOAILuaStrongRef	mInterfaceTable;		// interface shared by all instances of type

	//----------------------------------------------------------------//
	static int			_extendFactory				( lua_State* L );
	static int			_extendSingleton			( lua_State* L );
	static int			_get						( lua_State* L );
	static int			_getInterfaceTable			( lua_State* L );
	static int			_getUpvalue					( lua_State* L );
	static int			_new						( lua_State* L );

	//----------------------------------------------------------------//
	void				InitLuaFactoryClass					( MOAILuaObject& data, MOAILuaState& state );
	void				InitLuaSingletonClass				( MOAILuaObject& data, MOAILuaState& state );
	void				PushInterfaceTable					( MOAILuaState& state );
	
	//----------------------------------------------------------------//
	virtual void		MOAILuaClass_RegisterLuaClass		( MOAILuaState& state ) = 0;

public:

	friend class MOAILuaObject;

	//----------------------------------------------------------------//
	virtual MOAILuaObject*	GetSingleton			();
	bool					IsSingleton				();
	virtual void			Register				( MOAILuaObject* instance ) = 0;
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
	void							MOAILuaClass_RegisterLuaClass		( MOAILuaState& state );
	
public:
	
	//----------------------------------------------------------------//
//	static MOAILuaFactoryClass&		Get						();
									MOAILuaFactoryClass		();
	void							Register				( MOAILuaObject* instance );
};

//================================================================//
// MOAILuaSingletonClass
//================================================================//
template < typename TYPE >
class MOAILuaSingletonClass :
	public MOAILuaClass {
private:
	
	//----------------------------------------------------------------//
	static int						_get					( lua_State* L );
	static int						_getClassName			( lua_State* L );
	static int						_getTypeID				( lua_State* L );
	
	//----------------------------------------------------------------//
	void							MOAILuaClass_RegisterLuaClass		( MOAILuaState& state );
	
public:
	
	//----------------------------------------------------------------//
//	static MOAILuaSingletonClass&	Get						();
	MOAILuaObject*					GetSingleton			();
									MOAILuaSingletonClass	();
	void							Register				( MOAILuaObject* instance );
};

#endif
