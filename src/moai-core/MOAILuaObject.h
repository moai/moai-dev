// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAOBJECT_H
#define	MOAILUAOBJECT_H

#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAIRtti.h>

class MOAIDeserializer;
class MOAILuaCanary;
class MOAILuaClass;
class MOAILuaObject;
class MOAILuaState;
class MOAIScopedLuaState;
class MOAISerializer;

//================================================================//
// MOAILuaObject
//================================================================//
class MOAILuaObject :
	public virtual RTTIBase {
private:

	MOAILuaCanary*			mCanary;
	MOAILuaWeakRef			mUserdata;			// ref to userdata (weak)
	MOAILuaStrongRef		mFinalizer;			// ref to finalizer (strong)
	
protected:
	
	//----------------------------------------------------------------//
	static int				_gc					( lua_State* L );
	static int				_getClass			( lua_State* L );
	static int				_getClassName		( lua_State* L );
	static int				_getRefTable		( lua_State* L );
	static int				_index				( lua_State* L );
	static int				_pin				( lua_State* L );
	static int				_newindex			( lua_State* L );
	static int				_setFinalizer		( lua_State* L );
	static int				_setInterface		( lua_State* L );
	static int				_setMembers			( lua_State* L );
	static int				_tostring			( lua_State* L );
	static int				_unpin				( lua_State* L );

	//----------------------------------------------------------------//
	void					MakeLuaBinding		( MOAILuaState& state );
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
	MOAILuaCanary*			AffirmCanary		();
	void					BindToLua					( MOAILuaState& state );
	virtual MOAILuaClass*	GetLuaClass					();
	//cc8*					GetLuaClassName				();
	MOAIScopedLuaState		GetSelf						();
	void					GetRef						( MOAILuaRef& ref );
	bool					IsBound						();
	bool					IsSingleton					();
	void					LuaRelease					( MOAILuaObject* object );
	void					LuaRetain					( MOAILuaObject* object );
	void					PushLuaClassTable			( MOAILuaState& state );
	bool					PushLuaUserdata				( MOAILuaState& state );
	virtual void			RegisterLuaClass			( MOAILuaState& state );
	virtual void			RegisterLuaFuncs			( MOAILuaState& state );
	static void             ReportLeaks					( FILE *f, bool clearAfter );
	virtual	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	virtual	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
							MOAILuaObject				();
	virtual					~MOAILuaObject				();
};

#endif
