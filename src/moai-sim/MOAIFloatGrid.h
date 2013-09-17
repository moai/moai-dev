// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFLOATGRID_H
#define	MOAIFLOATGRID_H

#include <moai-sim/MOAITypedGrid.h>

//================================================================//
// MOAIFloatGrid
//================================================================//
/**	@name	MOAIFloatGrid
	@text	Grid data object. Grid cells are indexed starting and (1,1).
			Grid indices will wrap if out of range. Uses floats
			instead of u32.
*/
class MOAIFloatGrid :
	public MOAITypedGrid < float > {

public:
	DECL_LUA_FACTORY ( MOAIFloatGrid )
					MOAIFloatGrid			();
					~MOAIFloatGrid			();
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			RegisterLuaClass	( MOAILuaState& state );
};

#endif
