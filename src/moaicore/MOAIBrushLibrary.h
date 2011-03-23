// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBRUSHLIBRARY_H
#define	MOAIBRUSHLIBRARY_H

#include <moaicore/MOAIContentLibrary2D.h>

class MOAITexture;

//================================================================//
// MOAIBrushLibrary
//================================================================//
/**	@brief Brush library class.
*/
class MOAIBrushLibrary :
	public MOAIContentLibrary2D {
private:

	USRef < MOAITexture >			mTexture;
	USLeanArray < USGLQuad >		mQuads;
	
	//----------------------------------------------------------------//
	static int	_reserveBrushes			( lua_State* L );
	static int	_scaleCoords			( lua_State* L );
	static int	_scaleUVCoords			( lua_State* L );
	static int	_setQuad				( lua_State* L );
	static int	_setRect				( lua_State* L );
	static int	_setTexture				( lua_State* L );
	static int	_setUVQuad				( lua_State* L );
	static int	_setUVRect				( lua_State* L );
	
	//----------------------------------------------------------------//
	USGLQuad*	GetGLQuad				( u32 idx );
	void		LoadTexture				( cc8* filename );
	
public:
	
	DECL_LUA_DATA ( MOAIBrushLibrary )
	
	//----------------------------------------------------------------//
	bool		Bind					();
	void		Draw					( MOAIDrawingMtx2D& transform, u32 idx );
	void		Draw					( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 );
	USRect		GetBounds				( u32 idx );
				MOAIBrushLibrary		();
				~MOAIBrushLibrary		();
	void		RegisterLuaClass		( USLuaState& state );
	void		RegisterLuaFuncs		( USLuaState& state );
	void		ScaleScreenCoords		( float xScale, float yScale );
	void		ScaleUVCoords			( float xScale, float yScale );
	STLString	ToString				();
};

#endif
