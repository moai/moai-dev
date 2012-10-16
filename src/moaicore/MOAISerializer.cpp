// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAISerializer.h>
#include <moaicore/MOAILuaState.h>
#include <moaicore/MOAILuaStateHandle.h>
#include <moaicore/MOAILuaObject.h>
#include <moaicore/MOAILuaRuntime.h>
#include <moaicore/MOAILuaRef.h>
#include <moaicore/MOAILuaState-impl.h>

//================================================================//
// MOAISerializer
//================================================================//

//----------------------------------------------------------------//
/**	@name	exportToFile
	@text	Exports the contents of the serializer to a file.

	@in		MOAISerializer self
	@in		string filename
	@out	nil
*/
int MOAISerializer::_exportToFile ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISerializer, "US" )
	
	self->SerializeToFile ( lua_tostring ( state, 2 ));
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	exportToString
	@text	Exports the contents of the serializer to a string.

	@in		MOAISerializer self
	@out	string result
*/
int MOAISerializer::_exportToString ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISerializer, "U" )
	
	STLString retStr = self->SerializeToString ();
	
	lua_pushstring ( L, retStr.str() );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	serialize
	@text	Adds a table or object to the serializer.

	@overload

		@in		MOAISerializer self
		@in		table data				The table to serialize.
		@out	nil
	
	@overload

		@in		MOAISerializer self
		@in		MOAILuaObject data		The object to serialize.
		@out	nil
*/
int MOAISerializer::_serialize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAISerializer, "U" )

	if ( state.IsType ( 2, LUA_TTABLE ) || state.IsType ( 2, LUA_TUSERDATA )) {
		self->AddLuaReturn ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	serializeToFile
	@text	Serializes the specified table or object to a file.

	@overload

		@in		string filename			The file to create.
		@in		table data				The table to serialize.
		@out	nil
	
	@overload

		@in		string filename			The file to create.
		@in		MOAILuaObject data			The object to serialize.
		@out	nil
*/
int MOAISerializer::_serializeToFile ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !( state.IsType ( 1, LUA_TSTRING ))) return 0;
	if ( !( state.IsType ( 2, LUA_TTABLE ) || state.IsType ( 2, LUA_TUSERDATA ))) return 0;

	cc8* filename = state.GetValue < cc8* >( 1, "" );

	MOAISerializer serializer;
	serializer.AddLuaReturn ( state, 2 );
	serializer.SerializeToFile ( filename );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	serializeToString
	@text	Serializes the specified table or object to a string.

	@overload

		@in		table data				The table to serialize.
		@out	string serialized		The serialized string.
	
	@overload

		@in		MOAILuaObject data			The object to serialize.
		@out	string serialized		The serialized string.
*/
int MOAISerializer::_serializeToString ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !( state.IsType ( 1, LUA_TTABLE ) || state.IsType ( 1, LUA_TUSERDATA ))) return 0;

	MOAISerializer serializer;
	serializer.AddLuaReturn ( state, 1 );
	STLString result = serializer.SerializeToString ();

	lua_pushstring ( state, result );

	return 1;
}

//================================================================//
// MOAISerializer
//================================================================//

//----------------------------------------------------------------//
static STLString _escapeString ( cc8* str ) {
	
	u32 len = ( u32 )strlen ( str );

	STLString outStr;
	outStr.reserve ( len * 2 );

	for ( u32 i = 0; i < len; ++i ) {
		char c = str [ i ];
		if ( c == '\\' ) {
			outStr.append ( "\\\\" );
		}
		else if ( c == '"' ) {
			outStr.append ( "\\\"" );
		}
		else {
			outStr.push_back ( c );
		}
	}
	return outStr;
}

//----------------------------------------------------------------//
void MOAISerializer::AddLuaReturn ( MOAILuaObject* object ) {

	MOAILuaStateHandle state = object->GetSelf ();
	return this->AddLuaReturn ( state, -1 );
}

//----------------------------------------------------------------//
void MOAISerializer::AddLuaReturn ( MOAILuaState& state, int idx ) {

	uintptr memberID = this->AffirmMemberID ( state, idx );
	this->mReturnList.push_back ( memberID );
}

//----------------------------------------------------------------//
uintptr MOAISerializer::AffirmMemberID ( MOAILuaObject* object ) {

	MOAILuaStateHandle state = object->GetSelf ();
	return this->AffirmMemberID ( state, -1 );
}

//----------------------------------------------------------------//
uintptr MOAISerializer::AffirmMemberID ( MOAILuaState& state, int idx ) {

	idx = state.AbsIndex ( idx );
	uintptr memberID = ( uintptr )lua_topointer ( state, idx );

	// bail if the table's already been added
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
		
		entry.mLuaRef.SetStrongRef ( state, idx );
		entry.mObject = object;
		entry.mClassName = classname;
		
		this->mPending.push_back ( memberID );
		
		if ( !object->IsSingleton ()) {
			object->PushMemberTable ( state );
			this->AffirmMemberID ( state, -1 );
			state.Pop ( 1 );
		}
	}
	else if ( state.IsType ( idx, LUA_TTABLE )) {
		
		this->mTableMap [ memberID ].SetStrongRef ( state, idx );
		
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

	this->mPending.clear ();
	this->mReturnList.clear ();
}

