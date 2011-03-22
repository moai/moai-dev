// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef LUASERIALIZER_H
#define LUASERIALIZER_H

#include <uslscore/STLList.h>
#include <uslscore/STLMap.h>
#include <uslscore/USLua.h>
#include <uslscore/USRtti.h>

class USLuaData;
class USStream;

//================================================================//
// USLuaSerializer
//================================================================//
/**	@brief Manages a serialization session to and from Lua.
	@ingroup LuaLib
*/
class USLuaSerializer :
	public virtual USLuaData {
private:

	// list of instances waiting to be processed
	typedef STLList < USLuaData* >::iterator PendingIt;
	STLList < USLuaData* > mPending;

	// maps IDs onto instances
	typedef STLMap < u32, USLuaData* >::iterator InstanceMapIt;
	STLMap < u32, USLuaData* > mInstanceMap;

	// maps IDs onto tables
	typedef STLMap < u32, USLuaRef >::iterator TableMapIt;
	STLMap < u32, USLuaRef > mTableMap;

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
	u32				GetID						( USLuaData* object );
	u32				GetID						( USLuaState& state, int idx );
	void			Register					( USLuaData* object, u32 id );
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

	DECL_LUA_DATA ( USLuaSerializer )

	//----------------------------------------------------------------//
	u32				Affirm					( USLuaData* object );
	u32				Affirm					( USLuaState& state, int idx );
	void			AddLuaReturn			( USLuaData* object );
	void			AddLuaReturn			( USLuaState& state, int idx );
	void			Clear					();
	USLuaData*		Dereference				( USLuaState& state, int idx );
	USLuaData*		GetRefField				( USLuaState& state, int idx, cc8* name );
	u32				IsLuaFile				( cc8* filename );
	USLuaData*		PopRef					( USLuaState& state );
	void			PushRef					( USLuaState& state, USLuaData* object );
	u32				SerializeFromFile		( cc8* filename );
	STLString		SerializeToString		();
	void			SerializeToFile			( cc8* filename );
	void			SerializeToStream		( USStream& stream );
	void			SetRefField				( USLuaState& state, int idx, cc8* name, USLuaData* object );
					USLuaSerializer			();
	virtual			~USLuaSerializer		();
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* Dereference ( USLuaState& state, int idx ) {
	
		USLuaData* object = this->Dereference ( state, idx );
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
	
		USLuaData* object = this->PopRef ( state );
		if ( object ) {
			return object->AsType < TYPE >();
		}
		return 0;
	}
};

#endif
