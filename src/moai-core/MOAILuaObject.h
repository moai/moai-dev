// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAOBJECT_H
#define	MOAILUAOBJECT_H

#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAIObject.h>

class MOAIDeserializer;
class MOAILuaClass;
class MOAILuaState;
class MOAIScopedLuaState;
class MOAISerializer;

//================================================================//
// MOAILuaObject
//================================================================//
class MOAILuaObject :
	public virtual MOAIObject {
private:

	MOAILuaMemberRef	mContain;
	
protected:

	MOAILuaWeakRef		mMemberTable;		// ref to member table (weak for factory class instances; strong for singletons)
	MOAILuaWeakRef		mUserdata;			// ref to userdata (weak)
	
	//----------------------------------------------------------------//
	static int				_gc					( lua_State* L );
	static int				_getClass			( lua_State* L );
	static int				_getClassName		( lua_State* L );
	static int				_index				( lua_State* L );
	static int				_newindex			( lua_State* L );
	static int				_setInterface		( lua_State* L );
	static int				_tombstone			( lua_State* L );
	static int				_tostring			( lua_State* L );

	//----------------------------------------------------------------//
	void					OnRelease			( u32 refCount );
	void					OnRetain			( u32 refCount );
	bool					PushMemberTable		( MOAILuaState& state );
	bool					PushRefTable		( MOAILuaState& state );
	void					SetInterfaceTable	( MOAILuaState& state, int idx );
	void					SetMemberTable		( MOAILuaState& state, int idx );

public:

	friend class MOAILuaClass;
	friend class MOAILuaMemberRef;
	friend class MOAIDeserializer;
	friend class MOAISerializer;

	//----------------------------------------------------------------//
	void					BindToLua					( MOAILuaState& state );
	virtual MOAILuaClass*	GetLuaClass					();
	cc8*					GetLuaClassName				();
	MOAIScopedLuaState		GetSelf						();
	void					GetRef						( MOAILuaRef& ref );
	bool					IsBound						();
	bool					IsSingleton					();
	void					LuaRelease					( MOAILuaObject* object );
	void					LuaRetain					( MOAILuaObject* object );
	void					LuaUnbind					();
	void					PushLuaClassTable			( MOAILuaState& state );
	void					PushLuaUserdata				( MOAILuaState& state );
	virtual void			RegisterLuaClass			( MOAILuaState& state );
	virtual void			RegisterLuaFuncs			( MOAILuaState& state );
	static void             ReportLeaks					( FILE *f, bool clearAfter );
	virtual	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	virtual	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
							MOAILuaObject				();
	virtual					~MOAILuaObject				();
};

//================================================================//
// MOAILuaClass
//================================================================//
class MOAILuaClass :
	public MOAIObject {
protected:

	MOAILuaStrongRef	mClassTable;				// global factory class for type
	MOAILuaStrongRef	mInterfaceTable;			// interface shared by all instances of type
	MOAILuaStrongRef	mSingletonMemberTable;		// strong ref to member table for singletons

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

#endif
