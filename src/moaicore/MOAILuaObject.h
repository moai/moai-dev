// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAOBJECT_H
#define	MOAILUAOBJECT_H

#include <moaicore/MOAILuaRef.h>
#include <moaicore/MOAIObject.h>

class MOAIDeserializer;
class MOAILuaClass;
class MOAILuaState;
class MOAILuaStateHandle;
class MOAISerializer;

//================================================================//
// MOAILuaLocal
//================================================================//
class MOAILuaLocal {
private:

	friend class MOAILuaObject;
	int mRef;

public:
	
	//----------------------------------------------------------------//
			MOAILuaLocal		();
			~MOAILuaLocal		();
	
	//----------------------------------------------------------------//
	inline operator bool () {
		return ( this->mRef != LUA_NOREF );
	}
};

//================================================================//
// MOAILuaObject
//================================================================//
class MOAILuaObject :
	public virtual MOAIObject {
private:

	MOAILuaLocal	mContain;
	
protected:

	MOAILuaRef		mMemberTable;		// ref to member table (weak for factory class instances; strong for singletons)
	MOAILuaRef		mUserdata;			// ref to userdata (weak)
	
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
	void					ClearLocal			( MOAILuaLocal& ref );
	void					OnRelease			( u32 refCount );
	void					OnRetain			( u32 refCount );
	bool					PushLocal			( MOAILuaState& state, MOAILuaLocal& ref );
	void					PushMemberTable		( MOAILuaState& state );
	void					PushRefTable		( MOAILuaState& state );
	void					SetLocal			( MOAILuaState& state, int idx, MOAILuaLocal& ref );
	void					SetInterfaceTable	( MOAILuaState& state, int idx );
	void					SetMemberTable		( MOAILuaState& state, int idx );

public:

	friend class MOAILuaClass;
	friend class MOAIDeserializer;
	friend class MOAISerializer;

	//----------------------------------------------------------------//
	void					BindToLua					( MOAILuaState& state );
	virtual MOAILuaClass*	GetLuaClass					();
	cc8*					GetLuaClassName				();
	MOAILuaStateHandle		GetSelf						();
	bool					IsBound						();
	bool					IsSingleton					();
	void					LockToRefCount				();
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

	MOAILuaRef	mClassTable;		// global factory class for type
	MOAILuaRef	mInterfaceTable;	// interface shared by all instances of type

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
