// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISTREAMREADER_H
#define MOAISTREAMREADER_H

#include <moai-util/MOAIStream.h>

//================================================================//
// MOAIStreamReader
//================================================================//
/**	@name	MOAIStreamReader
	@text	MOAIStreamReader may be attached to another stream for the
			purpose of decoding and/or decompressing bytes read from that
			stream using a given algorithm (such as base64 or 'deflate'). 
*/
class MOAIStreamReader :
	public virtual MOAIStream {
private:
	
	MOAILuaSharedPtr < MOAIStream > mStream;
	ZLStreamReader* mReader;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_openBase64				( lua_State* L );
	static int		_openDeflate			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIStreamReader )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIStreamReader		();
					~MOAIStreamReader		();
	bool			Open					( MOAIStream* stream, ZLStreamReader* reader );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
