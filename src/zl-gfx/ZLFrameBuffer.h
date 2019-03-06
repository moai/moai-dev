// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLFRAMEBUFFER_H
#define	ZLFRAMEBUFFER_H

#include <zl-gfx/ZLGfxHandle.h>
#include <zl-gfx/ZLGfxListener.h>

class ZLGfxMgr;

//================================================================//
// MOAIFrameBuffer
//================================================================//
/**	@lua	MOAIFrameBuffer
	@text	MOAIFrameBuffer is responsible for drawing a list of MOAIRenderable
			objects. MOAIRenderable is the base class for any object that can be
			drawn. This includes MOAIProp and MOAIPartitionViewLayer. To use MOAIFrameBuffer
			pass a table of MOAIRenderable objects to setRenderTable ().
			The table will usually be a stack of MOAIPartitionViewLayer objects. The contents of
			the table will be rendered the next time a frame is drawn. Note that the
			table must be an array starting with index 1. Objects will be rendered
			counting from the base index until 'nil' is encountered. The render
			table may include other tables as entries. These must also be arrays
			indexed from 1.
*/
class ZLFrameBuffer :
	public virtual ZLGfxListener {
protected:
	
	friend class ZLGfxStateGPUCache;
	
	u32					mBufferWidth;
	u32					mBufferHeight;
	float				mBufferScale;
	bool				mLandscape;
	
	bool				mNeedsClear;
	
	ZLGfxHandle			mGLFrameBuffer;

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
	void				DetectGLFrameBufferID		( ZLGfxMgr& gfxMgr );
	ZLRect				GetBufferRect				() const;
	bool				NeedsClear					() const;
	void				NeedsClear					( bool needsClear );
	void				SetBufferSize				( u32 width, u32 height );
	void				SetGLFrameBuffer			( ZLGfxMgr& gfxMgr, const ZLGfxHandle& frameBuffer );
	ZLRect				WndRectToDevice				( ZLRect rect ) const;
						ZLFrameBuffer				();
						~ZLFrameBuffer				();
};

#endif
