// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITILEDECK2D_H
#define	MOAITILEDECK2D_H

#include <moaicore/MOAIDeck2D.h>
#include <moaicore/MOAILua.h>

class MOAITextureBase;

//================================================================//
// MOAITileDeck2D
//================================================================//
/**	@name	MOAITileDeck2D
	@text	Subdivides a single texture into uniform tiles enumerated
			from the texture's left top to right bottom.
*/
class MOAITileDeck2D :
	public MOAIDeck2D,
	public MOAIGridSpace {
private:
	
	USRect mRect;
	
	//----------------------------------------------------------------//
	static int		_setRect				( lua_State* L );
	static int		_setSize				( lua_State* L );
	
public:
	
	DECL_LUA_FACTORY ( MOAITileDeck2D )
	
	//----------------------------------------------------------------//
	void			DrawPatch				( u32 idx, float xOff, float yOff, float xScale, float yScale );
	USRect			GetRect					();
	USRect			GetRect					( u32 idx, MOAIDeckRemapper* remapper );
					MOAITileDeck2D			();
					~MOAITileDeck2D			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
