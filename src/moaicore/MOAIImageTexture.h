// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGETEXTURE_H
#define	MOAIIMAGETEXTURE_H

#include <moaicore/MOAIImage.h>
#include <moaicore/MOAITextureBase.h>

//================================================================//
// MOAIImageTexture
//================================================================//
/**	@name	MOAIImageTexture
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
		INVALID_REGION,
	};

	u32			mStatus;
	USIntRect	mRegion;

	//----------------------------------------------------------------//
	static int		_invalidate				( lua_State* L );

	//----------------------------------------------------------------//
	bool			IsRenewable				();
	bool			IsValid					();
	void			OnClear					();
	void			OnCreate				();
	void			OnLoad					();

public:
	
	DECL_LUA_FACTORY ( MOAIImageTexture )
	
	static const u32 DEFAULT_TRANSFORM = MOAIImageTransform::TRUECOLOR | MOAIImageTransform::PREMULTIPLY_ALPHA;
	
	//----------------------------------------------------------------//
	void			Invalidate				();
	void			Invalidate				( USIntRect rect );
					MOAIImageTexture		();
					~MOAIImageTexture		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SerializeIn				( MOAILuaState& state, MOAIDeserializer& serializer );
	void			SerializeOut			( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
