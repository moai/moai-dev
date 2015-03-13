// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAISerializer.h>
#include <moai-core/MOAILua.h>

//================================================================//
// MOAISerializer
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISerializer::_floatToHex ( lua_State* L ) {
	MOAILuaState state ( L );
	
	double num = state.GetValue < double >( 1, 0.0 );
	STLString hex;
	hex.write ( "%la", num );
	
	state.Push ( hex );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISerializer::_getObjectTables ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISerializer, "UU" );

	MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( 2, true );
	
	// user table
	state.Push ();
	
	// member table
	object->PushMemberTable ( state );
	
	// if the member table is empty, omit it
	if ( !state.HasKeys ( -1 )) {
		state.Pop ( 1 );
		state.Push ();
	}
	
	// init table
	lua_newtable ( state );
	object->SerializeOut ( state, *self );

	return 3;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISerializer::_hexToFloat ( lua_State* L ) {
	MOAILuaState state ( L );
	
	cc8* hex = state.GetValue < cc8* >( 1, 0 );
	if ( hex ) {
	
		double num;
		int result = sscanf ( hex, "%la", &num );
		
		if (( result != EOF ) && ( result > 0 )) {
			state.Push ( num );
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	serializeToFile
	@text	Serializes the specified table or object to a file.

	@overload

		@in		string filename			The file to create.
		@in		table data				The table to serialize.
		@out	nil
	
	@overload

		@in		string filename			The file to create.
		@in		MOAILuaObject data		The object to serialize.
		@out	nil
	
	@overload

		@in		MOAISerializer self
		@in		string filename			The file to create.
		@in		table data				The table to serialize.
		@out	nil
	
	@overload

		@in		MOAISerializer self
		@in		string filename			The file to create.
		@in		MOAILuaObject data		The object to serialize.
		@out	nil
*/
int MOAISerializer::_serializeToFile ( lua_State* L ) {

	MOAILuaState state ( L );
	
	MOAISerializer defaultSerializer;
	MOAISerializer* serializer = &defaultSerializer;
	int base = 2;
	
	if ( state.GetTop () == 3 ) {
		serializer = state.GetLuaObject < MOAISerializer >( 1, true );
		if ( !serializer ) return 0;
		base = 3;
	}
	
	if ( !( state.IsType ( base - 1, LUA_TSTRING ))) return 0;
	if ( !( state.IsType ( base, LUA_TTABLE ) || state.IsType ( base, LUA_TUSERDATA ))) return 0;
	
	cc8* filename = state.GetValue < cc8* >( base - 1, "" );

	serializer->mRoot = serializer->AffirmMemberID ( state, base );
	serializer->SerializeToFile ( filename );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	serializeToString
	@text	Serializes the specified table or object to a string.

	@overload

		@in		table data				The table to serialize.
		@out	string serialized		The serialized string.
	
	@overload

		@in		MOAILuaObject data		The object to serialize.
		@out	string serialized		The serialized string.

	@overload

		@in		MOAISerializer self
		@in		table data				The table to serialize.
		@out	string serialized		The serialized string.
	
	@overload

		@in		MOAISerializer self
		@in		MOAILuaObject data		The object to serialize.
		@out	string serialized		The serialized string.
*/
int MOAISerializer::_serializeToString ( lua_State* L ) {

	MOAILuaState state ( L );
	
	MOAISerializer defaultSerializer;
	MOAISerializer* serializer = &defaultSerializer;
	int base = 1;
	
	if ( state.GetTop () == 3 ) {
		serializer = state.GetLuaObject < MOAISerializer >( 1, true );
		if ( !serializer ) return 0;
		base = 2;
	}
	
	if ( !( state.IsType ( base, LUA_TTABLE ) || state.IsType ( base, LUA_TUSERDATA ))) return 0;

	serializer->mRoot = serializer->AffirmMemberID ( state, base );
	STLString result = serializer->SerializeToString ();

	lua_pushstring ( state, result );

	return 1;
}

//----------------------------------------------------------------//
int MOAISerializer::_setBase64Enabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISerializer, "U" );
	
	self->mBase64 = state.GetValue < bool >( 2, false );
	return  0;
}

//================================================================//
// MOAISerializer
//================================================================//

//----------------------------------------------------------------//
MOAISerializerBase::ObjID MOAISerializer::AffirmMemberID ( MOAILuaObject* object ) {

	if ( object ) {
		MOAIScopedLuaState state = object->GetSelf ();
		return this->AffirmMemberID ( state, -1 );
	}
	return NULL_OBJ_ID;
}

//----------------------------------------------------------------//
MOAISerializerBase::ObjID MOAISerializer::AffirmMemberID ( MOAILuaState& state, int idx ) {

	if ( !( state.IsType ( idx, LUA_TUSERDATA ) || state.IsType ( idx, LUA_TTABLE ))) return NULL_OBJ_ID;

	idx = state.AbsIndex ( idx );
	ObjID memberID = this->GetID ( state, idx );

	// bail if the table's already been added (or is empty)
	if ( this->mEmpties.contains ( memberID )) return NULL_OBJ_ID;
	if ( this->mObjectMap.contains ( memberID )) return memberID;
	if ( this->mTableMap.contains ( memberID )) return memberID;

	// if we're an object, affirm as such...
	if ( state.IsType ( idx, LUA_TUSERDATA )) {
		
		MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( idx, false );
		assert ( object );
		
		cc8* classname = object->TypeName ();
		lua_getfield ( state, idx, "getClassName" );
		
		if ( state.IsType ( -1, LUA_TFUNCTION )) {
			lua_pushvalue ( state, idx );
			state.DebugCall ( 1, 1 );
			classname = state.GetValue < cc8* >( -1, "" );
		}
		state.Pop ( 1 );
		
		MOAISerializerObjectEntry& entry = this->mObjectMap [ memberID ];
		
		entry.mLuaRef.SetRef ( state, idx );
		entry.mObject = object;
		entry.mClassName = classname;
		
		MOAISerializerObjectInfo& objectInfo = this->mObjectInfoTable [ memberID ];
		
		u32 top = state.GetTop ();
		this->PushLuaUserdata ( state );
		
		if ( state.GetFieldWithType ( -1, "getObjectTables", LUA_TFUNCTION )) {
		
			lua_pushvalue ( state, -2 );
			lua_pushvalue ( state, idx );
			lua_pushstring ( state, classname );
			int status = state.DebugCall ( 3, 3 );
			
			if ( status == 0 ) {
				objectInfo.mUserTableID		= this->AffirmMemberID ( state, -3 );
				objectInfo.mMemberTableID	= this->AffirmMemberID ( state, -2 );
				objectInfo.mInitTableID		= this->AffirmMemberID ( state, -1 );
				state.Pop ( 3 );
			}
		}
		state.Pop ( 1 );
		top = state.GetTop ();
	}
	else if ( state.IsType ( idx, LUA_TTABLE )) {
		
		this->mTableMap [ memberID ].SetRef ( state, idx );
		
		u32 itr = state.PushTableItr ( idx );
		while ( state.TableItrNext ( itr )) {
			this->AffirmMemberID ( state, -1 );
		}
	}
	return memberID;
}

//----------------------------------------------------------------//
void MOAISerializer::Clear () {

	MOAISerializerBase::Clear ();

	this->mObjectInfoTable.clear ();
}

//----------------------------------------------------------------//
STLString MOAISerializer::EscapeString ( cc8* str, size_t len ) {

	STLString outStr;
	outStr.reserve ( len * 2 );

	outStr.append ( "\'" );

	for ( u32 i = 0; i < len; ++i ) {
		
		char c = str [ i ];
		
		if (( c >= 32 ) && ( c <= 126 )) {
		
			switch ( c ) {
				case '\\':
					outStr.append ( "\\\\" );
					break;
				case '"':
					outStr.append ( "\\\"" );
					break;
				case '\'':
					outStr.append ( "\\\'" );
					break;
				default:
					outStr.push_back ( c );
			}
        }
		else {
		
			if ( this->mBase64 ) {
			
				STLString base64;
				base64.base_64_encode ( str, len );
				
				outStr = "";
				outStr.write ( "base64 ( '%s' )", base64.c_str ());
				
				return outStr;
			}
			int b = ( int )(( u8* )str )[ i ];
			outStr.write ( "\\%03d", b );
		}
	}

	outStr.append ( "\'" );
	return outStr;
}

//----------------------------------------------------------------//
cc8* MOAISerializer::GetDeserializerTypeName () {

	return "MOAIDeserializer";
}

//----------------------------------------------------------------//
bool MOAISerializer::IsSimpleStringKey ( cc8* str ) {

	for ( u32 i = 0; str [ i ]; ++i ) {
		u8 c = str [ i ];
		bool isAlpha		= (( c >= 'a' ) && ( c <= 'z' )) || (( c >= 'A' ) && ( c <= 'Z' ));
		bool isNumeric		= (( c >= '0' ) && ( c <= '9' ));
		
		if (( i == 0 ) && !isAlpha ) return false;
		if ( !( isAlpha || isNumeric )) return false;
	}
	return true;
}

//----------------------------------------------------------------//
MOAISerializer::MOAISerializer () :
	mBase64 ( true ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAISerializerBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISerializer::~MOAISerializer () {
}

//----------------------------------------------------------------//
void MOAISerializer::PrintObjectID ( ZLStream& stream, cc8* format, ObjID objID ) {

	char buffer [ 32 ];

	if ( objID != NULL_OBJ_ID ) {
		sprintf ( buffer, "objects [ 0x%02X ]", objID );
	}
	else {
		sprintf ( buffer, "nil" );
	}
	stream.Print ( format, ( cc8* )buffer );
}

//----------------------------------------------------------------//
void MOAISerializer::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "floatToHex",			_floatToHex },
		{ "hexToFloat",			_hexToFloat },
		{ "serializeToFile",	_serializeToFile },
		{ "serializeToString",	_serializeToString },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISerializer::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getObjectTables",		_getObjectTables },
		{ "serializeToFile",		_serializeToFile },
		{ "serializeToString",		_serializeToString },
		{ "setBase64Enabled",		_setBase64Enabled },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAISerializer::SerializeToString () {
	
	ZLMemStream memStream;
	this->SerializeToStream ( memStream );
	memStream.Seek ( 0, SEEK_SET );
	
	return memStream.ReadString ( memStream.GetLength ());
}

//----------------------------------------------------------------//
void MOAISerializer::SerializeToFile ( cc8* filename ) {

	ZLFileStream fileStream;
	fileStream.OpenWrite ( filename );
		
	this->SerializeToStream ( fileStream );
}
	
//----------------------------------------------------------------//
void MOAISerializer::SerializeToStream ( ZLStream& stream ) {
	
	stream.Print ( "%s\n", this->GetFileMagic ());
	stream.Print ( "local deserializer = ... or %s.new ()\n", this->GetDeserializerTypeName ());
	stream.Print ( "local base64 = MOAIDeserializer.base64Decode\n" );
	
	stream.Print ( "\n" );
	
	// write the initializer first, in a function
	stream.Print ( "local function init ( objects )\n\n" );
	this->WriteTableInits ( stream );
	this->WriteObjectInits ( stream );
	stream.Print ( "end\n\n" );
	
	// now write the decls
	this->WriteDecls ( stream );
	
	// call the initializer
	stream.Print ( "init ( objects )\n" );
	
	// and return the root
	stream.Print ( "\n" );
	stream.Print ( "--Returning Root\n" );
	
	this->PrintObjectID ( stream, "return %s\n", this->mRoot );
}

//----------------------------------------------------------------//
void MOAISerializer::WriteDecls ( ZLStream& stream ) {

	stream.Print ( "--Declaring Objects\n" );
	stream.Print ( "local objects = {\n\n" );
	
	this->WriteTableDecls ( stream );
	this->WriteObjectDecls ( stream );
	
	stream.Print ( "}\n\n" );
}

//----------------------------------------------------------------//
void MOAISerializer::WriteObjectDecls ( ZLStream& stream ) {

	if ( !this->mObjectMap.size ()) return;

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	stream.Print ( "\t--Declaring Instances\n" );
	ObjectMapIt objectIt;
	objectIt = this->mObjectMap.begin ();
	for ( ; objectIt != this->mObjectMap.end (); ++objectIt ) {
		
		ObjID id = objectIt->first;
		MOAISerializerObjectEntry& entry = objectIt->second;
		
		MOAILuaObject* object = entry.mObject;
		cc8* classname = entry.mClassName;
		
		if ( !object->IsSingleton ()) {
			stream.Print ( "\t[ 0x%02X ] = deserializer:registerObjectID ( deserializer:createObject ( \'%s\' ), 0x%02X ),\n", id, classname, id );
		}
	}
	stream.Print ( "\n" );
}

//----------------------------------------------------------------//
void MOAISerializer::WriteObjectInits ( ZLStream& stream ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	ObjectMapIt objectMapIt = this->mObjectMap.begin ();
	for ( ; objectMapIt != this->mObjectMap.end (); ++objectMapIt ) {
		
		ObjID id = objectMapIt->first;
		MOAISerializerObjectEntry& entry = objectMapIt->second;
		
		cc8* classname = entry.mClassName;
		
		MOAILuaObject* object = entry.mObject;
		if ( object->IsSingleton ()) {
			stream.Print ( "\tdeserializer:initObject ( %s, \'%s\', ", classname, classname );
		}
		else {
			stream.Print ( "\tdeserializer:initObject ( objects [ 0x%02X ], \'%s\', ", id, classname );
		}
		
		MOAISerializerObjectInfo& objectInfo = this->mObjectInfoTable [ id ];
		
		this->PrintObjectID ( stream, "%s, ", objectInfo.mUserTableID );
		this->PrintObjectID ( stream, "%s, ", objectInfo.mMemberTableID );
		this->PrintObjectID ( stream, "%s", objectInfo.mInitTableID );
		
		stream.Print ( objectInfo.mUserTableID == NULL_OBJ_ID ? ")" : " )" );
		stream.Print ( " --%s\n", classname );
	}
}

//----------------------------------------------------------------//
void MOAISerializer::WriteTableDecls ( ZLStream& stream ) {

	if ( !this->mTableMap.size ()) return;

	stream.Print ( "\t--Declaring Tables\n" );

	TableMapIt tableIt = this->mTableMap.begin ();
	for ( ; tableIt != this->mTableMap.end (); ++tableIt ) {
		ObjID tableID = tableIt->first;
		stream.Print ( "\t[ 0x%02X ] = {},\n", tableID );
	}
	
	stream.Print ( "\n" );
}

//----------------------------------------------------------------//
u32 MOAISerializer::WriteTableInitializer ( ZLStream& stream, MOAILuaState& state, int idx, cc8* prefix ) {

	u32 count = 0;
	u32 itr = state.PushTableItr ( idx );
	while ( state.TableItrNext ( itr )) {
		
		int keyType = lua_type ( state, -2 );
		int valType = lua_type ( state, -1 );
		cc8* keyName = lua_tostring ( state, -2 );
		
		switch ( valType ) {
			case LUA_TNONE:
			case LUA_TNIL:
			case LUA_TFUNCTION:
			case LUA_TTHREAD:
				continue;
		}
		
		// TODO: warn about unsupported key types
		switch ( keyType ) {
		
			case LUA_TSTRING: {
				if ( MOAISerializer::IsSimpleStringKey ( keyName )) {
					stream.Print ( "\t%s.%s = ", prefix, keyName );
				}
				else {
					stream.Print ( "\t%s [ \'%s\' ] = ", prefix, keyName );
				}
				break;
			}
			case LUA_TNUMBER: {
				stream.Print ( "\t%s [ %s ]\t= ", prefix, keyName );
				break;
			}
		};
		
		switch ( valType ) {
			
			case LUA_TBOOLEAN: {
				int value = lua_toboolean ( state, -1 );
				cc8* str = ( value ) ? "true": "false";
				stream.Print ( "%s\n", str );
				break;
			}
			case LUA_TTABLE: {
				ObjID tableID = this->GetID ( state, -1 );
				if ( this->mTableMap.contains ( tableID )) {
					stream.Print ( "objects [ 0x%02X ]\n", tableID );
				}
				break;
			}
			case LUA_TSTRING: {
				size_t len;
				cc8* rawStr = lua_tolstring ( state, -1, &len );
				STLString str = MOAISerializer::EscapeString ( rawStr, len );
				stream.Print ( "%s\n", str.c_str ());
				break;
			}
			case LUA_TNUMBER: {
				stream.Print ( "%s\n", lua_tostring ( state, -1 ));
				break;
			}
			case LUA_TUSERDATA: {
				MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( -1, false );
				ObjID instanceID = this->GetID ( object );
				stream.Print ( "objects [ 0x%02X ]\n", instanceID );
				break;
			}
			case LUA_TLIGHTUSERDATA: {
				stream.Print ( "%p,\n", lua_touserdata ( state, -1 ));
				break;
			}
		};
		
		++count;
	}
	
	return count;
}

//----------------------------------------------------------------//
void MOAISerializer::WriteTableInits ( ZLStream& stream ) {

	if ( !this->mTableMap.size ()) return;

	stream.Print ( "\t--Initializing Tables\n" );
	stream.Print ( "\tlocal table\n\n" );

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	TableMapIt tableIt = this->mTableMap.begin ();
	for ( ; tableIt != this->mTableMap.end (); ++tableIt ) {
		
		ObjID tableID = tableIt->first;
		stream.Print ( "\ttable = objects [ 0x%02X ]\n", tableID );
		
		MOAILuaStrongRef& tableRef = tableIt->second;
		state.Push ( tableRef );
		this->WriteTableInitializer ( stream, state, -1, "table" );
		state.Pop ( 1 );
		
		stream.Print ( "\n" );
	}
}
