// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIStream.h>

#define MAX_STACK_BUFFER 1024

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	flush
	@text	Forces any remaining buffered data into the stream.
	
	@in		MOAIStream self
	@out	nil
*/
int MOAIStream::_flush ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	self->Flush ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getCursor
	@text	Returns the current cursor position in the stream.
	
	@in		MOAIStream self
	@out	number cursor
*/
int MOAIStream::_getCursor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	size_t cursor = self->GetCursor ();
	state.Push (( u32 )cursor ); // TODO: overflow?
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getLength
	@text	Returns the length of the stream.
	
	@in		MOAIStream self
	@out	number length
*/
int MOAIStream::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	size_t length = self->GetLength ();
	state.Push (( u32 )length ); // TODO: overflow?
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	read
	@text	Reads bytes from the stream.
	
	@in		MOAIStream self
	@opt	number byteCount		Number of bytes to read. Default value is the length of the stream.
	@out	string bytes			Data read from the stream.
	@out	number actualByteCount	Size of data successfully read.
*/
int MOAIStream::_read ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );

	size_t len = state.GetValue < u32 >( 2, ( u32 )self->GetLength ());
	
	if ( !len ) {
		lua_pushstring ( state, "" );
		state.Push ( 0 );
		return 2;
	}
	
	char* buffer = 0;
	
	if ( len > MAX_STACK_BUFFER ) {
		buffer = ( char* )malloc ( len );
	}
	else {
		buffer = ( char* )alloca ( len );
	}
	
	len = self->ReadBytes ( buffer, len );
	
	if ( len ) {
		lua_pushlstring ( state, buffer, len );
	}
	else {
		state.Push ();
	}
	
	if ( len > MAX_STACK_BUFFER ) {
		free ( buffer );
	}
	
	state.Push (( u32 )len ); // TODO: overflow?
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	read8
	@text	Reads a signed 8-bit value from the stream.
	
	@in		MOAIStream self
	@out	number value		Value from the stream.
	@out	number size			Number of bytes successfully read.
*/
int MOAIStream::_read8 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < s8 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	read16
	@text	Reads a signed 16-bit value from the stream.
	
	@in		MOAIStream self
	@out	number value		Value from the stream.
	@out	number size			Number of bytes successfully read.
*/
int MOAIStream::_read16 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < s16 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	read32
	@text	Reads a signed 32-bit value from the stream.
	
	@in		MOAIStream self
	@out	number value		Value from the stream.
	@out	number size			Number of bytes successfully read.
*/
int MOAIStream::_read32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < s32 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	readDouble
	@text	Reads a 64-bit floating point value from the stream.
	
	@in		MOAIStream self
	@out	number value		Value from the stream.
	@out	number size			Number of bytes successfully read.
*/
int MOAIStream::_readDouble ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < double >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	readFloat
	@text	Reads a 32-bit floating point value from the stream.
	
	@in		MOAIStream self
	@out	number value		Value from the stream.
	@out	number size			Number of bytes successfully read.
*/
int MOAIStream::_readFloat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < float >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	readFormat
	@text	Reads a series of values from the stream given a format string.
			Valid tokens for the format string are: u8 u16 u32 f d s8 s16 s32.
			Tokens may be optionally separated by spaces or commas.
	
	@in		MOAIStream self
	@in		string format
	@out	...	values			Values read from the stream or 'nil'.
	@out	number size			Number of bytes successfully read.
