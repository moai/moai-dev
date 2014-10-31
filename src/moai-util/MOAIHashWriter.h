// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHASHWRITER_H
#define MOAIHASHWRITER_H

#include <moai-util/MOAIStreamAdapter.h>

//================================================================//
// MOAIHashWriter
//================================================================//
/**	@lua	MOAIHashWriter
	@text	MOAIHashWriter may be attached to another stream for the
			purpose of computing a hash while writing data to the other
			stream.
*/
class MOAIHashWriter :
	public MOAIStreamAdapter {
protected:

	ZLHashWriter*	mHashWriter;

	//----------------------------------------------------------------//
	static int		_getChecksum			( lua_State* L );
	static int		_getHash				( lua_State* L );
	static int		_getHashBase64			( lua_State* L );
	static int		_getHashHex				( lua_State* L );
	static int		_openAdler32			( lua_State* L );
	static int		_openCRC32				( lua_State* L );
	static int		_openCRC32b				( lua_State* L );
	static int		_openWhirlpool			( lua_State* L );
	static int		_setHMACKey				( lua_State* L );
	
	//----------------------------------------------------------------//
	ZLHashWriter*	GetHashWriter			();

public:
	
	DECL_LUA_FACTORY ( MOAIHashWriter )

	//----------------------------------------------------------------//
					MOAIHashWriter			();
					~MOAIHashWriter			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
