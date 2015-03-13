// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISERIALIZER_H
#define MOAISERIALIZER_H

#include <moai-core/MOAISerializerBase.h>

//================================================================//
// MOAISerializerObjectInfo
//================================================================//
class MOAISerializerObjectInfo {
private:

	friend class MOAISerializer;
	
	MOAISerializerBase::ObjID	mMemberTableID;
	MOAISerializerBase::ObjID	mInitTableID;
	MOAISerializerBase::ObjID	mUserTableID;
};

//================================================================//
// MOAISerializer
//================================================================//
/**	@lua	MOAISerializer
	@text	Manages serialization state of Lua tables and Moai objects.
			The serializer will produce a Lua script that, when executed,
			will return the ordered list of objects added to it using the
			serialize () function.
*/
class MOAISerializer :
	public MOAISerializerBase {
private:

	// IDs of support tables for object initialization
	typedef STLMap < ObjID, MOAISerializerObjectInfo >::iterator ObjectInfoTableIt;
	STLMap < ObjID, MOAISerializerObjectInfo > mObjectInfoTable;

	// cache empty objects
	STLSet < ObjID > mEmpties;

	ObjID				mRoot;
	bool				mBase64;

	//----------------------------------------------------------------//
	static int			_floatToHex					( lua_State* L );
	static int			_hexToFloat					( lua_State* L );
	static int			_getObjectTables			( lua_State* L );
	static int			_serializeToFile			( lua_State* L );
	static int			_serializeToString			( lua_State* L );
	static int			_setBase64Enabled			( lua_State* L );

	//----------------------------------------------------------------//
	STLString			EscapeString				( cc8* str, size_t len );
	static bool			IsSimpleStringKey			( cc8* str );
	void				PrintObjectID				( ZLStream& stream, cc8* format, ObjID objID );
	void				WriteDecls					( ZLStream& stream );
	void				WriteObjectDecls			( ZLStream& stream );
	void				WriteObjectInits			( ZLStream& stream );
	void				WriteRecords				( ZLStream& stream );
	void				WriteReturnList				( ZLStream& stream );
	void				WriteTableDecls				( ZLStream& stream );
	u32					WriteTableInitializer		( ZLStream& stream, MOAILuaState& state, int idx, cc8* prefix );
	void				WriteTableInits				( ZLStream& stream );

protected:

	//----------------------------------------------------------------//
	virtual cc8*	GetDeserializerTypeName		();
	
public:

	DECL_LUA_FACTORY ( MOAISerializer )

	//----------------------------------------------------------------//
	void			AddLuaReturn				( MOAILuaObject* object );
	void			AddLuaReturn				( MOAILuaState& state, int idx );
	ObjID			AffirmMemberID				( MOAILuaObject* object );
	ObjID			AffirmMemberID				( MOAILuaState& state, int idx );
	void			Clear						();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	STLString		SerializeToString			();
	void			SerializeToFile				( cc8* filename );
	void			SerializeToStream			( ZLStream& stream );
					MOAISerializer				();
	virtual			~MOAISerializer				();
};

#endif