*/
int MOAIStream::_readFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "US" );
	return self->ReadFormat ( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	readU8
	@text	Reads an unsigned 8-bit value from the stream.
	
	@in		MOAIStream self
	@out	number value		Value from the stream.
	@out	number size			Number of bytes successfully read.
*/
int MOAIStream::_readU8 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < u8 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	readU16
	@text	Reads an unsigned 16-bit value from the stream.
	
	@in		MOAIStream self
	@out	number value		Value from the stream.
	@out	number size			Number of bytes successfully read.
*/
int MOAIStream::_readU16 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < u16 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	readU32
	@text	Reads an unsigned 32-bit value from the stream.
	
	@in		MOAIStream self
	@out	number value		Value from the stream.
	@out	number size			Number of bytes successfully read.
*/
int MOAIStream::_readU32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->ReadValues < u32 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	seek
	@text	Repositions the cursor in the stream.
	
	@in		MOAIStream self
	@in		number offset		Value from the stream.
	@opt	number mode			One of MOAIStream.SEEK_CUR, MOAIStream.SEEK_END, MOAIStream.SEEK_SET.
								Default value is MOAIStream.SEEK_SET.
	@out	nil
*/
int MOAIStream::_seek ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	
	u32 offset	= state.GetValue < u32 >( 2, 0 );
	u32 mode	= state.GetValue < u32 >( 3, SEEK_SET );
	
	self->Seek ( offset, mode );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	write
	@text	Write binary data to the stream.
	
	@in		MOAIStream self
	@in		string bytes		Binary data to write.
	@opt	number size			Number of bytes to write. Default value is the size of the string.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_write ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "US" );

	size_t len;
	cc8* str = lua_tolstring ( state, 2, &len );
	
	// TODO: 64bit
	size_t writeLen = state.GetValue < u32 >( 3, ( u32 )len );
	if ( len < writeLen ) {
		writeLen = len;
	}
	
	writeLen = self->WriteBytes ( str, writeLen );
	
	state.Push (( u32 )writeLen ); // TODO: overflow?
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	write8
	@text	Writes a signed 8-bit value to the stream.
	
	@in		MOAIStream self
	@in		number value		Value to write.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_write8 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < s8 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	write16
	@text	Writes a signed 16-bit value to the stream.
	
	@in		MOAIStream self
	@in		number value		Value to write.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_write16 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < s16 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	write32
	@text	Writes a signed 32-bit value to the stream.
	
	@in		MOAIStream self
	@in		number value		Value to write.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_write32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < s32 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	writeDouble
	@text	Writes a 64-bit floating point value to the stream.
	
	@in		MOAIStream self
	@in		number value		Value to write.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_writeDouble ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < double >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	writeFloat
	@text	Writes a 32-bit floating point value to the stream.
	
	@in		MOAIStream self
	@in		number value		Value to write.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_writeFloat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < float >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	writeFormat
	@text	Writes a series of values to the stream given a format string.
			See 'readFormat' for a list of valid format tokens.
	
	@in		MOAIStream self
	@in		string format
	@in		... values			Values to be written to the stream.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_writeFormat ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "US" );
	return self->WriteFormat ( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	writeStream
	@text	Reads bytes from the given stream into the calling stream.
	
	@in		MOAIStream self
	@in		MOAIStream stream	Value to write.
	@opt	number size			Number of bytes to read/write. Default value is the length of the input stream.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_writeStream ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "UU" );
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	size_t result = 0;
	
	if ( stream ) {
		
		ZLStream* inStream = stream;
		ZLStream* outStream = self;
		
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
	
	state.Push (( u32 )result ); // TODO: overflow?
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	writeU8
	@text	Writes an unsigned 8-bit value to the stream.
	
	@in		MOAIStream self
	@in		number value		Value to write.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_writeU8 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < u8 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	writeU16
	@text	Writes an unsigned 16-bit value to the stream.
	
	@in		MOAIStream self
	@in		number value		Value to write.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_writeU16 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < u16 >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	writeU32
	@text	Writes an unsigned 32-bit value to the stream.
	
	@in		MOAIStream self
	@in		number value		Value to write.
	@out	number size			Number of bytes successfully written.
*/
int MOAIStream::_writeU32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStream, "U" );
	return self->WriteValues < u32 >( state, 2 );
}

//================================================================//
// MOAIStream
//================================================================//

//----------------------------------------------------------------//
MOAIStream::MOAIStream () {
	
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

	idx = state.AbsIndex ( idx );
	cc8* format = state.GetValue < cc8* >( idx++, "" );
	
	size_t size;
	size_t result = 0;
	size_t bytes = 0;
	u32 type = UNKNOWN;
	
	while ( format ) {
	
		size = 0;
		format = MOAIStream::ParseTypeToken ( format, type );	
		result = this->GetCursor ();
		
		switch ( type ) {
			case INT_8:
				size = sizeof ( s8 );
				this->Write < s8 >( state.GetValue < s8 >( idx++, 0 ));
				break;
			case INT_16:
				size = sizeof ( s16 );
				this->Write < s16 >( state.GetValue < s16 >( idx++, 0 ));
				break;
			case INT_32:
				size = sizeof ( s32 );
				this->Write < s32 >( state.GetValue < s32 >( idx++, 0 ));
				break;
			case DOUBLE:
				size = sizeof ( double );
				this->Write < double >( state.GetValue < double >( idx++, 0 ));
				break;
			case FLOAT:
				size = sizeof ( float );
				this->Write < float >( state.GetValue < float >( idx++, 0 ));
				break;
			case UINT_8:
				size = sizeof ( u8 );
				this->Write < u8 >( state.GetValue < u8 >( idx++, 0 ));
				break;
			case UINT_16:
				size = sizeof ( u16 );
				this->Write < u16 >( state.GetValue < u16 >( idx++, 0 ));
				break;
			case UINT_32:
				size = sizeof ( u32 );
				this->Write < u32 >( state.GetValue < u32 >( idx++, 0 ));
				break;
			default:
				format = 0;
		}
		
		result = this->GetCursor () - result;
		bytes += result;
		if ( result != size ) {
			break;
		}
	}
	
	state.Push (( u32 )bytes ); // TODO: overflow?
	return 1;
}