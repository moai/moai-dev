// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMULTITEXTURE_H
#define	MOAIMULTITEXTURE_H

#include <moai-sim/MOAITextureBase.h>

//================================================================//
// MOAIMultiTexture
//================================================================//
/**	@lua	MOAIMultiTexture
	@text	Array of textures for multi-texturing.
*/
class MOAIMultiTexture :
	public MOAITextureBase {
private:

	friend class MOAIGfxMgr;
	friend class MOAIGfxStateCache;

	ZLLeanArray < MOAISingleTexture* > mTextures;

	//----------------------------------------------------------------//
	static int			_release					( lua_State* L );
	static int			_reserve					( lua_State* L );
	static int			_setTexture					( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAIMultiTexture )
	
	//----------------------------------------------------------------//
	u32					CountActiveUnits			();
	MOAISingleTexture*	GetTextureForUnit			( u32 unit );
						MOAIMultiTexture			();
						~MOAIMultiTexture			();
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				Reserve						( u32 total );
	void				SetTexture					( u32 idx, MOAISingleTexture* texture );
};

#endif
