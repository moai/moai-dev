// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITILELIBRARY_H
#define	MOAITILELIBRARY_H

#include <moaicore/MOAIContentLibrary2D.h>

class MOAITexture;

//================================================================//
// MOAITileLibrary
//================================================================//
/**	@brief Tile library class.
*/
class MOAITileLibrary :
	public MOAIContentLibrary2D,
	public USGridSpace {
private:
	
	USRect					mRect;
	USRef < MOAITexture >	mTexture;
	
	u32						mFlags;
	
	//----------------------------------------------------------------//
	static int		_setFlip				( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setSize				( lua_State* L );
	static int		_setTexture				( lua_State* L );
	
	//----------------------------------------------------------------//
	void			DrawTile				( u32 tile, USRect rect );
	
public:
	
	GET_SET ( MOAITexture*, Texture, mTexture )
	
	DECL_LUA_DATA ( MOAITileLibrary )
	
	//----------------------------------------------------------------//
	bool			Bind					();
	void			Draw					( MOAIDrawingMtx2D& transform, u32 idx );
	void			Draw					( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 );
	USRect			GetBounds				( u32 idx );
					MOAITileLibrary			();
					~MOAITileLibrary		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void			SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	STLString		ToString				();
};

#endif