//----------------------------------------------------------------//
cc8* MOAISerializer::GetDeserializerTypeName () {

	return "MOAIDeserializer";
}

//----------------------------------------------------------------//
void MOAISerializer::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "serializeToFile",	_serializeToFile },
		{ "serializeToString",	_serializeToString },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISerializer::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "exportToFile",		_exportToFile },
		{ "exportToString",		_exportToString },
		{ "serialize",			_serialize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAISerializer::SerializeToString () {
	
	USMemStream memStream;
	this->SerializeToStream ( memStream );
	memStream.Seek ( 0, SEEK_SET );
	
	return memStream.ReadString ( memStream.GetLength ());
}

//----------------------------------------------------------------//
void MOAISerializer::SerializeToFile ( cc8* filename ) {

	USFileStream fileStream;
	fileStream.OpenWrite ( filename );
		
	this->SerializeToStream ( fileStream );
}
	
//----------------------------------------------------------------//
void MOAISerializer::SerializeToStream ( USStream& stream ) {
	
	stream.Print ( "%s\n", this->GetFileMagic ());
	stream.Print ( "serializer = ... or %s.new ()\n", this->GetDeserializerTypeName ());
	
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
	
	this->WriteReturnList ( stream );
}

//----------------------------------------------------------------//
MOAISerializer::MOAISerializer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAISerializerBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISerializer::~MOAISerializer () {
}

//----------------------------------------------------------------//
void MOAISerializer::WriteDecls ( USStream& stream ) {

	stream.Print ( "--Declaring Objects\n" );
	stream.Print ( "local objects = {\n\n" );
	
	this->WriteTableDecls ( stream );
	this->WriteObjectDecls ( stream );
	
	stream.Print ( "}\n\n" );
}

//----------------------------------------------------------------//
void MOAISerializer::WriteObjectDecls ( USStream& stream ) {

	if ( !this->mObjectMap.size ()) return;

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

	stream.Print ( "\t--Declaring Instances\n" );
	ObjectMapIt objectIt;
	objectIt = this->mObjectMap.begin ();
	for ( ; objectIt != this->mObjectMap.end (); ++objectIt ) {
		
		uintptr id = objectIt->first;
		MOAISerializerObjectEntry& entry = objectIt->second;
		
		MOAILuaObject* object = entry.mObject;
		cc8* classname = entry.mClassName;
		
		if ( !object->IsSingleton ()) {
			stream.Print ( "\t[ 0x%08X ] = serializer:registerObjectID ( %s.new (), 0x%08X ),\n", id, classname, id );
		}
	}
	stream.Print ( "\n" );
}

//----------------------------------------------------------------//
void MOAISerializer::WriteObjectInits ( USStream& stream ) {
	
	if ( !this->mPending.size ()) return;
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	while ( this->mPending.size ()) {
		
		uintptr id = this->mPending.front ();
		this->mPending.pop_front ();
		
		if ( !this->mObjectMap.contains ( id )) continue;
		MOAISerializerObjectEntry& entry = this->mObjectMap [ id ];
		
		MOAILuaObject* object = entry.mObject;
		cc8* classname = entry.mClassName;
		
		stream.Print ( "\t--%s\n", classname );
		stream.Print ( "\tserializer:initObject (\n" );
		
		if ( object->IsSingleton ()) {
			stream.Print ( "\t\t%s,\n", classname );
			stream.Print ( "\t\tnil,\n" );
		}
		else {
			stream.Print ( "\t\tobjects [ 0x%08X ],\n", id );
			
			object->PushMemberTable ( state );
			stream.Print ( "\t\tobjects [ 0x%08X ],\n", this->AffirmMemberID ( state, -1 ));
			state.Pop ( 1 );
		}
		
		// this should fill the table for the class
		lua_newtable ( state );
		object->SerializeOut ( state, *this );
		
		stream.Print ( "\t\t{" );
		
		if ( this->WriteTable ( stream, state, -1, 3 )) {
			stream.Print ( "\t\t}\n" );
		}
		else {
			stream.Print ( "}\n" );
		}
		state.Pop ( 1 );
		
		stream.Print ( "\t)\n\n" );
	}
}

//----------------------------------------------------------------//
void MOAISerializer::WriteReturnList ( USStream& stream ) {

	if ( !this->mReturnList.size ()) return;
	
	stream.Print ( "\n" );
	stream.Print ( "--Returning Tables\n" );
	stream.Print ( "return " );
	
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	ReturnListIt returnListIt = this->mReturnList.begin ();
	for ( ; returnListIt != this->mReturnList.end (); ++returnListIt ) {
		uintptr id = *returnListIt;
		
		if ( returnListIt != this->mReturnList.begin ()) {
			stream.Print ( ", " );
		}
		
		if ( this->mObjectMap.contains ( id )) {
		
			MOAISerializerObjectEntry& entry = this->mObjectMap [ id ];
			
			MOAILuaObject* object = entry.mObject;
			cc8* classname = entry.mClassName;
			
			if ( object->IsSingleton ()) {
				stream.Print ( "%s", classname );
				id = 0;
			}
		}
		
		if ( id ) {
			stream.Print ( "objects [ 0x%08X ]", id );
		}
	}
	stream.Print ( "\n" );
}

//----------------------------------------------------------------//
u32 MOAISerializer::WriteTable ( USStream& stream, MOAILuaState& state, int idx, u32 tab ) {

	STLString indent;
	
	for ( u32 i = 0; i < tab; ++i ) {
		indent.append ( "\t" );
	}
	
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
			case LUA_TUSERDATA:
			case LUA_TTHREAD:
				continue;
		}
		
		if ( count == 0 ) {
			stream.Print ( "\n" );
		}
		
		switch ( keyType ) {
		
			case LUA_TSTRING: {
				stream.Print ( "%s[ \"%s\" ] = ", indent.c_str (), keyName );
				break;
			}
			case LUA_TNUMBER: {
				stream.Print ( "%s[ %s ]\t= ", indent.c_str (), keyName );
				break;
			}
		};
		
		switch ( valType ) {
			
			case LUA_TBOOLEAN: {
				int value = lua_toboolean ( state, -1 );
				cc8* str = ( value ) ? "true": "false";
				stream.Print ( "%s,\n", str );
				break;
			}
			case LUA_TTABLE: {
				
				uintptr tableID = ( uintptr )lua_topointer ( state, -1 );
				if ( this->mTableMap.contains ( tableID )) {
					stream.Print ( "objects [ 0x%08X ],\n", tableID );
				}
				else {
					stream.Print ( "{" );
					if ( this->WriteTable ( stream, state, -1, tab + 1 )) {
						stream.Print ( "%s},\n", indent.c_str ());
					}
					else {
						stream.Print ( "},\n" );
					}
				}
				break;
			}
			case LUA_TSTRING: {
				STLString str = _escapeString ( lua_tostring ( state, -1 ));
				stream.Print ( "\"%s\",\n", str.c_str ());
				break;
			}
			case LUA_TNUMBER: {
				stream.Print ( "%s,\n", lua_tostring ( state, -1 ));
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
void MOAISerializer::WriteTableDecls ( USStream& stream ) {

	if ( !this->mTableMap.size ()) return;

	stream.Print ( "\t--Declaring Tables\n" );

	TableMapIt tableIt = this->mTableMap.begin ();
	for ( ; tableIt != this->mTableMap.end (); ++tableIt ) {
		uintptr tableID = tableIt->first;
		stream.Print ( "\t[ 0x%08X ] = {},\n", tableID );
	}
	
	stream.Print ( "\n" );
}

//----------------------------------------------------------------//
u32 MOAISerializer::WriteTableInitializer ( USStream& stream, MOAILuaState& state, int idx, cc8* prefix ) {

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
			case LUA_TUSERDATA:
			case LUA_TTHREAD:
				continue;
		}
		
		switch ( keyType ) {
		
			case LUA_TSTRING: {
				stream.Print ( "\t%s [ \"%s\" ] = ", prefix, keyName );
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
				uintptr tableID = ( uintptr )lua_topointer ( state, -1 );
				if ( this->mTableMap.contains ( tableID )) {
					stream.Print ( "objects [ 0x%08X ]\n", tableID );
				}
				break;
			}
			case LUA_TSTRING: {
				STLString str = _escapeString ( lua_tostring ( state, -1 ));
				stream.Print ( "\"%s\"\n", str.c_str ());
				break;
			}
			case LUA_TNUMBER: {
				stream.Print ( "%s\n", lua_tostring ( state, -1 ));
				break;
			}
			case LUA_TUSERDATA: {
				MOAILuaObject* object = state.GetLuaObject < MOAILuaObject >( -1, false );
				u32 instanceID = this->GetID ( object );
				stream.Print ( "objects [ 0x%08X ]\n", instanceID );
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
void MOAISerializer::WriteTableInits ( USStream& stream ) {

	if ( !this->mTableMap.size ()) return;

	stream.Print ( "\t--Initializing Tables\n" );
	stream.Print ( "\tlocal table\n\n" );

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

	TableMapIt tableIt = this->mTableMap.begin ();
	for ( ; tableIt != this->mTableMap.end (); ++tableIt ) {
		
		uintptr tableID = tableIt->first;
		stream.Print ( "\ttable = objects [ 0x%08X ]\n", tableID );
		
		MOAILuaRef& tableRef = tableIt->second;
		state.Push ( tableRef );
		this->WriteTableInitializer ( stream, state, -1, "table" );
		state.Pop ( 1 );
		
		stream.Print ( "\n" );
	}
}
