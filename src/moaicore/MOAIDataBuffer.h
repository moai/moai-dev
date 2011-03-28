// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIDATABUFFER_H
#define MOAIDATABUFFER_H

class MOAIDataIOAction;

//================================================================//
// MOAIDataBuffer
//================================================================//
/**	@brief Buffer for loading and holding data.
*/
class MOAIDataBuffer :
	public virtual USLuaObject,
	public USData {
private:
	
	//----------------------------------------------------------------//
	static int		_base64Decode	( lua_State* L );
	static int		_base64Encode	( lua_State* L );
	static int		_deflate		( lua_State* L );
	static int		_getSize		( lua_State* L );
	static int		_getString		( lua_State* L );
	static int		_inflate		( lua_State* L );
	static int		_load			( lua_State* L );
	static int		_loadAsync		( lua_State* L );
	static int		_save			( lua_State* L );
	static int		_saveAsync		( lua_State* L );
	static int		_setString		( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIDataBuffer )

	//----------------------------------------------------------------//
					MOAIDataBuffer			();
					~MOAIDataBuffer			();
	void			RegisterLuaClass	( USLuaState& state );
	void			RegisterLuaFuncs	( USLuaState& state );
	STLString		ToString			();
};

#endif
