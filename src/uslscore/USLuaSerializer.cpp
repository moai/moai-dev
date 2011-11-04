// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USFilename.h>
#include <uslscore/USFileStream.h>
#include <uslscore/USFileSys.h>
#include <uslscore/USDirectoryItr.h>
#include <uslscore/USLog.h>

#include <uslscore/USLuaSerializer.h>
#include <uslscore/USLuaState.h>
#include <uslscore/USLuaStateHandle.h>
#include <uslscore/USLuaObject.h>
#include <uslscore/USLuaRuntime.h>
#include <uslscore/USLuaRef.h>

#include <uslscore/USLuaState-impl.h>
#include <uslscore/USMemStream.h>

#include <uslscore/USRtti.h>

//----------------------------------------------------------------//
int USLuaSerializer::_deserialize ( lua_State* L ) {
	LUA_SETUP ( USLuaSerializer, "UUTT" );

	USLuaObject* object = state.GetLuaObject < USLuaObject >( 2 );
	if ( !object ) return 0;

	object->SetPrivateTable ( state, 3 );
	object->SerializeIn ( state, *self );

	return 0;
}

//----------------------------------------------------------------//
int USLuaSerializer::_exportToFile ( lua_State* L ) {
	LUA_SETUP ( USLuaSerializer, "US" )
	
	self->SerializeToFile ( lua_tostring ( state, 2 ));
	
	return 0;
}

//----------------------------------------------------------------//
int USLuaSerializer::_exportToString ( lua_State* L ) {
	LUA_SETUP ( USLuaSerializer, "U" )
	
	STLString retStr = self->SerializeToString ();
	
	lua_pushstring ( L, retStr.str() );

	return 1;
}

//----------------------------------------------------------------//
int USLuaSerializer::_register ( lua_State* L ) {
	LUA_SETUP ( USLuaSerializer, "UUN" );

	USLuaObject* object = state.GetLuaObject < USLuaObject >( 2 );
	if ( !object ) return 0;

	uintptr id = state.GetValue < uintptr >( 3, 0 );
	self->Register ( object, id );

	lua_pushvalue ( state, 2 );

	return 1;
}

//----------------------------------------------------------------//
int USLuaSerializer::_serialize ( lua_State* L ) {
	LUA_SETUP ( USLuaSerializer, "U" )

	USLuaObject* object = state.GetLuaObject < USLuaObject >( 2 );
	if ( object ) {
		self->AddLuaReturn ( object );
	}
	else if ( state.IsType ( 2, LUA_TTABLE )) {
		self->AddLuaReturn ( state, 2 );
	}
	
	return 0;
}

//================================================================//
// USLuaSerializer
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
		else {
			outStr.push_back ( c );
		}
	}
	return outStr;
}

//----------------------------------------------------------------//
void USLuaSerializer::AddLuaReturn ( USLuaObject* object ) {

	uintptr memberID = this->AffirmMemberID ( object );

	if ( this->mObjectMap.contains ( memberID )) {
		this->mReturnList.push_back ( memberID );
	}
}

//----------------------------------------------------------------//
void USLuaSerializer::AddLuaReturn ( USLuaState& state, int idx ) {

	uintptr memberID = this->AffirmMemberID ( state, idx );
	
	if ( this->mTableMap.contains ( memberID )) {
		this->mReturnList.push_back ( memberID );
	}
}

//----------------------------------------------------------------//
uintptr USLuaSerializer::AffirmMemberID ( USLuaObject* object ) {

	uintptr memberID = this->GetID ( object );
	
	this->Register ( object, memberID );
	return memberID;
}

//----------------------------------------------------------------//
uintptr USLuaSerializer::AffirmMemberID ( USLuaState& state, int idx ) {

	// if we're an object, affirm as such...
	if ( state.IsType ( idx, LUA_TUSERDATA )) {
		return this->AffirmMemberID ( state.GetLuaObject < USLuaObject >( -1 ));
	}

	// bail if we're not a table
	if ( !state.IsType ( idx, LUA_TTABLE )) return 0;

	// get the table's address
	uintptr memberID = ( uintptr )lua_topointer ( state, idx );
	
	// bail if the table's already been added
	if ( this->mTableMap.contains ( memberID )) return memberID;

	// add the ref now to avoid cycles
	this->mTableMap [ memberID ].SetStrongRef ( state, idx );

	// follow the table's refs to make sure everything gets added
	u32 itr = state.PushTableItr ( idx );
	while ( state.TableItrNext ( itr )) {
		this->AffirmMemberID ( state, -1 );
	}
	
	return memberID;
}

