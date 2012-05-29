// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIStream.h>

#define MAX_STACK_BUFFER 1024

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_flush ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	if ( self->mStream ) {
		self->mStream->Flush ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_getCursor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	if ( !self->mStream ) return 0;
	u32 cursor = self->mStream->GetCursor ();
	state.Push ( cursor );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	if ( !self->mStream ) return 0;
	u32 length = self->mStream->GetLength ();
	state.Push ( length );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_read ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );

	u32 len = 0;
	
	if ( self->mStream ) {
		len = state.GetValue < u32 >( 2, self->mStream->GetLength ());
	}
	
	if ( !len ) {
		state.Push ();
		state.Push (  0 );
		return 2;
	}
	
	char* buffer = 0;
	
	if ( len > MAX_STACK_BUFFER ) {
		buffer = ( char* )malloc ( len );
	}
	else {
		buffer = ( char* )alloca ( len );
	}
	
	len = self->mStream->ReadBytes ( buffer, len );
	
	if ( len ) {
		lua_pushlstring ( state, buffer, len );
	}
	else {
		state.Push ();
	}
	
	if ( len > MAX_STACK_BUFFER ) {
		free ( buffer );
	}
	
	state.Push ( len );
	return 2;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_read8 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < s8 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_read16 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < s16 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_read32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < s32 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_readDouble ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < double >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_readFloat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < float >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_readFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "US" );
	return self->ReadFormat ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_readU8 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < u8 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_readU16 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < u16 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_readU32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < u32 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_seek ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	
	u32 offset	= state.GetValue < u32 >( 2, 0 );
	u32 mode	= state.GetValue < u32 >( 3, SEEK_SET );
	
	if ( self->mStream ) {
		self->mStream->Seek ( offset, mode );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_write ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "US" );

	if ( !self->mStream ) return 0;

	size_t len;
	cc8* str = lua_tolstring ( state, 2, &len );
	
	size_t writeLen = state.GetValue < size_t >( 3, len );
	if ( len < writeLen ) {
		writeLen = len;
	}
	
	writeLen = self->mStream->WriteBytes ( str, writeLen );
	
	state.Push ( writeLen );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_write8 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < s8 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_write16 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < s16 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_write32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < s32 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_writeDouble ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < double >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_writeFloat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < float >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_writeFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "US" );
	return self->WriteFormat ( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_writeStream ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "UU" );
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	size_t result = 0;
	
	if ( stream ) {
		
		USStream* inStream = stream->GetUSStream ();
		USStream* outStream = self->GetUSStream ();
		
		if ( inStream && outStream ) {
		
			if ( state.IsType ( 3, LUA_TNUMBER )) {
				u32 size = state.GetValue < u32 >( 3, 0 );
				if ( size ) {
					result = outStream->WriteStream ( *inStream, size );
				}
			}
			else {
				result = outStream->WriteStream ( *inStream );
			}
		}
	}
	
	state.Push ( result );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_writeU8 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < u8 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_writeU16 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < u16 >( state, 2 );
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStream::_writeU32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < u32 >( state, 2 );
}

//================================================================//
// MOAIStream
//================================================================//

//----------------------------------------------------------------//
MOAIStream::MOAIStream () :
	mStream ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStream::~MOAIStream () {
}

//----------------------------------------------------------------//
cc8* MOAIStream::ParseTypeToken ( cc8* format, u32& type ) {

	bool more = true;
	u32 result = UNKNOWN;
	u32 size = 0;
	
	while ( more ) {
	
		char c = *( format++ );
		
		switch ( c ) {
			
			case 'd':
				result = DOUBLE;
				more = false;
				break;
			case 'f':
				result = FLOAT;
				more = false;
				break;
			case 's':
				result = SIGNED;
				break;
			case 'u':
				result = UNSIGNED;
				break;
			case '1':
				size = 16;
				format++;
				more = false;
				break;
			case '3':
				size = 32;
				format++;
				more = false;
				break;
			case '8':
				size = 8;
				more = false;
				break;
			case 0:
				format = 0;
				more = false;
				break;
		}
	}
	
	if ( format && format [ 0 ]) {
		for ( ; *format == ','; format++ );
	}
	else {
		format = 0;
	}
	
	if ( size ) {
	
		if ( result == UNKNOWN ) {
			result = SIGNED;
		}

		if ( result == SIGNED ) {
			switch ( size ) {
				case 8:
					result = INT_8;
					break;
				case 16:
					result = INT_16;
					break;
				case 32:
					result = INT_32;
					break;
			}
		}
		
		if ( result == UNSIGNED ) {
			switch ( size ) {
				case 8:
					result = UINT_8;
					break;
				case 16:
					result = UINT_16;
					break;
				case 32:
					result = UINT_32;
					break;
			}
		}
	}
	
	type = result;
	return format;
}

