// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBASE64WRITER_H
#define MOAIBASE64WRITER_H

#include <moaicore/MOAIStream.h>

//================================================================//
// MOAIBase64Writer
//================================================================//
// TODO: doxygen
class MOAIBase64Writer :
	public virtual MOAIStream {
private:
	
	USBase64Writer mWriter;
	MOAILuaSharedPtr < MOAIStream > mStream;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_open					( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIBase64Writer )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIBase64Writer		();
					~MOAIBase64Writer		();
	bool			Open					( MOAIStream* stream );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
