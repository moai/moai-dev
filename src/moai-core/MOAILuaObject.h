// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILUAOBJECT_H
#define	MOAILUAOBJECT_H

#include <moai-core/MOAILuaRef.h>

#define MOAI_LUA_OBJECT_RTTI_BEGIN(type) 													\
	RTTI_BEGIN ( type ) 																	\
	RTTI_VISITOR ( MOAIAbstractLuaObjectVisitor, MOAILuaObjectVisitor < type >)

#define MOAI_LUA_OBJECT_RTTI_SINGLE(type, super) 											\
	RTTI_BEGIN ( type ) 																	\
	RTTI_VISITOR ( MOAIAbstractLuaObjectVisitor, MOAILuaObjectVisitor < type >) 			\
	RTTI_EXTEND ( super ) 																	\
	RTTI_END

#define MOAI_LUA_OBJECT_VISITOR_FRIEND 														\
	template < typename TYPE > friend class MOAILuaObjectVisitor;							\

#define DECL_LUA_FACTORY(type)																\
	IMPLEMENT_DEPENDS_ON ( type )															\
	MOAI_LUA_OBJECT_VISITOR_FRIEND															\
	MOAILuaClass* GetLuaClass () { return &MOAILuaFactoryClass < type >::Get (); }			\
	static void RegisterLuaType () { MOAILuaFactoryClass < type >::Get ().Register (); }	\
	cc8* TypeName () const { return #type; }

#define DECL_LUA_ABSTRACT(type)																\
	IMPLEMENT_DEPENDS_ON ( type )															\
	MOAI_LUA_OBJECT_VISITOR_FRIEND															\
	MOAILuaClass* GetLuaClass () { return 0; }												\
	cc8* TypeName () const { return #type; }

#define DECL_LUA_OPAQUE(type)																\
	IMPLEMENT_DEPENDS_ON ( type )															\
	MOAI_LUA_OBJECT_VISITOR_FRIEND															\
	MOAILuaClass* GetLuaClass () { return &MOAILuaFactoryClass < type >::Get (); }			\
	cc8* TypeName () const { return #type; }

#define DECL_LUA_SINGLETON(type)															\
	IMPLEMENT_DEPENDS_ON ( type )															\
	MOAI_LUA_OBJECT_VISITOR_FRIEND 															\
	MOAILuaClass* GetLuaClass () { return &MOAILuaSingletonClass < type >::Get (); }		\
	static void RegisterLuaType () { MOAILuaSingletonClass < type >::Get ().Register (); }	\
	cc8* TypeName () const { return #type; }

class MOAIDeserializer;
class MOAILuaClass;
class MOAILuaObject;
class MOAIComposer;
class MOAILuaState;
class MOAIScopedLuaState;
class MOAISerializer;

//================================================================//
// MOAILuaObject
//================================================================//
class MOAILuaObject :
	public virtual RTTIBase,
	public virtual ZLRefCountedObject,
	public virtual ZLFinalizable {
private:

	friend class MOAILuaObjectMemo;
	template < typename TYPE > friend class MOAILuaObjectVisitor;

	u32						mActiveUserdataCount;
	MOAILuaWeakRef			mUserdata;		// ref to userdata (weak)
	MOAILuaStrongRef		mFinalizer;		// ref to finalizer (strong)

protected:

	static int				_gc						( lua_State* L );
	static int				_getClass				( lua_State* L );
	static int				_getClassName			( lua_State* L );
	static int				_getMemberTable			( lua_State* L );
	static int				_getRefTable			( lua_State* L );
	static int				_pin					( lua_State* L );
	static int				_serializeIn			( lua_State* L );
	static int				_serializeOut			( lua_State* L );
	static int				_setFinalizer			( lua_State* L );
	static int				_setInterface			( lua_State* L );
	static int				_setMembers				( lua_State* L );
	static int				_tostring				( lua_State* L );
	static int				_unpin					( lua_State* L );

	//----------------------------------------------------------------//
	static int				InjectAndCall			( lua_CFunction func, MOAILuaObject* self, lua_State* L );
	void					MakeLuaBinding			( MOAILuaState& state );
	bool					PushRefTable			( MOAILuaState& state );
	void					SetInterfaceTable		( MOAILuaState& state, int idx );
	void					SetMemberTable			( MOAILuaState& state, int idx );
	static void				Unbind					( MOAILuaObject* object, MOAILuaWeakRef& userdata );

	//----------------------------------------------------------------//
	virtual void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	virtual	void			MOAILuaObject_SerializeIn			( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer );
	virtual	void			MOAILuaObject_SerializeOut			( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer );
	void					ZLRefCountedObjectBase_OnRelease	( u32 refCount );

public:

	friend class MOAILuaCanary;
	friend class MOAILuaClass;
	friend class MOAILuaMemberRef;
	friend class MOAILuaRuntime;
	friend class MOAIDeserializer;
	friend class MOAISerializer;

	//----------------------------------------------------------------//
	static int				_alertNewIsUnsupported		( lua_State* L );

	//----------------------------------------------------------------//
	void					BindToLua					( MOAILuaState& state );
	virtual MOAILuaClass*	GetLuaClass					();
	MOAIScopedLuaState		GetSelf						();
	void					GetRef						( MOAILuaRef& ref );
	bool					IsBound						();
	static bool				IsMoaiUserdata				( MOAILuaState& state, int idx );
	bool					IsSingleton					();
	void					LuaRelease					();
	void					LuaRelease					( MOAILuaObject* object );
	void					LuaRetain					();
	void					LuaRetain					( MOAILuaObject* object );
							MOAILuaObject				();
	virtual					~MOAILuaObject				();
	void					PrintTracking				();
	void					PushLuaClassTable			( MOAILuaState& state );
	bool					PushLuaUserdata				( MOAILuaState& state );
	bool					PushMemberTable				( MOAILuaState& state );
	void					RegisterLuaClass			( MOAILuaState& state );
	void					RegisterLuaFuncs			( MOAILuaState& state );
	void					SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void					SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	
	//----------------------------------------------------------------//
	template < typename TYPE, lua_CFunction FUNC >
	static int WrapInstanceFuncAsGlobal ( lua_State* L ) {
	
		TYPE* type = ZLContextMgr::Get ()->GetGlobal < TYPE >();
		assert ( type );
		return InjectAndCall ( FUNC, type, L );
	}
};

//================================================================//
// MOAIAbstractLuaObjectVisitor
//================================================================//
class MOAIAbstractLuaObjectVisitor {
public:

	//----------------------------------------------------------------//
	MOAIAbstractLuaObjectVisitor () {
	}

	//----------------------------------------------------------------//
	virtual ~MOAIAbstractLuaObjectVisitor () {
	}

	//----------------------------------------------------------------//
	virtual void	RegisterLuaClass		( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state ) const = 0;
	virtual void	RegisterLuaFuncs		( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state ) const = 0;
	virtual void 	SerializeIn 			( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) const = 0;
	virtual void 	SerializeOut 			( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) const = 0;
};

//================================================================//
// MOAILuaObjectVisitor
//================================================================//
template < typename TYPE >
class MOAILuaObjectVisitor :
	public MOAIAbstractLuaObjectVisitor {
public:

	//----------------------------------------------------------------//
	void RegisterLuaClass ( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state ) const {
		self.AsType < TYPE >()->TYPE::MOAILuaObject_RegisterLuaClass ( history, state );
	}
	
	//----------------------------------------------------------------//
	void RegisterLuaFuncs ( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state ) const {
		self.AsType < TYPE >()->TYPE::MOAILuaObject_RegisterLuaFuncs ( history, state );
	}

	//----------------------------------------------------------------//
	void SerializeIn ( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) const {
		self.AsType < TYPE >()->TYPE::MOAILuaObject_SerializeIn ( history, state, serializer );
	}
	
	//----------------------------------------------------------------//
	void SerializeOut ( MOAILuaObject& self, RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) const {
		self.AsType < TYPE >()->TYPE::MOAILuaObject_SerializeOut ( history, state, serializer );
	}
};

#endif
