// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLFRAMEBUFFERTEXTUREGL_H
#define	ZLFRAMEBUFFERTEXTUREGL_H

#include <zl-gfx/ZLFrameBufferGL.h>
#include <zl-gfx/ZLTextureGL.h>

//================================================================//
// ZLFrameBufferTextureGL
//================================================================//
class ZLFrameBufferTextureGL :
	public virtual ZLFrameBufferGL,
	public virtual ZLTextureBaseGL {
private:
	
	ZLGfxHandleGL			mGLColorBuffer;
	ZLGfxHandleGL			mGLDepthBuffer;
	ZLGfxHandleGL			mGLStencilBuffer;
	
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
						ZLFrameBufferTextureGL		();
						~ZLFrameBufferTextureGL		();
};

#endif
