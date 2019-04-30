// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE_H
#define	MOAITEXTURE_H

#include <moai-gfx/MOAIGfxResource.h>
#include <moai-gfx/MOAITextureBase.h>

class MOAIDataBuffer;

//================================================================//
// MOAITexture
//================================================================//
/**	@lua	MOAITexture
	@text	Texture class.
*/
class MOAITexture :
	public MOAIGfxResource < ZLTexture >,
	public virtual MOAITextureBase {
private:

	//----------------------------------------------------------------//
	static int			_load						( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAITexture )
		
	//----------------------------------------------------------------//
	static MOAITextureBase*		AffirmTexture			( MOAILuaState& state, int idx );
	bool						Init					( MOAILuaState& state, int idx );
	void						Init					( MOAIDataBuffer& data, u32 transform, cc8* debugname );
	void						Init					( const ZLImage& image, cc8* debugname );
	void						Init					( const ZLImage& image, int srcX, int srcY, int width, int height, cc8* debugname );
	void						Init					( cc8* filename, u32 transform, cc8* debugname = NULL );
	void						Init					( ZLStream& stream, u32 transform, cc8* debugname );
	void						Init					( const void* data, size_t size, u32 transform, cc8* debugname );
								MOAITexture				();
								~MOAITexture			();
	void						RegisterLuaClass		( MOAILuaState& state );
	void						RegisterLuaFuncs		( MOAILuaState& state );
	void						SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void						SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
