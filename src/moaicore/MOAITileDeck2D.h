// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITILEDECK2D_H
#define	MOAITILEDECK2D_H

#include <moaicore/MOAIDeck2D.h>

class MOAITexture;

//================================================================//
// MOAITileDeck2D
//================================================================//
/**	@name	MOAITileDeck2D
	@text	Subdivides a single texture into uniform tiles enumerated
			from the texture's left top to right bottom.
*/
class MOAITileDeck2D :
	public MOAIDeck2D,
	public USGridSpace {
private:
	
	USRect					mRect;
	USRef < MOAITexture >	mTexture;
	
	//----------------------------------------------------------------//
	static int		_setRect				( lua_State* L );
	static int		_setSize				( lua_State* L );
	static int		_setTexture				( lua_State* L );
	
public:
	
	GET_SET ( MOAITexture*, Texture, mTexture )
	
	DECL_LUA_FACTORY ( MOAITileDeck2D )
	
	//----------------------------------------------------------------//
	bool			Bind					();
	void			DrawPatch				( u32 idx, float xOff, float yOff, float xScale, float yScale );
	USRect			GetBounds				( u32 idx, MOAIDeckRemapper* remapper );
					MOAITileDeck2D			();
					~MOAITileDeck2D			();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void			SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	STLString		ToString				();
};

#endif
