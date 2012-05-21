// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDEFLATEWRITER_H
#define MOAIDEFLATEWRITER_H

#include <moaicore/MOAIStream.h>

//================================================================//
// MOAIDeflateWriter
//================================================================//
// TODO: doxygen
class MOAIDeflateWriter :
	public virtual MOAIStream {
private:
	
	USDeflateWriter mWriter;
	MOAILuaSharedPtr < MOAIStream > mStream;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_open					( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIDeflateWriter )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIDeflateWriter		();
					~MOAIDeflateWriter		();
	bool			Open					( MOAIStream* stream, int level, int windowBits );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
