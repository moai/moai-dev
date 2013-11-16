// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICOLORGRID_H
#define	MOAICOLORGRID_H

#include <moai-sim/MOAITypedGrid.h>
#include <zl-util/ZLColor.h>

//================================================================//
// MOAIColorGrid
//================================================================//
/**	@name	MOAIColorGrid
	@text	Grid data object. Grid cells are indexed starting and (1,1).
			Grid indices will wrap if out of range. Uses floats
			instead of u32.
*/
class MOAIColorGrid :
	public MOAITypedGrid < ZLColorVec > {

public:
	DECL_LUA_FACTORY ( MOAIColorGrid )
					MOAIColorGrid			();
					~MOAIColorGrid			();
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			RegisterLuaClass	( MOAILuaState& state );
};

#endif
