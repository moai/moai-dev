// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISTREAM_H
#define MOAISTREAM_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIStream
//================================================================//
/**	@name	MOAIStream
	@text	Interface for reading/writing binary data.
	
	@const	SEEK_CUR
	@const	SEEK_END
	@const	SEEK_SET
*/
class MOAIStream :
	public virtual MOAILuaObject {
private:
	
	enum {
		UNKNOWN,
		SIGNED,
		UNSIGNED,
		INT_8,
		INT_16,
		INT_32,
		DOUBLE,
		FLOAT,
		UINT_8,
		UINT_16,
		UINT_32,
	};
	
	USStream* mStream;
	
	//----------------------------------------------------------------//
	static int		_flush				( lua_State* L );
	static int		_getCursor			( lua_State* L );
	static int		_getLength			( lua_State* L );
	static int		_read				( lua_State* L );
	static int		_read8				( lua_State* L );
	static int		_read16				( lua_State* L );
	static int		_read32				( lua_State* L );
	static int		_readDouble			( lua_State* L );
	static int		_readFloat			( lua_State* L );
	static int		_readFormat			( lua_State* L );
	static int		_readU8				( lua_State* L );
	static int		_readU16			( lua_State* L );
	static int		_readU32			( lua_State* L );
	static int		_seek				( lua_State* L );
	static int		_write				( lua_State* L );
	static int		_write8				( lua_State* L );
	static int		_write16			( lua_State* L );
	static int		_write32			( lua_State* L );
	static int		_writeDouble		( lua_State* L );
	static int		_writeFloat			( lua_State* L );
	static int		_writeFormat		( lua_State* L );
	static int		_writeStream		( lua_State* L );
	static int		_writeU8			( lua_State* L );
	static int		_writeU16			( lua_State* L );
	static int		_writeU32			( lua_State* L );

	//----------------------------------------------------------------//
	static cc8*		ParseTypeToken		( cc8* format, u32& type );
	int				ReadFormat			( MOAILuaState& state, int idx );
	int				WriteFormat			( MOAILuaState& state, int idx );

	//----------------------------------------------------------------//
	template < typename TYPE >
	size_t ReadValue ( MOAILuaState& state ) {
		
		if ( this->mStream ) {
			TYPE value;
			size_t size = sizeof ( TYPE );
			size_t bytes = this->mStream->ReadBytes ( &value, size );
			if ( bytes == size ) {
				state.Push ( value );
			}
			else {
				state.Push ();
			}
			return bytes;
		}
		
		state.Push ();
		return 0;
	}

	//----------------------------------------------------------------//
	template < typename TYPE >
	int ReadValues ( MOAILuaState& state, int idx ) {
		
		u32 total = state.GetValue < u32 >( idx, 1 );
		size_t size = sizeof ( TYPE );
		size_t bytes = 0;
		
		if ( this->mStream ) {
		
			for ( u32 i = 0; i < total; ++i ) {
				TYPE value;
				u32 result = this->mStream->ReadBytes ( &value, size );
				bytes += result;
				if ( result == size ) {
					state.Push ( value );
				}
				else {
					// TODO: report errors
					for ( u32 j = i; j < total; ++j ) {
						state.Push ();
					}
					break;
				} 
			}
		}
		else {
			for ( u32 i = 0; i < total; ++i ) {
				state.Push ();
			}
		}
		state.Push ( bytes );
		return total + 1;
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	int WriteValues ( MOAILuaState& state, int idx ) {
		
		idx = state.AbsIndex ( idx );
		u32 total = ( state.GetTop () - idx ) + 1;
		size_t size = sizeof ( TYPE );
		size_t bytes = 0;
		
		if ( this->mStream ) {
			for ( u32 i = 0; i < total; ++i ) {
				TYPE value = state.GetValue < TYPE >( idx + i, 0 );
				u32 result = this->mStream->WriteBytes ( &value, size );
				bytes += result;
				if ( result != size ) {
					// TODO: report errors
					break;
				}
			}
		}
		state.Push ( bytes );
		return 1;
	}

protected:

	SET ( USStream*, USStream, mStream )

public:

	GET ( USStream*, USStream, mStream )

	//----------------------------------------------------------------//
					MOAIStream			();
					~MOAIStream			();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
