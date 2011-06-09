// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USBase64Cipher.h>
#include <uslscore/USByteStream.h>
#include <uslscore/USCipherStream.h>
#include <uslscore/USDeflater.h>
#include <uslscore/USFileStream.h>
#include <uslscore/USInflater.h>
#include <uslscore/USLuaObject.h>
#include <uslscore/USLuaRef.h>
#include <uslscore/USLuaRuntime.h>
#include <uslscore/USLuaState.h>
#include <uslscore/USLuaState-impl.h>
#include <uslscore/USMemStream.h>

//================================================================//
// local
//================================================================//

#define LEVELS1	12	// size of the first part of the stack
#define LEVELS2	10	// size of the second part of the stack

//----------------------------------------------------------------//
static void _printStackTrace ( lua_State *L, int level ) {
  
	int firstpart = 1;  /* still before eventual `...' */
	lua_Debug ar;

	printf ( "stack traceback:" );
  
	while ( lua_getstack ( L, level++, &ar )) {
	
		if (level > LEVELS1 && firstpart) {
		
			if ( !lua_getstack ( L, level + LEVELS2, &ar )) {
				level--;
			}
			else {
				// too many levels
				printf ( "\n\t..." );  /* too many levels */
				
				// find last levels */
				while ( lua_getstack ( L, level + LEVELS2, &ar ))  
				level++;
			}
			firstpart = 0;
			continue;
		}
	
		printf ( "\n\t" );
		
		lua_getinfo ( L, "Snl", &ar );
		
		printf ( "%s:", ar.short_src );
		
		if ( ar.currentline > 0 ) {
			printf ( "%d:", ar.currentline );
		}
		
		if (*ar.namewhat != '\0' ) {
			printf ( " in function " LUA_QS, ar.name );
		}
		else {
			if ( *ar.what == 'm' ) {
				printf ( " in main chunk" );
			}
			else if ( *ar.what == 'C' || *ar.what == 't' ) {
				printf ( " ?" );
			}
			else {
				printf ( " in function <%s:%d>", ar.short_src, ar.linedefined );
			}
		}
	}
	
	printf ( "\n" );
}

//================================================================//
// USLuaState
//================================================================//

//----------------------------------------------------------------//
int USLuaState::AbsIndex ( int idx ) {

	if ( idx < 0 ) {
		return lua_gettop ( this->mState ) + idx + 1;
	}
	return idx;
}

//----------------------------------------------------------------//
bool USLuaState::Base64Decode ( int idx ) {

	USBase64Cipher base64;
	return this->Decode ( idx, base64 );
}

//----------------------------------------------------------------//
bool USLuaState::Base64Encode ( int idx ) {

	USBase64Cipher base64;
	return this->Encode ( idx, base64 );
}

//----------------------------------------------------------------//
bool USLuaState::CheckParams ( int idx, cc8* format ) {

	idx = this->AbsIndex ( idx );

	for ( int i = 0; format [ i ]; ++i ) {
	
		int pos = idx + i ;
		if ( pos > this->GetTop ()) return false;
		char c = format [ i ];
		
		switch ( c ) {
		
			// boolean
			case 'B': {
				if ( !this->IsType ( pos, LUA_TBOOLEAN )) return false;
				break;
			}
		
			// coroutine
			case 'C': {
				if ( !this->IsType ( pos, LUA_TTHREAD )) return false;
				break;
			}
		
			// function
			case 'F': {
				if ( !this->IsType ( pos, LUA_TFUNCTION )) return false;
				break;
			}
		
			// light userdata
			case 'L': {
				if ( !this->IsType ( pos, LUA_TLIGHTUSERDATA )) return false;
				break;
			}
		
			// number
			case 'N': {
				if ( !this->IsType ( pos, LUA_TNUMBER )) return false;
				break;
			}
			
			// string
			case 'S': {
				if ( !this->IsType ( pos, LUA_TSTRING )) return false;
				break;
			}
			
			// table
			case 'T': {
				if ( !this->IsType ( pos, LUA_TTABLE )) return false;
				break;
			}
			
			// userdata
			case 'U': {
				if ( !this->IsType ( pos, LUA_TUSERDATA )) return false;
				break;
			}

			// any type
			case '.':
				break;
		}
	}
	
	return true;
}

