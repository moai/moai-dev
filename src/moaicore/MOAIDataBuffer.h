// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDATABUFFER_H
#define MOAIDATABUFFER_H

#include <moaicore/MOAILua.h>

class MOAIDataIOAction;

//================================================================//
// MOAIDataBuffer
//================================================================//
/**	@name	MOAIDataBuffer
	@text	Buffer for loading and holding data. Data operations may be
			performed without additional penalty of marshalling buffers
			between Lua and C.
*/
class MOAIDataBuffer :
	public virtual MOAILuaObject,
	public USData {
private:
	
	//----------------------------------------------------------------//
	static int		_base64Decode	( lua_State* L );
	static int		_base64Encode	( lua_State* L );
	static int		_deflate		( lua_State* L );
	static int		_getSize		( lua_State* L );
	static int		_getString		( lua_State* L );
	static int		_hexDecode		( lua_State* L );
	static int		_hexEncode		( lua_State* L );
	static int		_inflate		( lua_State* L );
	static int		_load			( lua_State* L );
	static int		_loadAsync		( lua_State* L );
	static int		_save			( lua_State* L );
	static int		_saveAsync		( lua_State* L );
	static int		_setString		( lua_State* L );
	static int		_toCppHeader	( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIDataBuffer )

	//----------------------------------------------------------------//
					MOAIDataBuffer		();
					~MOAIDataBuffer		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
};

#endif
