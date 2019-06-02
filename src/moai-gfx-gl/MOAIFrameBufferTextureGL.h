// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERTEXTUREGL_H
#define	MOAIFRAMEBUFFERTEXTUREGL_H

#include <moai-gfx-gl/MOAIFrameBufferGL.h>
#include <moai-gfx-gl/MOAITextureGL.h>

//================================================================//
// MOAIFrameBufferTextureGL
//================================================================//
class MOAIFrameBufferTextureGL :
	public virtual MOAIFrameBufferGL,
	public virtual MOAITextureGL {
private:
	
	ZLGfxHandle			mGLColorBuffer;
	ZLGfxHandle			mGLDepthBuffer;
	ZLGfxHandle			mGLStencilBuffer;
	
	u32					mColorFormat;
	u32					mDepthFormat;
	u32					mStencilFormat;
	
	//----------------------------------------------------------------//
	static int			_init							( lua_State* L );
	
	//----------------------------------------------------------------//
	void				MOAIFrameBufferGL_AffirmBuffers				();
	void				MOAIGfxResourceGL_OnGPUBind					();
	bool				MOAIGfxResourceGL_OnGPUCreate				();
	void				MOAIGfxResourceGL_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut					( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferTextureGL )
	
	//----------------------------------------------------------------//
	void				Init							( u32 width, u32 height, u32 colorFormat, u32 depthFormat, u32 stencilFormat );
						MOAIFrameBufferTextureGL		();
						~MOAIFrameBufferTextureGL		();
};

#endif