//----------------------------------------------------------------//
void USLuaState::CopyToTop ( int idx ) {

	lua_pushvalue ( this->mState, idx );
}

//----------------------------------------------------------------//
int USLuaState::DebugCall ( int nArgs, int nResults ) {
	
	#ifdef _DEBUG
	
		int errIdx = this->AbsIndex ( -( nArgs + 1 ));
		
		this->Push ( USLuaRuntime::Get ().mTraceback );
		lua_insert ( this->mState, errIdx );

		int status = lua_pcall ( this->mState, nArgs, nResults, errIdx );

		if ( status ) {
			lua_settop ( this->mState, errIdx - 1 );
		}
		else {
			lua_remove ( this->mState, errIdx );
		}
	
	#else
	
		lua_call ( this->mState, nArgs, nResults );
		int status = 0;
	
	#endif
	
	return status;
}

//----------------------------------------------------------------//
bool USLuaState::Decode ( int idx, USCipher& cipher ) {

	if ( !this->IsType ( idx, LUA_TSTRING )) return false;

	size_t len;
	void* buffer = ( void* )lua_tolstring ( this->mState, idx, &len );
	if ( !len ) return false;
	
	USByteStream cryptStream;
	cryptStream.SetBuffer ( buffer, len );
	cryptStream.SetLength ( len );
	
	USCipherStream cipherStream;
	cipherStream.OpenCipher ( cryptStream, cipher );
	
	USMemStream plainStream;
	plainStream.Pipe ( cipherStream );
	
	cipherStream.CloseCipher ();
	
	len = plainStream.GetLength ();
	buffer = malloc ( len );
	
	plainStream.Seek ( 0, SEEK_SET );
	plainStream.ReadBytes ( buffer, len );
	
	lua_pushlstring ( this->mState, ( cc8* )buffer, len );
	
	free ( buffer );
	
	return true;
}

//----------------------------------------------------------------//
bool USLuaState::Deflate ( int idx, int level, int windowBits ) {

	USDeflater deflater;
	deflater.SetCompressionLevel ( level );
	deflater.SetWindowBits ( windowBits );

	return this->Transform ( idx, deflater );
}

//----------------------------------------------------------------//
bool USLuaState::Encode ( int idx, USCipher& cipher ) {

	if ( !this->IsType ( idx, LUA_TSTRING )) return false;

	size_t len;
	cc8* buffer = lua_tolstring ( this->mState, idx, &len );
	if ( !len ) return false;
	
	USCipherStream cipherStream;
	USMemStream stream;
	
	cipherStream.OpenCipher ( stream, cipher );
	cipherStream.WriteBytes ( buffer, len );
	cipherStream.CloseCipher ();
	
	len = stream.GetLength ();
	void* temp = malloc ( len );
	
	stream.Seek ( 0, SEEK_SET );
	stream.ReadBytes (( void* )temp, len );
	
	lua_pushlstring ( this->mState, ( cc8* )temp, len );
	
	free ( temp );
	
	return true;
}

//----------------------------------------------------------------//
void USLuaState::GetField ( int idx, cc8* name ) {

	lua_getfield ( this->mState, idx, name );
}

//----------------------------------------------------------------//
void USLuaState::GetField ( int idx, int key ) {

	idx = this->AbsIndex ( idx );

	lua_pushinteger ( this->mState, key );
	lua_gettable ( this->mState, idx );
}

//----------------------------------------------------------------//
STLString USLuaState::GetField ( int idx, cc8* key, cc8* value ) {

	STLString str;
	if ( this->GetFieldWithType ( idx, key, LUA_TSTRING )) {
		str = lua_tostring ( this->mState, -1 );
		lua_pop ( this->mState, 1 );
	}
	else {
		str = value;
	}
	return str;
}

