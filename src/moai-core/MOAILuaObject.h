// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAOBJECT_H
#define	MOAILUAOBJECT_H

#include <moai-core/MOAIGlobals.h>
#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAIObject.h>

class MOAIDeserializer;
class MOAILuaClass;
class MOAILuaObject;
class MOAILuaState;
class MOAIScopedLuaState;
class MOAISerializer;

//================================================================//
// MOAILuaObject
//================================================================//
class MOAILuaObject :
	public virtual MOAIObject {
private:

	bool					mCollected;
	MOAILuaWeakRef			mUserdata;			// ref to userdata (weak)
	MOAILuaStrongRef		mFinalizer;			// ref to finalizer (strong)

protected:

	//----------------------------------------------------------------//
	static int				_gc					( lua_State* L );
	static int				_getClass			( lua_State* L );
	static int				_getClassName		( lua_State* L );
	static int				_getMemberTable		( lua_State* L );
	static int				_getRefTable		( lua_State* L );
	static int				_pin				( lua_State* L );
	static int				_serializeIn		( lua_State* L );
	static int				_serializeOut		( lua_State* L );
	static int				_setFinalizer		( lua_State* L );
	static int				_setInterface		( lua_State* L );
	static int				_setMembers			( lua_State* L );
	static int				_tostring			( lua_State* L );
	static int				_unpin				( lua_State* L );

	//----------------------------------------------------------------//
	static int				InjectAndCall			( lua_CFunction func, MOAILuaObject* self, lua_State* L );
	void					MakeLuaBinding			( MOAILuaState& state );
	void					OnRelease				( u32 refCount );
	bool					PushRefTable			( MOAILuaState& state );
	void					SetInterfaceTable		( MOAILuaState& state, int idx );
	void					SetMemberTable			( MOAILuaState& state, int idx );

public:

	friend class MOAILuaCanary;
	friend class MOAILuaClass;
	friend class MOAILuaMemberRef;
	friend class MOAILuaRuntime;
	friend class MOAIDeserializer;
	friend class MOAISerializer;

	//----------------------------------------------------------------//
	void					BindToLua					( MOAILuaState& state );
	virtual MOAILuaClass*	GetLuaClass					();
	MOAIScopedLuaState		GetSelf						();
	void					GetRef						( MOAILuaRef& ref );
	bool					IsBound						();
	static bool				IsMoaiUserdata				( MOAILuaState& state, int idx );
	bool					IsSingleton					();
	void					LuaRelease					( MOAILuaObject* object );
	void					LuaRetain					( MOAILuaObject* object );
							MOAILuaObject				();
	virtual					~MOAILuaObject				();
	void					PrintTracking				();
	void					PushLuaClassTable			( MOAILuaState& state );
	bool					PushLuaUserdata				( MOAILuaState& state );
	bool					PushMemberTable			( MOAILuaState& state );
	virtual void			RegisterLuaClass			( MOAILuaState& state );
	virtual void			RegisterLuaFuncs			( MOAILuaState& state );
	virtual	void			SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	virtual	void			SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	bool					WasCollected				();
	
	//----------------------------------------------------------------//
	template < typename TYPE, lua_CFunction FUNC >
	static int WrapInstanceFuncAsGlobal ( lua_State* L ) {
	
		TYPE* type = MOAIGlobalsMgr::Get ()->GetGlobal < TYPE >();
		assert ( type );
		return InjectAndCall ( FUNC, type, L );
	}
};

#endif
