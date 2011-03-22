// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE_H
#define	MOAITEXTURE_H

#include <moaicore/MOAIContentLibrary2D.h>

class MOAIData;

//================================================================//
// MOAITexture
//================================================================//
/**	@brief Texture class.
*/
class MOAITexture :
	virtual public MOAIContentLibrary2D,
	public USTexture {
private:

	STLString		mTexturePath;
	USRect			mRect;
	USRect			mUVRect;

	//----------------------------------------------------------------//
	static int		_bind				( lua_State* L );
	static int		_getTextureSize		( lua_State* L );
	static int		_load				( lua_State* L );
	static int		_release			( lua_State* L );
	static int		_setFilter			( lua_State* L );
	static int		_setRect			( lua_State* L );
	static int		_setUVRect			( lua_State* L );
	static int		_setWrap			( lua_State* L );

public:
	
	DECL_LUA_DATA ( MOAITexture )
	
	//----------------------------------------------------------------//
	static MOAITexture*		AffirmTexture			( USLuaState& state, int idx );
	bool					Bind					();
	void					Draw					( MOAIDrawingMtx2D& transform, u32 idx );
	void					Draw					( MOAIDrawingMtx2D& transform, MOAIGrid& grid, USTileCoord& c0, USTileCoord& c1 );
	USRect					GetBounds				( u32 idx );
	void					Load					( MOAIData& data );
	void					Load					( cc8* filename );
							MOAITexture				();
							~MOAITexture			();
	void					SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	void					SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
	void					RegisterLuaClass		( USLuaState& state );
	void					RegisterLuaFuncs		( USLuaState& state );
	STLString				ToString				();
};

#endif
