// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGRID_H
#define	MOAIGRID_H

#include <moai-sim/MOAITypedGrid.h>

//================================================================//
// MOAIGrid
//================================================================//
/**	@name	MOAIGrid
	@text	Grid data object. Grid cells are indexed starting and (1,1).
			Grid indices will wrap if out of range. Tiles are
			u32 values, which may include flags in their high
			bits.
*/
class MOAIGrid :
	public MOAITypedGrid < u32 > {
private:


	static int		_clearTileFlags		( lua_State* L );
	static int		_getTileFlags		( lua_State* L );
	static int		_setTileFlags		( lua_State* L );
	static int		_toggleTileFlags	( lua_State* L );

public:
	DECL_LUA_FACTORY ( MOAIGrid )
					MOAIGrid			();
					~MOAIGrid			();
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			RegisterLuaClass	( MOAILuaState& state );
};

#endif
