// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAILuaObject.h>
#include <moaicore/MOAILuaRef.h>
#include <moaicore/MOAILuaRuntime.h>
#include <moaicore/MOAILuaState.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAILuaState-impl.h>

#define LEVELS1	12	// size of the first part of the stack
#define LEVELS2	10	// size of the second part of the stack

//================================================================//
// MOAILuaState
//================================================================//

//----------------------------------------------------------------//
int MOAILuaState::AbsIndex ( int idx ) {

	if ( idx < 0 ) {
		return lua_gettop ( this->mState ) + idx + 1;
	}
	return idx;
}

//----------------------------------------------------------------//
bool MOAILuaState::Base64Decode ( int idx ) {

	USBase64Reader base64;
	return this->Decode ( idx, base64 );
}

//----------------------------------------------------------------//
bool MOAILuaState::Base64Encode ( int idx ) {

	USBase64Writer base64;
	return this->Encode ( idx, base64 );
}

//----------------------------------------------------------------//
bool MOAILuaState::CheckParams ( int idx, cc8* format, bool verbose ) {

	idx = this->AbsIndex ( idx );

	for ( int i = 0; format [ i ]; ++i ) {
	
		int pos = idx + i ;
		int type = LUA_TNIL;
		int expected = LUA_TNONE;
		
		if ( pos <= this->GetTop ()) {
			type = lua_type ( this->mState, pos );
		}
		
		switch ( format [ i ]) {
		
			// boolean
			case 'B':
				if ( type != LUA_TBOOLEAN ) expected = LUA_TBOOLEAN;
				break;
		
			// coroutine
			case 'C':
				if ( type != LUA_TTHREAD ) expected = LUA_TTHREAD;
				break;
		
			// function
			case 'F':
				if ( type != LUA_TFUNCTION ) expected = LUA_TFUNCTION;
				break;
		
			// light userdata
			case 'L':
				if ( type != LUA_TLIGHTUSERDATA ) expected = LUA_TLIGHTUSERDATA;
				break;
		
			// number
			case 'N':
				if ( type != LUA_TNUMBER ) expected = LUA_TNUMBER;
				break;
			
			// string
			case 'S':
				if ( type != LUA_TSTRING ) expected = LUA_TSTRING;
				break;
			
			// table
			case 'T':
				if ( type != LUA_TTABLE ) expected = LUA_TTABLE;
				break;
			
			// userdata
			case 'U':
				if ( type != LUA_TUSERDATA ) expected = LUA_TUSERDATA;
				break;

			// any type
			case '*':
			case '.':
				break;
		}
		
		if ( expected != LUA_TNONE ) {
		
			if ( verbose ) {
				
				cc8* expectedName = MOAILuaState::GetLuaTypeName ( expected );
				cc8* gotName = MOAILuaState::GetLuaTypeName ( type );
			
				MOAILog ( *this, MOAILogMessages::MOAI_ParamTypeMismatch_DSS, pos, expectedName, gotName );
			}
			return false;
		}
	}
	
	return true;
}

//----------------------------------------------------------------//
void MOAILuaState::ClearField ( int idx, cc8* key ) {

	if ( this->IsTableOrUserdata ( idx )) {
		idx = this->AbsIndex ( idx );
		lua_pushnil ( this->mState );
		lua_setfield ( this->mState, idx, key );
	}
}

//----------------------------------------------------------------//
void MOAILuaState::CloneTable ( int idx ) {
	
	lua_pushvalue ( this->mState, idx );
	idx = lua_gettop ( this->mState );

	if ( !this->IsType ( idx, LUA_TTABLE )) {
		lua_pushnil ( this->mState );
		return;
	}
	
	lua_newtable ( this->mState );
	int tableIdx = idx + 1;
	
	u32 itr = this->PushTableItr ( idx );
	while ( this->TableItrNext ( itr )) {
		lua_pushvalue ( this->mState, -2 );
		lua_pushvalue ( this->mState, -2 );
		lua_settable ( this->mState, tableIdx );
	}
	
	if ( lua_getmetatable ( this->mState, idx )) {
		lua_setmetatable ( this->mState, tableIdx );
	}
	
	lua_replace ( this->mState, idx );
}

