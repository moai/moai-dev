// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIFILESTREAM_H
#define MOAIFILESTREAM_H

#include <moai-util/MOAIStream.h>

//================================================================//
// MOAIFileStream
//================================================================//
/**	@lua	MOAIFileStream
	@text	MOAIFileStream opens a system file handle for reading or writing.
	
	@const	READ
	@const	READ_WRITE
	@const	READ_WRITE_AFFIRM
	@const	READ_WRITE_NEW
	@const	WRITE
*/
class MOAIFileStream :
	public virtual ZLFileStream,
	public virtual MOAIStream {
private:
	
	//----------------------------------------------------------------//
	static int		_close					( lua_State* L );
	static int		_open					( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIFileStream )

	//----------------------------------------------------------------//
					MOAIFileStream			();
					~MOAIFileStream			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
