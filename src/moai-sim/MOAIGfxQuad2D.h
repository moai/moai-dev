// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXQUAD2D_H
#define	MOAIGFXQUAD2D_H

#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIQuadBrush.h>

class MOAIGfxState;

//================================================================//
// MOAIGfxQuad2D
//================================================================//
/**	@lua	MOAIGfxQuad2D
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
	ZLBox			ComputeMaxBounds		();
	ZLBox			GetItemBounds			( u32 idx );

public:
	
	DECL_LUA_FACTORY ( MOAIGfxQuad2D )
	
	//----------------------------------------------------------------//
	void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
	bool			Inside					( u32 idx, ZLVec3D vec, float pad );
					MOAIGfxQuad2D			();
					~MOAIGfxQuad2D			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Transform				( const ZLAffine3D& mtx );
	void			TransformUV				( const ZLAffine3D& mtx );
};

#endif
