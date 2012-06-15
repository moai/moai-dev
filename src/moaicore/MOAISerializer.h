// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISERIALIZER_H
#define MOAISERIALIZER_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAISerializerBase.h>

//================================================================//
// MOAISerializer
//================================================================//
/**	@name	MOAISerializer
	@text	Manages serialization state of Lua tables and Moai objects.
			The serializer will produce a Lua script that, when executed,
			will return the ordered list of objects added to it using the
			serialize () function.
*/
class MOAISerializer :
	public MOAISerializerBase {
private:

	// list of instances waiting to be processed
	typedef STLList < uintptr >::iterator PendingIt;
	STLList < uintptr > mPending;

	// return list for Lua runtime
	typedef STLList < uintptr >::iterator ReturnListIt;
	STLList < uintptr > mReturnList;

	//----------------------------------------------------------------//
	static int		_exportToFile				( lua_State* L );
	static int		_exportToString				( lua_State* L );
	static int		_serialize					( lua_State* L );
	static int		_serializeToFile			( lua_State* L );
	static int		_serializeToString			( lua_State* L );

	//----------------------------------------------------------------//
	void			WriteDecls					( USStream& stream );
	void			WriteObjectDecls			( USStream& stream );
	void			WriteObjectInits			( USStream& stream );
	void			WriteRecords				( USStream& stream );
	void			WriteReturnList				( USStream& stream );
	u32				WriteTable					( USStream& stream, MOAILuaState& state, int idx, u32 tab );
	void			WriteTableDecls				( USStream& stream );
	u32				WriteTableInitializer		( USStream& stream, MOAILuaState& state, int idx, cc8* prefix );
	void			WriteTableInits				( USStream& stream );

protected:

	//----------------------------------------------------------------//
	virtual cc8*	GetDeserializerTypeName		();
	
public:

	DECL_LUA_FACTORY ( MOAISerializer )

	//----------------------------------------------------------------//
	void			AddLuaReturn				( MOAILuaObject* object );
	void			AddLuaReturn				( MOAILuaState& state, int idx );
	uintptr			AffirmMemberID				( MOAILuaObject* object );
	uintptr			AffirmMemberID				( MOAILuaState& state, int idx );
	void			Clear						();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	STLString		SerializeToString			();
	void			SerializeToFile				( cc8* filename );
	void			SerializeToStream			( USStream& stream );
					MOAISerializer				();
	virtual			~MOAISerializer			();
};

#endif
