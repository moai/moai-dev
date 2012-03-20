// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUAD2D_H
#define	MOAIGFXQUAD2D_H

#include <moaicore/MOAIDeck2D.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAIQuadBrush.h>

class MOAIGfxState;

//================================================================//
// MOAIGfxQuad2D
//================================================================//
/**	@name	MOAIGfxQuad2D
	@text	Single textured quad.
*/
class MOAIGfxQuad2D :
	public MOAIDeck2D {
private:

	USRect	mRect;
	USRect	mUVRect;

	//----------------------------------------------------------------//
	static int		_setRect			( lua_State* L );
	static int		_setUVRect			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIGfxQuad2D )
	
	//----------------------------------------------------------------//
	void			DrawPatch				( u32 idx, float xOff, float yOff, float xScale, float yScale );
	USRect			GetRect					( u32 idx, MOAIDeckRemapper* remapper );
	USRect			GetRect					( );
					MOAIGfxQuad2D			();
					~MOAIGfxQuad2D			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
