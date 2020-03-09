// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERGL_H
#define	MOAIFRAMEBUFFERGL_H

class MOAIGfxMgrGL;

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
	public virtual ZLGfxListener {
protected:
	
	friend class MOAIGfxMgrGL_GPUCacheGL;
	
	ZLGfxHandle			mGLFrameBuffer;

	bool								mGrabNextFrame;
	MOAILuaMemberRef					mOnFrameFinish;
	MOAILuaSharedPtr < MOAIImage >		mFrameImage;

	//----------------------------------------------------------------//
	static int			_getGrabbedImage			( lua_State* L );
	static int			_grabNextFrame				( lua_State* L );
	static int			_isPendingGrab				( lua_State* L );

	//----------------------------------------------------------------//
	void				AffirmBuffers					();
	
	//----------------------------------------------------------------//
	virtual void		MOAIFrameBufferGL_AffirmBuffers			();
	void				MOAILuaObject_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				ZLGfxListener_OnReadPixels				( const ZLCopyOnWrite& buffer, void* userdata );

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferGL )
	
	//----------------------------------------------------------------//
	void				DetectGLFrameBufferID		( MOAIGfxMgrGL& gfxMgr );
	ZLRect				GetBufferRect				() const;
	void				GrabImage					( MOAIImage* image );
						MOAIFrameBufferGL			();
						~MOAIFrameBufferGL			();
	void				SetGLFrameBuffer			( MOAIGfxMgrGL& gfxMgr, const ZLGfxHandle& frameBuffer );
//	ZLRect				WndRectToDevice				( ZLRect rect ) const;
};

#endif
