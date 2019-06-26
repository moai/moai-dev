// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGETEXTUREVK_H
#define	MOAIIMAGETEXTUREVK_H

#include <moai-gfx-vk/MOAITextureVK.h>

//================================================================//
// MOAIImageTextureVK
//================================================================//
/**	@lua	MOAIImageTextureVK
	@text	Binds an image (CPU memory) to a texture (GPU memory).
			Regions of the texture (or the entire texture) may be
			invalidated. Invalidated regions will be reloaded
			into GPU memory the next time the texture is bound.
*/
class MOAIImageTextureVK :
	public MOAIImageTexture,
	public MOAITextureVK {
private:

	//----------------------------------------------------------------//
	bool			MOAIGfxResourceVK_OnGPUCreate		();
	bool			MOAIGfxResourceVK_OnGPUUpdate		();
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIImageTextureVK )
	
//	static const u32 DEFAULT_TRANSFORM = ZLImageTransform::TRUECOLOR | ZLImageTransform::PREMULTIPLY_ALPHA;
	
	//----------------------------------------------------------------//
					MOAIImageTextureVK		();
					~MOAIImageTextureVK		();
};

#endif
