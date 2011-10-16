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
// USLuaLocal
//================================================================//
class USLuaLocal {
private:

	friend class USLuaObject;
	int mRef;

public:
	
	//----------------------------------------------------------------//
			USLuaLocal		();
			~USLuaLocal		();
	
	//----------------------------------------------------------------//
	inline operator bool () {
		return ( this->mRef != LUA_NOREF );
	}
};

//================================================================//
// USLuaObject
//================================================================//
class USLuaObject :
	public virtual USObject {
private:

	USLuaLocal		mContain;

protected:

	USLuaRef		mInstanceTable;		// weak ref to instance table stack
	USLuaRef		mPrivateTable;		// weak ref to private local reference table
	USLuaRef		mUserdata;			// weak/strong ref to userdata
	

	//----------------------------------------------------------------//
	static int				_gc						( lua_State* L );
	static int				_getClass				( lua_State* L );
	static int				_getClassName			( lua_State* L );
	static int				_tostring				( lua_State* L );

	//----------------------------------------------------------------//
	void					ClearLocal			( USLuaLocal& ref );
	void					OnRelease			( u32 refCount );
	void					OnRetain			( u32 refCount );
	bool					PushLocal			( USLuaState& state, USLuaLocal& ref );
	void					SetLocal			( USLuaState& state, int idx, USLuaLocal& ref );

public:

	friend class USLuaClass;

	//----------------------------------------------------------------//
	void					BindToLuaWithTable		( USLuaState& state ); // push table at top of stack!
	virtual USLuaClass*		GetLuaClass				();
	USLuaStateHandle		GetSelf					();
	void					InsertObject			( USLuaObject& object );
	bool					IsBound					();
	void					LuaUnbind				( USLuaState& state );
	void					PushLuaClassTable		( USLuaState& state );
	void					PushLuaUserdata			( USLuaState& state );
	virtual void			RegisterLuaClass		( USLuaState& state );
	virtual void			RegisterLuaFuncs		( USLuaState& state );
	void					RemoveObject			( USLuaObject& object );
	static void             ReportLeaks				( FILE *f, bool clearAfter );
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

	USLuaRef	mClassTable;	// global factory class for type
	USLuaRef	mMemberTable;	// metatable shared by all instances of type

	//----------------------------------------------------------------//
	void				InitLuaFactoryClass			( USLuaObject& data, USLuaState& state );
	void				InitLuaSingletonClass		( USLuaObject& data, USLuaState& state );
	void				PushMemberTable				( USLuaState& state );
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