//----------------------------------------------------------------//
void USLuaSerializer::Clear () {

	this->mPending.clear ();
	this->mObjectMap.clear ();
	this->mTableMap.clear ();
	this->mReturnList.clear ();
}

//----------------------------------------------------------------//
cc8* USLuaSerializer::GetFileMagic () {
	return "--LUA";
}

//----------------------------------------------------------------//
uintptr USLuaSerializer::GetID ( USLuaObject* object ) {

	return ( uintptr )object;
}

//----------------------------------------------------------------//
uintptr USLuaSerializer::GetID ( USLuaState& state, int idx ) {

	return ( uintptr )lua_topointer ( state, idx );
}

//----------------------------------------------------------------//
u32 USLuaSerializer::IsLuaFile ( cc8* filename ) {

	ZIPFSFILE* file = ( ZIPFSFILE* )zipfs_fopen ( filename, "r" );
	if ( !file ) return LOAD_ERROR;
	
	char magic [ 256 ];
	char* str = zipfs_fgets ( magic, 6, file );
	UNUSED ( str );
	zipfs_fclose ( file );
	
	if ( strcmp ( magic, this->GetFileMagic ()) != 0 ) return INVALID_FILE;
	
	return SUCCESS;
}

//----------------------------------------------------------------//
USLuaObject* USLuaSerializer::MemberIDToObject ( uintptr objectID ) {

	if ( this->mObjectMap.contains ( objectID )) {
		return this->mObjectMap [ objectID ];
	}
	return 0;
}

//----------------------------------------------------------------//
void USLuaSerializer::Register ( USLuaObject* object, uintptr id ) {

	if ( !this->mObjectMap.contains ( id )) {
	
		this->mObjectMap [ id ] = object;
		
		if ( object ) {
			this->mPending.push_back ( object );
			
			USLuaStateHandle state = USLuaRuntime::Get ().State ();
			object->PushPrivateTable ( state );
			this->AffirmMemberID ( state, -1 );
		}
	}
}

//----------------------------------------------------------------//
void USLuaSerializer::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void USLuaSerializer::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "deserialize",		_deserialize },
		{ "exportToFile",		_exportToFile },
		{ "exportToString",		_exportToString },
		{ "register",			_register },
		{ "serialize",			_serialize },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
u32 USLuaSerializer::SerializeFromFile ( cc8* filename ) {
	
	int status;
	USLuaStateHandle state = USLuaRuntime::Get ().State ();

	// load the lua file
	status = luaL_loadfile ( state, filename );
	if ( state.PrintErrors ( USLog::CONSOLE, status )) return LOAD_ERROR;
	
	// load self as the func param
	this->PushLuaUserdata ( state );
	
	// call the chunk
	if ( state.DebugCall ( 1, 0 )) return LUA_ERROR;
	
	lua_gc ( state, LUA_GCCOLLECT, 0 );
	
	// done!
	return SUCCESS;
}

//----------------------------------------------------------------//
STLString USLuaSerializer::SerializeToString () {
	
	USMemStream memStream;
	this->SerializeToStream( memStream );
	memStream.Seek ( 0, SEEK_SET );
	return memStream.ToString ( memStream.GetLength ());
}

//----------------------------------------------------------------//
void USLuaSerializer::SerializeToFile ( cc8* filename ) {

	USFileStream fileStream;
	fileStream.OpenWrite ( filename );
		
	this->SerializeToStream ( fileStream );
}
	
//----------------------------------------------------------------//
void USLuaSerializer::SerializeToStream ( USStream& stream ) {
	
	stream.Print ( "%s\n", this->GetFileMagic ());
	stream.Print ( "serializer = ... or %s.new ()\n", this->TypeName ());
	
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
USLuaSerializer::USLuaSerializer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaSerializer )
	RTTI_END
}

//----------------------------------------------------------------//
USLuaSerializer::~USLuaSerializer () {

	this->Clear ();
}

//----------------------------------------------------------------//
void USLuaSerializer::WriteDecls ( USStream& stream ) {

	stream.Print ( "--Declaring Objects\n" );
	stream.Print ( "local objects = {\n\n" );
	
	this->WriteTableDecls ( stream );
	this->WriteObjectDecls ( stream );
	
	stream.Print ( "}\n\n" );
}

