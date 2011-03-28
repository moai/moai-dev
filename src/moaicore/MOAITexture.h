// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE2D_H
#define	MOAITEXTURE2D_H

#include <moaicore/MOAIDeck.h>

class MOAIDataBuffer;

//================================================================//
// MOAITexture
//================================================================//
/**	@brief Texture class.
*/
class MOAITexture :
	public virtual USLuaObject,
	public USTexture {
private:

	STLString		mTexturePath;

	//----------------------------------------------------------------//
	static int		_bind				( lua_State* L );
	static int		_getSize			( lua_State* L );
	static int		_load				( lua_State* L );
	static int		_release			( lua_State* L );
	static int		_setFilter			( lua_State* L );
	static int		_setWrap			( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAITexture )
	
	//----------------------------------------------------------------//
	static MOAITexture*		AffirmTexture			( USLuaState& state, int idx );
	bool					Bind					();
	void					Load					( MOAIDataBuffer& data );
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
