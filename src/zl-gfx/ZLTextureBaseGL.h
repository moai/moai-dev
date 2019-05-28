// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLTEXTUREBASEGL_H
#define	ZLTEXTUREBASEGL_H

#include <zl-gfx/ZLAbstractGfxResourceGL.h>
#include <zl-gfx/ZLAbstractTexture.h>
#include <zl-gfx/ZLGfxHandleGL.h>

class ZLImage;

//================================================================//
// ZLTextureBaseGL
//================================================================//
/**	@lua	ZLTextureBaseGL
	@text	Base class for texture resources.

	@const	GL_LINEAR
	@const	GL_LINEAR_MIPMAP_LINEAR
	@const	GL_LINEAR_MIPMAP_NEAREST
	@const	GL_NEAREST
	@const	GL_NEAREST_MIPMAP_LINEAR
	@const	GL_NEAREST_MIPMAP_NEAREST
*/
class ZLTextureBaseGL :
	public virtual ZLAbstractTexture,
	public virtual ZLAbstractGfxResourceGL {
protected:

	friend class ZLGfxStateGPUCacheGL;
	friend class ZLTextureFormat;

	// debug name for memory use reporting
	STLString			mDebugName;

	// GL texture
	ZLGfxHandleGL			mGLTexture;
	
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
	void				CleanupOnError									();
	bool				CreateTextureFromImage							( ZLImage& srcImage );
	void				SetGLTexture									( const ZLGfxHandleGL& glTexture, int internalFormat, int pixelType, size_t textureSize );
	bool				ShouldGenerateMipmaps							();
	bool				UpdateTextureFromImage							( ZLImage& image, ZLIntRect rect );
	
	//----------------------------------------------------------------//
	bool				ZLAbstractGfxResource_OnCPUCreate				();
	void				ZLAbstractGfxResource_OnCPUDestroy				();
	void				ZLAbstractGfxResource_OnGPUBind					();
	void				ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				ZLAbstractGfxResource_OnGPUUnbind				();
	bool				ZLAbstractGfxResource_OnGPUUpdate				();
	u32					ZLAbstractTexture_GetHeight						() const;
	u32					ZLAbstractTexture_GetWidth						() const;
	void				ZLGfxListener_OnGfxEvent						( u32 event, void* userdata );
	
public:
	
	GET_SET ( cc8*, DebugName, mDebugName );
	GET_CONST ( ZLGfxHandleGL&, GLTexture, mGLTexture );

	//----------------------------------------------------------------//
	static void			CheckFilterModes			( int min, int mag );
	bool				IsValid						();
	void				SetFilter					( int filter );
	void				SetFilter					( int min, int mag );
	void				SetWrap						( int wrap );
						ZLTextureBaseGL				();
						~ZLTextureBaseGL			();
};

#endif
