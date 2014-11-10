// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIMULTITEXTURE_H
#define	MOAIMULTITEXTURE_H

#include <moai-sim/MOAIGfxResource.h>

class MOAITextureBase;

//================================================================//
// MOAIMultiTexture
//================================================================//
/**	@lua	MOAIMultiTexture
	@text	Array of textures for multi-texturing.
*/
class MOAIMultiTexture :
	public virtual MOAILuaObject,
	public MOAIGfxState {
private:

	friend class MOAIGfxDevice;

	ZLLeanArray < MOAITextureBase* > mTextures;

	//----------------------------------------------------------------//
	static int		_reserve				( lua_State* L );
	static int		_setTexture				( lua_State* L );

	//----------------------------------------------------------------//
	bool			LoadGfxState			();

public:
	
	DECL_LUA_FACTORY ( MOAIMultiTexture )
	
	//----------------------------------------------------------------//
					MOAIMultiTexture		();
					~MOAIMultiTexture		();
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Reserve					( u32 total );
	void			SetTexture				( u32 idx, MOAITextureBase* texture );
};

#endif
