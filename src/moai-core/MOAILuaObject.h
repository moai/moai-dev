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

#endif
