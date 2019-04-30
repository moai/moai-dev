// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLFRAMEBUFFERTEXTURE_H
#define	ZLFRAMEBUFFERTEXTURE_H

#include <zl-gfx/ZLFrameBuffer.h>
#include <zl-gfx/ZLTexture.h>

//================================================================//
// ZLFrameBufferTexture
//================================================================//
class ZLFrameBufferTexture :
	public virtual ZLFrameBuffer,
	public virtual ZLTextureBase {
private:
	
	ZLGfxHandle			mGLColorBuffer;
	ZLGfxHandle			mGLDepthBuffer;
	ZLGfxHandle			mGLStencilBuffer;
	
	u32					mColorFormat;
	u32					mDepthFormat;
	u32					mStencilFormat;
	
	//----------------------------------------------------------------//
	void				ZLAbstractGfxResource_OnGPUBind					();
	bool				ZLAbstractGfxResource_OnGPUCreate				();
	void				ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				ZLFrameBuffer_AffirmBuffers						();

public:
	
	//----------------------------------------------------------------//
	void				Init						( u32 width, u32 height, u32 colorFormat, u32 depthFormat, u32 stencilFormat );
						ZLFrameBufferTexture		();
						~ZLFrameBufferTexture		();
};

#endif
