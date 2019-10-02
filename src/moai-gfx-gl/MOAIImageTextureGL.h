// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGETEXTUREGL_H
#define	MOAIIMAGETEXTUREGL_H

#include <moai-gfx-gl/MOAITextureGL.h>

//================================================================//
// MOAIImageTextureGL
//================================================================//
/**	@lua	MOAIImageTextureGL
	@text	Binds an image (CPU memory) to a texture (GPU memory).
			Regions of the texture (or the entire texture) may be
			invalidated. Invalidated regions will be reloaded
			into GPU memory the next time the texture is bound.
*/
class MOAIImageTextureGL :
	public MOAIImageTexture,
	public MOAITextureGL {
private:

	//----------------------------------------------------------------//
	void			MOAIGfxResource_Clear				();
	bool			MOAIGfxResourceGL_OnGPUCreate		();
	bool			MOAIGfxResourceGL_OnGPUUpdate		();
	void			MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void			MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );

public:
	
	DECL_LUA_FACTORY ( MOAIImageTextureGL )
	
	static const u32 DEFAULT_TRANSFORM = ZLImageTransform::TRUECOLOR | ZLImageTransform::PREMULTIPLY_ALPHA;
	
	//----------------------------------------------------------------//
					MOAIImageTextureGL		();
					~MOAIImageTextureGL		();
};

#endif
