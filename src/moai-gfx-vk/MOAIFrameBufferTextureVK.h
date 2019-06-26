// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERTEXTUREVK_H
#define	MOAIFRAMEBUFFERTEXTUREVK_H

#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

//================================================================//
// MOAIFrameBufferTextureVK
//================================================================//
class MOAIFrameBufferTextureVK :
	public virtual MOAIFrameBufferVK,
	public virtual MOAITextureVK {
private:
	
//	ZLGfxHandle			mGLColorBuffer;
//	ZLGfxHandle			mGLDepthBuffer;
//	ZLGfxHandle			mGLStencilBuffer;
//
//	u32					mColorFormat;
//	u32					mDepthFormat;
//	u32					mStencilFormat;
	
	//----------------------------------------------------------------//
//	static int			_init							( lua_State* L );
	
	//----------------------------------------------------------------//
	void				MOAIFrameBufferVK_AffirmBuffers				();
	void				MOAIGfxResourceVK_OnGPUBind					();
	bool				MOAIGfxResourceVK_OnGPUCreate				();
	void				MOAIGfxResourceVK_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				MOAILuaObject_RegisterLuaClass				( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs				( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_SerializeIn					( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer );
	void				MOAILuaObject_SerializeOut					( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer );

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferTextureVK )
	
	//----------------------------------------------------------------//
//	void				Init							( u32 width, u32 height, u32 colorFormat, u32 depthFormat, u32 stencilFormat );
						MOAIFrameBufferTextureVK		();
						~MOAIFrameBufferTextureVK		();
};

#endif
