// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDEFLATEREADER_H
#define MOAIDEFLATEREADER_H

#include <moaicore/MOAIStream.h>

//================================================================//
// MOAIDeflateReader
//================================================================//
// TODO: doxygen
class MOAIDeflateReader :
	public virtual MOAIStream {
private:
	
	USDeflateReader mReader;
	MOAILuaSharedPtr < MOAIStream > mStream;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_open					( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIDeflateReader )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIDeflateReader		();
					~MOAIDeflateReader		();
	bool			Open					( MOAIStream* stream );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
