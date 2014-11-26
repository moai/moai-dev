// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGETEXTURE_H
#define	MOAIIMAGETEXTURE_H

#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// MOAIImageTexture
//================================================================//
/**	@lua	MOAIImageTexture
	@text	Binds an image (CPU memory) to a texture (GPU memory).
			Regions of the texture (or the entire texture) may be
			invalidated. Invalidated regions will be reloaded
			into GPU memory the next time the texture is bound.
*/
class MOAIImageTexture :
	public MOAITextureBase,
	public MOAIImage {
private:

	enum {
		VALID,
		INVALID,
	};

	u32			mStatus;
	ZLIntRect	mRegion;

	//----------------------------------------------------------------//
	static int		_updateRegion			( lua_State* L );

	//----------------------------------------------------------------//
	void			OnGPUBind				();
	bool			OnGPUCreate				();
	void			OnImageStatusChanged	( bool isOK );

public:
	
	DECL_LUA_FACTORY ( MOAIImageTexture )
	
	static const u32 DEFAULT_TRANSFORM = MOAIImageTransform::TRUECOLOR | MOAIImageTransform::PREMULTIPLY_ALPHA;
	
	//----------------------------------------------------------------//
					MOAIImageTexture		();
					~MOAIImageTexture		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
	void			UpdateRegion			();
	void			UpdateRegion			( ZLIntRect rect );
};

#endif
