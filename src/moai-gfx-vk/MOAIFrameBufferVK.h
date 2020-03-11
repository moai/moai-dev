// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERVK_H
#define	MOAIFRAMEBUFFERVK_H

class MOAIGfxMgrVK;

//================================================================//
// MOAIFrameBufferVK
//================================================================//
/**	@lua	MOAIFrameBufferVK
	@text	MOAIFrameBufferVK is responsible for drawing a list of MOAIRenderable
			objects. MOAIRenderable is the base class for any object that can be
			drawn. This includes MOAIProp and MOAIPartitionViewLayer. To use MOAIFrameBufferVK
			pass a table of MOAIRenderable objects to setRenderTable ().
			The table will usually be a stack of MOAIPartitionViewLayer objects. The contents of
			the table will be rendered the next time a frame is drawn. Note that the
			table must be an array starting with index 1. Objects will be rendered
			counting from the base index until 'nil' is encountered. The render
			table may include other tables as entries. These must also be arrays
			indexed from 1.
*/
class MOAIFrameBufferVK :
	public virtual MOAIFrameBuffer {
protected:
	
	friend class MOAIGfxMgrVK_GPUCacheVK;
	
//	ZLGfxHandle			mGLFrameBuffer;
//
//	bool								mGrabNextFrame;
//	MOAILuaMemberRef					mOnFrameFinish;
//	MOAILuaSharedPtr < MOAIImage >		mFrameImage;
//
//	//----------------------------------------------------------------//
//	static int			_getGrabbedImage			( lua_State* L );
//	static int			_grabNextFrame				( lua_State* L );
//	static int			_isPendingGrab				( lua_State* L );
//
//	//----------------------------------------------------------------//
//	void				AffirmBuffers					();
	
	//----------------------------------------------------------------//
	virtual void		MOAIFrameBufferVK_AffirmBuffers			();
//	void				ZLGfxListener_OnReadPixels				( const ZLCopyOnWrite& buffer, void* userdata );

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferVK )
	
	//----------------------------------------------------------------//
//	void				DetectGLFrameBufferID		( MOAIGfxMgrVK& gfxMgr );
//	ZLRect				GetBufferRect				() const;
//	void				GrabImage					( MOAIImage* image );
						MOAIFrameBufferVK			();
						~MOAIFrameBufferVK			();
//	void				SetGLFrameBuffer			( MOAIGfxMgrVK& gfxMgr, const ZLGfxHandle& frameBuffer );
//	ZLRect				WndRectToDevice				( ZLRect rect ) const;
};

#endif
