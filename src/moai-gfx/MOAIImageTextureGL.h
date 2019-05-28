// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGETEXTUREGL_H
#define	MOAIIMAGETEXTUREGL_H

#include <moai-gfx/MOAITextureBaseGL.h>

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
	public MOAIGfxResourceGL < ZLTextureBaseGL >,
	public MOAIImage,
	public MOAITextureBaseGL {
private:

	ZLIntRect	mRegion;

	//----------------------------------------------------------------//
	static int		_updateRegion			( lua_State* L );

	//----------------------------------------------------------------//
	void			MOAIImage_OnImageStatusChanged			( bool isOK );
	void			ZLAbstractGfxResource_OnClearDirty		();
	bool			ZLAbstractGfxResource_OnGPUCreate		();
	bool			ZLAbstractGfxResource_OnGPUUpdate		();

public:
	
	DECL_LUA_FACTORY ( MOAIImageTextureGL )
	
	static const u32 DEFAULT_TRANSFORM = ZLImageTransform::TRUECOLOR | ZLImageTransform::PREMULTIPLY_ALPHA;
	
	//----------------------------------------------------------------//
					MOAIImageTextureGL		();
					~MOAIImageTextureGL		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			UpdateRegion			();
	void			UpdateRegion			( ZLIntRect rect );
};

#endif
