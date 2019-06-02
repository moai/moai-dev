// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDATABUFFERSTREAM_H
#define MOAIDATABUFFERSTREAM_H

#include <moai-util/MOAIStream.h>

class MOAIDataBuffer;

//================================================================//
// MOAIDataBufferStream
//================================================================//
/**	@lua	MOAIDataBufferStream
	@text	MOAIDataBufferStream locks an associated MOAIDataBuffer for
			reading and writing.
*/
class MOAIDataBufferStream :
	public virtual ZLByteStream,
	public virtual MOAIStream {
private:
	
	MOAILuaSharedPtr < MOAIDataBuffer > mDataBuffer;
	
	//----------------------------------------------------------------//
	static int			_close		( lua_State* L );
	static int			_open		( lua_State* L );

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass			( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( MOAIComposer& composer, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIDataBufferStream )

	//----------------------------------------------------------------//
	void				Close						();
						MOAIDataBufferStream		();
						~MOAIDataBufferStream		();
	bool				Open						( MOAIDataBuffer* buffer );
};

#endif
