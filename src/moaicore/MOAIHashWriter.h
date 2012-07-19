// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHASHWRITER_H
#define MOAIHASHWRITER_H

#include <moaicore/MOAIStream.h>
#include <uslscore/USMD5Writer.h>

//================================================================//
// MOAIHashWriter
//================================================================//
/**	@name	MOAIHashWriter
	@text	MOAIHashWriter may be attached to another stream for the
			purpose of computing a hash while writing data to the other
			stream. Currently only MD5 hash is available. 
*/
class MOAIHashWriter :
	public virtual MOAIStream {
private:
	
	MOAILuaSharedPtr < MOAIStream > mStream;
	USMD5Writer* mWriter;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_openMD5				( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIHashWriter )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIHashWriter			();
					~MOAIHashWriter			();
	bool			OpenMD5					( MOAIStream* stream, USMD5Writer* writer );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
