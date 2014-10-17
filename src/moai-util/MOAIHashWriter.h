// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIHASHWRITER_H
#define MOAIHASHWRITER_H

#include <moai-util/MOAIStream.h>

//================================================================//
// MOAIHashWriter
//================================================================//
/**	@lua	MOAIHashWriter
	@text	MOAIHashWriter may be attached to another stream for the
			purpose of computing a hash while writing data to the other
			stream.
*/
class MOAIHashWriter :
	public virtual MOAIStream {
private:
	
	MOAILuaSharedPtr < MOAIStream > mStream;
	ZLHashWriter* mWriter;
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_getChecksum			( lua_State* L );
	static int		_getHash				( lua_State* L );
	static int		_getHashBase64			( lua_State* L );
	static int		_getHashHex				( lua_State* L );
	static int		_openAdler32			( lua_State* L );
	static int		_openCRC32				( lua_State* L );
	static int		_openCRC32b				( lua_State* L );
	static int		_openWhirlpool			( lua_State* L );
	static int		_setHMACKey				( lua_State* L );

protected:

	//----------------------------------------------------------------//
	static int		ImplementLuaHash		( lua_State* L, ZLHashWriter* writer );

public:
	
	DECL_LUA_FACTORY ( MOAIHashWriter )

	//----------------------------------------------------------------//
	void			Close					();
					MOAIHashWriter			();
					~MOAIHashWriter			();
	bool			Open					( MOAIStream* stream, ZLHashWriter* writer );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