//----------------------------------------------------------------//
int MOAIStream::ReadFormat ( MOAILuaState& state, int idx ) {

	idx = state.AbsIndex ( idx );
	cc8* format = state.GetValue < cc8* >( idx, "" );
	
	u32 bytes = 0;
	u32 type = UNKNOWN;
	
	while ( format ) {
		
		format = MOAIStream::ParseTypeToken ( format, type );	
		
		switch ( type ) {
			case INT_8:
				bytes += this->ReadValue < s8 >( state );
				break;
			case INT_16:
				bytes += this->ReadValue < s16 >( state );
				break;
			case INT_32:
				bytes += this->ReadValue < s32 >( state );
				break;
			case DOUBLE:
				bytes += this->ReadValue < double >( state );
				break;
			case FLOAT:
				bytes += this->ReadValue < float >( state );
				break;
			case UINT_8:
				bytes += this->ReadValue < u8 >( state );
				break;
			case UINT_16:
				bytes += this->ReadValue < u16 >( state );
				break;
			case UINT_32:
				bytes += this->ReadValue < u32 >( state );
				break;
			default:
				format = 0;
		}
	}
	
	state.Push ( bytes );
	return ( state.GetTop () - idx );
}

//----------------------------------------------------------------//
void MOAIStream::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "SEEK_CUR", ( u32 )SEEK_CUR );
	state.SetField ( -1, "SEEK_END", ( u32 )SEEK_END );
	state.SetField ( -1, "SEEK_SET", ( u32 )SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIStream::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "flush",				_flush },
		{ "getCursor",			_getCursor },
		{ "getLength",			_getLength },
		{ "read",				_read },
		{ "read8",				_read8 },
		{ "read16",				_read16 },
		{ "read32",				_read32 },
		{ "readDouble",			_readDouble },
		{ "readFloat",			_readFloat },
		{ "readFormat",			_readFormat },
		{ "readU8",				_readU8 },
		{ "readU16",			_readU16 },
		{ "readU32",			_readU32 },
		{ "seek",				_seek },
		{ "write",				_write },
		{ "write8",				_write8 },
		{ "write16",			_write16 },
		{ "write32",			_write32 },
		{ "writeDouble",		_writeDouble },
		{ "writeFloat",			_writeFloat },
		{ "writeFormat",		_writeFormat },
		{ "writeStream",		_writeStream },
		{ "writeU8",			_writeU8 },
		{ "writeU16",			_writeU16 },
		{ "writeU32",			_writeU32 },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
int MOAIStream::WriteFormat ( MOAILuaState& state, int idx ) {

	if ( !this->mStream ) {
		state.Push ( 0 );
		return 1;
	}

	idx = state.AbsIndex ( idx );
	cc8* format = state.GetValue < cc8* >( idx++, "" );
	
	size_t size;
	u32 result = 0;
	u32 bytes = 0;
	u32 type = UNKNOWN;
	
	while ( format ) {
	
		size = 0;
		format = MOAIStream::ParseTypeToken ( format, type );	
		result = this->mStream->GetCursor ();
		
		switch ( type ) {
			case INT_8:
				size = sizeof ( s8 );
				this->mStream->Write < s8 >( state.GetValue < s8 >( idx++, 0 ));
				break;
			case INT_16:
				size = sizeof ( s16 );
				this->mStream->Write < s16 >( state.GetValue < s16 >( idx++, 0 ));
				break;
			case INT_32:
				size = sizeof ( s32 );
				this->mStream->Write < s32 >( state.GetValue < s32 >( idx++, 0 ));
				break;
			case DOUBLE:
				size = sizeof ( double );
				this->mStream->Write < double >( state.GetValue < double >( idx++, 0 ));
				break;
			case FLOAT:
				size = sizeof ( float );
				this->mStream->Write < float >( state.GetValue < float >( idx++, 0 ));
				break;
			case UINT_8:
				size = sizeof ( u8 );
				this->mStream->Write < u8 >( state.GetValue < u8 >( idx++, 0 ));
				break;
			case UINT_16:
				size = sizeof ( u16 );
				this->mStream->Write < u16 >( state.GetValue < u16 >( idx++, 0 ));
				break;
			case UINT_32:
				size = sizeof ( u32 );
				this->mStream->Write < u32 >( state.GetValue < u32 >( idx++, 0 ));
				break;
			default:
				format = 0;
		}
		
		result = this->mStream->GetCursor () - result;
		bytes += result;
		if ( result != size ) {
			break;
		}
	}
	
	state.Push ( bytes );
	return 1;
}