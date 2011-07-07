// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USLUAOBJECT_H
#define	USLUAOBJECT_H

#include <uslscore/STLString.h>
#include <uslscore/USLuaRef.h>
#include <uslscore/USObject.h>

class USLuaClass;
class USLuaSerializer;
class USLuaState;
class USLuaStateHandle;

//================================================================//
// USLuaObject
//================================================================//
class USLuaObject :
	public virtual USObject {
protected:

	USLuaRef		mInstanceTable;		// strong ref to instance table stack
	USLuaRef		mUserdata;			// weak ref to handle userdata 


	//----------------------------------------------------------------//
	static int				_gc						( lua_State* L );
	static int				_getClass				( lua_State* L );
	static int				_getClassName			( lua_State* L );
	//static int			_tostring				( lua_State* L );

	//----------------------------------------------------------------//
	void					OnRelease				( u32 refCount );
	void					OnRetain				( u32 refCount );

public:

	friend class USLuaClass;

	//----------------------------------------------------------------//
	void					DebugDump				();
	virtual STLString		ToString				();
	STLString				ToStringWithType		();
	virtual USLuaClass*		GetLuaClass				();
	USLuaStateHandle		GetSelf					();
	bool					IsBound					();
	void					LuaUnbind				( USLuaState& state );
	void					PushLuaClassTable		( USLuaState& state );
	void					PushLuaUserdata			( USLuaState& state );
	virtual void			RegisterLuaClass		( USLuaState& state );
	virtual void			RegisterLuaFuncs		( USLuaState& state );
	virtual	void			SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	virtual	void			SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	void					SetLuaInstanceTable		( USLuaState& state, int idx );
							USLuaObject				();
	virtual					~USLuaObject			();
};

//================================================================//
// USLuaClass
//================================================================//
class USLuaClass :
	public USObject {
protected:

	USLuaRef mClassTable;	// global factory class for type
	USLuaRef mMemberTable;	// metatable shared by all instances of type

	//----------------------------------------------------------------//
	void				InitLuaFactoryClass			( USLuaObject& data, USLuaState& state );
	void				InitLuaInstanceTable		( USLuaObject* data, USLuaState& state, int idx );
	void				InitLuaSingletonClass		( USLuaObject& data, USLuaState& state );
	virtual void		RegisterLuaClass			( USLuaState& state ) = 0;

public:

	friend class USLuaObject;

	//----------------------------------------------------------------//
	virtual USLuaObject*	GetSingleton			();
	bool					IsSingleton				();
	virtual void			Register				() = 0;
							USLuaClass				();
	virtual					~USLuaClass				();
};

#endif
