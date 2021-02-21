// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERGL_H
#define	MOAIFRAMEBUFFERGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>

class MOAIGfxMgrGL;
class MOAIFrameBufferAttachableGL;

//================================================================//
// MOAIFrameBufferAttachmentGL
//================================================================//
class MOAIFrameBufferAttachmentGL {
public:

	ZLStrongPtr < MOAIFrameBufferAttachableGL > mAttachable;
	ZLGfxEnum::_		mAttachment;
	ZLGfxEnum::_		mTarget;
	u32					mLevel;
	u32					mLayer;
};

//================================================================//
// MOAIFrameBufferGL
//================================================================//
// TODO: doxygen
class MOAIFrameBufferGL :
	public virtual MOAIFrameBuffer,
	public virtual MOAIGfxResourceGL {
protected:
	
	friend class MOAIGfxMgrGL_GPUCacheGL;
	
	bool				mIsDefaultBuffer; // TODO: this is a hack; should probably be its own class (w/o attachments)
	ZLGfxHandle			mGLFrameBuffer;

	ZLLeanArray < MOAIFrameBufferAttachmentGL >	mAttachments;

	//----------------------------------------------------------------//
	static int			_addAttachment				( lua_State* L );
	
	//----------------------------------------------------------------//
	void				_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAIGfxResourceGL_OnGPUBind					();
	bool				MOAIGfxResourceGL_OnGPUCreate				();
	void				MOAIGfxResourceGL_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				MOAIGfxResourceGL_OnGPUUnbind				(); 
	bool				MOAIGfxResourceGL_OnGPUUpdate				();

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferGL )
	
	//----------------------------------------------------------------//
	void				AddAttachment				( ZLStrongPtr < MOAIFrameBufferAttachableGL > attachable, ZLGfxEnum::_ attachment, ZLGfxEnum::_ target, u32 level, u32 layer );
	void				DetectGLFrameBufferID		( MOAIGfxMgrGL& gfxMgr );
//	ZLRect				GetBufferRect				() const;
						MOAIFrameBufferGL			();
						~MOAIFrameBufferGL			();
	void				SetGLFrameBuffer			( MOAIGfxMgrGL& gfxMgr, const ZLGfxHandle& frameBuffer );
};

#endif
