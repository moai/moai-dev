// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERBUFFERGL_H
#define	MOAIRENDERBUFFERGL_H

#include <moai-gfx-gl/MOAIFrameBufferAttachableGL.h>
#include <moai-gfx-gl/MOAIGfxResourceGL.h>

class ZLImage;

//================================================================//
// MOAIRenderBufferGL
//================================================================//
// TODO: doxygen
class MOAIRenderBufferGL :
	public virtual MOAIFrameBufferAttachableGL {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	friend class MOAIGfxMgrGL_GPUCacheGL;
	friend class ZLTextureFormat;

	// GL texture
	ZLGfxHandle			mGLRenderBuffer;
	
	u32					mWidth;
	u32					mHeight;
	ZLGfxEnum::_		mGLPixelFormat;

	//----------------------------------------------------------------//
	static int			_getSize					( lua_State* L );
	static int			_init						( lua_State* L );
	
	//----------------------------------------------------------------//
	void				_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	bool				MOAIFrameBufferAttachableGL_Attach			( ZLGfx& gfx, ZLGfxEnum::_ attachment, ZLGfxEnum::_ target, s32 level, s32 layer );
	void				MOAIGfxResourceGL_OnGPUBind					();
	bool				MOAIGfxResourceGL_OnGPUCreate				();
	void				MOAIGfxResourceGL_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				MOAIGfxResourceGL_OnGPUUnbind				();
	bool				MOAIGfxResourceGL_OnGPUUpdate				();

public:
	
	DECL_LUA_FACTORY ( MOAIRenderBufferGL )
	
	GET_CONST ( ZLGfxHandle&, GLRenderBuffer, mGLRenderBuffer );

	//----------------------------------------------------------------//
						MOAIRenderBufferGL			();
						~MOAIRenderBufferGL			();
};

#endif
