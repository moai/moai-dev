// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLGFXBUFFERGL_H
#define	ZLGFXBUFFERGL_H

#include <zl-gfx/ZLAbstractGfxResourceGL.h>
#include <zl-gfx/ZLGfxHandleGL.h>

class MOAIVertexFormatGL;

//================================================================//
// ZLGfxBufferGL
//================================================================//
/**	@lua	ZLGfxBufferGL
	@text	Base class for MOAIVertexBufferGL and MOAIIndexBufferGL.
*/
class ZLGfxBufferGL :
	public virtual ZLAbstractGfxResourceGL,
	public virtual ZLStream,
	public ZLCopyOnWrite {
protected:
	
	friend class ZLGfxPipelineClerkGL;
	friend class ZLGfxStateGPUCacheGL;
	friend class MOAIVertexArrayItem;
	friend class ZLVertexArrayGL;
	
	enum {
		UPDATE_MODE_MAPBUFFER,
		UPDATE_MODE_ORPHAN,
		UPDATE_MODE_SUBDATA,
	};
	
	ZLLeanArray < ZLGfxHandleGL >	mVBOs;
	ZLIndex							mCurrentVBO;
	u32								mTarget;

	bool							mUseVBOs;
	bool							mCopyOnUpdate;
	
	//----------------------------------------------------------------//
	ZLSharedConstBuffer*	GetBufferForBind		( ZLGfx& gfx );
	
	//----------------------------------------------------------------//
	bool					ZLAbstractGfxResource_OnCPUCreate				();
	void					ZLAbstractGfxResource_OnCPUDestroy				();
	void					ZLAbstractGfxResource_OnGPUBind					();
	bool					ZLAbstractGfxResource_OnGPUCreate				();
	void					ZLAbstractGfxResource_OnGPUDeleteOrDiscard		( bool shouldDelete );
	void					ZLAbstractGfxResource_OnGPUUnbind				();
	bool					ZLAbstractGfxResource_OnGPUUpdate				();

public:
	
	GET ( size_t, BufferCount, mVBOs.Size ())
	GET ( u32, Target, mTarget )
	GET_SET ( bool, CopyOnUpdate, mCopyOnUpdate )
	
	IS ( UsingVBOs, mUseVBOs, true )
	
	//----------------------------------------------------------------//
	void						Clear					();
	void						CopyFromStream			( ZLStream& stream, size_t size );
	size_t						CountVBOs				();
//	ZLSharedConstBuffer*		GetBuffer				();
	void						Reserve					( ZLSize size );
	void						ReserveVBOs				( ZLSize gpuBuffers );
								ZLGfxBufferGL			();
								~ZLGfxBufferGL			();
};

#endif
