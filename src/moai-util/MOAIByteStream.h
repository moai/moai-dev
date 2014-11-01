// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBYTESTREAM_H
#define MOAIBYTESTREAM_H

#include <moai-util/MOAIStream.h>

//================================================================//
// MOAIByteStream
//================================================================//
// TODO: doxygen
class MOAIByteStream :
	public virtual ZLByteStream,
	public virtual MOAIStream {
private:
	
	void*			mData;
	
	//----------------------------------------------------------------//
	static int		_close				( lua_State* L );
	static int		_open				( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIByteStream )

	//----------------------------------------------------------------//
	void			Close				();
					MOAIByteStream		();
					~MOAIByteStream		();
	void			Open				( size_t size );
	void			Open				( void* data, size_t size );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
