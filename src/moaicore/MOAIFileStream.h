// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFILESTREAM_H
#define MOAIFILESTREAM_H

#include <moaicore/MOAIStream.h>

//================================================================//
// MOAIStream
//================================================================//
// TODO: doxygen
class MOAIFileStream :
	public virtual MOAIStream {
private:
	
	USFileStream mFileStream;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_open					( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIFileStream )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIFileStream			();
					~MOAIFileStream			();
	bool			Open					( cc8* filename, u32 mode );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
