// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMEMSTREAM_H
#define MOAIMEMSTREAM_H

#include <moaicore/MOAIStream.h>

//================================================================//
// MOAIStream
//================================================================//
// TODO: doxygen
class MOAIMemStream :
	public virtual MOAIStream {
private:
	
	//USFileStream mFileStream;
	
	//----------------------------------------------------------------//
	//static int		_close		( lua_State* L );
	//static int		_open		( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIMemStream )

	//----------------------------------------------------------------//
					MOAIMemStream		();
					~MOAIMemStream		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
