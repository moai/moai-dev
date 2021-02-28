// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
	public virtual MOAIHashWriter {
private:
	
	//----------------------------------------------------------------//
	static int		_openMD5				( lua_State* L );
	static int		_openSHA1				( lua_State* L );
	static int		_openSHA224				( lua_State* L );
	static int		_openSHA256				( lua_State* L );
	static int		_openSHA384				( lua_State* L );
	static int		_openSHA512				( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIHashWriterCrypto )

	//----------------------------------------------------------------//
					MOAIHashWriterCrypto	( ZLContext& context );
					~MOAIHashWriterCrypto	();
};

#endif
