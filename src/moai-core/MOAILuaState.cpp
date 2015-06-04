// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAILuaRuntime.h>
#include <moai-core/MOAILuaState.h>
#include <moai-core/MOAILogMessages.h>
#include <moai-core/MOAILuaState-impl.h>

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

	ZLBase64Reader base64;
	return this->Decode ( idx, base64 );
}

//----------------------------------------------------------------//
bool MOAILuaState::Base64Encode ( int idx ) {

	ZLBase64Writer base64;
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
			
			// any non-nil type
			case '@':
				if ( type == LUA_TNIL ) return false; // TODO: log a message
				break;
			
			// nil
			case '-':
				if ( type != LUA_TNIL ) expected = LUA_TNIL;
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
bool MOAILuaState::CheckVector ( int idx, u32 n, lua_Number match, lua_Number fallback ) {

	return this->CheckVector ( idx, n, match, match, fallback );
}

//----------------------------------------------------------------//
bool MOAILuaState::CheckVector ( int idx, u32 n, lua_Number min, lua_Number max, lua_Number fallback ) {

	for ( u32 i = 0; i < n; ++i ) {
		lua_Number element = lua_type ( this->mState, idx + i ) == LUA_TNUMBER ? lua_tonumber ( this->mState, idx + i ) : fallback;
		if (( element < min ) || ( element > max )) return false;
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
	
	int errIdx = this->AbsIndex ( -( nArgs + 1 ));

	MOAILuaRuntime::Get ().PushTraceback ( *this );
	lua_insert ( this->mState, errIdx );

	int status = lua_pcall ( this->mState, nArgs, nResults, errIdx );

	if ( status ) {
		this->PrintErrors ( ZLLog::CONSOLE, status );
	}
	else {
		lua_remove ( this->mState, errIdx );
	}
	return status;
}

//----------------------------------------------------------------//
bool MOAILuaState::Decode ( int idx, ZLStreamAdapter& reader ) {

	if ( !this->IsType ( idx, LUA_TSTRING )) return false;

	size_t len;
	void* buffer = ( void* )lua_tolstring ( this->mState, idx, &len );
	
	if ( !len ) {
		lua_pushstring ( this->mState, "" );
		return true;
	}
	
	ZLByteStream cryptStream;
	cryptStream.SetBuffer ( buffer, len );
	cryptStream.SetLength ( len );
	
	ZLMemStream plainStream;
	
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

	ZLDeflateWriter deflater;
	deflater.SetCompressionLevel ( level );
	deflater.SetWindowBits ( windowBits );

	return this->Encode ( idx, deflater );
}

//----------------------------------------------------------------//
bool MOAILuaState::Encode ( int idx, ZLStreamAdapter& writer ) {

	if ( !this->IsType ( idx, LUA_TSTRING )) return false;

	size_t len;
	cc8* buffer = lua_tolstring ( this->mState, idx, &len );
	
	if ( !len ) {
		lua_pushstring ( this->mState, "" );
		return true;
	}
	
	ZLMemStream stream;
	
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
ZLBox MOAILuaState::GetBox ( int idx ) {

	ZLBox box;

	box.mMin.mX = this->GetValue < float >( idx++, 0.0f );
	box.mMin.mY = this->GetValue < float >( idx++, 0.0f );
	box.mMin.mZ = this->GetValue < float >( idx++, 0.0f );
	
	box.mMax.mX = this->GetValue < float >( idx++, 0.0f );
	box.mMax.mY = this->GetValue < float >( idx++, 0.0f );
	box.mMax.mZ = this->GetValue < float >( idx++, 0.0f );
	
	return box;
}

//----------------------------------------------------------------//
ZLColorVec MOAILuaState::GetColor ( int idx, float r, float g, float b, float a ) {

	ZLColorVec color;
	color.mR = this->GetValue < float >( idx++, r );
	color.mG = this->GetValue < float >( idx++, g );
	color.mB = this->GetValue < float >( idx++, b );
	color.mA = this->GetValue < float >( idx++, a );
	
	return color;
}

//----------------------------------------------------------------//
u32 MOAILuaState::GetColor32 ( int idx, float r, float g, float b, float a ) {

	ZLColorVec color = this->GetColor ( idx, r, g, b, a );
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
bool MOAILuaState::GetSubfieldWithType ( int idx, cc8* format, int type, ... ) {

	va_list args;
	va_start ( args, type );
	
	idx = this->AbsIndex ( idx );
	lua_pushvalue ( this->mState, idx );

	for ( cc8* c = format; *c; ++c ) {
		
		switch ( *c ) {
		
			// number
			case 'N':
				lua_pushnumber ( this->mState, va_arg ( args, int ));
				lua_gettable ( this->mState, -1 );
				break;
			
			// string
			case 'S':
				lua_getfield ( this->mState, -1, va_arg ( args, char* ));
				break;
			
			default:
				lua_pushnil ( this->mState );
		}
	
		if ( lua_isnil ( this->mState, -1 )) break;
		lua_replace ( this->mState, -2 );
	}
	
	va_end ( args );
	
	if ( lua_type ( this->mState, -1 ) != type ) {
		lua_pop ( this->mState, 1 );
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
int MOAILuaState::GetLuaThreadStatus ( int idx ) {

	if ( lua_type ( this->mState, idx ) != LUA_TTHREAD ) return THREAD_UNKNOWN;
	return this->GetLuaThreadStatus ( lua_tothread ( this->mState, idx ));
}

//----------------------------------------------------------------//
int MOAILuaState::GetLuaThreadStatus ( lua_State* thread ) {

	lua_pushthread ( this->mState );
	lua_State* running = lua_tothread ( this->mState, -1 );
	lua_pop ( this->mState, 1 );

	if ( thread == running ) return THREAD_RUNNING;

	switch ( lua_status ( thread )) {
	
		case LUA_YIELD:
			return THREAD_SUSPENDED;

		case 0: {
			lua_Debug ar;
			if ( lua_getstack ( thread, 0, &ar ) > 0 ) {
				return THREAD_NORMAL;
			}
			else if ( lua_gettop ( thread ) == 0 ) {
				return THREAD_DEAD;
			}
			else {
				return THREAD_SUSPENDED;
			}
		}
		default:  // some error occured
			return THREAD_ERROR;
	}
	
	return THREAD_UNKNOWN;
}

//----------------------------------------------------------------//
cc8* MOAILuaState::GetLuaThreadStatusName ( int status ) {

	switch ( status ) {
		case THREAD_DEAD:		return "dead";
		case THREAD_ERROR:		return "error";
		case THREAD_NORMAL:		return "normal";
		case THREAD_RUNNING:	return "running";
		case THREAD_SUSPENDED:	return "suspended";
	}
	return "unknown";
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
		case LUA_TTHREAD:			return "thread";
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
STLString MOAILuaState::GetStackDump () {
	STLString out;
	int top = GetTop ();
	out.write ( "Lua stack: %d element(s)", top );

	for ( int index = top; index >= 1; index-- ) {
		int type = lua_type ( this->mState, index );

		// Print index and type
		int relativeIndex = index - top - 1;
		out.write ( "\n[ %d | %d ] = %s", index, relativeIndex, GetLuaTypeName ( type ) );

		// Print value, if possible
		switch ( type ) {
		case LUA_TBOOLEAN:
			// boolean
			out.write ( ": %s", lua_toboolean ( this->mState, index ) ? "true" : "false" );
			break;
		case LUA_TNUMBER:
			// number
			out.write ( ": %g", lua_tonumber ( this->mState, index ) );
			break;
		case LUA_TSTRING:
			// string
			out.write ( ": \"%s\"", lua_tostring ( this->mState, index ) );
			break;
		case LUA_TUSERDATA:
			// userdata
		{
			// Moai uses userdata exclusively for pointers to MOAILuaObject instances.
			// This code will most likely crash if it encounters userdata that is used differently.
			MOAILuaObject* luaObject = ( MOAILuaObject* )this->GetPtrUserData ( index );
			if ( luaObject ) {
				out.write ( ": %s at %p", luaObject->TypeName (), luaObject );
			}
			break;
		}
		case LUA_TLIGHTUSERDATA:
		case LUA_TTABLE:
		case LUA_TFUNCTION:
		case LUA_TTHREAD:
			// anything with an address
			out.write ( " at %p", lua_topointer ( this->mState, index ) );
			break;
		}
	}

	out.write("\n");
	return out;
}

//----------------------------------------------------------------//
STLString MOAILuaState::GetStackTrace ( cc8* title, int level ) {

	int firstpart = 1;  /* still before eventual `...' */
	lua_Debug ar;
	
	lua_State* L = this->mState;

	STLString out;
	
	out.append ( title ? title : "stack traceback:" );
	
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
size_t MOAILuaState::GetTableSize ( int idx ) {

	return lua_objlen ( this->mState, idx );
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
bool MOAILuaState::GetValue < bool >( int idx, const bool value ) {

	if ( this->IsType ( idx, LUA_TBOOLEAN )) {
		return ( lua_toboolean ( this->mState, idx ) != 0 );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
cc8* MOAILuaState::GetValue < cc8* >( int idx, const cc8* value ) {

	if ( this->IsType ( idx, LUA_TSTRING )) {
		return lua_tostring ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
STLString MOAILuaState::GetValue ( int idx, const cc8* value ) {

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
double MOAILuaState::GetValue < double >( int idx, const double value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
float MOAILuaState::GetValue < float >( int idx, const float value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( float )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
//template <>
//int MOAILuaState::GetValue < int >( int idx, const int value ) {
//
//	if ( this->IsType ( idx, LUA_TNUMBER )) {
//		return ( int )lua_tonumber ( this->mState, idx );
//	}
//	return value;
//}

//----------------------------------------------------------------//
template <>
s8 MOAILuaState::GetValue < s8 >( int idx, const s8 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( s8 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
s16 MOAILuaState::GetValue < s16 >( int idx, const s16 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( s16 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
s32 MOAILuaState::GetValue < s32 >( int idx, const s32 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( s32 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
s64 MOAILuaState::GetValue < s64 >( int idx, const s64 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( s64 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u8 MOAILuaState::GetValue < u8 >( int idx, const u8 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u8 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u16 MOAILuaState::GetValue < u16 >( int idx, const u16 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u16 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u32 MOAILuaState::GetValue < u32 >( int idx, const u32 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u32 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
u64 MOAILuaState::GetValue < u64 >( int idx, const u64 value ) {

	if ( this->IsType ( idx, LUA_TNUMBER )) {
		return ( u64 )lua_tonumber ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
const void* MOAILuaState::GetValue < const void* >( int idx, const void* value ) {

	if ( this->IsType ( idx, LUA_TLIGHTUSERDATA )) {
		return ( void* )lua_touserdata ( this->mState, idx );
	}
	return value;
}

//----------------------------------------------------------------//
template <>
ZLBox MOAILuaState::GetValue < ZLBox >( int idx, const ZLBox value ) {

	if ( this->CheckParams ( idx, "NNNNNN" )) {
	
		ZLBox box;
		
		box.mMin.mX		= lua_tonumber ( this->mState, idx + 0 );
		box.mMin.mY		= lua_tonumber ( this->mState, idx + 1 );
		box.mMin.mZ		= lua_tonumber ( this->mState, idx + 2 );
		
		box.mMax.mX		= lua_tonumber ( this->mState, idx + 3 );
		box.mMax.mY		= lua_tonumber ( this->mState, idx + 4 );
		box.mMax.mZ		= lua_tonumber ( this->mState, idx + 5 );
		
		return box;
	}
	return value;
}

//----------------------------------------------------------------//
template <>
ZLColorVec MOAILuaState::GetValue < ZLColorVec >( int idx, const ZLColorVec value ) {

	if ( this->CheckParams ( idx, "NNNN", false )) {
	
		ZLColorVec color;
		
		color.mR		= lua_tonumber ( this->mState, idx + 0 );
		color.mG		= lua_tonumber ( this->mState, idx + 1 );
		color.mB		= lua_tonumber ( this->mState, idx + 2 );
		color.mA		= lua_tonumber ( this->mState, idx + 3 );
		
		return color;
	}
	return value;
}

//----------------------------------------------------------------//
template <>
ZLRect MOAILuaState::GetValue < ZLRect >( int idx, const ZLRect value ) {

	if ( this->CheckParams ( idx, "NNNN", false )) {
	
		ZLRect rect;
		
		rect.mXMin		= lua_tonumber ( this->mState, idx + 0 );
		rect.mYMin		= lua_tonumber ( this->mState, idx + 1 );
		rect.mXMax		= lua_tonumber ( this->mState, idx + 2 );
		rect.mYMax		= lua_tonumber ( this->mState, idx + 3 );
		
		return rect;
	}
	return value;
}

//----------------------------------------------------------------//
template <>
ZLVec2D MOAILuaState::GetValue < ZLVec2D >( int idx, const ZLVec2D value ) {

	if ( this->CheckParams ( idx, "NN", false )) {
	
		ZLVec2D vec;
		
		vec.mX			= lua_tonumber ( this->mState, idx + 0 );
		vec.mY			= lua_tonumber ( this->mState, idx + 1 );
		
		return vec;
	}
	return value;
}

//----------------------------------------------------------------//
template <>
ZLVec3D MOAILuaState::GetValue < ZLVec3D >( int idx, const ZLVec3D value ) {

	if ( this->CheckParams ( idx, "NN", false )) {
	
		ZLVec3D vec;
		
		vec.mX			= lua_tonumber ( this->mState, idx + 0 );
		vec.mY			= lua_tonumber ( this->mState, idx + 1 );
		vec.mZ			= lua_tonumber ( this->mState, idx + 2 );
		
		return vec;
	}
	return value;
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
bool MOAILuaState::HasKeys ( int idx ) {

	idx = this->AbsIndex ( idx );

	lua_pushnil ( this->mState );  /* first key */
	if ( lua_next ( this->mState, idx ) != 0 ) {
		lua_pop ( this->mState, 2 );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAILuaState::HexDecode ( int idx ) {

	ZLHexAdapter hex;
	return this->Decode ( idx, hex );
}

//----------------------------------------------------------------//
bool MOAILuaState::HexEncode ( int idx ) {

	ZLHexAdapter hex;
	return this->Encode ( idx, hex );
}

//----------------------------------------------------------------//
bool MOAILuaState::Inflate ( int idx, int windowBits ) {

	ZLDeflateReader inflater;
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
bool MOAILuaState::IsTrueOrNotNil ( int idx ) {

	if ( lua_isboolean ( this->mState, idx )) {
		return lua_toboolean ( this->mState, idx );
	}
	return !lua_isnil ( this->mState, idx );
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
			ZLLog::LogF ( file, "-- %s\n", msg.c_str ());
		}
		lua_pop ( this->mState, 1 ); // pop error message
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAILuaState::PrintStackDump () {
	STLString stackDump = this->GetStackDump ();
	ZLLog::LogF ( ZLLog::CONSOLE, stackDump );
}

//----------------------------------------------------------------//
void MOAILuaState::PrintStackDump ( FILE* file  ) {
	STLString stackDump = this->GetStackDump ();
	ZLLog::LogF ( file, stackDump );
}

//----------------------------------------------------------------//
void MOAILuaState::PrintStackTrace ( FILE* file, cc8* title, int level ) {
	STLString stackTrace = this->GetStackTrace ( title, level );
	ZLLog::LogF ( file, stackTrace.str ());
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

	// TODO: check for overflow
	lua_pushnumber ( this->mState, ( double )value );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( const ZLBox& value ) {

	lua_pushnumber ( this->mState, value.mMin.mX );
	lua_pushnumber ( this->mState, value.mMin.mY );
	lua_pushnumber ( this->mState, value.mMin.mZ );
	
	lua_pushnumber ( this->mState, value.mMax.mX );
	lua_pushnumber ( this->mState, value.mMax.mY );
	lua_pushnumber ( this->mState, value.mMax.mZ );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( const ZLColorVec& value ) {

	lua_pushnumber ( this->mState, value.mR );
	lua_pushnumber ( this->mState, value.mG );
	lua_pushnumber ( this->mState, value.mB );
	lua_pushnumber ( this->mState, value.mA );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( const ZLRect& value ) {

	lua_pushnumber ( this->mState, value.mXMin );
	lua_pushnumber ( this->mState, value.mYMin );
	lua_pushnumber ( this->mState, value.mXMax );
	lua_pushnumber ( this->mState, value.mYMax );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( const ZLVec2D& value ) {

	lua_pushnumber ( this->mState, value.mX );
	lua_pushnumber ( this->mState, value.mY );
}

//----------------------------------------------------------------//
void MOAILuaState::Push ( const ZLVec3D& value ) {

	lua_pushnumber ( this->mState, value.mX );
	lua_pushnumber ( this->mState, value.mY );
	lua_pushnumber ( this->mState, value.mZ );
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
void MOAILuaState::Push ( const void* value ) {

	lua_pushlightuserdata ( this->mState, ( void* )value );
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
int MOAILuaState::YieldThread ( int nResults ) {

	return lua_yield ( this->mState, nResults );
}