//----------------------------------------------------------------//
STLString USLuaState::GetField ( int idx, int key, cc8* value ) {

	STLString str;
	if ( this->GetFieldWithType ( idx, key, LUA_TSTRING )) {
		str = lua_tostring ( this->mState, -1 );
		lua_pop ( this->mState, 1 );
	}
	else {
		str = value;
	}
	return str;
}

//----------------------------------------------------------------//
bool USLuaState::GetFieldWithType ( int idx, cc8* name, int type ) {

	lua_getfield ( this->mState, idx, name );
	if ( lua_type ( this->mState, -1 ) != type ) {
		lua_pop ( this->mState, 1 );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
bool USLuaState::GetFieldWithType ( int idx, int key, int type ) {

	this->GetField ( idx, key );
	if ( lua_type ( this->mState, -1 ) != type ) {
		lua_pop ( this->mState, 1 );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void* USLuaState::GetPtrUserData ( int idx ) {

	void* ptr = 0;

	if ( this->IsType ( idx, LUA_TUSERDATA )) {
		ptr = *( void** )lua_touserdata ( this->mState, idx );
	}
	
	return ptr;
}

//----------------------------------------------------------------//
USLuaRef USLuaState::GetStrongRef ( int idx ) {

	USLuaRef ref;
	ref.SetRef ( *this, idx, false );
	return ref;
}

//----------------------------------------------------------------//
int USLuaState::GetTop () {

	return lua_gettop ( this->mState );
}

//----------------------------------------------------------------//
void* USLuaState::GetUserData ( int idx, void* value ) {

	if ( lua_type ( this->mState, idx ) == LUA_TLIGHTUSERDATA ) {
		return lua_touserdata ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
void* USLuaState::GetUserData ( int idx, cc8* name, void* value ) {

	if ( this->GetFieldWithType ( idx, name, LUA_TLIGHTUSERDATA )) {
		value = lua_touserdata ( this->mState, -1 );
		lua_pop ( this->mState, 1 );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
bool USLuaState::GetValue < bool >( int idx, bool value ) {

	if ( this->IsType ( idx, LUA_TBOOLEAN )) {
		return ( lua_toboolean ( this->mState, idx ) != 0 );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
cc8* USLuaState::GetValue < cc8* >( int idx, cc8* value ) {

	if ( this->IsType ( idx, LUA_TSTRING )) {
		return lua_tostring ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
STLString USLuaState::GetValue ( int idx, cc8* value ) {

	STLString str;
	if ( lua_type ( this->mState, idx ) == LUA_TSTRING ) {
		str = lua_tostring ( this->mState, idx );
	}
	else {
		str = value;
	}
	return str;
}

//----------------------------------------------------------------//
template <>
double USLuaState::GetValue < double >( int idx, double value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
float USLuaState::GetValue < float >( int idx, float value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( float )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
int USLuaState::GetValue < int >( int idx, int value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( int )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
uintptr USLuaState::GetValue < uintptr >( int idx, uintptr value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( uintptr )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u8 USLuaState::GetValue < u8 >( int idx, u8 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u8 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u16 USLuaState::GetValue < u16 >( int idx, u16 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u16 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u32 USLuaState::GetValue < u32 >( int idx, u32 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u32 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u64 USLuaState::GetValue < u64 >( int idx, u64 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u64 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
USLuaRef USLuaState::GetWeakRef ( int idx ) {

	USLuaRef ref;
	ref.SetRef ( *this, idx, true );
	return ref;
}

//----------------------------------------------------------------//
bool USLuaState::HasField ( int idx, cc8* name ) {

	lua_getfield ( this->mState, idx, name );
	bool hasField = ( lua_isnil ( this->mState, -1 ) == false );
	lua_pop ( this->mState, 1 );
	
	return hasField;
}

//----------------------------------------------------------------//
bool USLuaState::HasField ( int idx, int key ) {

	this->GetField ( idx, key );
	bool hasField = ( lua_isnil ( this->mState, -1 ) == false );
	lua_pop ( this->mState, 1 );
	
	return hasField;
}

//----------------------------------------------------------------//
bool USLuaState::HasField ( int idx, cc8* name, int type ) {

	lua_getfield ( this->mState, idx, name );
	bool hasField = ( lua_type ( this->mState, -1 ) == type );
	lua_pop ( this->mState, 1 );
	
	return hasField;
}

//----------------------------------------------------------------//
bool USLuaState::HasField ( int idx, int key, int type ) {

	this->GetField ( idx, key );
	bool hasField = ( lua_type ( this->mState, -1 ) == type );
	lua_pop ( this->mState, 1 );
	
	return hasField;
}

//----------------------------------------------------------------//
bool USLuaState::Inflate ( int idx, int windowBits ) {

	USInflater inflater;
	inflater.SetWindowBits ( windowBits );
	
	return this->Transform ( idx, inflater );
}

//----------------------------------------------------------------//
bool USLuaState::IsNil () {

	return ( !this->mState );
}

//----------------------------------------------------------------//
bool USLuaState::IsTableOrUserdata ( int idx ) {

	int check = lua_type ( this->mState, idx );
	return (( check == LUA_TTABLE ) || ( check == LUA_TUSERDATA ));
}

//----------------------------------------------------------------//
bool USLuaState::IsType ( int idx, int type ) {

	return ( lua_type ( this->mState, idx ) == type );
}

//----------------------------------------------------------------//
bool USLuaState::IsType ( int idx, cc8* name, int type ) {
	
	return this->HasField ( idx, name, type );
}

//----------------------------------------------------------------//
void USLuaState::LoadLibs () {

	luaL_openlibs ( this->mState );
}

//----------------------------------------------------------------//
void USLuaState::MoveToTop ( int idx ) {

	// moves a stack element to the top (removing it from the previous location)
	idx = this->AbsIndex ( idx );				// adjusted index after copying element to top
	lua_pushvalue ( this->mState, idx );	// copy element to top of stack
	lua_remove ( this->mState, idx );		// remove original copy from stack
}

//----------------------------------------------------------------//
void USLuaState::Pop ( int n ) {

	lua_pop ( this->mState, n );
}

//----------------------------------------------------------------//
bool USLuaState::PrepMemberFunc ( int idx, cc8* name ) {

	idx = this->AbsIndex ( idx );
	
	if ( !this->GetFieldWithType ( idx, name, LUA_TFUNCTION )) return false;
	this->CopyToTop ( idx );
	
	return true;
}

//----------------------------------------------------------------//
bool USLuaState::PrintErrors ( int status ) {

	if ( status != 0 ) {
	
		cc8* error = lua_tostring ( this->mState, -1 );
		if ( error ) {
			STLString msg = lua_tostring ( this->mState, -1 );
			printf ( "-- %s\n", msg.c_str ());
		}
		lua_pop ( this->mState, 1 ); // pop error message
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void USLuaState::PrintStackTrace ( int level ) {

	_printStackTrace ( this->mState, level  );
}

//----------------------------------------------------------------//
void USLuaState::Push ( bool value ) {

	lua_pushboolean ( this->mState, value ? 1 : 0 );
}

//----------------------------------------------------------------//
void USLuaState::Push ( cc8* value ) {

	lua_pushstring ( this->mState, value );
}

//----------------------------------------------------------------//
void USLuaState::Push ( double value ) {

	lua_pushnumber ( this->mState, value );
}

//----------------------------------------------------------------//
void USLuaState::Push ( float value ) {

	lua_pushnumber ( this->mState, value );
}

//----------------------------------------------------------------//
void USLuaState::Push ( int value ) {

	lua_pushnumber ( this->mState, value );
}

//----------------------------------------------------------------//
void USLuaState::Push ( u16 value ) {

	lua_pushnumber ( this->mState, value );
}

//----------------------------------------------------------------//
void USLuaState::Push ( u32 value ) {

	lua_pushnumber ( this->mState, value );
}

//----------------------------------------------------------------//
void USLuaState::Push ( u64 value ) {

	lua_pushnumber ( this->mState, ( double )value );
}

//----------------------------------------------------------------//
void USLuaState::Push ( lua_CFunction value ) {

	lua_pushcfunction ( this->mState, value );
}

//----------------------------------------------------------------//
void USLuaState::Push ( USLuaRef& ref ) {

	ref.PushRef ( *this );
}

//----------------------------------------------------------------//
void USLuaState::PushPtrUserData ( void* ptr ) {

	void** handle = ( void** )lua_newuserdata ( this->mState, sizeof ( void* ));
	assert ( handle );
	( *handle ) = ptr;
}

//----------------------------------------------------------------//
int USLuaState::PushTableItr ( int idx ) {

	int itr = this->AbsIndex ( idx );

	lua_pushnil ( this->mState );
	lua_pushnil ( this->mState );
	lua_pushnil ( this->mState );
	
	return itr;
}

//----------------------------------------------------------------//
void USLuaState::RegisterModule ( cc8* name, lua_CFunction loader, bool autoLoad ) {

	lua_getglobal ( this->mState, "package" );
	lua_getfield ( this->mState, -1, "preload" );

	lua_pushstring ( this->mState, name );
	lua_pushcfunction ( this->mState, loader );
	lua_settable ( this->mState, -3 );
	
	// pop 'preload'
	lua_pop ( this->mState, 1 );
	
	if ( autoLoad ) {
	
		lua_getfield ( this->mState, -1, "loaded" );
		
		// push the name
		lua_pushstring ( this->mState, name );
		
		// push the table
		lua_pushcfunction ( this->mState, loader );
		lua_pushstring ( this->mState, name );
		lua_pcall ( this->mState, 1, 1, 0 );
		
		// loaded [ name ] = table
		lua_settable ( this->mState, -3 );
		
		// pop 'loaded'
		lua_pop ( this->mState, 1 );
	}

	// pop 'package'
	lua_pop ( this->mState, 1 );
}

//----------------------------------------------------------------//
int USLuaState::RelIndex ( int idx ) {

	if ( idx > 0 ) {
		return idx - lua_gettop ( this->mState );
	}
	return idx;
}

//----------------------------------------------------------------//
void USLuaState::SetPath ( cc8* path ) {

	int top = lua_gettop ( this->mState );

	lua_getglobal ( this->mState, "package" );
	int packageIdx = lua_gettop ( this->mState );
	
	lua_pushstring ( this->mState, "path" );
	lua_pushstring ( this->mState, path );
	lua_settable ( this->mState, packageIdx );
	
	lua_settop ( this->mState, top );
}

//----------------------------------------------------------------//
void USLuaState::SetTop ( int top ) {

	lua_settop ( this->mState, top );
}

//----------------------------------------------------------------//
bool USLuaState::TableItrNext ( int itr ) {

	// pop the prev key/value; leave the key
	lua_pop ( this->mState, 2 );
	
	if ( lua_next ( this->mState, itr ) != 0 ) {
		this->CopyToTop ( -2 );
		this->MoveToTop ( -2 );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool USLuaState::Transform ( int idx, USStreamFormatter& formatter ) {

	if ( !this->IsType ( idx, LUA_TSTRING )) return false;

	size_t len;
	cc8* buffer = lua_tolstring ( this->mState, idx, &len );
	if ( !len ) return false;
	
	USMemStream stream;
	
	formatter.SetStream ( &stream );
	formatter.WriteBytes ( buffer, len );
	formatter.Flush ();
	
	len = stream.GetLength ();
	void* temp = malloc ( len );
	
	stream.Seek ( 0, SEEK_SET );
	stream.ReadBytes (( void* )temp, len );
	
	lua_pushlstring ( this->mState, ( cc8* )temp, len );
	
	free ( temp );
	
	return true;
}

//----------------------------------------------------------------//
USLuaState::USLuaState () :
	mState ( 0 ) {
}

//----------------------------------------------------------------//
USLuaState::USLuaState ( lua_State* state ) :
	mState ( state ) {
}

//----------------------------------------------------------------//
USLuaState::~USLuaState () {
}

//----------------------------------------------------------------//
int USLuaState::YieldThread ( int nResults ) {

	return lua_yield ( this->mState, nResults );
}
