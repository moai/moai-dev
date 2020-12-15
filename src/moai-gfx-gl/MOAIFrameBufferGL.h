// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERGL_H
#define	MOAIFRAMEBUFFERGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>

class MOAIGfxMgrGL;
class MOAIFrameBufferAttachmentGL;

//================================================================//
// MOAIFrameBufferGL
//================================================================//
/**	@lua	MOAIFrameBufferGL
	@text	MOAIFrameBufferGL is responsible for drawing a list of MOAIRenderable
			objects. MOAIRenderable is the base class for any object that can be
			drawn. This includes MOAIProp and MOAIPartitionViewLayer. To use MOAIFrameBufferGL
			pass a table of MOAIRenderable objects to setRenderTable ().
			The table will usually be a stack of MOAIPartitionViewLayer objects. The contents of
			the table will be rendered the next time a frame is drawn. Note that the
			table must be an array starting with index 1. Objects will be rendered
			counting from the base index until 'nil' is encountered. The render
			table may include other tables as entries. These must also be arrays
			indexed from 1.
*/
class MOAIFrameBufferGL :
	public virtual MOAIFrameBuffer,
	public virtual MOAIGfxResourceGL {
protected:
	
	friend class MOAIGfxMgrGL_GPUCacheGL;
	
	bool				mIsDefaultBuffer; // TODO: this is a hack; should probably be its own class (w/o attachments)
	ZLGfxHandle			mGLFrameBuffer;

	ZLStrongPtr < MOAIFrameBufferAttachmentGL >		mColorAttachment;
	ZLStrongPtr < MOAIFrameBufferAttachmentGL >		mDepthAttachment;
	ZLStrongPtr < MOAIFrameBufferAttachmentGL >		mStencilAttachment;

	//----------------------------------------------------------------//
	static int			_getAttachment				( lua_State* L );
	static int			_setAttachment				( lua_State* L );
	
	//----------------------------------------------------------------//
	void				_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAIGfxResourceGL_OnGPUBind					();
	bool				MOAIGfxResourceGL_OnGPUCreate				();
	void				MOAIGfxResourceGL_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void				MOAIGfxResourceGL_OnGPUUnbind				(); 
	bool				MOAIGfxResourceGL_OnGPUUpdate				();
	void				MOAILuaObject_OnPooledRemit					();

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferGL )
	
	//----------------------------------------------------------------//
	void				DetectGLFrameBufferID		( MOAIGfxMgrGL& gfxMgr );
	ZLRect				GetBufferRect				() const;
						MOAIFrameBufferGL			();
						~MOAIFrameBufferGL			();
	void				SetGLFrameBuffer			( MOAIGfxMgrGL& gfxMgr, const ZLGfxHandle& frameBuffer );
};

#endif
