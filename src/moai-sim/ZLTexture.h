// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTEXTURE_H
#define	ZLTEXTURE_H

#include <moai-sim/MOAIGfxResource.h>

class MOAIImage;

//================================================================//
// ZLTexture
//================================================================//
/**	@lua	ZLTexture
	@text	Base class for texture resources.

	@const	GL_LINEAR
	@const	GL_LINEAR_MIPMAP_LINEAR
	@const	GL_LINEAR_MIPMAP_NEAREST
	@const	GL_NEAREST
	@const	GL_NEAREST_MIPMAP_LINEAR
	@const	GL_NEAREST_MIPMAP_NEAREST
*/
class ZLTexture :
	public virtual ZLAbstractGfxResource {
protected:

	friend class MOAIGfxMgr;
	friend class ZLGfxStateGPUCache;
	friend class MOAIImageFormat;

	// debug name for memory use reporting
	STLString			mDebugName;

	// GL texture
	ZLGfxHandle			mGLTexture;
	
	// size of the original texture
	u32					mWidth;
	u32					mHeight;

	// GL_LINEAR
	// GL_LINEAR_MIPMAP_LINEAR
	// GL_LINEAR_MIPMAP_NEAREST
	// GL_NEAREST
	// GL_NEAREST_MIPMAP_LINEAR
	// GL_NEAREST_MIPMAP_NEAREST
	int					mMinFilter;
	int					mMagFilter;
	
	// GL_CLAMP_TO_EDGE
	// GL_REPEAT
	int					mWrap;
	
	int					mGLInternalFormat;
	int					mGLPixelType;
	
	size_t				mTextureSize;

	//----------------------------------------------------------------//
	void				CleanupOnError				();
	bool				CreateTextureFromImage		( MOAIImage& srcImage );
	bool				ZLAbstractGfxResource_OnCPUCreate					();
	void				ZLAbstractGfxResource_OnCPUDestroy				();
	void				ZLGfxListener_OnGfxEvent					( u32 event, void* userdata );
	void				ZLAbstractGfxResource_OnGPUBind					();
	void				ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				ZLAbstractGfxResource_OnGPUUnbind					();
	bool				ZLAbstractGfxResource_OnGPUUpdate					();
	void				SetGLTexture				( const ZLGfxHandle& glTexture, int internalFormat, int pixelType, size_t textureSize );
	bool				ShouldGenerateMipmaps		();
	bool				UpdateTextureFromImage		( MOAIImage& image, ZLIntRect rect );
	
public:
	
	GET_SET ( cc8*, DebugName, mDebugName );
	GET_CONST ( ZLGfxHandle&, GLTexture, mGLTexture );

	//----------------------------------------------------------------//
	static void			CheckFilterModes			( int min, int mag );
	void				DeleteTexture				();
	u32					GetHeight					();
	u32					GetWidth					();
	bool				IsValid						();
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
	void				SetFilter					( int filter );
	void				SetFilter					( int min, int mag );
	void				SetWrap						( int wrap );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
						ZLTexture					();
						~ZLTexture					();
};

#endif
