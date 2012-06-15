// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUAD2D_H
#define	MOAIGFXQUAD2D_H

#include <moaicore/MOAIDeck.h>
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
	public MOAIDeck {
private:

	MOAIQuadBrush mQuad;

	//----------------------------------------------------------------//
	static int		_setQuad			( lua_State* L );
	static int		_setRect			( lua_State* L );
	static int		_setUVQuad			( lua_State* L );
	static int		_setUVRect			( lua_State* L );
	static int		_transform			( lua_State* L );
	static int		_transformUV		( lua_State* L );

	//----------------------------------------------------------------//
	USBox			ComputeMaxBounds		();
	USBox			GetItemBounds			( u32 idx );

public:
	
	DECL_LUA_FACTORY ( MOAIGfxQuad2D )
	
	//----------------------------------------------------------------//
	void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
					MOAIGfxQuad2D			();
					~MOAIGfxQuad2D			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Transform				( const USAffine3D& mtx );
	void			TransformUV				( const USAffine3D& mtx );
};

#endif
