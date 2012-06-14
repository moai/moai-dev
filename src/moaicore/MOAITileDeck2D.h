// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITILEDECK2D_H
#define	MOAITILEDECK2D_H

#include <moaicore/MOAIDeck.h>
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
	public MOAIDeck,
	public MOAIGridSpace {
private:
	
	MOAIQuadBrush mQuad;
	
	//----------------------------------------------------------------//
	static int		_setQuad				( lua_State* L );
	static int		_setRect				( lua_State* L );
	static int		_setUVQuad				( lua_State* L );
	static int		_setUVRect				( lua_State* L );
	static int		_setSize				( lua_State* L );
	static int		_transform				( lua_State* L );
	static int		_transformUV			( lua_State* L );
	
	//----------------------------------------------------------------//
	USBox			ComputeMaxBounds		();
	USBox			GetItemBounds			( u32 idx );
	
public:
	
	DECL_LUA_FACTORY ( MOAITileDeck2D )
	
	//----------------------------------------------------------------//
	void			DrawIndex				( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl );
					MOAITileDeck2D			();
					~MOAITileDeck2D			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			Transform				( const USAffine3D& mtx );
	void			TransformUV				( const USAffine3D& mtx );
};

#endif
