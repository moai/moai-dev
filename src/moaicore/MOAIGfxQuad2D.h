// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUAD2D_H
#define	MOAIGFXQUAD2D_H

#include <moaicore/MOAIDeck2D.h>

class MOAITexture;

//================================================================//
// MOAIGfxQuad2D
//================================================================//
/**	@name	MOAIGfxQuad2D
	@text	Single textured quad.
*/
class MOAIGfxQuad2D :
	public MOAIDeck2D {
private:

	USRef < MOAITexture	>	mTexture;
	USRect					mRect;
	USRect					mUVRect;

	//----------------------------------------------------------------//
	static int		_setRect			( lua_State* L );
	static int		_setTexture			( lua_State* L );
	static int		_setUVRect			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIGfxQuad2D )
	
	//----------------------------------------------------------------//
	bool					Bind					();
	void					DrawPatch				( u32 idx, float xOff, float yOff, float xScale, float yScale );
	USRect					GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
							MOAIGfxQuad2D			();
							~MOAIGfxQuad2D			();
	void					RegisterLuaClass		( USLuaState& state );
	void					RegisterLuaFuncs		( USLuaState& state );
	STLString				ToString				();
};

#endif
