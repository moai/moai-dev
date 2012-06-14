// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDATABUFFERSTREAM_H
#define MOAIDATABUFFERSTREAM_H

#include <moaicore/MOAIStream.h>

class MOAIDataBuffer;

//================================================================//
// MOAIDataBufferStream
//================================================================//
/**	@name	MOAIDataBufferStream
	@text	MOAIDataBufferStream locks an associated MOAIDataBuffer for
			reading and writing.
*/
class MOAIDataBufferStream :
	public virtual MOAIStream {
private:
	
	USByteStream mByteStream;
	MOAILuaSharedPtr < MOAIDataBuffer > mDataBuffer;
	
	//----------------------------------------------------------------//
	static int			_close		( lua_State* L );
	static int			_open		( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIDataBufferStream )

	//----------------------------------------------------------------//
	void				Close						();
						MOAIDataBufferStream		();
						~MOAIDataBufferStream		();
	bool				Open						( MOAIDataBuffer* buffer );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
};

#endif
