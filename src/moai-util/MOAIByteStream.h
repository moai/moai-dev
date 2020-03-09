// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBYTESTREAM_H
#define MOAIBYTESTREAM_H

#include <moai-util/MOAIStream.h>

//================================================================//
// MOAIByteStream
//================================================================//
/**	@lua	MOAIByteStream
	@text	MOAIByteStream implements a stream with a fixed-size internal buffer.
*/
class MOAIByteStream :
	public virtual ZLByteStream,
	public virtual MOAIStream {
private:
	
	void*			mData;
	
	//----------------------------------------------------------------//
	static int		_close				( lua_State* L );
	static int		_open				( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIByteStream )

	//----------------------------------------------------------------//
	void			Close				();
					MOAIByteStream		();
					~MOAIByteStream		();
	void			Open				( size_t size );
	void			Open				( void* data, size_t size );
};

#endif
