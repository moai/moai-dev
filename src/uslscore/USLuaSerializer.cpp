// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USFilename.h>
#include <uslscore/USFileStream.h>
#include <uslscore/USFileSys.h>
#include <uslscore/USDirectoryItr.h>

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
int USLuaSerializer::_initInstance ( lua_State* L ) {
	LUA_SETUP ( USLuaSerializer, "UUT" );

	USLuaObject* object = state.GetLuaObject < USLuaObject >( 2 );
	if ( !object ) return 0;

	object->SerializeIn ( state, *self );

	return 0;
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
		self->Affirm ( object );
		self->AddLuaReturn ( object );
	}
	else if ( state.IsType ( 2, LUA_TTABLE )) {
		self->Affirm ( state, 2 );
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

	uintptr instanceID = this->GetID ( object );

	if ( this->mInstanceMap.contains ( instanceID )) {
		this->mReturnList.push_back ( instanceID );
	}
}

//----------------------------------------------------------------//
void USLuaSerializer::AddLuaReturn ( USLuaState& state, int idx ) {

	uintptr tableID = this->GetID ( state, idx );

	if ( this->mTableMap.contains ( tableID )) {
		this->mReturnList.push_back ( tableID );
	}
}

//----------------------------------------------------------------//
uintptr USLuaSerializer::Affirm ( USLuaObject* object ) {

	uintptr instanceID = this->GetID ( object );
	this->Register ( object, instanceID );
	return instanceID;
}

//----------------------------------------------------------------//
uintptr USLuaSerializer::Affirm ( USLuaState& state, int idx ) {

	// if we're an object, affirm as such...
	if ( state.IsType ( idx, LUA_TUSERDATA )) {
		return this->Affirm ( state.GetLuaObject < USLuaObject >( -1 ));
	}

	// bail if we're not a table
	if ( !state.IsType ( idx, LUA_TTABLE )) return 0;

	// get the table's address
	uintptr tableID = ( uintptr )lua_topointer ( state, idx );
	
	// bail if the table's already been added
	if ( this->mTableMap.contains ( tableID )) return tableID;

	// add the ref now to avoid cycles
	this->mTableMap [ tableID ].SetRef ( state, idx, false );

	// follow the table's refs to make sure everything gets added
	u32 itr = state.PushTableItr ( idx );
	while ( state.TableItrNext ( itr )) {
		this->Affirm ( state, -1 );
	}
	
	return tableID;
}

//----------------------------------------------------------------//
void USLuaSerializer::Clear () {

	this->mPending.clear ();
	this->mInstanceMap.clear ();
	this->mTableMap.clear ();
	this->mReturnList.clear ();
}

//----------------------------------------------------------------//
USLuaObject* USLuaSerializer::Dereference ( USLuaState& state, int idx ) {

	uintptr id = state.GetValue ( idx, 0 );
	
	if ( this->mInstanceMap.contains ( id )) {
		return this->mInstanceMap [ id ];
	}
	return 0;
}

//----------------------------------------------------------------//
USLuaObject* USLuaSerializer::GetRefField ( USLuaState& state, int idx, cc8* name ) {

	if ( state.GetFieldWithType ( idx, name, LUA_TNUMBER )) {
		return this->PopRef ( state );
	}
	return 0;
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

	FILE* file = fopen ( filename, "r" );
	if ( !file ) return LOAD_ERROR;
	
	char magic [ 256 ];
	char* str = fgets ( magic, 6, file );
	UNUSED ( str );
	fclose ( file );
	
	if ( strcmp ( magic, this->GetFileMagic ()) != 0 ) return INVALID_FILE;
	
	return SUCCESS;
}

//----------------------------------------------------------------//
USLuaObject* USLuaSerializer::PopRef ( USLuaState& state ) {

	uintptr id = state.GetValue < uintptr >( -1, 0 );
	state.Pop ( 1 );
	
	if ( this->mInstanceMap.contains ( id )) {
		return this->mInstanceMap [ id ];
	}
	return 0;
}

