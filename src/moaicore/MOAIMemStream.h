// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMEMSTREAM_H
#define MOAIMEMSTREAM_H

#include <moaicore/MOAIStream.h>

//================================================================//
// MOAIMemStream
//================================================================//
/**	@name	MOAIMemStream
	@text	MOAIMemStream implements an in-memory stream and grows as
			needed. The mem stream expands on demands by allocating
			additional 'chunks' or memory. The chunk size may be configured
			by the user. Note that the chunks are not guaranteed to be
			contiguous in memory.
*/
class MOAIMemStream :
	public virtual MOAIStream {
private:
	
	USMemStream mMemStream;
	
	//----------------------------------------------------------------//
	static int		_close				( lua_State* L );
	static int		_getString			( lua_State* L );
	static int		_open				( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIMemStream )

	//----------------------------------------------------------------//
	void			Close				();
					MOAIMemStream		();
					~MOAIMemStream		();
	bool			Open				( u32 reserve, u32 chunkSize );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
