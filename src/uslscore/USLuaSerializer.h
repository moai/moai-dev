// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LUASERIALIZER_H
#define LUASERIALIZER_H

#include <uslscore/STLList.h>
#include <uslscore/STLMap.h>
#include <uslscore/USLua.h>
#include <uslscore/USRtti.h>

class USLuaObject;
class USStream;

//================================================================//
// USLuaSerializer
//================================================================//
class USLuaSerializer :
	public virtual USLuaObject {
private:

	// list of instances waiting to be processed
	typedef STLList < USLuaObject* >::iterator PendingIt;
	STLList < USLuaObject* > mPending;

	// maps IDs onto instances
	typedef STLMap < uintptr, USLuaObject* >::iterator InstanceMapIt;
	STLMap < uintptr, USLuaObject* > mInstanceMap;

	// maps IDs onto tables
	typedef STLMap < uintptr, USLuaRef >::iterator TableMapIt;
	STLMap < uintptr, USLuaRef > mTableMap;

	// return list for Lua runtime
	typedef STLList < u32 >::iterator ReturnListIt;
	STLList < u32 > mReturnList;

	//----------------------------------------------------------------//
	static int		_exportToFile				( lua_State* L );
	static int		_exportToString				( lua_State* L );
	static int		_initInstance				( lua_State* L );
	static int		_register					( lua_State* L );
	static int		_serialize					( lua_State* L );

	//----------------------------------------------------------------//
	uintptr			GetID						( USLuaObject* object );
	uintptr			GetID						( USLuaState& state, int idx );
	void			Register					( USLuaObject* object, uintptr id );
	void			WriteDecls					( USStream& stream );
	void			WriteInstanceDecls			( USStream& stream );
	void			WriteInstanceInits			( USStream& stream );
	void			WriteRecords				( USStream& stream );
	void			WriteReturnList				( USStream& stream );
	u32				WriteTable					( USStream& stream, USLuaState& state, int idx, u32 tab );
	void			WriteTableDecls				( USStream& stream );
	u32				WriteTableInitializer		( USStream& stream, USLuaState& state, int idx, cc8* prefix );
	void			WriteTableInits				( USStream& stream );

protected:

	//----------------------------------------------------------------//
	virtual cc8*	GetFileMagic				();
	void			RegisterLuaClass			( USLuaState& state );
	void			RegisterLuaFuncs			( USLuaState& state );

public:

	enum {
		SUCCESS,
		INVALID_FILE,
		LOAD_ERROR,
		LUA_ERROR,
	};

	DECL_LUA_FACTORY ( USLuaSerializer )

	//----------------------------------------------------------------//
	uintptr			Affirm					( USLuaObject* object );
	uintptr			Affirm					( USLuaState& state, int idx );
	void			AddLuaReturn			( USLuaObject* object );
	void			AddLuaReturn			( USLuaState& state, int idx );
	void			Clear					();
	USLuaObject*		Dereference				( USLuaState& state, int idx );
	USLuaObject*		GetRefField				( USLuaState& state, int idx, cc8* name );
	u32				IsLuaFile				( cc8* filename );
	USLuaObject*		PopRef					( USLuaState& state );
	void			PushRef					( USLuaState& state, USLuaObject* object );
	u32				SerializeFromFile		( cc8* filename );
	STLString		SerializeToString		();
	void			SerializeToFile			( cc8* filename );
	void			SerializeToStream		( USStream& stream );
	void			SetRefField				( USLuaState& state, int idx, cc8* name, USLuaObject* object );
					USLuaSerializer			();
	virtual			~USLuaSerializer		();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* Dereference ( USLuaState& state, int idx ) {
	
		USLuaObject* object = this->Dereference ( state, idx );
		if ( object ) {
			return object->AsType < TYPE >();
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* GetRefField ( USLuaState& state, int idx, cc8* name ) {

		if ( state.GetFieldWithType ( idx, name, LUA_TNUMBER )) {
			return this->PopRef < TYPE >( state );
		}
		return 0;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* PopRef ( USLuaState& state ) {
	
		USLuaObject* object = this->PopRef ( state );
		if ( object ) {
			return object->AsType < TYPE >();
		}
		return 0;
	}
};

#endif
