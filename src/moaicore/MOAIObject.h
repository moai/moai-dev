// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAOBJECT_H
#define	MOAILUAOBJECT_H

#include <moaicore/MOAILuaRef.h>

class MOAILuaClass;
class MOAISerializer;
class MOAILuaState;
class MOAILuaStateHandle;

//================================================================//
// MOAILuaLocal
//================================================================//
class MOAILuaLocal {
private:

	friend class MOAIObject;
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
// MOAIObject
//================================================================//
class MOAIObject :
	public virtual USObject {
private:

	MOAILuaLocal	mContain;

protected:

	
	MOAILuaRef		mInstanceTable;		// ref to instance table (weak for factory class instances; strong for singletons)
	MOAILuaRef		mUserdata;			// ref to userdata (weak)
	

	//----------------------------------------------------------------//
	static int				_gc						( lua_State* L );
	static int				_getClass				( lua_State* L );
	static int				_getClassName			( lua_State* L );
	static int				_index					( lua_State* L );
	static int				_newindex				( lua_State* L );
	static int				_tombstone				( lua_State* L );
	static int				_tostring				( lua_State* L );

	//----------------------------------------------------------------//
	void					ClearLocal			( MOAILuaLocal& ref );
	void					OnRelease			( u32 refCount );
	void					OnRetain			( u32 refCount );
	bool					PushLocal			( MOAILuaState& state, MOAILuaLocal& ref );
	void					PushMemberTable		( MOAILuaState& state );
	void					SetLocal			( MOAILuaState& state, int idx, MOAILuaLocal& ref );
	void					SetMemberTable		( MOAILuaState& state, int idx );

public:

	friend class MOAILuaClass;
	friend class MOAIDeserializer;
	friend class MOAISerializer;

	//----------------------------------------------------------------//
	void					BindToLuaWithTable		( MOAILuaState& state ); // push table at top of stack!
	virtual MOAILuaClass*	GetLuaClass				();
	MOAILuaStateHandle		GetSelf					();
	bool					IsBound					();
	void					LockToRefCount			();
	void					LuaRelease				( MOAIObject& object );
	void					LuaRetain				( MOAIObject& object );
	void					LuaUnbind				();
	void					PushLuaClassTable		( MOAILuaState& state );
	void					PushLuaUserdata			( MOAILuaState& state );
	virtual void			RegisterLuaClass		( MOAILuaState& state );
	virtual void			RegisterLuaFuncs		( MOAILuaState& state );
	static void             ReportLeaks				( FILE *f, bool clearAfter );
	virtual	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	virtual	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
							MOAIObject				();
	virtual					~MOAIObject				();
};

//================================================================//
// MOAILuaClass
//================================================================//
class MOAILuaClass :
	public USObject {
protected:

	MOAILuaRef	mClassTable;		// global factory class for type
	MOAILuaRef	mInterfaceTable;	// interface shared by all instances of type

	//----------------------------------------------------------------//
	void				InitLuaFactoryClass			( MOAIObject& data, MOAILuaState& state );
	void				InitLuaSingletonClass		( MOAIObject& data, MOAILuaState& state );
	void				PushInterfaceTable			( MOAILuaState& state );
	virtual void		RegisterLuaClass			( MOAILuaState& state ) = 0;

public:

	friend class MOAIObject;

	//----------------------------------------------------------------//
	virtual MOAIObject*		GetSingleton			();
	bool					IsSingleton				();
	virtual void			Register				() = 0;
							MOAILuaClass			();
	virtual					~MOAILuaClass			();
};

#endif
