// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVECTORIMAGE2D_H
#define	MOAIVECTORIMAGE2D_H

#include <moai-sim/MOAIDeck.h>

class MOAIShape;

//================================================================//
// MOAIVectorImage
//================================================================//
/**	@name	MOAIVectorImage
	@text	Display object made of vector shapes.
*/
class MOAIVectorImage :
	public MOAIDeck {
private:

	static const u32 MAX_POLYGON_VERTS = 256;

	ZLLeanArray < MOAIShape* > mShapes;

	//----------------------------------------------------------------//
	static int		_addContour			( lua_State* L );
	static int		_reserveShapes		( lua_State* L );
	static int		_setFillColor		( lua_State* L );
	static int		_setFillStyle		( lua_State* L );
	static int		_setLineColor		( lua_State* L );
	static int		_setLineStyle		( lua_State* L );
	static int		_setPolygon			( lua_State* L );
	static int		_tessalate			( lua_State* L );

	//----------------------------------------------------------------//
	ZLBox			ComputeMaxBounds		();
	ZLBox			GetItemBounds			( u32 idx );
	void			SetShape				( u32 idx, MOAIShape* shape );

public:
	
	DECL_LUA_FACTORY ( MOAIVectorImage )
	
	//----------------------------------------------------------------//
	void			Clear					();
	void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
					MOAIVectorImage			();
					~MOAIVectorImage		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
