// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUAD2D_H
#define	MOAIGFXQUAD2D_H

#include <moaicore/MOAIDeck.h>

class MOAITexture;

//================================================================//
// MOAIGfxQuad2D
//================================================================//
/**	@brief Texture class.
*/
class MOAIGfxQuad2D :
	public MOAIDeck {
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
	void					Draw					( const USAffine2D& transform, u32 idx );
	void					Draw					( const USAffine2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 );
	USRect					GetBounds				( u32 idx );
							MOAIGfxQuad2D			();
							~MOAIGfxQuad2D			();
	void					RegisterLuaClass		( USLuaState& state );
	void					RegisterLuaFuncs		( USLuaState& state );
	STLString				ToString				();
};

#endif
