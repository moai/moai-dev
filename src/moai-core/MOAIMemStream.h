// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMEMSTREAM_H
#define MOAIMEMSTREAM_H

#include <moai-core/MOAIStream.h>

//================================================================//
// MOAIMemStream
//================================================================//
/**	@lua	MOAIMemStream
	@text	MOAIMemStream implements an in-memory stream and grows as
			needed. The memory stream expands on demands by allocating
			additional 'chunks' or memory. The chunk size may be configured
			by the user. Note that the chunks are not guaranteed to be
			contiguous in memory.
*/
class MOAIMemStream :
	public virtual ZLMemStream,
	public virtual MOAIStream {
private:
	
	//----------------------------------------------------------------//
	static int		_close				( lua_State* L );
	static int		_discardAll			( lua_State* L );
	static int		_discardBack		( lua_State* L );
	static int		_discardFront		( lua_State* L );
	static int		_getString			( lua_State* L );
	static int		_open				( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIMemStream )

	//----------------------------------------------------------------//
					MOAIMemStream		( ZLContext& context );
					~MOAIMemStream		();
	bool			Open				( u32 reserve, u32 chunkSize );
};

#endif