//----------------------------------------------------------------//
void USLuaSerializer::PushRef ( USLuaState& state, USLuaObject* object ) {

	this->Affirm ( object );
	lua_pushlightuserdata ( state, ( void* )this->GetID ( object ));
}

//----------------------------------------------------------------//
void USLuaSerializer::Register ( USLuaObject* object, uintptr id ) {

	if ( !this->mInstanceMap.contains ( id )) {
	
		this->mInstanceMap [ id ] = object;
		
		if ( object ) {
			this->mPending.push_back ( object );
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
		{ "exportToFile",		_exportToFile },
		{ "exportToString",		_exportToString },
		{ "initInstance",		_initInstance },
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
	if ( state.PrintErrors ( status )) return LOAD_ERROR;
	
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
	this->WriteInstanceInits ( stream );
	stream.Print ( "end\n\n" );
	
	// now write the decls
	this->WriteDecls ( stream );
	
	// call the initializer
	stream.Print ( "init ( objects )\n" );
	
	this->WriteReturnList ( stream );
}

//----------------------------------------------------------------//
void USLuaSerializer::SetRefField ( USLuaState& state, int idx, cc8* name, USLuaObject* object ) {

	if ( state.IsType ( idx, LUA_TTABLE )) {
		
		this->PushRef ( state, object );
		lua_setfield ( state, -2, name );
	}
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
	this->WriteInstanceDecls ( stream );
	
	stream.Print ( "}\n\n" );
}

//----------------------------------------------------------------//
void USLuaSerializer::WriteInstanceDecls ( USStream& stream ) {

	if ( !this->mInstanceMap.size ()) return;

	stream.Print ( "\t--Declaring Instances\n" );
	InstanceMapIt instanceIt;
	instanceIt = this->mInstanceMap.begin ();
	for ( ; instanceIt != this->mInstanceMap.end (); ++instanceIt ) {
		
		USLuaObject* object = instanceIt->second;
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
void USLuaSerializer::WriteInstanceInits ( USStream& stream ) {
	
	if ( !this->mPending.size ()) return;
	
	while ( this->mPending.size ()) {
	
		USLuaObject* object = this->mPending.front ();
		assert ( object );
		this->mPending.pop_front ();
		
		u32 id = this->GetID ( object );
		stream.Print ( "\t--%s\n", object->TypeName ());
		
		USLuaClass* type = object->GetLuaClass ();
		if ( type->IsSingleton ()) {
			stream.Print ( "\tserializer:initInstance ( %s.get (), {", object->TypeName ());
		}
		else {
			stream.Print ( "\tserializer:initInstance ( objects [ 0x%08X ], {", id );
		}
		
		USLuaStateHandle state = USLuaRuntime::Get ().State ();
		lua_newtable ( state );
		
		// this should push the table for the class
		object->SerializeOut ( state, *this );
		
		if ( this->WriteTable ( stream, state, -1, 2 )) {
			stream.Print ( "\t})\n" );
		}
		else {
			stream.Print ( "})\n" );
		}
		
		stream.Print ( "\n" );
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
		
		if ( count == 0 ) {
			stream.Print ( "\n" );
		}
		
		switch ( lua_type ( state, -2 )) {
		
			case LUA_TSTRING: {
				stream.Print ( "%s[ \"%s\" ] = ", indent.c_str (), lua_tostring ( state, -2 ));
				break;
			}
			case LUA_TNUMBER: {
				stream.Print ( "%s[ %s ]\t= ", indent.c_str (), lua_tostring ( state, -2 ));
				break;
			}
		};
		
		switch ( lua_type ( state, -1 )) {
			
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
		
		switch ( lua_type ( state, -2 )) {
		
			case LUA_TSTRING: {
				stream.Print ( "\t%s [ \"%s\" ] = ", prefix, lua_tostring ( state, -2 ));
				break;
			}
			case LUA_TNUMBER: {
				stream.Print ( "\t%s [ %s ]\t= ", prefix, lua_tostring ( state, -2 ));
				break;
			}
		};
		
		switch ( lua_type ( state, -1 )) {
			
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
