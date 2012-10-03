// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHASHWRITER_H
#define MOAIHASHWRITER_H

#include <moaicore/MOAIStream.h>

//================================================================//
// MOAIHashWriter
//================================================================//
/**	@name	MOAIHashWriter
	@text	MOAIHashWriter may be attached to another stream for the
			purpose of computing a hash while writing data to the other
			stream. Currently only MD5 and SHA256 are available. 
*/
class MOAIHashWriter :
	public virtual MOAIStream {
private:
	
	MOAILuaSharedPtr < MOAIStream > mStream;
	USHashWriter* mWriter;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_getChecksum			( lua_State* L );
	static int		_getHash				( lua_State* L );
	static int		_getHashBase64			( lua_State* L );
	static int		_getHashHex				( lua_State* L );
	static int		_openAdler32			( lua_State* L );
	static int		_openCRC32				( lua_State* L );
	static int		_openCRC32b				( lua_State* L );
	static int		_openMD5				( lua_State* L );
	static int		_openSHA1				( lua_State* L );
	static int		_openSHA224				( lua_State* L );
	static int		_openSHA256				( lua_State* L );
	static int		_openSHA384				( lua_State* L );
	static int		_openSHA512				( lua_State* L );
	static int		_openWhirlpool			( lua_State* L );
	static int		_setHMACKey				( lua_State* L );

	//----------------------------------------------------------------//
	static int		ImplementLuaHash		( lua_State* L, USHashWriter* writer );

public:
	
	DECL_LUA_FACTORY ( MOAIHashWriter )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIHashWriter			();
					~MOAIHashWriter			();
	bool			Open					( MOAIStream* stream, USHashWriter* writer );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
