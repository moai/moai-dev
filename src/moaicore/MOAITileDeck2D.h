// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITILEDECK2D_H
#define	MOAITILEDECK2D_H

#include <moaicore/MOAIDeck.h>

class MOAITexture;

//================================================================//
// MOAITileDeck2D
//================================================================//
/**	@brief Tile library class.
*/
class MOAITileDeck2D :
	public MOAIDeck,
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
	
	DECL_LUA_FACTORY ( MOAITileDeck2D )
	
	//----------------------------------------------------------------//
	bool			Bind					();
	void			Draw					( const USAffine2D& transform, u32 idx );
	void			Draw					( const USAffine2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 );
	USRect			GetBounds				( u32 idx );
					MOAITileDeck2D			();
					~MOAITileDeck2D			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void			SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	STLString		ToString				();
};

#endif
