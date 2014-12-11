// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHASHWRITERCRYPTO_H
#define MOAIHASHWRITERCRYPTO_H

//================================================================//
// MOAIHashWriter
//================================================================//
/**	@lua	MOAIHashWriterCrypto
	@text	MOAIHashWriterCrypto may be attached to another stream for the
			purpose of computing a hash while writing data to the other
			stream. Currently only MD5 and SHA are available. 
*/
class MOAIHashWriterCrypto :
	public MOAIHashWriter {
private:
	
	//----------------------------------------------------------------//
	static int		_openMD5				( lua_State* L );
	static int		_openSHA1				( lua_State* L );
	static int		_openSHA224				( lua_State* L );
	static int		_openSHA256				( lua_State* L );
	static int		_openSHA384				( lua_State* L );
	static int		_openSHA512				( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIHashWriterCrypto )

	//----------------------------------------------------------------//
					MOAIHashWriterCrypto	();
					~MOAIHashWriterCrypto	();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
