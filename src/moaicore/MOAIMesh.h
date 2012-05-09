// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMESH_H
#define	MOAIMESH_H

#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAILua.h>

class MOAIIndexBuffer;
class MOAITextureBase;
class MOAIVertexBuffer;

//================================================================//
// MOAIMesh
//================================================================//
/**	@name	MOAIMesh
	@text	Loads a texture and renders the contents of a vertex buffer.
			Grid drawing not supported.
	
	@const	GL_POINTS
	@const	GL_LINES
	@const	GL_TRIANGLES
	@const	GL_LINE_LOOP
	@const	GL_LINE_STRIP
	@const	GL_TRIANGLE_FAN
	@const	GL_TRIANGLE_STRIP
*/
class MOAIMesh :
	public MOAIDeck {
private:

	MOAILuaSharedPtr < MOAIIndexBuffer >	mIndexBuffer;
	MOAILuaSharedPtr < MOAIVertexBuffer >	mVertexBuffer;

	u32		mPrimType;
	
	float	mPenWidth;
	float	mPointSize;

	//----------------------------------------------------------------//
	static int		_setIndexBuffer		( lua_State* L );
	static int		_setPenWidth		( lua_State* L );
	static int		_setPointSize		( lua_State* L );
	static int		_setPrimType		( lua_State* L );
	static int		_setVertexBuffer	( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIMesh )
	
	//----------------------------------------------------------------//
	void			Draw					( const USAffine3D& transform, u32 idx, MOAIDeckRemapper* remapper );
	void            Draw					( const USAffine3D& transform, MOAIGrid& grid, MOAIDeckRemapper* remapper, USVec2D& gridScale, MOAICellCoord& c0, MOAICellCoord& c1 );
	USBox			GetBounds				();
	USBox			GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
	MOAIGfxState*	GetShaderDefault		();
					MOAIMesh				();
					~MOAIMesh				();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
