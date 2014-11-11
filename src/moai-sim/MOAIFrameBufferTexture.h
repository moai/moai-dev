// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
			attached to a MOAILayer for offscreen rendering. It is
			also a texture that may be bound and used like any other.
*/
class MOAIFrameBufferTexture :
	public MOAIFrameBuffer,
	public MOAITextureBase {
private:
	
	u32					mGLColorBufferID;
	u32					mGLDepthBufferID;
	u32					mGLStencilBufferID;
	
	u32					mColorFormat;
	u32					mDepthFormat;
	u32					mStencilFormat;
	
	//----------------------------------------------------------------//
	static int			_init					( lua_State* L );
	
	//----------------------------------------------------------------//
	bool				OnGPUCreate					();
	void				OnGPUDestroy				();
	void				OnGPULost					();

public:
	
	friend class MOAIGfxDevice;
	friend class MOAITextureBase;
	
	DECL_LUA_FACTORY ( MOAIFrameBufferTexture )
	
	//----------------------------------------------------------------//
	void				Init						( u32 width, u32 height, u32 colorFormat, u32 depthFormat, u32 stencilFormat );
						MOAIFrameBufferTexture		();
						~MOAIFrameBufferTexture		();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				Render						();
	void				SerializeIn					( MOAILuaState& state, MOAIDeserializer& serializer );
	void				SerializeOut				( MOAILuaState& state, MOAISerializer& serializer );
};

#endif
