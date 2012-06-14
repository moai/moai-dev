// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAISTREAMWRITER_H
#define MOAISTREAMWRITER_H

#include <moaicore/MOAIStream.h>

//================================================================//
// MOAIStreamWriter
//================================================================//
/**	@name	MOAIStreamWriter
	@text	MOAIStreamWriter may be attached to another stream for the
			purpose of encoding and/or compressing bytes written to that
			stream using a given algorithm (such as base64 or 'deflate'). 
*/
class MOAIStreamWriter :
	public virtual MOAIStream {
private:
	
	MOAILuaSharedPtr < MOAIStream > mStream;
	USStreamWriter* mWriter;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_openBase64				( lua_State* L );
	static int		_openDeflate			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIStreamWriter )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIStreamWriter		();
					~MOAIStreamWriter		();
	bool			Open					( MOAIStream* stream, USStreamWriter* writer );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
