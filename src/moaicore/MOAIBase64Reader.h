// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBASE64READER_H
#define MOAIBASE64READER_H

#include <moaicore/MOAIStream.h>

//================================================================//
// MOAIBase64Reader
//================================================================//
// TODO: doxygen
class MOAIBase64Reader :
	public virtual MOAIStream {
private:
	
	USBase64Reader mReader;
	MOAILuaSharedPtr < MOAIStream > mStream;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_open					( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIBase64Reader )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIBase64Reader		();
					~MOAIBase64Reader		();
	bool			Open					( MOAIStream* stream );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