//----------------------------------------------------------------//
void USLuaSerializer::WriteObjectDecls ( USStream& stream ) {

	if ( !this->mObjectMap.size ()) return;

	stream.Print ( "\t--Declaring Instances\n" );
	ObjectMapIt objectIt;
	objectIt = this->mObjectMap.begin ();
	for ( ; objectIt != this->mObjectMap.end (); ++objectIt ) {
		
		USLuaObject* object = objectIt->second;
		if ( !object ) continue;
		
		uintptr id = this->GetID ( object );
		
		USLuaClass* type = object->GetLuaClass ();
		if ( !type->IsSingleton ()) {
			stream.Print ( "\t[ 0x%08X ] = serializer:register ( %s.new (), 0x%08X ),\n", id, object->TypeName (), id );
		}
	}
	stream.Print ( "\n" );
}

//----------------------------------------------------------------//
void USLuaSerializer::WriteObjectInits ( USStream& stream ) {
	
	if ( !this->mPending.size ()) return;
	
	while ( this->mPending.size ()) {
	
		USLuaObject* object = this->mPending.front ();
		assert ( object );
		this->mPending.pop_front ();
		
		u32 id = this->GetID ( object );
		stream.Print ( "\t--%s\n", object->TypeName ());
		
		stream.Print ( "\tserializer:deserialize (\n" );
		
		USLuaClass* type = object->GetLuaClass ();
		if ( type->IsSingleton ()) {
			stream.Print ( "\t\t%s.get (),\n", object->TypeName ());
		}
		else {
			stream.Print ( "\t\tobjects [ 0x%08X ],\n", id );
		}
		
		USLuaStateHandle state = USLuaRuntime::Get ().State ();
		
		object->PushPrivateTable ( state );
		stream.Print ( "\t\tobjects [ 0x%08X ],\n", this->AffirmMemberID ( state, -1 ));
		state.Pop ( 1 );
		
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
void USLuaSerializer::WriteReturnList ( USStream& stream ) {

	if ( !this->mReturnList.size ()) return;
	
	stream.Print ( "\n" );
	stream.Print ( "--Returning Tables\n" );
	stream.Print ( "return " );
	
	ReturnListIt returnListIt = this->mReturnList.begin ();
	for ( ; returnListIt != this->mReturnList.end (); ++returnListIt ) {
		
		if ( returnListIt != this->mReturnList.begin ()) {
			stream.Print ( ", " );
		}
		
		u32 id = *returnListIt;
		stream.Print ( "objects [ 0x%08X ]", id );
	}
	stream.Print ( "\n" );
}

//----------------------------------------------------------------//
u32 USLuaSerializer::WriteTable ( USStream& stream, USLuaState& state, int idx, u32 tab ) {

	STLString indent;
	
	for ( u32 i = 0; i < tab; ++i ) {
		indent.append ( "\t" );
	}
	
	u32 count = 0;
	u32 itr = state.PushTableItr ( idx );
	while ( state.TableItrNext ( itr )) {
		
		cc8* keyName = lua_tostring ( state, -2 );
		int keyType = lua_type ( state, -2 );
		int valType = lua_type ( state, -1 );
		
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
void USLuaSerializer::WriteTableDecls ( USStream& stream ) {

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
u32 USLuaSerializer::WriteTableInitializer ( USStream& stream, USLuaState& state, int idx, cc8* prefix ) {

	u32 count = 0;
	u32 itr = state.PushTableItr ( idx );
	while ( state.TableItrNext ( itr )) {
		
		cc8* keyName = lua_tostring ( state, -2 );
		int keyType = lua_type ( state, -2 );
		int valType = lua_type ( state, -1 );
		
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
				USLuaObject* object = state.GetLuaObject < USLuaObject >( -1 );
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
void USLuaSerializer::WriteTableInits ( USStream& stream ) {

	if ( !this->mTableMap.size ()) return;

	stream.Print ( "\t--Initializing Tables\n" );
	stream.Print ( "\tlocal table\n\n" );

	USLuaStateHandle state = USLuaRuntime::Get ().State ();

	TableMapIt tableIt = this->mTableMap.begin ();
	for ( ; tableIt != this->mTableMap.end (); ++tableIt ) {
		
		uintptr tableID = tableIt->first;
		stream.Print ( "\ttable = objects [ 0x%08X ]\n", tableID );
		
		USLuaRef& tableRef = tableIt->second;
		state.Push ( tableRef );
		this->WriteTableInitializer ( stream, state, -1, "table" );
		state.Pop ( 1 );
		
		stream.Print ( "\n" );
	}
}
