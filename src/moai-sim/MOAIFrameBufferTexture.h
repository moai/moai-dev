// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERTEXTURE_H
#define	MOAIFRAMEBUFFERTEXTURE_H

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// MOAIFrameBufferTexture
//================================================================//
/**	@lua	MOAIFrameBufferTexture
	@text	This is an implementation of a frame buffer that may be
			attached to a MOAIPartitionViewLayer for offscreen rendering. It is
			also a texture that may be bound and used like any other.
*/
class MOAIFrameBufferTexture :
	public MOAIGfxResource < ZLFrameBufferTexture >,
	public MOAIFrameBuffer,
	public MOAITextureBase {
private:
	
	ZLGfxHandle			mGLColorBuffer;
	ZLGfxHandle			mGLDepthBuffer;
	ZLGfxHandle			mGLStencilBuffer;
	
	u32					mColorFormat;
	u32					mDepthFormat;
	u32					mStencilFormat;
	
	//----------------------------------------------------------------//
	static int			_init					( lua_State* L );
	
	//----------------------------------------------------------------//
	void				ZLAbstractGfxResource_OnGPUBind					();
	bool				ZLAbstractGfxResource_OnGPUCreate				();
	void				ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				ZLFrameBuffer_AffirmBuffers						();

public:
	
	friend class MOAIGfxMgr;
	friend class MOAITextureBase;
	
	DECL_LUA_FACTORY ( MOAIFrameBufferTexture )
	
	//----------------------------------------------------------------//
	void				Init						( u32 width, u32 height, u32 colorFormat, u32 depthFormat, u32 stencilFormat );
						MOAIFrameBufferTexture		();
						~MOAIFrameBufferTexture		();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