//----------------------------------------------------------------//
void MOAILuaState::CopyToTop ( int idx ) {

	lua_pushvalue ( this->mState, idx );
}

//----------------------------------------------------------------//
int MOAILuaState::DebugCall ( int nArgs, int nResults ) {
	
	int status;
	
	if ( MOAILuaRuntime::Get ().mTraceback ) {
	
		int errIdx = this->AbsIndex ( -( nArgs + 1 ));
		
		this->Push ( MOAILuaRuntime::Get ().mTraceback );
		lua_insert ( this->mState, errIdx );

		status = lua_pcall ( this->mState, nArgs, nResults, errIdx );

		if ( status ) {
			lua_settop ( this->mState, errIdx - 1 );
		}
		else {
			lua_remove ( this->mState, errIdx );
		}
	}
	else {
	
		lua_call ( this->mState, nArgs, nResults );
		status = 0;
	}
	return status;
}

//----------------------------------------------------------------//
bool MOAILuaState::Decode ( int idx, USStreamReader& reader ) {

	if ( !this->IsType ( idx, LUA_TSTRING )) return false;

	size_t len;
	void* buffer = ( void* )lua_tolstring ( this->mState, idx, &len );
	if ( !len ) return false;
	
	USByteStream cryptStream;
	cryptStream.SetBuffer ( buffer, len );
	cryptStream.SetLength ( len );
	
	USMemStream plainStream;
	
	reader.Open ( &cryptStream );
	plainStream.WriteStream ( reader );
	reader.Close ();
	
	len = plainStream.GetLength ();
	buffer = malloc ( len );
	
	plainStream.Seek ( 0, SEEK_SET );
	plainStream.ReadBytes ( buffer, len );
	
	lua_pushlstring ( this->mState, ( cc8* )buffer, len );
	
	free ( buffer );
	
	return true;
}

//----------------------------------------------------------------//
bool MOAILuaState::Deflate ( int idx, int level, int windowBits ) {

	USDeflateWriter deflater;
	deflater.SetCompressionLevel ( level );
	deflater.SetWindowBits ( windowBits );

	return this->Encode ( idx, deflater );
}

//----------------------------------------------------------------//
bool MOAILuaState::Encode ( int idx, USStreamWriter& writer ) {

	if ( !this->IsType ( idx, LUA_TSTRING )) return false;

	size_t len;
	cc8* buffer = lua_tolstring ( this->mState, idx, &len );
	if ( !len ) return false;
	
	USMemStream stream;
	
	writer.Open ( &stream );
	writer.WriteBytes ( buffer, len );
	writer.Close ();
	
	len = stream.GetLength ();
	void* temp = malloc ( len );
	
	stream.Seek ( 0, SEEK_SET );
	stream.ReadBytes (( void* )temp, len );
	
	lua_pushlstring ( this->mState, ( cc8* )temp, len );
	
	free ( temp );
	
	return true;
}

//----------------------------------------------------------------//
USBox MOAILuaState::GetBox ( int idx ) {

	USBox box;

	box.mMin.mX = this->GetValue < float >( idx++, 0.0f );
	box.mMin.mY = this->GetValue < float >( idx++, 0.0f );
	box.mMin.mZ = this->GetValue < float >( idx++, 0.0f );
	
	box.mMax.mX = this->GetValue < float >( idx++, 0.0f );
	box.mMax.mY = this->GetValue < float >( idx++, 0.0f );
	box.mMax.mZ = this->GetValue < float >( idx++, 0.0f );
	
	return box;
}

//----------------------------------------------------------------//
USColorVec MOAILuaState::GetColor ( int idx, float r, float g, float b, float a ) {

	USColorVec color;
	color.mR = this->GetValue < float >( idx++, r );
	color.mG = this->GetValue < float >( idx++, g );
	color.mB = this->GetValue < float >( idx++, b );
	color.mA = this->GetValue < float >( idx++, a );
	
	return color;
}

