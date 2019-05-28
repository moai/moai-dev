// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLFRAMEBUFFERGL_H
#define	ZLFRAMEBUFFERGL_H

#include <zl-gfx/ZLGfxHandleGL.h>
#include <zl-gfx/ZLGfxListener.h>

class ZLGfxMgrGL;

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
class ZLFrameBufferGL :
	public virtual ZLGfxListener {
protected:
	
	friend class ZLGfxStateGPUCacheGL;
	
	u32					mBufferWidth;
	u32					mBufferHeight;
	float				mBufferScale;
	bool				mLandscape;
	
	bool				mNeedsClear;
	
	ZLGfxHandleGL		mGLFrameBuffer;

	//----------------------------------------------------------------//
	void				AffirmBuffers					();

	//----------------------------------------------------------------//
	virtual void		ZLFrameBuffer_AffirmBuffers		();

public:
		
	GET_CONST	( u32, BufferWidth, mBufferWidth )
	GET_CONST	( u32, BufferHeight, mBufferHeight )
	GET_SET		( float, BufferScale, mBufferScale )
	GET_SET		( bool, Landscape, mLandscape )
	
	//----------------------------------------------------------------//
	void				DetectGLFrameBufferID		( ZLGfxMgrGL& gfxMgr );
	ZLRect				GetBufferRect				() const;
	bool				NeedsClear					() const;
	void				NeedsClear					( bool needsClear );
	void				SetBufferSize				( u32 width, u32 height );
	void				SetGLFrameBuffer			( ZLGfxMgrGL& gfxMgr, const ZLGfxHandleGL& frameBuffer );
	ZLRect				WndRectToDevice				( ZLRect rect ) const;
						ZLFrameBufferGL				();
						~ZLFrameBufferGL			();
};

#endif