//----------------------------------------------------------------//
u32 MOAILuaState::GetColor32 ( int idx, float r, float g, float b, float a ) {

	USColorVec color = this->GetColor ( idx, r, g, b, a );
	return color.PackRGBA ();
}

//----------------------------------------------------------------//
void MOAILuaState::GetField ( int idx, cc8* name ) {

	lua_getfield ( this->mState, idx, name );
}

//----------------------------------------------------------------//
void MOAILuaState::GetField ( int idx, int key ) {

	idx = this->AbsIndex ( idx );

	lua_pushinteger ( this->mState, key );
	lua_gettable ( this->mState, idx );
}

//----------------------------------------------------------------//
STLString MOAILuaState::GetField ( int idx, cc8* key, cc8* value ) {

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
STLString MOAILuaState::GetField ( int idx, int key, cc8* value ) {

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
STLString MOAILuaState::GetField ( int idx, cc8* key, const STLString& value ) {

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
STLString MOAILuaState::GetField ( int idx, int key, const STLString& value ) {

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
bool MOAILuaState::GetFieldWithType ( int idx, cc8* name, int type ) {

	lua_getfield ( this->mState, idx, name );
	if ( lua_type ( this->mState, -1 ) != type ) {
		lua_pop ( this->mState, 1 );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
bool MOAILuaState::GetFieldWithType ( int idx, int key, int type ) {

	this->GetField ( idx, key );
	if ( lua_type ( this->mState, -1 ) != type ) {
		lua_pop ( this->mState, 1 );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
cc8* MOAILuaState::GetLuaTypeName ( int type ) {

	switch ( type ) {
	
		case LUA_TNONE:				return "none";
		case LUA_TNIL:				return "nil";
		case LUA_TBOOLEAN:			return "boolean";
		case LUA_TLIGHTUSERDATA:	return "lightuserdata";
		case LUA_TNUMBER:			return "number";
		case LUA_TSTRING:			return "string";
		case LUA_TTABLE:			return "table";
		case LUA_TFUNCTION:			return "function";
		case LUA_TUSERDATA:			return "userdata";
		case LUA_TTHREAD:			return "coroutine";
	}
	return "unknown";
}

//----------------------------------------------------------------//
void* MOAILuaState::GetPtrUserData ( int idx ) {

	void* ptr = 0;

	if ( this->IsType ( idx, LUA_TUSERDATA )) {
		ptr = *( void** )lua_touserdata ( this->mState, idx );
	}
	return ptr;
}

//----------------------------------------------------------------//
STLString MOAILuaState::GetStackTrace ( int level ) {

	int firstpart = 1;  /* still before eventual `...' */
	lua_Debug ar;
	
	lua_State* L = this->mState;

	STLString out;
	
	out.append ( "stack traceback:" );
	
	while ( lua_getstack ( L, level++, &ar )) {
		
		if ( level > LEVELS1 && firstpart ) {
			
			if ( !lua_getstack ( L, level + LEVELS2, &ar )) {
				level--;
			}
			else {
				// too many levels
				out.append ( "\n\t..." );  /* too many levels */
				
				// find last levels */
				while ( lua_getstack ( L, level + LEVELS2, &ar ))  
					level++;
			}
			firstpart = 0;
			continue;
		}
		
		out.append ( "\n\t" );
		
		lua_getinfo ( L, "Snl", &ar );
		
		out.append ( ar.short_src );
		
		if ( ar.currentline > 0 ) {
			out.write ( ":%d", ar.currentline );
		}
		
		if ( *ar.namewhat != '\0' ) {
			out.write ( " in function '%s'", ar.name );
		}
		else {
			if ( *ar.what == 'm' ) {
				out.write ( " in main chunk" );
			}
			else if ( *ar.what == 'C' || *ar.what == 't' ) {
				out.write ( " ?" );
			}
			else {
				out.write ( " in function <%s:%d>", ar.short_src, ar.linedefined );
			}
		}
	}
	
	out.append ( "\n" );

	return out;
}

//----------------------------------------------------------------//
MOAILuaRef MOAILuaState::GetStrongRef ( int idx ) {

	MOAILuaRef ref;
	ref.SetStrongRef ( *this, idx );
	return ref;
}

//----------------------------------------------------------------//
int MOAILuaState::GetTop () {

	return lua_gettop ( this->mState );
}

//----------------------------------------------------------------//
void* MOAILuaState::GetUserData ( int idx, void* value ) {

	if ( lua_type ( this->mState, idx ) == LUA_TLIGHTUSERDATA ) {
		return lua_touserdata ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
void* MOAILuaState::GetUserData ( int idx, cc8* name, void* value ) {

	if ( this->GetFieldWithType ( idx, name, LUA_TLIGHTUSERDATA )) {
		value = lua_touserdata ( this->mState, -1 );
		lua_pop ( this->mState, 1 );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
bool MOAILuaState::GetValue < bool >( int idx, bool value ) {

	if ( this->IsType ( idx, LUA_TBOOLEAN )) {
		return ( lua_toboolean ( this->mState, idx ) != 0 );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
cc8* MOAILuaState::GetValue < cc8* >( int idx, cc8* value ) {

	if ( this->IsType ( idx, LUA_TSTRING )) {
		return lua_tostring ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
STLString MOAILuaState::GetValue ( int idx, cc8* value ) {

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
double MOAILuaState::GetValue < double >( int idx, double value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
float MOAILuaState::GetValue < float >( int idx, float value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( float )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
//template <>
//int MOAILuaState::GetValue < int >( int idx, int value ) {
//
//	if ( this->IsType ( idx, LUA_TNUMBER )) {
//		return ( int )lua_tonumber ( this->mState, idx );
//	}
//	return value;
//}

//----------------------------------------------------------------//
template <>
s8 MOAILuaState::GetValue < s8 >( int idx, s8 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( s8 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
s16 MOAILuaState::GetValue < s16 >( int idx, s16 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( s16 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
s32 MOAILuaState::GetValue < s32 >( int idx, s32 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( s32 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
s64 MOAILuaState::GetValue < s64 >( int idx, s64 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( s64 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u8 MOAILuaState::GetValue < u8 >( int idx, u8 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u8 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u16 MOAILuaState::GetValue < u16 >( int idx, u16 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u16 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u32 MOAILuaState::GetValue < u32 >( int idx, u32 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u32 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u64 MOAILuaState::GetValue < u64 >( int idx, u64 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u64 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
uintptr MOAILuaState::GetValue < uintptr >( int idx, uintptr value ) {

	if ( this->IsType ( idx, LUA_TLIGHTUSERDATA )) {
		return ( uintptr )lua_touserdata ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
MOAILuaRef MOAILuaState::GetWeakRef ( int idx ) {

	MOAILuaRef ref;
	ref.SetWeakRef ( *this, idx );
	return ref;
}

//----------------------------------------------------------------//
bool MOAILuaState::HasField ( int idx, cc8* name ) {

	lua_getfield ( this->mState, idx, name );
	bool hasField = ( lua_isnil ( this->mState, -1 ) == false );
	lua_pop ( this->mState, 1 );
	
	return hasField;
}

//----------------------------------------------------------------//
bool MOAILuaState::HasField ( int idx, int key ) {

	this->GetField ( idx, key );
	bool hasField = ( lua_isnil ( this->mState, -1 ) == false );
	lua_pop ( this->mState, 1 );
	
	return hasField;
}

//----------------------------------------------------------------//
bool MOAILuaState::HasField ( int idx, cc8* name, int type ) {

	lua_getfield ( this->mState, idx, name );
	bool hasField = ( lua_type ( this->mState, -1 ) == type );
	lua_pop ( this->mState, 1 );
	
	return hasField;
}

//----------------------------------------------------------------//
bool MOAILuaState::HasField ( int idx, int key, int type ) {

	this->GetField ( idx, key );
	bool hasField = ( lua_type ( this->mState, -1 ) == type );
	lua_pop ( this->mState, 1 );
	
	return hasField;
}

//----------------------------------------------------------------//
bool MOAILuaState::HexDecode ( int idx ) {

	USHexReader hex;
	return this->Decode ( idx, hex );
}

//----------------------------------------------------------------//
bool MOAILuaState::HexEncode ( int idx ) {

	USHexWriter hex;
	return this->Encode ( idx, hex );
}

//----------------------------------------------------------------//
bool MOAILuaState::Inflate ( int idx, int windowBits ) {

	USDeflateReader inflater;
	inflater.SetWindowBits ( windowBits );
	
	return this->Decode ( idx, inflater );
}

//----------------------------------------------------------------//
bool MOAILuaState::IsNil () {

	return ( !this->mState );
}

//----------------------------------------------------------------//
bool MOAILuaState::IsNil ( int idx ) {

	return lua_isnil ( this->mState, idx );
}

//----------------------------------------------------------------//
bool MOAILuaState::IsTableOrUserdata ( int idx ) {

	int check = lua_type ( this->mState, idx );
	return (( check == LUA_TTABLE ) || ( check == LUA_TUSERDATA ));
}

//----------------------------------------------------------------//
bool MOAILuaState::IsType ( int idx, int type ) {

	return ( lua_type ( this->mState, idx ) == type );
}

//----------------------------------------------------------------//
bool MOAILuaState::IsType ( int idx, cc8* name, int type ) {
	
	return this->HasField ( idx, name, type );
}

//----------------------------------------------------------------//
void MOAILuaState::LoadLibs () {

	luaL_openlibs ( this->mState );
}

//----------------------------------------------------------------//
void MOAILuaState::MoveToTop ( int idx ) {

	// moves a stack element to the top (removing it from the previous location)
	idx = this->AbsIndex ( idx );				// adjusted index after copying element to top
	lua_pushvalue ( this->mState, idx );	// copy element to top of stack
	lua_remove ( this->mState, idx );		// remove original copy from stack
}

//----------------------------------------------------------------//
void MOAILuaState::Pop ( int n ) {

	lua_pop ( this->mState, n );
}

//----------------------------------------------------------------//
bool MOAILuaState::PrepMemberFunc ( int idx, cc8* name ) {

	idx = this->AbsIndex ( idx );
	
	if ( !this->GetFieldWithType ( idx, name, LUA_TFUNCTION )) return false;
	this->CopyToTop ( idx );
	
	return true;
}

//----------------------------------------------------------------//
bool MOAILuaState::PrintErrors ( FILE* file, int status ) {

	if ( status != 0 ) {
	
		cc8* error = lua_tostring ( this->mState, -1 );
		if ( error ) {
			STLString msg = lua_tostring ( this->mState, -1 );
			USLog::PrintFile ( file, "-- %s\n", msg.c_str ());
		}
		lua_pop ( this->mState, 1 ); // pop error message
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAILuaState::PrintStackTrace ( FILE* file, int level ) {

	STLString stackTrace = this->GetStackTrace ( level );
	USLog::PrintFile ( file, stackTrace.str ());
}

//----------------------------------------------------------------//
void MOAILuaState::Push () {

	lua_pushnil ( this->mState );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( bool value ) {

	lua_pushboolean ( this->mState, value ? 1 : 0 );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( cc8* value ) {

	lua_pushstring ( this->mState, value );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( double value ) {

	lua_pushnumber ( this->mState, value );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( float value ) {

	lua_pushnumber ( this->mState, value );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( int value ) {

	lua_pushnumber ( this->mState, value );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( u16 value ) {

	lua_pushnumber ( this->mState, value );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( u32 value ) {

	lua_pushnumber ( this->mState, value );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( u64 value ) {

	lua_pushnumber ( this->mState, ( double )value );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( uintptr value ) {

	lua_pushlightuserdata ( this->mState, ( void* )value );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( lua_CFunction value ) {

	lua_pushcfunction ( this->mState, value );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( MOAILuaObject* luaObject ) {

	if ( luaObject ) {
		luaObject->PushLuaUserdata ( *this );
		return;
	}
	lua_pushnil ( this->mState );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( MOAILuaRef& ref ) {

	ref.PushRef ( *this );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( void* data, size_t size ) {

	lua_pushlstring ( this->mState, ( cc8* )data, size );
}

//----------------------------------------------------------------//
void MOAILuaState::PushPtrUserData ( void* ptr ) {

	void** handle = ( void** )lua_newuserdata ( this->mState, sizeof ( void* ));
	assert ( handle );
	( *handle ) = ptr;
}

//----------------------------------------------------------------//
int MOAILuaState::PushTableItr ( int idx ) {

	int itr = this->AbsIndex ( idx );

	lua_pushnil ( this->mState );
	lua_pushnil ( this->mState );
	lua_pushnil ( this->mState );
	
	return itr;
}

//----------------------------------------------------------------//
void MOAILuaState::RegisterModule ( int idx, cc8* name, bool autoload ) {

	idx = this->AbsIndex ( idx );

	lua_getglobal ( this->mState, "package" );
	lua_getfield ( this->mState, -1, "preload" );

	lua_pushstring ( this->mState, name );
	lua_pushvalue ( this->mState, idx );
	lua_settable ( this->mState, -3 );
	
	// pop 'preload'
	lua_pop ( this->mState, 1 );
	
	if ( autoload ) {
	
		lua_getfield ( this->mState, -1, "loaded" );
		
		// push the name
		lua_pushstring ( this->mState, name );
		
		// push the table
		lua_pushvalue ( this->mState, idx );
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
void MOAILuaState::RegisterModule ( lua_CFunction loader, cc8* name, bool autoload ) {

	lua_pushcfunction ( this->mState, loader );
	this->RegisterModule ( -1, name, autoload );
	lua_pop ( this->mState, 1 );
}

//----------------------------------------------------------------//
void MOAILuaState::RegisterModule ( void* data, size_t size, cc8* name, bool autoload ) {

	lua_getglobal ( this->mState, "loadstring" );
	this->Push ( data, size );
	this->DebugCall ( 1, 1 );

	this->RegisterModule ( -1, name, autoload );
	lua_pop ( this->mState, 1 );
}

//----------------------------------------------------------------//
int MOAILuaState::RelIndex ( int idx ) {

	if ( idx > 0 ) {
		return idx - lua_gettop ( this->mState );
	}
	return idx;
}

//----------------------------------------------------------------//
void MOAILuaState::ReportBadCast ( int idx, cc8* typeName ) {
	MOAILog ( *this, MOAILogMessages::MOAI_BadCast_DS, this->AbsIndex ( idx ), typeName );
}

//----------------------------------------------------------------//
int MOAILuaState::Run ( void* data, size_t size, int nArgs, int nResults ) {

	lua_getglobal ( this->mState, "loadstring" );
	this->Push ( data, size );
	this->DebugCall ( 1, 1 );
	
	if ( nArgs ) {
		lua_insert ( this->mState, -( nArgs + 1 ));
	}
	
	return this->DebugCall ( nArgs, nResults );
}

//----------------------------------------------------------------//
void MOAILuaState::SetPath ( cc8* path ) {

	int top = lua_gettop ( this->mState );

	lua_getglobal ( this->mState, "package" );
	int packageIdx = lua_gettop ( this->mState );
	
	lua_pushstring ( this->mState, "path" );
	lua_pushstring ( this->mState, path );
	lua_settable ( this->mState, packageIdx );
	
	lua_settop ( this->mState, top );
}

//----------------------------------------------------------------//
void MOAILuaState::SetTop ( int top ) {

	lua_settop ( this->mState, top );
}

//----------------------------------------------------------------//
bool MOAILuaState::TableItrNext ( int itr ) {

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
MOAILuaState::MOAILuaState () :
	mState ( 0 ) {
}

//----------------------------------------------------------------//
MOAILuaState::MOAILuaState ( lua_State* state ) :
	mState ( state ) {
}

//----------------------------------------------------------------//
MOAILuaState::~MOAILuaState () {
}

//----------------------------------------------------------------//
int MOAILuaState::YieldThread ( int nResults ) {

	return lua_yield ( this->mState, nResults );
}
